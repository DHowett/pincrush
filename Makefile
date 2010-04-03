TOOL_NAME = pincrush
pincrush_C_FILES = pincrush.c
pincrush_CFLAGS = -I./libpng -Wno-format -std=c99 -DVERSION="\"$(shell grep Version "$(FW_PROJECT_DIR)/layout/DEBIAN/control" | cut -d' ' -f2)\""
pincrush_LDFLAGS = -lz
pincrush_OBJ_FILES = libpng/$(FW_OBJ_DIR_NAME)/png.a
SUBPROJECTS = libpng

include framework/makefiles/common.mk
include framework/makefiles/aggregate.mk
include framework/makefiles/tool.mk

ifeq ($(FW_TARGET_NAME),macosx)
export ADDITIONAL_LDFLAGS+=-mmacosx-version-min=10.4
export ADDITIONAL_CFLAGS+=-mmacosx-version-min=10.4
export ARCHS=i386 ppc x86_64
else ifeq ($(FW_TARGET_NAME),linux)
export GO_EASY_ON_ME := 1
else ifeq ($(FW_TARGET_NAME),windows)
export GO_EASY_ON_ME := 1
endif
