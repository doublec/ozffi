#include "mozart.h"
#include <ffi.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dlfcn.h>

class FFI : public OZ_Extension {
public:
  static int id;
  void* handle;

  FFI() : handle(0) { }
  bool load(char const* name);
  void* bind(char const* name);
  bool unload();

  // OZ_Extension requirements
  virtual int getIdV();
  virtual OZ_Extension* gCollectV();
  virtual OZ_Extension* sCloneV();
  virtual void gCollectRecurseV();
  virtual void sCloneRecurseV();
  virtual OZ_Term typeV();
};

int FFI::id;

bool FFI::load(char const* name)
{
  handle = dlopen(name, RTLD_NOW | RTLD_GLOBAL);
  return handle;
}

void* FFI::bind(char const* name)
{
  if(!handle)
    return 0;

  void* func = dlsym(handle, name);
  if(!func)
    return 0;

  return func;
}

bool FFI::unload()
{
  if(handle) {
    dlclose(handle);
    handle = 0;
  }   
}

int FFI::getIdV()
{
  return id;
}

OZ_Extension* FFI::gCollectV()
{
  return new FFI(*this);
}

OZ_Extension* FFI::sCloneV()
{
  return 0;
}

void FFI::gCollectRecurseV()
{
}

void FFI::sCloneRecurseV()
{
}

OZ_Term FFI::typeV()
{
  return OZ_atom("FFI");
}


OZ_Boolean OZ_isFFI(OZ_Term t)
{
  t = OZ_deref(t);
  return OZ_isExtension(t) &&
    OZ_getExtension(t)->getIdV()==FFI::id;
}

FFI* OZ_FFIToC(OZ_Term t)
{
  return (FFI*)OZ_getExtension(OZ_deref(t));
}

#define OZ_declareFFI(ARG, VAR) \
  OZ_declareType(ARG,VAR,FFI*,"FFI",OZ_isFFI,OZ_FFIToC)

OZ_BI_define(FFI_load,1,1)
{
  OZ_declareDetTerm(0, Name);
  FFI* ffi = new FFI();
  int len = 0;
  if(ffi->load(OZ_isNil(Name) ? 0 : OZ_stringToC(Name, &len))) {
    OZ_RETURN(OZ_extension(ffi));
  }
  else {
    delete ffi;
    return OZ_FAILED;
  }
}
OZ_BI_end

OZ_BI_define(FFI_bind,2,1)
{
  OZ_declareFFI(0, ffi);
  OZ_declareDetTerm(1, Name);

  int len = 0;
  void* func = ffi->bind(OZ_stringToC(Name, &len));
  if(!func)
    return OZ_FAILED;

  OZ_out(0) = OZ_makeForeignPointer(func);
  return OZ_ENTAILED;
}
OZ_BI_end

OZ_BI_define(FFI_unload,1,0)
{
  OZ_declareFFI(0, ffi);
  ffi->unload();
  return PROCEED;
}
OZ_BI_end

OZ_BI_define(FFI_is,1,1)
{
  OZ_declareDetTerm(0,t);
  OZ_RETURN_BOOL(OZ_isFFI(t));
}
OZ_BI_end

ffi_type* getFFIType(char const* name) 
{
  if(strcmp(name, "pointer") == 0)
    return &ffi_type_pointer;
  else if(strcmp(name, "string") == 0)
    return &ffi_type_pointer;
  else if(strcmp(name, "void") == 0)
    return &ffi_type_void;
  else if(strcmp(name, "uint8") == 0)
    return &ffi_type_uint8;
  else if(strcmp(name, "sint8") == 0)
    return &ffi_type_sint8;
  else if(strcmp(name, "uint16") == 0)
    return &ffi_type_uint16;
  else if(strcmp(name, "sint16") == 0)
    return &ffi_type_sint16;
  else if(strcmp(name, "uint32") == 0)
    return &ffi_type_uint32;
  else if(strcmp(name, "sint32") == 0)
    return &ffi_type_sint32;
  else if(strcmp(name, "uint64") == 0)
    return &ffi_type_uint64;
  else if(strcmp(name, "sint64") == 0)
    return &ffi_type_sint64;
  else if(strcmp(name, "float") == 0)
    return &ffi_type_float;
  else if(strcmp(name, "double") == 0)
    return &ffi_type_double;
  else
    return 0;
}

union Value
{
  void* p;
  unsigned char  u8;
  signed char s8;
  unsigned short u16;
  signed short s16;
  unsigned long  u32;
  signed long s32;
  signed long long  u64;
  unsigned long long s64;
  float f;
  double d;
};

void* getFFIValue(char const* type, OZ_Term Term)
{
  Value* v = (Value*)calloc(sizeof(Value*), 1);

  if(strcmp(type, "pointer") == 0) {
    if(OZ_isForeignPointer(Term))
      v->p = OZ_getForeignPointer(Term);
  }
  else if(strcmp(type, "string") == 0) {
    int len = 0;
    v->p = strdup(OZ_stringToC(Term, &len));
  }
  else if(strcmp(type, "void") == 0) {
    /* Do nothing */
  }
  else if(strcmp(type, "uint8") == 0) {
    if(OZ_isNumber(Term)) 
      v->u8 = OZ_intToC(Term);
  }
  else if(strcmp(type, "sint8") == 0) {
    if(OZ_isNumber(Term)) 
      v->s8 = OZ_intToC(Term);
  }
  else if(strcmp(type, "uint16") == 0) {
    if(OZ_isNumber(Term)) 
      v->u16 = OZ_intToC(Term);
  }
  else if(strcmp(type, "sint16") == 0) {
    if(OZ_isNumber(Term)) 
      v->s16 = OZ_intToC(Term);
  }
  else if(strcmp(type, "uint32") == 0) {
    if(OZ_isNumber(Term)) 
      v->u32 = OZ_intToC(Term);
  }
  else if(strcmp(type, "sint32") == 0) {
    if(OZ_isNumber(Term)) 
      v->s32 = OZ_intToC(Term);
  }
  else if(strcmp(type, "uint64") == 0) {
    if(OZ_isNumber(Term)) 
      v->u64 = OZ_intToC(Term);
  }
  else if(strcmp(type, "sint64") == 0) {
    if(OZ_isNumber(Term)) 
      v->s64 = OZ_intToC(Term);
  }
  else if(strcmp(type, "float") == 0) {
    if(OZ_isFloat(Term))
      v->f = OZ_floatToC(Term);
  }
  else if(strcmp(type, "double") == 0) {
    if(OZ_isFloat(Term))
      v->d = OZ_floatToC(Term);
  }
  return v;
}

OZ_BI_define(BIcall,3,1)
{
  OZ_declareDetTerm(0, Pointer);
  OZ_declareDetTerm(1, Definition);
  OZ_declareDetTerm(2, Args);

  char const* call_type = strdup(OZ_atomToC(OZ_label(Definition)));
  char const* return_type = strdup(OZ_atomToC(OZ_label(OZ_getArg(Definition, 0))));

  OZ_Term Function = OZ_getArg(Definition, 1);
  char const* function_name = strdup(OZ_atomToC(OZ_label(OZ_getArg(Function, 0))));
  int number_args = OZ_width(Function);
  
  ffi_cif cif;
  ffi_type* args[number_args];
  void* values[number_args];
  OZ_Term cons = Args;
  int i = 0;
  while(OZ_isCons(cons)) {
    OZ_Term hd = OZ_head(cons);
    char* type = strdup(OZ_atomToC(OZ_getArg(Function, i)));
    args[i] = getFFIType(type);
    values[i] = getFFIValue(type, hd);
    free(type);

    ++i;
    cons = OZ_tail(cons);    
  }  

  if(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, i, getFFIType(return_type), args) == FFI_OK) {
    Value rc;
    typedef void (*fn_t)();
    fn_t fn = (fn_t)OZ_getForeignPointer(Pointer);
    ffi_call(&cif, fn, &rc, values); 
    OZ_Term result;
    if(strcmp(return_type, "pointer") == 0) 
      result = OZ_makeForeignPointer(rc.p);
    else if(strcmp(return_type, "string") == 0) 
      result = OZ_string((char*)rc.p);
    else if(strcmp(return_type, "void") == 0)
      result = OZ_unit();
    else if(strcmp(return_type, "uint8") == 0)
      result = OZ_int(rc.u8);
    else if(strcmp(return_type, "sint8") == 0)
      result = OZ_int(rc.s8);
    else if(strcmp(return_type, "uint16") == 0)
      result = OZ_int(rc.u16);
    else if(strcmp(return_type, "sint16") == 0)
      result = OZ_int(rc.s16);
    else if(strcmp(return_type, "uint32") == 0) 
      result = OZ_int(rc.u32);
    else if(strcmp(return_type, "sint32") == 0)
      result = OZ_int(rc.s32);
    else if(strcmp(return_type, "uint64") == 0)
      result = OZ_int(rc.u64);
    else if(strcmp(return_type, "sint64") == 0)
      result = OZ_int(rc.s64);
    else if(strcmp(return_type, "float") == 0)
      result = OZ_float(rc.f);
    else if(strcmp(return_type, "double") == 0)
      result = OZ_float(rc.f);

    OZ_out(0) = result;
    return OZ_ENTAILED;
  }
  return OZ_FAILED;
} OZ_BI_end

OZ_BI_define(BIfindFunction,2,1)
{
  OZ_declareDetTerm(0, library);
  OZ_declareAtom(1, function);
  
  void* lib = dlopen(OZ_isNil(library) ? 0 : OZ_atomToC(library), RTLD_NOW | RTLD_GLOBAL);
  if(!lib) {
    printf("Lib Load Failed: %s\n", dlerror());
    return OZ_FAILED;
  }

  void* func = dlsym(lib, function);
  if(!func) {
    printf("Function Lookup Failed: %s\n", dlerror());
     dlclose(lib);
    return OZ_FAILED;
  }

  OZ_out(0) = OZ_makeForeignPointer(func);
  return OZ_ENTAILED;
} OZ_BI_end

static OZ_C_proc_interface oz_interface[] = {
  { "load", 1, 1, FFI_load },
  { "bind", 2, 1, FFI_bind },
  { "unload", 1, 0, FFI_unload },
  { "is", 1, 1, FFI_is },
  { "call", 3, 1, BIcall },
  { "findFunction", 2, 1, BIfindFunction },
  { 0,0,0,0}
};

OZ_C_proc_interface *oz_init_module() {
  FFI::id = OZ_getUniqueId();
  return oz_interface;
}
