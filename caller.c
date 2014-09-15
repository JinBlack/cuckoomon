#include "caller.h"

PEB *get_PEB(){
  void *ptr;
  __asm__ __volatile__("movl %%fs:0x30, %0;"
                      :"=r"(ptr) //output
                      );
  return ptr; //the address in the stack point to next instraction afetr call and call is a 5 bytes instruction
}

uint8_t check_caller_address_in_text(void *caller){
  PPEB peb = get_PEB();
  //Assupmtion: the first loaded module is the code base.
  PLDR_MODULE module = (PLDR_MODULE) peb->LoaderData->InLoadOrderModuleList.Flink;
  if (caller-module->BaseAddress < module->SizeOfImage){
    return 1;
  }
  return 0;
}

uint8_t check_caller_address_in_module(void *caller){
  PPEB peb = get_PEB();
  PLDR_MODULE stop;
  //Assupmtion: the first loaded module is the code base.
  PLDR_MODULE module = (PLDR_MODULE) peb->LoaderData->InLoadOrderModuleList.Flink;
  stop = (PLDR_MODULE) peb->LoaderData->InLoadOrderModuleList.Blink;
  do{
    module = (PLDR_MODULE) module->InLoadOrderModuleList.Flink;
    if (caller-module->BaseAddress < module->SizeOfImage){
      return 1;
    }
  }while(module != stop);
  return 0;
}