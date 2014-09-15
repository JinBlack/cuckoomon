
#include "ntapi.h"
#include "hooking.h"


#define MAX_CALLER_BACK 10

//inline void * getPEB() __attribute__((always_inline));
PEB *get_PEB();

static __inline__ void * get_caller_address()  __attribute__((always_inline));
static __inline__ void * get_caller_address(){

//Find the original return pointer
    return (void *)*((int *)hook_info()->retaddr_esp)-5;
/*  void *ptr;
  __asm__ __volatile__("movl 16(%%ebp),%0;"
                "add  $-5,%0;"
                      :"=r"(ptr) //output
                      );
  return ptr; //the address in the stack point to next instraction afetr call and call is a 5 bytes instruction*/
//  __asm__ __volatile__("int3;");

//    return  __builtin_return_address(0);

}

uint8_t check_caller_address_in_text(void *caller);

uint8_t check_caller_address_in_module(void *caller);


static __inline__ void * get_real_caller_address()  __attribute__((always_inline));
static __inline__ void * get_real_caller_address(){
  void *caller;
  // i start from i because first caller address is in hook_info
  int i=1;
  caller = (void *)*((int *)hook_info()->retaddr_esp)-5;
  if (check_caller_address_in_text(caller)){
    return caller;
  }
/*  else if (check_caller_address_in_module(caller)){
    caller = __builtin_return_address(1)-5;
  }*/
  while(check_caller_address_in_module(caller) && i <= MAX_CALLER_BACK){
    switch(i){
      case 1:
        caller = __builtin_return_address(1);
        break;
      case 2:
        caller = __builtin_return_address(2);
        break;
      case 3:
        caller = __builtin_return_address(3);
        break;
      case 4:
        caller = __builtin_return_address(4);
        break;
      case 5:
        caller = __builtin_return_address(5);
        break;
      case 6:
        caller = __builtin_return_address(6);
        break;
      case 7:
        caller = __builtin_return_address(7);
        break;
      case 8:
        caller = __builtin_return_address(8);
        break;
      case 9:
        caller = __builtin_return_address(9);
        break;
      case 10:
        caller = __builtin_return_address(10);
        break;
      default:
        return caller;
    }
    ++i;
  }
  if (caller == 0){
    caller = get_caller_address();
  }
  else {
    caller -= 5;
  }
  return caller;
}