declare
[FFI]={Module.link ['ozffi.so{native}']}

declare GetEnv = {FFI.findFunction nil 'getenv'}
{Browse GetEnv}
declare Puts = {FFI.findFunction nil 'puts'}
{Browse Puts}
{Browse {FFI.call GetEnv stdcall(string getenv(string)) ["OZHOME"]}}
{Browse {FFI.call Puts stdcall(uint32 getenv(string)) ["OZHOME"]}}
{Browse "hello"}
{Browse [ 1 2 3 ] }