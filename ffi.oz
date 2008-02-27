% Copyright (C) 2007 Chris Double.
% 
% Redistribution and use in source and binary forms, with or without
% modification, are permitted provided that the following conditions are met:
% 
% 1. Redistributions of source code must retain the above copyright notice,
%    this list of conditions and the following disclaimer.
% 
% 2. Redistributions in binary form must reproduce the above copyright notice,
%    this list of conditions and the following disclaimer in the documentation
%    and/or other materials provided with the distribution.
% 
% THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
% INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
% FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
% DEVELOPERS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
% SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
% PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
% OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
% WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
% OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
% ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
%
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
