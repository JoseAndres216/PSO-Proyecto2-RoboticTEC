CC = gcc
TARGET = server
SOURCE = Server.c
PORT = 8080

GRAY = \033[38;5;250m
GREEN = \033[1;32m
RED = \033[0;31m
NC = \033[0m

BOLD = \033[1m
NC = \033[0m

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	@echo "${BOLD}Compiling server...${NC}${GRAY}"
	$(CC) $(SOURCE) -o $(TARGET)
	@echo "${BOLD}${GREEN}Server compiled successfully.${NC}\n"

run: $(TARGET)
	@echo "${BOLD}Starting server on port ${PORT}...${NC}${GRAY}"
	./$(TARGET)
	@echo "${BOLD}${GREEN}Server stopped.${NC}\n"

clean:
	@echo "${BOLD}Cleaning generated files...${NC}${GRAY}"
	rm -f $(TARGET)
	@echo "${BOLD}${GREEN}Generated files removed successfully.${NC}\n"