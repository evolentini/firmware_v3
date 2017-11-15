###############################################################################
#
# Copyright 2017, Esteban Volentini
# Copyright 2015, Mariano Cerdeiro
# All rights reserved.
#
# This file is part of CIAA Firmware.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
###############################################################################

###############################################################################
# get root dir
ROOT_DIR := .
# out dir
OUT_DIR = $(ROOT_DIR)/out
# object dir
OBJ_DIR = $(OUT_DIR)/obj
# lib dir
LIB_DIR = $(OUT_DIR)/lib
# bin dir
BIN_DIR = $(OUT_DIR)/bin
# rtos gen dir
GEN_DIR = $(OUT_DIR)/gen
# etc dir (configuration dir)
ETC_DIR = $(OUT_DIR)/etc

$(OUT_DIR):
	-mkdir -p $(OUT_DIR)

$(OBJ_DIR):
	-mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	-mkdir -p $(LIB_DIR)

$(BIN_DIR):
	-mkdir -p $(BIN_DIR)

$(GEN_DIR):
	-mkdir -p $(GEN_DIR)

$(ETC_DIR):
	-mkdir -p $(ETC_DIR)

directories: $(OUT_DIR) $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR) $(GEN_DIR) $(ETC_DIR)

###############################################################################
# include needed project

ifneq ($(PROJECT), )
    include $(PROJECT)/mak/makefile
    project_name ?= $(lastword $(subst /, , $(PROJECT)))
else
    PROJECT = .
    project_name ?= $(lastword $(subst /, , $(CURDIR)))
endif

# Defintion of project name based on project paths if it was not defined in 
# project makefile, used to define binary file and default target

project_src ?= $(PROJECT)/src

project_inc ?= $(PROJECT)/inc

# base module is always needed and included
ifneq ($(BOARD), )
    include boards/$(BOARD)/mak/makefile
endif

###############################################################################
# get root dir
base_module_path := $(ROOT_DIR)/modules/base/
base_external_path := $(ROOT_DIR)/externals/base/
base_search_c := src $(ARCH)/src $(ARCH)/$(CPUTYPE)/src/
base_search_h := inc $(ARCH)/inc $(ARCH)/$(CPUTYPE)/inc/

define base_find_header
    $(strip $(foreach path,$2,$(if $(wildcard $1$(path)/*.h),$1$(path),)))
endef
define base_find_code
    $(strip $(foreach path,$2,$(if $(wildcard $1$(path)/*.c),$1$(path),)))
endef

###############################################################################
# Search code and headers in modules/base
base_module_header_paths := $(strip $(call base_find_header,$(base_module_path),$(base_search_h)))
ifneq ($(base_module_header_paths), )
    $(info Native headers found at $(base_module_header_paths))
    project_inc += $(base_module_header_paths)
endif
base_module_code_paths := $(strip $(call base_find_code,$(base_module_path),$(base_search_c)))
ifneq ($(base_module_code_paths), )
    $(info Native code found at $(base_module_code_paths))
    project_src += $(base_module_code_paths)
endif
include $(base_module_path)mak/$(ARCH)/$(COMPILER)/makefile

###############################################################################
# Search code and headers in externals/base
base_external_header_paths := $(strip $(call base_find_header,$(base_external_path),$(base_search_h)))
ifneq ($(base_external_header_paths), )
    $(info External headers found at $(base_external_header_paths))
    project_inc += $(base_external_header_paths)
endif
base_external_code_paths := $(strip $(call base_find_code,$(base_external_path),$(base_search_c)))
ifneq ($(base_external_code_paths), )
    $(info External code found at $(base_external_code_paths))
    project_src += $(base_external_code_paths)
endif
include $(base_external_path)/mak/makefile

###############################################################################
# Export arquitecture variables
export ARCH
export CPUTYPE
export CPU
export COMPILER
export BOARD

###############################################################################
# Function to obtain the last directory of a path
define lastdir
$(strip $(lastword $(subst /, ,$1)))
endef

###############################################################################
# Function to obtain the second directory of a path
define secondir
$(strip $(word 2,$(subst /, ,$(subst ./, ,$1))))
endef

###############################################################################
# Function to generate the compilation parameters with the list of include paths
define includes
$(foreach file,$1,-I $(file))
endef

###############################################################################
# Function to generate the list of object files from a path list
define objects_list
$(foreach path,$1,$(patsubst $(path)/%.$2,$(OBJ_DIR)/%.o,$(wildcard $(path)/*.$2)))
endef

###############################################################################
# Function to chech module depdencies with other modules
define check_module_dependence
    ifeq ($(findstring $(subst ./,,$2),$(MODULES)),)
        $$(info Module $(subst ./,,$2) added by the dependence of $(subst ./,,$1) with him)
        MODULES += $2
        $(eval $(call enable_module,$2))
    endif
    $(call secondir,$1)_inc += $$($(call secondir,$2)_inc) 
endef
define module_dependencies
	$(foreach module,$2,$(call check_module_dependence,$(strip $1),$(module)))
endef
###############################################################################
# Includes the module makefile or adds it to compile from the main makefile
define enable_module
    ifneq ($$(wildcard $1/mak/makefile), )
        $$(info Full module found in $1)
        include $1/mak/makefile
    else
        ifneq ($$(wildcard $1/src/*.c), )
            $$(info Simple module found in $1)
            BUILD_LIBS += $(call secondir,$1)
            PATH_LIBS += $1
        else
            $$(warning Module in $1 not identified)
        endif
    endif
endef
define enable_modules
$(foreach path,$1,$(eval $(call enable_module,$(ROOT_DIR)/$(path))))
endef
$(eval $(call enable_modules,$(MODULES)))

###############################################################################
# Function to generate a compilation rule from a single path
define compile_rule
$$(OBJ_DIR)/%.o: $1/%.c
	@echo ' '
	@echo ===============================================================================
	$$(CC) $$(CFLAGS) $$(call includes,$2) $$< -o $$@
endef

###############################################################################
# Function to generate a assemble rule from a single path
define assembler_rule
$$(OBJ_DIR)/%.o: $1/%.s
	@echo ' '
	@echo ===============================================================================
	$$(AS) $$(AFLAGS) $$(call includes,$2) $$< -o $$@
endef

###############################################################################
# Generate rules to build all libraries
define library_rule
#    $$(info $n$1:$$($1_inc),$$($1_src))
    $1_inc ?= $(foreach path,$2,$(strip $(path))/inc)
    $1_src ?= $(foreach path,$2,$(strip $(path))/src)
#    $$(info $n$1:$$($1_inc),$$($1_src))
    $1_objects = $$($1_obj) $$(call objects_list,$$($1_src),c)
    $(foreach path,$($1_src),$(eval $(call compile_rule,$(path),$$(project_inc))))
    $1_objects += $$(call objects_list,$$($1_asm),s)
    $(foreach path,$($1_asm),$(eval $(call assembler_rule,$(path),$$(project_inc))))
$$(LIB_DIR)/$1.a: $$($1_objects)
	@echo ' '
	@echo ===============================================================================
	@echo Creating library $1
	@echo $(lpc43xx_inc)
	$$(AR) -crs $$@ $$?
endef
define libraries_rules
    $(call library_rule,$1,$(foreach path,$(PATH_LIBS),$(if $(findstring $1,$(path)),$(path),)))
endef

###############################################################################
# Generate rule to build project target
define modules_list
    $(foreach module,$(MODULES),$(strip $(call secondir,$(module))))
endef
define module_includes
    project_inc += $($1_inc)
endef
define libraries_files
    $(foreach file,$(sort $(BUILD_LIBS) $(USE_LIBS)),$(LIB_DIR)/$(file).a)
endef
define project_rule
    project_objects = $(call objects_list,$(project_src),c)
    $(foreach path,$(project_src),$(eval $(call compile_rule,$(path),$$(project_inc))))

    TARGET_NAME ?= $(BIN_DIR)/$(project_name)
    LD_TARGET = $(TARGET_NAME).$(LD_EXTENSION)
    lib_params = -Xlinker --start-group $(strip $(call libraries_files)) -Xlinker --end-group
$$(project_name): directories $(call libraries_files) $(project_objects) 
	@echo ' '
	@echo ===============================================================================
	@echo Linking file: $(LD_TARGET)
	@echo ' '
	$(CC) $(LFLAGS) $(project_objects) $(lib_params) -o $(LD_TARGET) 
	@echo ' '
	@echo ===============================================================================
	@echo Post Building $(project_name)
	@echo ' '
endef
#	$(POST_BUILD)

.DEFAULT_GOAL := $(project_name)

###############################################################################
# Generate rules to build all libraries
$(foreach lib,$(BUILD_LIBS),$(eval $(call libraries_rules,$(lib))))

###############################################################################
# Generate rule to build project target
$(foreach module,$(call modules_list),$(eval $(call module_includes,$(module))))

###############################################################################
# Generate rule to build project target
    project_objects = $(call objects_list,$(project_src),c)
    $(foreach path,$(project_src),$(eval $(call compile_rule,$(path),$$(project_inc))))

    TARGET_NAME ?= $(BIN_DIR)/$(project_name)
    LD_TARGET = $(TARGET_NAME).$(LD_EXTENSION)
    lib_params = -Xlinker --start-group $(strip $(call libraries_files)) -Xlinker --end-group
$(project_name): $(call libraries_files) $(project_objects) 
	@echo ' '
	@echo ===============================================================================
	@echo Linking file: $(LD_TARGET)
	@echo ' '
	$(CC) $(LFLAGS) $(project_objects) $(lib_params) -o $(LD_TARGET) 
	@echo ' '
	@echo ===============================================================================
	@echo Post Building $(project_name)
	@echo ' '

###############################################################################
# information
info:
	@echo "+-----------------------------------------------------------------------------+"
	@echo "|               Enable Config Info                                            |"
	@echo "+-----------------------------------------------------------------------------+"
	@echo Project Path.......: $(PROJECT)
	@echo Project Name.......: $(project_name)
	@echo Target Info........: $(BOARD)/$(ARCH)/$(CPUTYPE)/$(CPU)
	@echo Enabled modules....: $(MODULES)
	@echo Libraries..........: $(BUILD_LIBS)
	@echo libraris with srcs.: $(PATH_LIBS)
	@echo RTOS config........: $(POIL_FILES) $(OIL_FILES)
#	@echo Lib Src dirs.......: $(LIBS_SRC_DIRS)
#	@echo Lib Src Files......: $(LIBS_SRC_FILES)
#	@echo Lib Obj Files......: $(LIBS_OBJ_FILES)
#	@echo Project Src Path...: $($(project_name)_SRC_PATH)
	@echo Includes...........: $(INCLUDE)
	@echo use make info_\<mod\>: to get information of a specific module. eg: make info_posix
	@echo "+-----------------------------------------------------------------------------+"
	@echo "|               CIAA Firmware Info                                            |"
	@echo "+-----------------------------------------------------------------------------+"
	@echo CIAA Firmware ver..: $(CIAA_FIRMWARE_VER)
	@echo Available modules..: $(ALL_MODS)
	@echo "+-----------------------------------------------------------------------------+"
	@echo "|               Compiler Info                                                 |"
	@echo "+-----------------------------------------------------------------------------+"
	@echo Compiler...........: $(COMPILER)
	@echo CC.................: $(CC)
	@echo AR.................: $(AR)
	@echo LD.................: $(LD)
	@echo GE.................: $(GE)
	@echo Compile C Flags....: $(CFLAGS)
	@echo Compile ASM Flags..: $(AFLAGS)
	@echo Target Name........: $(TARGET_NAME)
	@echo Src Files..........: $(SRC_FILES)
	@echo Obj Files..........: $(OBJ_FILES)
	@echo Linker Flags.......: $(LFLAGS)
	@echo Linker Extension...: $(LD_EXTENSION)
	@echo Linker Target......: $(LD_TARGET)

