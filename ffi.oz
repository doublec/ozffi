functor
import
   FFI(load:LOAD  bind:BIND unload:UNLOAD is:IS call:CALL)
   at 'ozffi.so{native}'
   Finalize(guardian)
export
   load : Load
   bind : BIND
   unload : UNLOAD
   is : IS
   call : CALL
define
   Register = {Finalize.guardian unload}
   proc {Load Name F}
      {LOAD Name F}
      {Register F}
   end
end
