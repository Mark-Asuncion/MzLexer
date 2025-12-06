CCFLAGS=-Wextra -Wall -pedantic -std=c++17 -I./include/
LDFLAGS=
CC=g++
BUILD_DIR=./build
SOURCES=$(wildcard ./src/*.cpp)
APPNAME=mzlex

.PHONY: compile_flags

all: compile_flags

compile_flags:
	@if [ ! -f compile_flags.txt ]; then \
		echo "$(CCFLAGS) $(LDFLAGS)" | tr ' ' '\n' > compile_flags.txt; \
	fi

debug: all
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CCFLAGS) $(SOURCES) $(RES) -o $(BUILD_DIR)/$(APPNAME) $(LDFLAGS)

run:
	@$(BUILD_DIR)/$(APPNAME)
