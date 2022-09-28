.PHONY: all target_lib target_example target_tools
#target_example target_tools
all: target_example target_tools target_lib

# Values definition
ROOT_DIR := $(abspath .)
SCRIPT_DIR := $(ROOT_DIR)/scripts
EXAMPLE_DIR := $(ROOT_DIR)/example
TOOLS_DIR := $(ROOT_DIR)/tools
SRC_DIRS := $(ROOT_DIR)/src
DRV_DIRS := $(ROOT_DIR)/src/jl61xx
SRC_DIRS += $(DRV_DIRS)
INC_DIR := $(ROOT_DIR)/inc
DRIVER_DIR := $(ROOT_DIR)/src/driver
PORTABLE_DIR := $(ROOT_DIR)/src/portable
SRC_FILES := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))

# Function definition
# Upper code translation
UC = $(shell echo '$1' | tr '[:lower:]' '[:upper:]')

# Built objects
obj-y :=
sobj-y :=

# Build options
CFLAGS :=
CFLAGS_SO :=
LDFLAGS :=
LDFLAGS_SO :=
INCLUDE :=
INCLUDE += -I$(INC_DIR)
CFLAGS += -fgnu89-inline -Wall -Werror -Wno-error=deprecated-declarations -Wextra $(INCLUDE)
# CFLAGS += -ffunction-sections -fdata-sections
# LDFLAGS += -Wl,-gc-sections



#Build procedure
$(if $(wildcard $(ROOT_DIR)/.config),, $(error No .config exists!))
$(if $(wildcard $(SCRIPT_DIR)/Makefile.conf),, $(error No Makefile.conf exists!))
$(if $(wildcard $(SCRIPT_DIR)/Makefile.userconf),, $(error No Makefile.userconf exists!))
$(if $(wildcard $(SCRIPT_DIR)/Makefile.build),, $(error No Makefile.build exists!))
$(if $(wildcard $(SCRIPT_DIR)/Makefile.libs),, $(error No Makefile.libs exists!))
include .config
include $(SCRIPT_DIR)/Makefile.conf
include $(SCRIPT_DIR)/Makefile.userconf
include $(SCRIPT_DIR)/Makefile.build
include $(SCRIPT_DIR)/Makefile.libs

export CC ROOT_DIR CONFIG_OS_LINUX CONFIG_OS_WIN TARGET_STATIC_LIB CFLAGS LDFLAGS

target_example: target_lib
	$(MAKE) -C $(EXAMPLE_DIR)

target_tools: target_lib
	$(MAKE) -C $(TOOLS_DIR)

clean: libs_clean
	$(MAKE) clean -C $(EXAMPLE_DIR)
	$(MAKE) clean -C $(TOOLS_DIR)

