/**
* Copyright (C) 2014 MK124
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef __SIMEPLINLINEHOOK_CPP__
#define __SIMEPLINLINEHOOK_CPP__

#include "SimpleInlineHook.hpp"

#if defined(WIN32)

#include <windows.h>

void SimpleInlineHook::removePageProtect(void* address, size_t size)
{
	DWORD oldState;
	VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldState);
}

#else

#include <sys/mman.h>
#include <unistd.h>

void SimpleInlineHook::removePageProtect(void* address, size_t size)
{
    long pagesize;

    pagesize = sysconf(_SC_PAGESIZE);
    address = (void *)((long)address & ~(pagesize - 1));

    mprotect(address, size, PROT_READ | PROT_WRITE | PROT_EXEC);
}

#endif
#endif