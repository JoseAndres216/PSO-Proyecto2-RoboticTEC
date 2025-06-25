# Driver kernel Makefile con estilo gráfico

DRIVER = arduino_cnc_driver
OBJ = $(DRIVER).o

GRAY = \033[38;5;250m
GREEN = \033[1;32m
RED = \033[0;31m
NC = \033[0m

BOLD = \033[1m
NC = \033[0m

obj-m += $(OBJ)

.PHONY: all clean insert remove

all:
	@echo "${BOLD}Compilando driver del kernel...${NC}${GRAY}"
	@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	@echo "${BOLD}${GREEN}Driver compilado exitosamente.${NC}\n"

insert: all
	@echo "${BOLD}Insertando el módulo...${NC}${GRAY}"
	@sudo insmod $(DRIVER).ko && echo "${BOLD}${GREEN}Módulo insertado.${NC}\n" || echo "${RED}Error al insertar el módulo.${NC}"

remove:
	@echo "${BOLD}Removiendo el módulo...${NC}${GRAY}"
	@sudo rmmod $(DRIVER) && echo "${BOLD}${GREEN}Módulo removido.${NC}\n" || echo "${RED}Error al remover el módulo.${NC}"

clean:
	@echo "${BOLD}Limpiando archivos generados...${NC}${GRAY}"
	@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	@echo "${BOLD}${GREEN}Limpieza completada.${NC}\n"
