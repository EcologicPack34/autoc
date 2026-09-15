# general-makefile
#
# Copyright (c) 2025 Daniel Gómez Rodríguez
#
# This Makefile is licensed under the MIT License.
# See LICENSE file for details.
# SPDX-License-Identifier: MIT

#===== VARIABLES - Config =======
COMPILER := g++


CFLAGS := -Wall -Werror -pedantic-errors
#debug
CFLAGS += -ggdb


EXE_NAME := autoc

#source, object and include directories
#ALL DIRECTORIES SHOULD BE LIMITED TO JUST ONE
SRC_DIR := src
OBJ_DIR := obj
INCLUDE_DIR := include

SRC_EXTENSION = .cpp

#directory for compiled libraries
LIB_DIR := lib

#External libraries: not in lib folder
#Write here a list with the name of all libs (Ex: if lib is libscreen -> write: screen)
EXT_LIBS :=

BUILD_DIR := build

RUN_ARGS ?= 

#other variables
DIRS = $(INCLUDE_DIR) $(SRC_DIR) $(OBJ_DIR) $(LIB_DIR) $(BUILD_DIR) docs

#===== INSTALL =====

INSTALL_DIR_PREFIX ?= $$HOME/.local
INSTALL_BIN_DIR := $(INSTALL_DIR_PREFIX)/bin


#====== SOURCES =======

#Search source files and create paths for objects and dependencies
SRC := $(if $(wildcard $(SRC_DIR)), $(shell find $(SRC_DIR) -name "*$(SRC_EXTENSION)" -printf '%T@ %p\n' | sort -nr | cut -d' ' -f2-))
OBJ := $(SRC:$(SRC_DIR)/%$(SRC_EXTENSION)=$(OBJ_DIR)/%.o)
DEPS := $(OBJ:.o=.d)

#Search library files and create compiler flags for them
STATIC_LIBS := $(if $(wildcard $(LIB_DIR)), $(shell find $(LIB_DIR) -name "*.a"))
#SHARED_LIBS := $(shell find $(LIB_DIR) -name "*.so")

LIB_PATHS := $(addprefix -L,$(sort $(dir $(STATIC_LIBS) ))) #$(SHARED_LIBS))))

LIB_NAMES := $(notdir $(STATIC_LIBS)) # $(notdir $(SHARED_LIBS))
LIB_NAMES := $(LIB_NAMES:lib%.a=%) $(EXT_LIBS)
#LIB_NAMES := $(LIB_NAMES:lib%.so=%)
LIB_NAMES := $(addprefix -l, $(LIB_NAMES))

#generate include paths for compiler
INCLUDES := $(if $(wildcard $(INCLUDE_DIR)), $(addprefix -I,$(shell find $(INCLUDE_DIR) -type d)))

#====== RULES ======

## build: Compiles the project
build: $(BUILD_DIR)/$(EXE_NAME) 
## build_clean: Compiles the project deleting first previous executables and object files
build_clean: clean_all build

$(BUILD_DIR)/$(EXE_NAME): $(OBJ)
	$(COMPILER) $(CFLAGS) $(INCLUDES) $^ $(LIB_PATHS) $(LIB_NAMES) -o $(BUILD_DIR)/$(EXE_NAME) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%$(SRC_EXTENSION)
	@mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@


## init: Initializes project structure and creates a main file
init:
	@if [ ! -f "$(SRC_DIR)/main.$(SRC_EXTENSION)" ]; then \
		echo "Initializing project..."; \
		mkdir -p $(DIRS); \
		printf '#include <stdio.h>\n\nint main() {\n\tprintf("Hello, World!\\n");\n\n\treturn 0;\n}\n' > ./$(SRC_DIR)/main$(SRC_EXTENSION); \
		echo "Project Initialized"; \
	else \
		echo "Project already initialized. Skipping."; \
	fi

## delete_all: Removes all files in current dir except for the makefile
delete_all:
	@printf "Delete all directories?(DELETES ALL PROYECT FILES) (y/N): "; \
	read ans; \
	if [ "$$ans" = "y" ] || [ "$$ans" = "Y" ]; then \
		rm -rf $(DIRS); \
		printf "Removed all files\n"; \
	else \
		echo "Aborted."; \
	fi

## clean: Deletes all object files and dependency files
clean:
	@test -d $(OBJ_DIR) && find $(OBJ_DIR) -mindepth 1 -delete

## clean_exe: Deletes excutable
clean_exe:
	@test -f $(BUILD_DIR)/$(EXE_NAME) && rm $(BUILD_DIR)/$(EXE_NAME)

## clean_all: Deletes excutable, object files and dependency files
clean_all: clean clean_exe

## run: Runs the program without arguments
run:
	@printf "Running app...\n"
	./$(BUILD_DIR)/$(EXE_NAME)

## run_args: Runs the program using arguments espcified in RUN_ARGS variable
run_args: 
	@printf "Running app with args $(RUN_ARGS)...\n"
	./$(BUILD_DIR)/$(EXE_NAME) $(RUN_ARGS)

## install: Compiles and installs app to specified directory
install: build_clean
	@if [ ! -f "$(INSTALL_BIN_DIR)/$(EXE_NAME)" ]; then \
		echo "Installing $(EXE_NAME) to $(INSTALL_BIN_DIR)"; \
		install -d $(INSTALL_BIN_DIR); \
		install -m 755 $(BUILD_DIR)/$(EXE_NAME) $(INSTALL_BIN_DIR); \
	else \
		echo "$(EXE_NAME) already installed"; \
	fi

## install_path: Adds path to .bashrc file.
install_path:
	@grep -qxF 'export PATH="$(INSTALL_BIN_DIR):$$PATH"' $$HOME/.bashrc || \
	echo 'export PATH="$(INSTALL_BIN_DIR):$$PATH"' >> $$HOME/.bashrc
	@echo "Added to ~/.bashrc. Reload with: source ~/.bashrc"

## uninstall: Removes app from specified directory
uninstall: 
	@echo "Removing $(INSTALL_BIN_DIR)/$(EXE_NAME)"
	@rm -rf $(INSTALL_BIN_DIR)

## help: Show this help message
help:
	@echo "Available targets:"
	@grep -h '^##' $(MAKEFILE_LIST) | sed 's/^## /\t- /'

#=== DEPS ===
-include $(DEPS)

.PHONY: init delete_all clean clean_exe clean_all run run_args help install install_path uninstall