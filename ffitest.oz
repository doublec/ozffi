declare [FFI]={Module.link ['ffi.ozf']}
declare Std = {FFI.load nil}
declare GetEnv = {FFI.bind Std "getenv" string [string]}
{Browse {GetEnv "OZHOME"}}

declare Puts = {FFI.bind Std "puts" uint32 [string]}
{Browse {Puts "Hello World!"}}