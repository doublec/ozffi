DLL=ozffi.so-$(shell oztool platform)

.PHONY: all
all: $(DLL) ffi.ozf CurlExample

ozffi.o: ozffi.cpp
	oztool c++ -c $< -o $@

$(DLL): ozffi.o
	oztool ld $< -o $(DLL) `pkg-config --libs libffi`

ffi.ozf: ffi.oz
	ozc -c $< -o $@

CurlExample.oza: CurlExample.oz
	ozc -c $< -o $@

CurlExample: CurlExample.oza
	ozl -x $< -o $@

.PHONY: clean
clean:
	-rm $(DLL)
	-rm ozffi.o
	-rm ffi.ozf
	-rm CurlExample
	-rm CurlExample.oza
	-rm *~
