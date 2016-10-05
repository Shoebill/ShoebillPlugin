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
#ifndef __AMXINSTANCEMANAGER__
#define __AMXINSTANCEMANAGER__

#include <unordered_set>
#include "amx/amx.h"
#include <map>
#include <vector>

class AmxInstanceManager
{
public:
    static AmxInstanceManager &GetInstance()
    {
        static AmxInstanceManager instance;
        return instance;
    }

    AmxInstanceManager()
    { }

    ~AmxInstanceManager()
    { }

    void RegisterAmx(AMX *amx)
    {
        if (!IsValid(amx))
        {
            amxInstances.insert(amx);
            registeredFunctions[amx] = std::map<std::string, std::vector<std::string>>();
        }
    }

    void UnregisterAmx(AMX *amx)
    {
        if (IsValid(amx))
        {
            registeredFunctions.erase(registeredFunctions.find(amx));
            amxInstances.erase(amx);
            if (mainAmx == amx) mainAmx = nullptr;
        }
    }

    bool IsValid(AMX *amx)
    {
        return amxInstances.find(amx) != amxInstances.end();
    }

    void MarkAsMainAmx(AMX *amx)
    {
        if (!IsValid(amx)) return;
        mainAmx = amx;
    }

    bool RegisteredFunctionExists(AMX *amx, std::string functionName)
    {
        return registeredFunctions[amx].find(functionName) != registeredFunctions[amx].end();
    }

    bool RegisterFunction(AMX *amx, std::string functionName, std::vector<std::string> types)
    {
        if (RegisteredFunctionExists(amx, functionName))
            return false;
        registeredFunctions[amx][functionName] = types;
        return true;
    }

    std::vector<std::string> GetParameterTypes(AMX *amx, std::string functionName)
    {
        if (!RegisteredFunctionExists(amx, functionName))
            return std::vector<std::string>();
        return registeredFunctions[amx][functionName];
    }

    bool UnregisterFunction(AMX *amx, std::string functionName)
    {
        if (!RegisteredFunctionExists(amx, functionName))
            return false;
        registeredFunctions[amx].erase(registeredFunctions[amx].find(functionName));
        return true;
    }

    AMX *GetMainAmx() const
    {
        return mainAmx;
    }

    AMX *GetAvailableAmx()
    {
        if (mainAmx != nullptr) return mainAmx;
        if (amxInstances.empty()) return nullptr;
        return *amxInstances.begin();
    }

    std::unordered_set<AMX *> GetInstances()
    {
        std::unordered_set<AMX *> copyOfInstances;
        for (std::unordered_set<AMX *>::iterator it = amxInstances.begin(); it != amxInstances.end(); ++it)
        {
            copyOfInstances.insert(*it);
        }
        return copyOfInstances;
    }

private:
    std::unordered_set<AMX *> amxInstances;
    AMX *mainAmx;
    std::map<AMX *, std::map<std::string, std::vector<std::string>>> registeredFunctions;

    AmxInstanceManager(const AmxInstanceManager &) = delete;

    AmxInstanceManager &operator=(const AmxInstanceManager &) = delete;
};

#endif