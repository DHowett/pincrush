TOOL_NAME = pincrush
export TARGET = native
pincrush_CC_FILES = pincrush.cc
pincrush_CCFLAGS = -I./libpng -Wno-format
pincrush_OBJ_FILES = libpng/$(FW_OBJ_DIR_NAME)/png.a zlib/$(FW_OBJ_DIR_NAME)/z.a
SUBPROJECTS = libpng zlib

include framework/makefiles/common.mk
include framework/makefiles/aggregate.mk
include framework/makefiles/tool.mk
