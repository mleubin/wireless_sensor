## Makefile
##


#####  DEFINITIONS  #####

## directories
BIN_DIR = bin
LIB_DIR = lib
OBJ_DIR = obj
DEP_DIR = $(OBJ_DIR)/.dep
SRC_DIR = .

## install directories
INSTALL_BIN_DIR = /bin

## binary names
BINARY_NAME = main

## source files
SRC = $(wildcard  $(SRC_DIR)/*.c)
SRC += $(wildcard  $(SRC_DIR)/*.cpp)

## object files
OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(addsuffix .o, $(basename $(SRC)))))

## target binary
TARGET_BINARY = $(BIN_DIR)/$(BINARY_NAME)


#####  PROGRAMMS AND TOOLS  #####

## program definitions
TOOLCHAIN_PREFIX = #
CC = $(TOOLCHAIN_PREFIX)gcc
CXX = $(TOOLCHAIN_PREFIX)g++
MAKEDIR = mkdir -p
COPY = cp -f
REMOVE = rm -f
TOUCH = touch


#####  COMPILER OPTIONS  #####

## language standards
CSTD = -std=gnu99
CXXSTD = -std=c++11

## libraries
LIBS = 
#LIBS = -pthread
#LIBS += -lprussdrv

## warnings
WARNINGS = -Wall -Wextra -fmessage-length=0

## optimization (at max -O1 when when checking for memory leaks)
OPT = -O2 # -flto

## debug (at least -g when checking for memory leaks)
DEBUG = #-g3 -ggdb

## dependency file configurations
GENDEP = -MMD -MP -MF $(DEP_DIR)/$(@F).d

## misc C flags
COTHER = #
#COTHER += 

## misc C++ flags
CXXOTHER = -ffunction-sections -fdata-sections
CXXOTHER += -finline-functions -finline-functions-called-once

## C compiler flags
CFLAGS = $(WARNINGS) $(GENDEP) $(OPT) $(DEBUG) $(CSTD) $(DEFS) $(COTHER) $(INCS)

## C++ compiler flags
CXXFLAGS = $(WARNINGS) $(GENDEP) $(OPT) $(DEBUG) $(CXXSTD) $(DEFS) $(CXXOTHER) $(INCS)

## Linker options
LDFLAGS = $(LIBS)


#####  TARGETS  #####

## build targets
all:  $(BIN_DIR) $(TARGET_BINARY)

install: all
	$(COPY) $(TARGET_BINARY) $(INSTALL_BIN_DIR)
	chmod 4755 $(INSTALL_BIN_DIR)/$(BINARY_NAME)

uninstall:
	$(REMOVE) $(INSTALL_BIN_DIR)/$(BINARY_NAME)

$(TARGET_BINARY): $(filter-out $(OBJ_DIR)/rl_deamon.o $(OBJ_DIR)/rl_server.o, $(OBJ))
	@$(MAKEDIR) $(@D)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MAKEDIR) $(OBJ_DIR) $(DEP_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(MAKEDIR) $(OBJ_DIR) $(DEP_DIR)
	$(CXX) $(CXXFLAGS) -o $@ -c $<


## directories
$(OBJ_DIR):
	$(MAKEDIR) $(OBJ_DIR)

$(DEP_DIR):
	$(MAKEDIR) $(DEP_DIR)

$(BIN_DIR):
	$(MAKEDIR) $(BIN_DIR)


## phony targets
.PHONY: clean

clean:
	$(REMOVE) $(OBJ_DIR)/*.o $(DEP_DIR)/*.d

clear:
	$(TOUCH) *
	$(REMOVE) $(OBJ_DIR)/*.o $(DEP_DIR)/*.d $(BIN_DIR)/*

#####  INCLUDE DEPENDENCIES  #####
-include $(wildcard  $(DEP_DIR)/*.d)
