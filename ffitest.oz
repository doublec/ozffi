declare [FFI]={Module.link ['ffi.ozf']}
declare Std = {FFI.load nil}
declare GetEnv = {FFI.bind Std "getenv" stdcall(string getenv(string))}
{Browse {GetEnv "OZHOME"}}

declare Puts = {FFI.bind Std "puts" stdcall(uint32 getenv(string))}
{Browse {Puts "Hello World!"}}