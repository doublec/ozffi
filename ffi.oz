functor
import
   FFI(load:LOAD  bind:BIND unload:UNLOAD is:IS call:CALL)
   at 'ozffi.so{native}'
   Finalize(guardian)
export
   load : Load
   bind : Bind
   unload : UNLOAD
   is : IS
   call : CALL
define
   Register = {Finalize.guardian unload}
   proc {Load Name F}
      {LOAD Name F}
      {Register F}
   end

   fun {Bind F Name Type}
      NumArgs = {Width Type.2}
   in      
      if NumArgs == 0 then
	 proc {$ R}
	    Fun
	 in
	    {FFI.bind F Name Fun}
	    {FFI.call Fun Type nil R}
	 end
      elseif NumArgs == 1 then
	 proc {$ A R}
	    Fun
	 in
	    {FFI.bind F Name Fun}
	    {FFI.call Fun Type [ A ] R}
	 end
      elseif NumArgs == 2 then
	 proc {$ A B R}
	    Fun
	 in
	    {FFI.bind F Name Fun}
	    {FFI.call Fun Type [ A B ] R}
	 end
      elseif NumArgs == 3 then
	 proc {$ A B C R}
	    Fun
	 in
	    {FFI.bind F Name Fun}
	    {FFI.call Fun Type [ A B C ] R}
	 end
      end      	 
   end
end
