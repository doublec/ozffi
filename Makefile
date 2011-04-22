DLL=ozffi.so-$(shell oztool platform)

.PHONY: all
all: $(DLL) ffi.ozf

ozffi.o: ozffi.cpp
	oztool c++ -c $< -o $@

$(DLL): ozffi.o
	oztool ld $< -o $(DLL) `pkg-config --libs libffi`

ffi.ozf: ffi.oz
	ozc -c $< -o $@

.PHONY: clean
clean:
	-rm $(DLL)
	-rm ozffi.o
	-rm ffi.ozf
