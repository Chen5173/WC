// lookaside.cpp:Define the entry point for the console application
//

#include "stdafx.h"
#include <windows.h>
void main()
{
	HLOCAL h1, h2, h3, h4;
	HANDLE hp;
	hp = HeapCreate(0, 0, 0);     //Creat the heap
	__asm int 3                   //make a break piont to stop the process
	h1 = HeapAlloc(hp, HEAP_ZERO_MEMORY, 8);
        //Apply for 8 bytes of heap space,and return the point to the h1
	h2 = HeapAlloc(hp, HEAP_ZERO_MEMORY, 8);
	h3 = HeapAlloc(hp, HEAP_ZERO_MEMORY, 16);
	h4 = HeapAlloc(hp, HEAP_ZERO_MEMORY, 24);

        //free the heap that apply before
	HeapFree(hp, 0, h1);           
	HeapFree(hp, 0, h2);
	HeapFree(hp, 0, h3);
	HeapFree(hp, 0, h4);
 
        //
        //apply again
	h2 = HeapAlloc(hp, HEAP_ZERO_MEMORY, 16);
	HeapFree(hp, 0, h2);
}