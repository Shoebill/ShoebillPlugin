/**
 * Copyright (C) 2011-2014 MK124
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
#include "AmxHelper.h"
#include "Callbacks.h"

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
    return sampgdk::Supports() | SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
    if (sampgdk::Load(ppData))
    {
        pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
        return Shoebill::GetInstance().OnPluginLoad();
    }
    return false;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
    Shoebill::GetInstance().OnPluginUnload();
    sampgdk::Unload();

}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
    Shoebill::GetInstance().OnAmxLoad(amx);
    return AMX_ERR_NONE;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
    Shoebill::GetInstance().OnAmxUnload(amx);
    return AMX_ERR_NONE;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
    sampgdk::ProcessTick();
    Shoebill::GetInstance().OnProcessTick();
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPublicCall(AMX *amx, const char *name, cell *params, cell *retval)
{
    bool foundFunction = false;
    auto result = InvokeCallback(amx, name, params, foundFunction); //call Java function if there is any to execute
    if (foundFunction)
    {
        if (retval) *retval = result; //Set returnvalue
        if (ShouldCancelCallback(name, result)) return false;
    }
    auto hook = CallHookedCallback(amx, name, params); //Execute custom hook
    if (hook)
    {
        auto willCancel = hook[1];
        if (retval) *retval = hook[0];
        delete hook;
        if (willCancel == 1)
            return false;
    }
    return true;
}