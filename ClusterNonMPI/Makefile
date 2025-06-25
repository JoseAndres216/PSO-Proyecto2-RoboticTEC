CC = gcc
TARGET = ClusterNonMPI
SOURCE = ClusterNonMPI.c

GRAY = \033[38;5;250m
GREEN = \033[1;32m
RED = \033[0;31m
BOLD = \033[1m
NC = \033[0m

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCE)
	@echo "${BOLD}Compiling file processor...${NC}${GRAY}"
	$(CC) $(SOURCE) -o $(TARGET)
	@echo "${BOLD}${GREEN}Compilation finished successfully.${NC}\n"

run: $(TARGET)
	@echo "${BOLD}Running file processor...${NC}${GRAY}"
	./$(TARGET)
	@echo "${BOLD}${GREEN}Execution completed.${NC}\n"

clean:
	@echo "${BOLD}Cleaning up...${NC}${GRAY}"
	rm -f $(TARGET)
	@echo "${BOLD}${GREEN}Cleaned successfully.${NC}\n"