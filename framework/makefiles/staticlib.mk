ifeq ($(FW_INSTANCE),)
	include $(FW_MAKEDIR)/master/staticlib.mk
else
	ifeq ($(FW_TYPE),staticlib)
		include $(FW_MAKEDIR)/instance/staticlib.mk
	endif
endif
