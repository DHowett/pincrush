LIBRARY_NAME := $(strip $(LIBRARY_NAME))

ifeq ($(FW_RULES_LOADED),)
include $(FW_MAKEDIR)/rules.mk
endif

internal-all:: $(LIBRARY_NAME:=.all.staticlib.variables);

internal-package:: $(LIBRARY_NAME:=.package.staticlib.variables);

LIBRARYS_WITH_SUBPROJECTS = $(strip $(foreach staticlib,$(LIBRARY_NAME),$(patsubst %,$(staticlib),$($(staticlib)_SUBPROJECTS))))
ifneq ($(LIBRARYS_WITH_SUBPROJECTS),)
internal-clean:: $(LIBRARYS_WITH_SUBPROJECTS:=.clean.staticlib.subprojects)
endif

$(LIBRARY_NAME):
	@$(MAKE) -f $(MAKEFILE_NAME) --no-print-directory --no-keep-going $@.all.staticlib.variables
