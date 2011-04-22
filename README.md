# Mozart/Oz wrapper for libffi

This is an experimental wrapper for libffi so I can access functions in shared libraries without writing wrappers in C. It's very simple at the moment and has only been tested on Linux.

There is a 'ffitest.oz' file which shows very simple example of usage, and can be tried line by line from the interactive environment. Here's some simple usage:

First load the module:

    declare [FFI]={Module.link ['ffi.ozf']}

This provides a few procedures. The first, 'load' takes a path or name for a shared library and loads it. It returns an extension object that needs to be passed to the other calls. The name of the shared library can be 'nil' to represent using a name from the shared libraries loaded by the current process. This is what the following line does:

    declare Std = {FFI.load nil}

The second procedure is 'bind'. This will take the extension object returned above, the name of a function, and a tuple describing the functions return types and arguments. It will return a procedure, that when called, will marshall types, call the C function, and return the relevant value. e.g:

    declare GetEnv = {FFI.bind Std "getenv" stdcall(string getenv(string))}
    {Browse {GetEnv "OZHOME}}

This will show the value of the OZHOME environment variable in the browser. Hopefully. I haven't decided on the final syntax of the tuple describing the arguments yet. At the moment it looks like:

    stdcall(<returntype> <ignored>(<argtype1> <argtype2> ...))

stdcall and <ignored> are ignored. Originally I was going to use them for the calling argument convention and the function name but ended up not doing this. The types can be any of:

* pointer - any C pointer type, from Oz it needs to be a foreignPointer
* string - a C char*, or Oz String
* uint8 - 8 bit unsigned integer from C, Number in Oz
* sint8 - 8 bit signed integer from C, Number in Oz
* uint16 - 16 bit unsigned integer from C, Number in Oz
* sint16 - 16 bit signed integer from C, Number in Oz
* uint32 - 32 bit unsigned integer from C, Number in Oz
* sint32 - 32 bit signed integer from C, Number in Oz
* uint64 - 64 bit unsigned integer from C, Number in Oz
* sint64 - 64 bit signed integer from C, Number in Oz
* float - float in C, float in Oz
* double - double in C, float in Oz

Another example:

    declare Pow = {FFI.bind Std "pow" stdcall(double pow(double double))}
    {Browse {Pow 2.0 6.0}}

This should show 64.0 in the browser.

The library has a number of limitations at the moment which I hope to fix. This first is that there is no support for defining and using C structures. I need to think of a good way of doing this. There are memory leaks when calling C functions due to be not correctly cleaning up copies of atoms, etc. There is a hard coded limit on the number of arguments. Currently it's 9.

My original announcement of the library on the mailing list and subsequent discussion can be [read here](http://lists.gforge.info.ucl.ac.be/pipermail/mozart-users/2008/009731.html).


