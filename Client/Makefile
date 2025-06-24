CC = gcc
TARGET = client
SOURCE = Client.c
PORT = 8080
IP = 127.0.0.1

GRAY = \033[38;5;250m
GREEN = \033[1;32m
RED = \033[0;31m
NC = \033[0m

BOLD = \033[1m
NC = \033[0m

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	@echo "${BOLD}Compiling client...${NC}${GRAY}"
	$(CC) $(SOURCE) -o $(TARGET)
	@echo "${BOLD}${GREEN}Client compiled successfully.${NC}\n"

run: $(TARGET)
	@echo "${BOLD}Running client (connect to ${IP}:${PORT})...${NC}${GRAY}"
	@if [ -z "$(FILE)" ]; then \
		echo "${RED}Error: Specify file to send with 'make run FILE=path/to/file'.${NC}"; \
		exit 1; \
	else \
		./$(TARGET) $(IP) $(FILE); \
	fi
	@echo "${BOLD}${GREEN}Client execution completed.${NC}\n"

clean:
	@echo "${BOLD}Cleaning generated files...${NC}${GRAY}"
	rm -f $(TARGET)
	@echo "${BOLD}${GREEN}Generated files removed successfully.${NC}\n"