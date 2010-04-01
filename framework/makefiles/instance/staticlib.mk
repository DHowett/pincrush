ifeq ($(FW_RULES_LOADED),)
include $(FW_MAKEDIR)/rules.mk
endif

.PHONY: internal-staticlib-all_ internal-staticlib-package_ internal-staticlib-compile

ifeq ($(FW_MAKE_PARALLEL_BUILDING), no)
internal-staticlib-all_:: $(FW_OBJ_DIR) $(FW_OBJ_DIR)/$(FW_INSTANCE).a
else
internal-staticlib-all_:: $(FW_OBJ_DIR)
	$(ECHO_NOTHING)$(MAKE) --no-print-directory --no-keep-going \
		internal-staticlib-compile \
		FW_TYPE=$(FW_TYPE) FW_INSTANCE=$(FW_INSTANCE) FW_OPERATION=compile \
		FW_BUILD_DIR="$(FW_BUILD_DIR)" _FW_MAKE_PARALLEL=yes$(ECHO_END)

internal-staticlib-compile: $(FW_OBJ_DIR)/$(FW_INSTANCE).a
endif

$(FW_OBJ_DIR)/$(FW_INSTANCE).a: $(OBJ_FILES_TO_LINK)
	$(ECHO_LINKING)$(TARGET_CXX) $(TARGET_LDFLAGS) -nostdlib -r -d -o $@ $^$(ECHO_END)


internal-staticlib-package_::
