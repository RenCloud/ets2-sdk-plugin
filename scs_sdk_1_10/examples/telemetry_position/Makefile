SDK_HEADERS=\
	../../include/*.h \
	../../include/common/*.h \
	../../include/amtrucks/*.h \
	../../include/eurotrucks2/*.h

SDK_INCLUDES=\
	-I../../include \
	-I../../include/common/ \
	-I../../include/amtrucks/ \
	-I../../include/eurotrucks2

UNAME:= $(shell uname -s)

ifeq ($(UNAME),Darwin)
LIB_NAME_OPTION=-install_name
else
LIB_NAME_OPTION=-soname
endif

telemetry_position.so:  *.cpp $(SDK_HEADERS)
	gcc -o $@ -fPIC -Wall --shared -Wl,$(LIB_NAME_OPTION),$@ $(SDK_INCLUDES) *.cpp

.PHONY: clean
clean:
	@rm -f -- *.so
