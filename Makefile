TOOL_NAME = pincrush
pincrush_C_FILES = pincrush.c
pincrush_CFLAGS = -I./libpng -Wno-format -std=c99 -DVERSION="\"$(shell grep Version "$(FW_PROJECT_DIR)/layout/DEBIAN/control" | cut -d' ' -f2)\""
pincrush_LDFLAGS = -lz
pincrush_OBJ_FILES = libpng/$(FW_OBJ_DIR_NAME)/png.a
SUBPROJECTS = libpng

include framework/makefiles/common.mk
include framework/makefiles/aggregate.mk
include framework/makefiles/tool.mk
