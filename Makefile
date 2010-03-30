TOOL_NAME = pincrush
export TARGET = native
pincrush_CC_FILES = pincrush.cc
pincrush_CCFLAGS = -I./libpng-theos -Wno-format
pincrush_LDFLAGS = -lz #-L/usr/X11/lib -lpng -lz
pincrush_OBJ_FILES = libpng-theos/$(FW_OBJ_DIR_NAME)/png.a
SUBPROJECTS = libpng-theos

include framework/makefiles/common.mk
include framework/makefiles/aggregate.mk
include framework/makefiles/tool.mk
