TOOL_NAME = pincrush
pincrush_C_FILES = pincrush.c
pincrush_CFLAGS = -I./libpng -Wno-format -std=c99 -DVERSION="\"$(shell grep Version "$(FW_PROJECT_DIR)/layout/DEBIAN/control" | cut -d' ' -f2)\""
pincrush_LDFLAGS = -lz
pincrush_SUBPROJECTS = libpng

include framework/makefiles/common.mk

ifeq ($(FW_TARGET_NAME)$(FW_PLATFORM_NAME),iphonemacosx)
export SDKVERSION=2.0
export TARGET_CC=$(SDKBINPATH)/gcc-4.0
export TARGET_CXX=$(SDKBINPATH)/gcc-4.0
else ifeq ($(FW_TARGET_NAME),macosx)
export ADDITIONAL_LDFLAGS+=-mmacosx-version-min=10.4
export ADDITIONAL_CFLAGS+=-mmacosx-version-min=10.4
export ARCHS=i386 ppc x86_64
else ifeq ($(FW_TARGET_NAME),linux)
export GO_EASY_ON_ME := 1
else ifeq ($(FW_TARGET_NAME),windows)
export GO_EASY_ON_ME := 1
pincrush_CFLAGS += -I./zlib
pincrush_LDFLAGS =
pincrush_SUBPROJECTS += zlib
endif

include framework/makefiles/tool.mk
