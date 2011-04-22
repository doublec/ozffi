functor
import
  Module
  Application
define
  [FFI] = {Module.link ['ffi.ozf']}
  Curl = {FFI.load "libcurl.so"}
  EasyInit = {FFI.bind Curl "curl_easy_init" pointer nil}
  EasyCleanup = {FFI.bind Curl "curl_easy_cleanup" void [pointer]}
  EasySetString = {FFI.bind Curl "curl_easy_setopt" uint32 [pointer uint32 string]}
  EasyPerform = {FFI.bind Curl "curl_easy_perform" uint32 [pointer]}
  C = {EasyInit}
  _ = {EasySetString C 10002 "http://www.mozart-oz.org/"}
  _ = {EasyPerform C}
  _ = {EasyCleanup C}
in
  {Application.exit 0}
end
