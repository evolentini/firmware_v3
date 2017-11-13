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
    base_inc += $(base_module_header_paths)
endif
base_module_code_paths := $(strip $(call base_find_code,$(base_module_path),$(base_search_c)))
ifneq ($(base_module_code_paths), )
    $(info Native code found at $(base_module_code_paths))
    base_src += $(base_module_code_paths)
endif
include $(base_module_path)mak/$(ARCH)/$(COMPILER)/makefile

###############################################################################
# Search code and headers in externals/base
base_external_header_paths := $(strip $(call base_find_header,$(base_external_path),$(base_search_h)))
ifneq ($(base_external_header_paths), )
    $(info External headers found at $(base_external_header_paths))
    base_inc += $(base_external_header_paths)
endif
base_external_code_paths := $(strip $(call base_find_code,$(base_external_path),$(base_search_c)))
ifneq ($(base_external_code_paths), )
    $(info External code found at $(base_external_code_paths))
    base_src += $(base_external_code_paths)
endif
include $(base_external_path)/mak/makefile

###############################################################################
# Export arquitecture variables
export ARCH
export CPUTYPE
export CPU
export COMPILER
export BOARD
