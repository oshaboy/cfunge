/* -*- mode: C; coding: utf-8; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * cfunge - A standard-conforming Befunge93/98/109 interpreter in C.
 * Copyright (C) 2008-2013 Arvid Norlander <VorpalBlade AT users.noreply.github.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at the proxy's option) any later version. Arvid Norlander is a
 * proxy who can decide which future versions of the GNU General Public
 * License can be used.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <dlfcn.h>
#include <ffi.h>
#include <stdint.h>
#include <stdlib.h>
#include <alloca.h>
#include <stdio.h>
#include "../../stack.h"
#include "CFFI.h"
void ** pstack=NULL;
int stack_ptr;
static inline void * popp(void){
  register void * p = pstack[stack_ptr-1];
  stack_ptr--;
  pstack=realloc(pstack,stack_ptr*sizeof(void *));
  return p;

}

static inline void pushp(void * p){
  stack_ptr++;
  pstack=realloc(pstack,stack_ptr*sizeof(void *));
  pstack[stack_ptr-1]=p;
  
}
/* B - get char from string */
static void finger_CFFI_getbyte(instructionPointer * ip)
{

    char * str=popp();
    funge_cell index=stack_pop(ip->stack);
    stack_push(ip->stack,str[index]);
}
/* F - free pointer */
static void finger_CFFI_free(instructionPointer * ip){
	(void) ip;
    free(popp());
}
/* S - generate_string */
static void finger_CFFI_generate_string(instructionPointer * ip)
{
    char * str=malloc(512*sizeof(char));
    int i=0;
    char c;
    do{
       c=(char)stack_pop(ip->stack);
       str[i]=c;
       i++;
       if (i%512==0){
         str=realloc(str,(i+512)*sizeof(char));
       }
        

    } while(c);
    str=realloc(str, i+1);
    pushp(str);
        
        
}
static void finger_CFFI_swap(instructionPointer * ip)
{
  (void) ip;
  void * ptr1=popp();
  void * ptr2=popp();
  pushp(ptr1);
  pushp(ptr2);
}
static void finger_CFFI_dereference(instructionPointer * ip){
  funge_cell index=stack_pop(ip->stack);
  funge_cell * arr=popp();
  stack_push(ip->stack,arr[index]);
}
static void finger_CFFI_write_to_arr(instructionPointer * ip){
  funge_cell index=stack_pop(ip->stack);
  funge_cell data=stack_pop(ip->stack);
  funge_cell * arr=popp();
  arr[index]=data;
}
static void finger_CFFI_dlclose(instructionPointer * ip){
  if(dlclose(popp())){
    ip_reverse(ip);
  }
}
static void finger_CFFI_dlsym(instructionPointer * ip){
  char * token_name=popp();
  void * library=popp();
  dlerror();
  void * token=dlsym(library,token_name);
  if (dlerror()){
    ip_reverse(ip);
  } else {
    pushp(token);
  }
}
static void finger_CFFI_funge_to_pointer(instructionPointer * ip){
  funge_cell low=stack_pop(ip->stack);
  funge_cell high=stack_pop(ip->stack);
  uint64_t ptr=(((uint64_t)(uint32_t)high)<<32)|((uint32_t)low);
  pushp((void*)ptr);
}
static void finger_CFFI_print_pstack(instructionPointer * ip){
  (void) ip;
  for (int i=stack_ptr-1; i>=0; i--){
    if (pstack[i]){
      printf("%d: %lx (First Word is %d)\n",i, (long unsigned int)pstack[i],*(int *)(pstack[i]));
    } else {
      printf("%d: NULL\n",i);
    }
  }
  printf("\n");
}
static void finger_CFFI_malloc(instructionPointer * ip){
  funge_cell len=stack_pop(ip->stack);
  funge_cell * arr=malloc(sizeof(funge_cell)*sizeof(len));
  for (int i=0; i<len; i++){
    arr[i]=stack_pop(ip->stack);
  }
  pushp(arr);
}
static void finger_CFFI_dlopen(instructionPointer * ip){
  char * libname=popp();
  void * library=dlopen(libname,RTLD_LAZY);
  if (!library){
    ip_reverse(ip);
  } else {
    pushp(library);
  }
}
static void finger_CFFI_popp(instructionPointer * ip){
  (void) ip;
  popp();
}
static void finger_CFFI_pointer_to_funge(instructionPointer * ip){
  uint64_t ptr=(uint64_t)popp();
  funge_cell high=(funge_cell)(ptr/(1L<<32));
  funge_cell low=(funge_cell)(ptr%(1L<<32));
  stack_push(ip->stack, high); stack_push(ip->stack,low);
}
static void finger_CFFI_duplicate(instructionPointer * ip){
  (void) ip;
  pushp(pstack[stack_ptr-1]);
}

void finger_CFFI_ccall(instructionPointer * ip){
  /* Figure out Return Type */
  funge_cell return_type_i=stack_pop(ip->stack);
  ffi_type * return_type;
  switch(return_type_i){
    case 0:
      return_type=&ffi_type_uint32;
      break;
    case 1:
      return_type=&ffi_type_pointer;
    break;
    default:
      return_type=&ffi_type_void;
      break;
  }
  
  /* get the types (funge/pointer) of all the arguments */
  funge_cell len=stack_pop(ip->stack); /* Amount of arguments */
  ffi_type ** argtypes=alloca(len*sizeof(ffi_type*)); 
  {
  signed long long lensave=len;
  int count=0;
  /* set argument to pointer if pointer and uint32 if funge */
  while(lensave>0){	
    /* Get (part of if over 32 arguments) the bitmask for the arguments*/
    funge_cell cur=stack_pop(ip->stack); 
    for (int i=0; (i<sizeof(funge_cell)*8)&&(lensave>0); i++){
      if (cur&1){
        argtypes[count]=&ffi_type_pointer;
      } else {
        argtypes[count]=&ffi_type_uint32;
      }
      cur>>=1;
      count++;
     lensave--;
    }
    }
  }
  /* pop the arguments from the correct stack based on the bitmask */
  void ** arguments=alloca(len*sizeof(void *));
  {
    for (int i=0; i<len; i++){
      if(argtypes[i]==&ffi_type_uint32){
        funge_cell * p=arguments[i]=alloca(sizeof(funge_cell *));
        *p=stack_pop(ip->stack);
      }else if(argtypes[i]==&ffi_type_pointer){
        void ** p=arguments[i]=alloca(sizeof(void *));
        *p=popp();
      } else {
        fprintf(stderr, "This isn't supposed to happen\n");
        exit(3);
      }
    }
  }
  /* Pop Function Pointer */
  void (*func)(void)=popp();
  ffi_cif cif;
  void * return_value=alloca(16); /* CFFI doesn't support raw structs the biggest data type would be 8 bytes, but I am allocating 16 for good measure */
  
  /* Call the function using libffi */
  ffi_prep_cif(&cif,FFI_DEFAULT_ABI, len, return_type, argtypes);
  ffi_call(&cif,func,return_value,arguments);

  /* Push the return value to the correct stack based on the return value */
  switch(return_type_i){
    case 0:
    //return_type=&ffi_type_uint32;
      stack_push(ip->stack,*(funge_cell*)return_value);
    break;
    case 1:
      //return_type=&ffi_type_pointer;
      pushp(*(void**)return_value);
    break;
  }
}
bool finger_CFFI_load(instructionPointer * ip)
{
	manager_add_opcode(CFFI, 'B', getbyte);
	manager_add_opcode(CFFI, 'C', ccall);
	manager_add_opcode(CFFI, 'D', duplicate);
	manager_add_opcode(CFFI, 'F', free);
	manager_add_opcode(CFFI, 'G', pointer_to_funge);
	manager_add_opcode(CFFI, 'I', popp);
	manager_add_opcode(CFFI, 'L', dlopen);
	manager_add_opcode(CFFI, 'M', malloc);
	manager_add_opcode(CFFI, 'O', print_pstack);
	manager_add_opcode(CFFI, 'P', funge_to_pointer);
	manager_add_opcode(CFFI, 'S', generate_string);
	manager_add_opcode(CFFI, 'T', dlsym);
	manager_add_opcode(CFFI, 'U', dlclose);
	manager_add_opcode(CFFI, 'W', dereference);
	manager_add_opcode(CFFI, 'X', write_to_arr);
	manager_add_opcode(CFFI, 'Y', swap);
	if (pstack!=NULL){
		free(pstack);
	}
	pstack=malloc(sizeof(void*));
	*pstack=NULL;
	stack_ptr=1;
	return true;
}
