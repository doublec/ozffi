oztool c++ -c ozffi.cpp -o ozffi.o
oztool ld ozffi.o -o ozffi.so-`oztool platform` -lffi

