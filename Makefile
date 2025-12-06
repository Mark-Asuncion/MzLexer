CCFLAGS=-Wextra -Wall -pedantic -std=c++17 -I./include/
LDFLAGS=
CC=g++
BUILD_DIR=./build
SOURCES=$(wildcard ./src/*.cpp)
LIB=$(wildcard ./lib/*.cpp)
TARGET=mzlex
TARGETLIB=libmzlex
PATHTARGETLIB=$(BUILD_DIR)/lib/libmzlex.a

.PHONY: compile_flags

all: compile_flags $(TARGET) $(TARGETLIB)

compile_flags:
	@if [ ! -f compile_flags.txt ]; then \
		echo "$(CCFLAGS) $(LDFLAGS)" | tr ' ' '\n' > compile_flags.txt; \
	fi

$(TARGETLIB): $(LIB)
	@mkdir -p $(BUILD_DIR)/lib
	$(CC) $(CCFLAGS) -c $(LIB) -o $(BUILD_DIR)/lib/$(TARGETLIB).o
	@ar rvs $(BUILD_DIR)/lib/$(TARGETLIB).a $(BUILD_DIR)/lib/$(TARGETLIB).o

$(TARGET): $(SOURCES) $(TARGETLIB)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CCFLAGS) $(SOURCES) $(PATHTARGETLIB) -o $(BUILD_DIR)/$(TARGET) $(LDFLAGS)
