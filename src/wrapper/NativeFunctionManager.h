/**
* Copyright (C) 2015 MK124 & Marvin H
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
#ifndef _NATIVEFUNCTIONMANAGER_H_
#define _NATIVEFUNCTIONMANAGER_H_

#include <unordered_map>
#include <string>
#include <memory>
#include "AmxInstanceManager.hpp"
#include <algorithm>
#include <functional>
#include <hook/SimpleInlineHook.hpp>
#include "sampgdk.h"

#include "AmxHelper.h"
#include "amx/amx.h"
#include "Callbacks.h"

struct HookedNative
{
    AMX_NATIVE function;
    std::string name;
    std::shared_ptr<SimpleInlineHook> *hook;
};

class NativeFunctionManager
{
public:

    static NativeFunctionManager &GetInstance();

    NativeFunctionManager();

    ~NativeFunctionManager();

    void RegisterFunction(AMX *amx, std::string name, AMX_NATIVE functionAddr, int index);

    /*std::map<std::string, HookedNative *> getNatives();

    AMX_NATIVE findFunction(char const *name);

    std::string getFunctionName(int index);*/

    void HookFunction(AMX *amx, AMX_NATIVE function, const char *name, int index);

    void ClearFunctions();

private:
    NativeFunctionManager(const NativeFunctionManager &) = delete;

    NativeFunctionManager &operator=(const NativeFunctionManager &) = delete;
};

#endif