/*
 * Driver: arduino_cnc_driver.c
 * Description: Driver personalizado para detectar y comunicarse con un Arduino por USB (CH340)
 *              y enviarle strings de control para accionar motores (CNC).
 *
 * NOTA: Este driver evita usar tty/USB estándar (ch341.ko), y accede directamente al dispositivo USB.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define DRIVER_NAME "arduino_cnc_driver"
#define DRIVER_CLASS "arduinoCNCClass"
#define VENDOR_ID  0x1A86   // CH340/CH341 Vendor ID
#define PRODUCT_ID 0x7523   // CH340/CH341 Product ID

MODULE_LICENSE("GPL");
MODULE_AUTHOR("César Mora, Andrés Rodríguez, Crhistopher Castro");
MODULE_DESCRIPTION("Driver para controlar Arduino CNC por USB personalizado");

static dev_t dev_number;
static struct cdev arduino_cdev;
static struct class *arduino_class;
static struct usb_device *connected_device;
static struct usb_class_driver arduino_usb_class;
static struct usb_interface *stored_interface;

static DEFINE_MUTEX(usb_lock);

// Interfaz de escritura al Arduino desde user space
static ssize_t arduino_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *ppos) {
    char buffer[64];
    int retval;

    if (!connected_device)
        return -ENODEV;

    if (count > sizeof(buffer))
        return -EINVAL;

    if (copy_from_user(buffer, user_buffer, count))
        return -EFAULT;

    mutex_lock(&usb_lock);
    retval = usb_bulk_msg(connected_device,
                          usb_sndbulkpipe(connected_device, 0x02), // endpoint OUT
                          buffer,
                          count,
                          NULL,
                          1000);
    mutex_unlock(&usb_lock);

    if (retval) {
        printk(KERN_ERR "[arduino_cnc_driver] Error enviando datos: %d\n", retval);
        return retval;
    }

    return count;
}

static int arduino_open(struct inode *inode, struct file *file) {
    return 0;
}

static int arduino_release(struct inode *inode, struct file *file) {
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = arduino_write,
    .open = arduino_open,
    .release = arduino_release
};

// Función cuando el dispositivo es conectado
static int arduino_probe(struct usb_interface *interface, const struct usb_device_id *id) {
    int result;
    printk(KERN_INFO "[arduino_cnc_driver] Arduino detectado!\n");

    connected_device = interface_to_usbdev(interface);
    stored_interface = interface;

    result = alloc_chrdev_region(&dev_number, 0, 1, DRIVER_NAME);
    if (result < 0) {
        printk(KERN_ERR "[arduino_cnc_driver] No se pudo asignar major number\n");
        return result;
    }

    cdev_init(&arduino_cdev, &fops);
    if (cdev_add(&arduino_cdev, dev_number, 1) == -1) {
        unregister_chrdev_region(dev_number, 1);
        return -1;
    }

    arduino_class = class_create(DRIVER_CLASS);

    if (IS_ERR(arduino_class)) {
        cdev_del(&arduino_cdev);
        unregister_chrdev_region(dev_number, 1);
        return PTR_ERR(arduino_class);
    }

    device_create(arduino_class, NULL, dev_number, NULL, "arduino_cnc");
    return 0;
}

static void arduino_disconnect(struct usb_interface *interface) {
    printk(KERN_INFO "[arduino_cnc_driver] Arduino desconectado\n");

    device_destroy(arduino_class, dev_number);
    class_destroy(arduino_class);
    cdev_del(&arduino_cdev);
    unregister_chrdev_region(dev_number, 1);

    connected_device = NULL;
    stored_interface = NULL;
}

static struct usb_device_id arduino_table[] = {
    { USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
    {} /* Termina */
};
MODULE_DEVICE_TABLE(usb, arduino_table);

static struct usb_driver arduino_usb_driver = {
    .name = DRIVER_NAME,
    .id_table = arduino_table,
    .probe = arduino_probe,
    .disconnect = arduino_disconnect,
};

static int __init arduino_init(void) {
    return usb_register(&arduino_usb_driver);
}

static void __exit arduino_exit(void) {
    usb_deregister(&arduino_usb_driver);
}

module_init(arduino_init);
module_exit(arduino_exit);
