declare [FFI]={Module.link ['ffi.ozf']}
declare Std = {FFI.load nil}
declare GetEnv = {FFI.bind Std "getenv"}
{Browse {FFI.call GetEnv stdcall(string getenv(string)) ["OZHOME"]}}

declare Puts = {FFI.bind Std "puts"}
{Browse {FFI.call Puts stdcall(uint32 getenv(string)) ["OZHOME"]}}
