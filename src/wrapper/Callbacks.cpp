/**
 * Copyright (C) 2011-2015 MK124
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
#include "Callbacks.h"

cell InvokeCallback(AMX *amx, std::string name, cell *params, bool &foundFunction)
{
    static std::map<std::string, AMX_NATIVE> callbackMap = {
            {"OnGameModeInit",              n_OnGameModeInit},
            {"OnGameModeExit",              n_OnGameModeExit},
            {"OnPlayerConnect",             n_OnPlayerConnect},
            {"OnPlayerDisconnect",          n_OnPlayerDisconnect},
            {"OnPlayerSpawn",               n_OnPlayerSpawn},
            {"OnPlayerDeath",               n_OnPlayerDeath},
            {"OnVehicleSpawn",              n_OnVehicleSpawn},
            {"OnVehicleDeath",              n_OnVehicleDeath},
            {"OnPlayerText",                n_OnPlayerText},
            {"OnPlayerCommandText",         n_OnPlayerCommandText},
            {"OnPlayerRequestClass",        n_OnPlayerRequestClass},
            {"OnPlayerEnterVehicle",        n_OnPlayerEnterVehicle},
            {"OnPlayerExitVehicle",         n_OnPlayerExitVehicle},
            {"OnPlayerStateChange",         n_OnPlayerStateChange},
            {"OnPlayerEnterCheckpoint",     n_OnPlayerEnterCheckpoint},
            {"OnPlayerLeaveCheckpoint",     n_OnPlayerLeaveCheckpoint},
            {"OnPlayerEnterRaceCheckpoint", n_OnPlayerEnterRaceCheckpoint},
            {"OnPlayerLeaveRaceCheckpoint", n_OnPlayerLeaveRaceCheckpoint},
            {"OnRconCommand",               n_OnRconCommand},
            {"OnPlayerRequestSpawn",        n_OnPlayerRequestSpawn},
            {"OnObjectMoved",               n_OnObjectMoved},
            {"OnPlayerObjectMoved",         n_OnPlayerObjectMoved},
            {"OnPlayerPickUpPickup",        n_OnPlayerPickUpPickup},
            {"OnVehicleMod",                n_OnVehicleMod},
            {"OnEnterExitModShop",          n_OnEnterExitModShop},
            {"OnVehiclePaintjob",           n_OnVehiclePaintjob},
            {"OnVehicleRespray",            n_OnVehicleRespray},
            {"OnVehicleDamageStatusUpdate", n_OnVehicleDamageStatusUpdate},
            {"OnUnoccupiedVehicleUpdate",   n_OnUnoccupiedVehicleUpdate},
            {"OnPlayerSelectedMenuRow",     n_OnPlayerSelectedMenuRow},
            {"OnPlayerExitedMenu",          n_OnPlayerExitedMenu},
            {"OnPlayerInteriorChange",      n_OnPlayerInteriorChange},
            {"OnPlayerKeyStateChange",      n_OnPlayerKeyStateChange},
            {"OnRconLoginAttempt",          n_OnRconLoginAttempt},
            {"OnPlayerUpdate",              n_OnPlayerUpdate},
            {"OnPlayerStreamIn",            n_OnPlayerStreamIn},
            {"OnPlayerStreamOut",           n_OnPlayerStreamOut},
            {"OnVehicleStreamIn",           n_OnVehicleStreamIn},
            {"OnVehicleStreamOut",          n_OnVehicleStreamOut},
            {"OnDialogResponse",            n_OnDialogResponse},
            {"OnPlayerTakeDamage",          n_OnPlayerTakeDamage},
            {"OnPlayerGiveDamage",          n_OnPlayerGiveDamage},
            {"OnPlayerClickMap",            n_OnPlayerClickMap},
            {"OnPlayerClickTextDraw",       n_OnPlayerClickTextDraw},
            {"OnPlayerClickPlayerTextDraw", n_OnPlayerClickPlayerTextDraw},
            {"OnPlayerClickPlayer",         n_OnPlayerClickPlayer},
            {"OnPlayerEditObject",          n_OnPlayerEditObject},
            {"OnPlayerEditAttachedObject",  n_OnPlayerEditAttachedObject},
            {"OnPlayerSelectObject",        n_OnPlayerSelectObject},
            {"OnPlayerWeaponShot",          n_OnPlayerWeaponShot},
            {"OnIncomingConnection",        n_OnIncomingConnection},
            {"OnTrailerUpdate",             n_OnTrailerUpdate},
            {"OnActorStreamIn",             n_OnActorStreamIn},
            {"OnActorStreamOut",            n_OnActorStreamOut},
            {"OnPlayerGiveDamageActor",     n_OnPlayerGiveDamageActor},
            {"OnVehicleSirenStateChange",   n_OnVehicleSirenStateChange}
    };
    auto it = callbackMap.find(name);
    if (it != callbackMap.end())
    {
        foundFunction = true;
        return it->second(amx, params);
    }
    foundFunction = false;
    return 0;
}

bool ShouldCancelCallback(std::string callbackName, cell returnValue)
{
    static std::map<std::string, int> callbackCancelValues = {
            {"OnPlayerText",                0},
            {"OnPlayerCommandText",         1},
            {"OnRconCommand",               1},
            {"OnPlayerRequestClass",        0},
            {"OnDialogResponse",            1},
            {"OnPlayerClickPlayerTextDraw", 1},
            {"OnPlayerClickTextDraw",       1},
            {"OnPlayerConnect",             0},
            {"OnPlayerDisconnect",          0},
            {"OnPlayerKeyStateChange",      0},
            {"OnPlayerRequestSpawn",        0},
            {"OnPlayerSpawn",               0},
            {"OnPlayerTakeDamage",          0},
            {"OnPlayerUpdate",              0},
            {"OnPlayerWeaponShot",          0},
            {"OnTrailerUpdate",             0},
            {"OnUnoccupiedVehicleUpdate",   0},
            {"OnVehicleMod",                0},
            {"OnVehicleRespray",            0}
    };
    auto it = callbackCancelValues.find(callbackName);
    if (it != callbackCancelValues.end())
    {
        return it->second == returnValue;
    }
    return false;
}

std::map<std::string, std::string> shoebillHookedCallbacks;
Shoebill &shoebill = Shoebill::GetInstance();

std::map<std::string, std::string>::iterator getIterator(std::string name)
{
    return shoebillHookedCallbacks.find(name);
}

std::map<std::string, std::string>::const_iterator getEnd()
{
    return shoebillHookedCallbacks.end();
}

bool HookCallback(std::string name, std::string types)
{
    shoebillHookedCallbacks[name] = types;
    return true;
}

bool UnhookCallback(std::string name)
{
    auto it = shoebillHookedCallbacks.find(name);
    if (it == shoebillHookedCallbacks.end())
        return false;
    shoebillHookedCallbacks.erase(name);
    return true;
}

cell AMX_NATIVE_CALL CallShoebillFunction(AMX *amx, cell *params)
{
    int parameterCount = (int) (params[0] / sizeof(cell));
    char* function = NULL;
    amx_StrParam(amx, params[1], function);
    std::string functionName = std::string(function);
    if (!AmxInstanceManager::GetInstance().RegisteredFunctionExists(amx, functionName))
    {
        sampgdk_logprintf("[SHOEBILL] Function %s is not registered.", function);
        return -1;
    }
    auto definedParameters = AmxInstanceManager::GetInstance().GetRegisteredParameters(amx, functionName);
    signed int shouldHaveParameterCount = definedParameters.size();
    for (auto string : definedParameters)
    {
        if (string.find("[") == 0)
            shouldHaveParameterCount += 1;
    }
    if (parameterCount - 1 != shouldHaveParameterCount)
    {
        sampgdk_logprintf("[SHOEBILL] Calling %s with %i parameters, but was expecting %i parameters.", function,
                          parameterCount - 1, shouldHaveParameterCount);
        return -1;
    }
    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static auto integerClass = env->FindClass("java/lang/Integer");
    static auto floatClass = env->FindClass("java/lang/Float");
    static auto boolClass = env->FindClass("java/lang/Boolean");
    static auto objectClass = env->FindClass("java/lang/Object");

    auto objectArray = env->NewObjectArray(definedParameters.size(), objectClass, NULL);
    env->NewGlobalRef(objectArray);
    std::vector<std::pair<cell *, std::string>> referenceValues;
    std::map<cell *, int> arrayLengths;
    for (unsigned int i = 0, pawnIndex = 0; i < definedParameters.size(); i++, pawnIndex++)
    {
        auto iterationCell = params[pawnIndex + 2];
        if (definedParameters[i] == "java.lang.String")
        {
            char* parameterString = NULL;
            cell *phys_addr = nullptr;
            amx_StrParam(amx, iterationCell, parameterString);
            amx_GetAddr(amx, iterationCell, &phys_addr);

            jchar wstr[sizeof parameterString];
            auto len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), parameterString, -1, wstr,
                               sizeof(wstr) / sizeof(wstr[0]));

            auto string = env->NewString(wstr, len);
            env->SetObjectArrayElement(objectArray, i, string);
            referenceValues.push_back(std::pair<cell *, std::string>(phys_addr, definedParameters[i]));
        }
        else if (definedParameters[i] == "java.lang.Integer")
        {
            static auto methodId = env->GetMethodID(integerClass, "<init>", "(I)V");
            cell *integerValue;
            amx_GetAddr(amx, iterationCell, &integerValue);
            auto integerObject = env->NewObject(integerClass, methodId, *integerValue);
            env->SetObjectArrayElement(objectArray, i, integerObject);
            referenceValues.push_back(std::pair<cell *, std::string>(integerValue, definedParameters[i]));
        }
        else if (definedParameters[i] == "java.lang.Boolean")
        {
            static auto methodId = env->GetMethodID(boolClass, "<init>", "(Z)V");
            cell *boolValue;
            amx_GetAddr(amx, iterationCell, &boolValue);
            auto boolObject = env->NewObject(boolClass, methodId, *boolValue);
            env->SetObjectArrayElement(objectArray, i, boolObject);
            referenceValues.push_back(std::pair<cell *, std::string>(boolValue, definedParameters[i]));
        }
        else if (definedParameters[i] == "java.lang.Float")
        {
            static auto methodId = env->GetMethodID(floatClass, "<init>", "(F)V");
            cell *floatValue;
            amx_GetAddr(amx, iterationCell, &floatValue);
            auto floatObject = env->NewObject(floatClass, methodId, amx_ctof(*floatValue));
            env->SetObjectArrayElement(objectArray, i, floatObject);
            referenceValues.push_back(std::pair<cell *, std::string>(floatValue, definedParameters[i]));
        }
/*else if (definedParameters[i] == "[Ljava.lang.String;")
{
    cell* phys_addr = NULL;
    amx_GetAddr(amx, params[2 + i + 1], &phys_addr);
    auto arrayLength = *phys_addr;
    cell* array = &iterationCell;
    auto javaArray = env->NewObjectArray(arrayLength, stringClass, NULL);
    arrayLengths[array] = arrayLength;
    for (auto a = 0; a < arrayLength; a++)
    {
        char parameterString[1024];
        amx_GetString(amx, *(array + a) + 8, parameterString, sizeof(parameterString));
        env->SetObjectArrayElement(javaArray, a, env->NewStringUTF(parameterString));
    }
    env->SetObjectArrayElement(objectArray, lastArrayIndex, javaArray);
    referenceValues.push_back(std::pair<cell*, std::string>(array, definedParameters[i]));
    i += 1; //skip next iteration because of length
    lastArrayIndex += 1;
}*/
        else if (definedParameters[i] == "[Ljava.lang.Boolean;")
        {
            cell *phys_addr = nullptr;
            amx_GetAddr(amx, params[2 + i + 1], &phys_addr);
            auto arrayLength = *phys_addr;
            cell *array = nullptr;
            amx_GetAddr(amx, iterationCell, &array);
            arrayLengths[array] = arrayLength;
            auto javaArray = env->NewObjectArray(arrayLength, boolClass, nullptr);
            static auto methodId = env->GetMethodID(boolClass, "<init>", "(Z)V");
            for (auto a = 0; a < arrayLength; a++)
            {
                env->SetObjectArrayElement(javaArray, a, env->NewObject(boolClass, methodId, *(array + a)));
            }
            env->SetObjectArrayElement(objectArray, i, javaArray);
            referenceValues.push_back(std::pair<cell *, std::string>(array, definedParameters[i]));
            pawnIndex += 1;
        }
        else if (definedParameters[i] == "[Ljava.lang.Integer;")
        {
            cell *phys_addr = nullptr;
            amx_GetAddr(amx, params[2 + i + 1], &phys_addr);
            auto arrayLength = *phys_addr;
            cell *array = nullptr;
            amx_GetAddr(amx, iterationCell, &array);
            arrayLengths[array] = arrayLength;
            auto javaArray = env->NewObjectArray(arrayLength, integerClass, nullptr);
            static auto methodId = env->GetMethodID(integerClass, "<init>", "(I)V");
            for (auto a = 0; a < arrayLength; a++)
            {
                env->SetObjectArrayElement(javaArray, a, env->NewObject(integerClass, methodId, *(array + a)));
            }
            env->SetObjectArrayElement(objectArray, i, javaArray);
            referenceValues.push_back(std::pair<cell *, std::string>(array, definedParameters[i]));
            pawnIndex += 1;
        }
        else if (definedParameters[i] == "[Ljava.lang.Float;")
        {
            cell *phys_addr = nullptr;
            amx_GetAddr(amx, params[2 + i + 1], &phys_addr);
            auto arrayLength = *phys_addr;
            cell *array = nullptr;
            amx_GetAddr(amx, iterationCell, &array);
            arrayLengths[array] = arrayLength;
            auto javaArray = env->NewObjectArray(arrayLength, floatClass, nullptr);
            static auto methodId = env->GetMethodID(floatClass, "<init>", "(F)V");
            for (auto a = 0; a < arrayLength; a++)
            {
                env->SetObjectArrayElement(javaArray, a, env->NewObject(floatClass, methodId, amx_ctof(*(array + a))));
            }
            env->SetObjectArrayElement(objectArray, i, javaArray);
            referenceValues.push_back(std::pair<cell *, std::string>(array, definedParameters[i]));
            pawnIndex += 1;
        }
    }
    auto result = shoebill.CallRegisteredFunction(amx, std::string(function), objectArray);
    for (unsigned int i = 0; i < referenceValues.size(); i++)
    {
        if (referenceValues[i].second == "java.lang.String")
        {
            auto string = static_cast<jstring>(env->GetObjectArrayElement(objectArray, i));
            auto stringObject = env->GetStringChars(string, NULL);
            int len = env->GetStringLength(string);

            char str[1024];
            wcs2mbs((unsigned int) shoebill.getServerCodepage(), stringObject, len, str, sizeof(str));
            env->ReleaseStringChars(string, stringObject);

            amx_SetString(referenceValues[i].first, str, 0, 0, strlen(str) + 1);
        }
        else if (referenceValues[i].second == "java.lang.Integer")
        {
            auto integer = env->GetObjectArrayElement(objectArray, i);
            static auto methodId = env->GetMethodID(integerClass, "intValue", "()I");
            *referenceValues[i].first = env->CallIntMethod(integer, methodId);
        }
        else if (referenceValues[i].second == "java.lang.Float")
        {
            auto floatObject = env->GetObjectArrayElement(objectArray, i);
            static auto methodId = env->GetMethodID(floatClass, "floatValue", "()F");
            auto res = env->CallFloatMethod(floatObject, methodId);
            *referenceValues[i].first = amx_ftoc(res);
        }
        else if (referenceValues[i].second == "java.lang.Boolean")
        {
            auto boolObject = env->GetObjectArrayElement(objectArray, i);
            static auto methodId = env->GetMethodID(boolClass, "booleanValue", "()Z");
            *referenceValues[i].first = static_cast<int>(env->CallBooleanMethod(boolObject, methodId));
        }
/*else if (referenceValues[i].second == "[Ljava.lang.String;")
{
    auto stringArrayObject = static_cast<jobjectArray>(env->GetObjectArrayElement(objectArray, i));
    int newArrayLength = env->GetArrayLength(stringArrayObject);
    if (newArrayLength > params[2 + i + 1])
        logprintf("[SHOEBILL] %s has a bigger array than pawn. New values are ignored.", function);
    cell* pawnArray = referenceValues[i].first;
    auto arrayLength = arrayLengths[pawnArray];
    for (int a = 0; a < arrayLength; a++) {
        amx_Release(amx, *(pawnArray + a));
        auto stringObject = static_cast<jstring>(env->GetObjectArrayElement(stringArrayObject, a));
        auto string = env->GetStringUTFChars(stringObject, false);
        *(pawnArray + a) = amx_NewString(amx, string, strlen(string) + 1);
        env->ReleaseStringUTFChars(stringObject, string);
    }
}*/
        else if (referenceValues[i].second == "[Ljava.lang.Integer;")
        {
            auto intArrayObject = static_cast<jobjectArray>(env->GetObjectArrayElement(objectArray, i));
            int newArrayLength = env->GetArrayLength(intArrayObject);
            if (newArrayLength > params[2 + i + 1])
                sampgdk_logprintf("[SHOEBILL] %s has a bigger array than pawn. New values are ignored.", function);
            cell *pawnArray = referenceValues[i].first;
            auto arrayLength = arrayLengths[pawnArray];
            static auto methodId = env->GetMethodID(integerClass, "intValue", "()I");
            for (int a = 0; a < arrayLength; a++)
                *(pawnArray + a) = env->CallIntMethod(env->GetObjectArrayElement(intArrayObject, a), methodId);
        }
        else if (referenceValues[i].second == "[Ljava.lang.Boolean;")
        {
            auto boolArrayObject = static_cast<jobjectArray>(env->GetObjectArrayElement(objectArray, i));
            int newArrayLength = env->GetArrayLength(boolArrayObject);
            if (newArrayLength > params[2 + i + 1])
                sampgdk_logprintf("[SHOEBILL] %s has a bigger array than pawn. New values are ignored.", function);
            cell *pawnArray = referenceValues[i].first;
            auto arrayLength = arrayLengths[pawnArray];
            static auto methodId = env->GetMethodID(boolClass, "booleanValue", "()Z");
            for (int a = 0; a < arrayLength; a++)
                *(pawnArray + a) = env->CallByteMethod(env->GetObjectArrayElement(objectArray, a), methodId);
        }
        else if (referenceValues[i].second == "[Ljava.lang.Float;")
        {
            auto floatArrayObject = static_cast<jobjectArray>(env->GetObjectArrayElement(objectArray, i));
            int newArrayLength = env->GetArrayLength(floatArrayObject);
            if (newArrayLength > params[2 + i + 1])
                sampgdk_logprintf("[SHOEBILL] %s has a bigger array than pawn. New values are ignored.", function);
            cell *pawnArray = referenceValues[i].first;
            auto arrayLength = arrayLengths[pawnArray];
            static auto methodId = env->GetMethodID(floatClass, "floatValue", "()F");
            for (int a = 0; a < arrayLength; a++)
            {
                auto newValue = env->CallFloatMethod(env->GetObjectArrayElement(floatArrayObject, a), methodId);
                *(pawnArray + a) = amx_ftoc(newValue);
            }
        }
    }
    env->DeleteGlobalRef(objectArray);
    return result;
}

int *CallHookedCallback(AMX *amx, std::string name, cell *params)
{
    auto it = getIterator(name);
    if (it != getEnd())
    {
        JNIEnv *env;
        jvm->AttachCurrentThread((void **) &env, NULL);
        if (!env) return 0;
        std::string types = it->second;
        unsigned int count = params[0] / sizeof(cell);
        if (count != types.length())
        {
            sampgdk_logprintf("%s did not equal count! Correct: %i | Wrong: %i", name.c_str(), count, types.length());
            return nullptr;
        }
        static auto objectClass = env->FindClass("java/lang/Object");
        static auto integerClass = env->FindClass("java/lang/Integer");
        static auto floatClass = env->FindClass("java/lang/Float");
        static auto integerMethodID = env->GetMethodID(integerClass, "<init>", "(I)V");
        static auto floatMethodID = env->GetMethodID(floatClass, "<init>", "(F)V");
        jobjectArray objectArray = (jobjectArray) env->NewObjectArray(count, objectClass, 0);
        for (std::string::size_type i = 0; i < types.size(); ++i)
        {
            char paramType = types[i];
            if (paramType == 's')
            {
                char text[1024];
                amx_GetString(amx, params[i + 1], text, sizeof(text));
                jchar wtext[1024];
                int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), text, -1, wtext,
                                  sizeof(wtext) / sizeof(wtext[0]));
                env->SetObjectArrayElement(objectArray, i, env->NewString(wtext, len));
            }
            else if (paramType == 'i')
            {
                env->SetObjectArrayElement(objectArray, i,
                                           env->NewObject(integerClass, integerMethodID, params[i + 1]));
            }
            else if (paramType == 'f')
            {
                float value = amx_ctof(params[i + 1]);
                env->SetObjectArrayElement(objectArray, i, env->NewObject(floatClass, floatMethodID, value));
            }
        }

        jchar wtext[256];
        int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), name.c_str(), -1, wtext,
                          sizeof(wtext) / sizeof(wtext[0]));

        static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onHookCall",
                                                 "(Ljava/lang/String;[Ljava/lang/Object;)[I");
        if (!jmid) return 0;
        jintArray event = (jintArray) env->CallObjectMethod(shoebill.getCallbackHandlerObject(), jmid,
                                                            env->NewString(wtext, len),
                                                            objectArray);
        jni_jvm_printExceptionStack(env);
        jint *values = env->GetIntArrayElements(event, NULL);
        int *returnObject = new int[2]{values[0], values[1]};
        env->ReleaseIntArrayElements(event, values, 0);
        return returnObject;
    }
    else return nullptr;
}

cell AMX_NATIVE_CALL n_OnGameModeInit(AMX *, cell *)
{
    if (!shoebill.getCallbackHandlerObject()) return false;

    JNIEnv *env = NULL;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onGameModeInit", "()Z");
    if (!jmid) return 0;

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnGameModeExit(AMX *, cell *)
{
    if (!shoebill.getCallbackHandlerObject()) return false;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onGameModeExit", "()Z");
    if (!jmid) return 0;

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerConnect(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return false;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerConnect", "(I)Z");
    if (!jmid) return 0;

    int playerid = params[1];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerDisconnect(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return false;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerDisconnect", "(II)Z");
    if (!jmid) return 0;

    int playerid = params[1], reason = params[2];
    shoebill.setPlayerCodepage(playerid, 0);

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, reason);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerSpawn(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return false;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerSpawn", "(I)Z");
    if (!jmid) return 0;

    int playerid = params[1];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerDeath(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return false;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerDeath", "(III)Z");
    if (!jmid) return 0;

    int playerid = params[1], killerid = params[2], reason = params[3];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, killerid, reason);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnVehicleSpawn(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return false;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onVehicleSpawn", "(I)Z");
    if (!jmid) return 0;

    int vehicleid = params[1];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, vehicleid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnVehicleDeath(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return false;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onVehicleDeath", "(II)Z");
    if (!jmid) return 0;

    int vehicleid = params[1], killerid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, vehicleid, killerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerText(AMX *amx, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return false;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerText",
                                             "(ILjava/lang/String;)I");
    if (!jmid) return 0;

    char text[1024];
    int playerid = params[1];
    amx_GetString(amx, params[2], text, sizeof(text));

    jchar wtext[1024];
    int len = mbs2wcs((unsigned int) shoebill.getPlayerCodepage(playerid), text, -1, wtext,
                      sizeof(wtext) / sizeof(wtext[0]));

    jstring str = env->NewString(wtext, len);
    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, str);
    jni_jvm_printExceptionStack(env);
    return (bool) ret;
}

cell n_OnPlayerCommandText(AMX *amx, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerCommandText",
                                             "(ILjava/lang/String;)I");
    if (!jmid) return 0;

    int playerid = params[1];
    char cmdtext[1024];
    amx_GetString(amx, params[2], cmdtext, sizeof(cmdtext));

    jchar wtext[1024];
    int len = mbs2wcs((unsigned int) shoebill.getPlayerCodepage(playerid), cmdtext, -1, wtext,
                      sizeof(wtext) / sizeof(wtext[0]));

    jstring str = env->NewString(wtext, len);
    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, str);
    jni_jvm_printExceptionStack(env);

    return (bool) ret;
}

cell n_OnPlayerRequestClass(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerRequestClass", "(II)Z");
    if (!jmid) return 0;

    int playerid = params[1], classid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, classid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerEnterVehicle(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerEnterVehicle", "(III)Z");
    if (!jmid) return 0;

    int playerid = params[1], vehicleid = params[2], isPassenger = params[3];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, vehicleid, isPassenger);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerExitVehicle(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerExitVehicle", "(II)Z");
    if (!jmid) return 0;

    int playerid = params[1], vehicleid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, vehicleid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerStateChange(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerStateChange", "(III)Z");
    if (!jmid) return 0;

    int playerid = params[1], newstate = params[2], oldstate = params[3];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, newstate, oldstate);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerEnterCheckpoint(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerEnterCheckpoint", "(I)Z");
    if (!jmid) return 0;

    int playerid = params[1];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerLeaveCheckpoint(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerLeaveCheckpoint", "(I)Z");
    if (!jmid) return 0;

    int playerid = params[1];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerEnterRaceCheckpoint(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerEnterRaceCheckpoint", "(I)Z");
    if (!jmid) return 0;

    int playerid = params[1];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerLeaveRaceCheckpoint(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerLeaveRaceCheckpoint", "(I)Z");
    if (!jmid) return 0;

    int playerid = params[1];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnRconCommand(AMX *amx, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onRconCommand",
                                             "(Ljava/lang/String;)I");
    if (!jmid) return 0;

    char cmd[1024];
    amx_GetString(amx, params[1], cmd, sizeof(cmd));

    jchar wtext[sizeof cmd];
    int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), cmd, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

    jstring str = env->NewString(wtext, len);
    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, str);
    jni_jvm_printExceptionStack(env);
    return (bool) ret;
}

cell n_OnPlayerRequestSpawn(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerRequestSpawn", "(I)Z");
    if (!jmid) return 0;

    int playerid = params[1];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnObjectMoved(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onObjectMoved", "(I)Z");
    if (!jmid) return 0;

    int objectid = params[1];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, objectid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerObjectMoved(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerObjectMoved", "(II)Z");
    if (!jmid) return 0;

    int playerid = params[1], objectid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, objectid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerPickUpPickup(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerPickUpPickup", "(II)Z");
    if (!jmid) return 0;

    int playerid = params[1], pickupid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, pickupid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnVehicleMod(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onVehicleMod", "(III)Z");
    if (!jmid) return 0;

    int playerid = params[1], vehicleid = params[2], componentid = params[3];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, vehicleid, componentid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnEnterExitModShop(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onEnterExitModShop", "(III)Z");
    if (!jmid) return 0;

    int playerid = params[1], enterexit = params[2], interiorid = params[3];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, enterexit, interiorid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnVehiclePaintjob(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onVehiclePaintjob", "(III)Z");
    if (!jmid) return 0;

    int playerid = params[1], vehicleid = params[2], paintjobid = params[3];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, vehicleid, paintjobid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnVehicleRespray(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onVehicleRespray", "(IIII)Z");
    if (!jmid) return 0;

    int playerid = params[1], vehicleid = params[2], color1 = params[3], color2 = params[4];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, vehicleid, color1,
                                          color2);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnVehicleDamageStatusUpdate(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onVehicleDamageStatusUpdate",
                                             "(II)Z");
    if (!jmid) return 0;

    int vehicleid = params[1], playerid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, vehicleid, playerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnUnoccupiedVehicleUpdate(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onUnoccupiedVehicleUpdate",
                                             "(IIIFFFFFF)Z");
    if (!jmid) return 0;

    int vehicleid = params[1], playerid = params[2], passenger_seat = params[3];
    float new_x = amx_ctof(params[4]), new_y = amx_ctof(params[5]), new_z = amx_ctof(params[6]), vel_x = amx_ctof(
            params[7]), vel_y = amx_ctof(params[8]), vel_z = amx_ctof(params[9]);

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, vehicleid, playerid,
                                          passenger_seat, new_x,
                                          new_y, new_z, vel_x, vel_y, vel_z);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerSelectedMenuRow(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerSelectedMenuRow", "(II)Z");
    if (!jmid) return 0;

    int playerid = params[1], row = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, row);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerExitedMenu(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerExitedMenu", "(I)Z");
    if (!jmid) return 0;

    int playerid = params[1];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerInteriorChange(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerInteriorChange", "(III)Z");
    if (!jmid) return 0;

    int playerid = params[1], newinteriorid = params[2], oldinteriorid = params[3];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, newinteriorid,
                                          oldinteriorid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerKeyStateChange(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerKeyStateChange", "(III)Z");
    if (!jmid) return 0;

    int playerid = params[1], newkeys = params[2], oldkeys = params[3];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, newkeys, oldkeys);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnRconLoginAttempt(AMX *amx, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onRconLoginAttempt",
                                             "(Ljava/lang/String;Ljava/lang/String;I)I");
    if (!jmid) return 0;

    char ip[16], password[64];
    amx_GetString(amx, params[1], ip, sizeof ip);
    amx_GetString(amx, params[2], password, sizeof password);

    int success = params[3];

    jstring iptext = env->NewStringUTF(ip);

    jchar wtext[1024];
    int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), password, -1, wtext,
                      sizeof(wtext) / sizeof(wtext[0]));
    jstring str = env->NewString(wtext, len);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, iptext, str, success);
    jni_jvm_printExceptionStack(env);
    return (bool) ret;
}

cell n_OnPlayerUpdate(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerUpdate", "(I)Z");
    if (!jmid) return 0;

    int playerid = params[1];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerStreamIn(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerStreamIn", "(II)Z");
    if (!jmid) return 0;

    int playerid = params[1], forplayerid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, forplayerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerStreamOut(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerStreamOut", "(II)Z");
    if (!jmid) return 0;

    int playerid = params[1], forplayerid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, forplayerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnVehicleStreamIn(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onVehicleStreamIn", "(II)Z");
    if (!jmid) return 0;

    int vehicleid = params[1], forplayerid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, vehicleid, forplayerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnVehicleStreamOut(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onVehicleStreamOut", "(II)Z");
    if (!jmid) return 0;

    int vehicleid = params[1], forplayerid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, vehicleid, forplayerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnDialogResponse(AMX *amx, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onDialogResponse",
                                             "(IIIILjava/lang/String;)I");
    if (!jmid) return 0;

    char inputtext[1024];
    amx_GetString(amx, params[5], inputtext, sizeof inputtext);

    int playerid = params[1], dialogid = params[2], response = params[3], listitem = params[4];

    jchar wtext[1024];
    int len = mbs2wcs((unsigned int) shoebill.getPlayerCodepage(playerid), inputtext, -1, wtext,
                      sizeof(wtext) / sizeof(wtext[0]));

    jstring str = env->NewString(wtext, len);
    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, dialogid, response, listitem,
                                  str);
    jni_jvm_printExceptionStack(env);
    return (bool) ret;
}

cell n_OnPlayerTakeDamage(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerTakeDamage", "(IIFII)Z");
    if (!jmid) return 0;

    float amount = amx_ctof(params[3]);
    int playerid = params[1], issuerid = params[2], weaponid = params[4], bodypart = params[5];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, issuerid, amount,
                                          weaponid, bodypart);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerGiveDamage(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerGiveDamage", "(IIFII)Z");
    if (!jmid) return 0;

    int playerid = params[1], damagedid = params[2], weaponid = params[4], bodypart = params[5];
    float amount = amx_ctof(params[3]);

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, damagedid, amount,
                                          weaponid, bodypart);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerClickMap(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerClickMap", "(IFFF)Z");
    if (!jmid) return 0;

    int playerid = params[1];
    float x = amx_ctof(params[2]), y = amx_ctof(params[3]), z = amx_ctof(params[4]);

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, x, y, z);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerClickTextDraw(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerClickTextDraw", "(II)Z");
    if (!jmid) return 0;

    int playerid = params[1], clickedid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, clickedid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerClickPlayerTextDraw(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerClickPlayerTextDraw",
                                             "(II)Z");
    if (!jmid) return 0;

    int playerid = params[1], playertextid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, playertextid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerClickPlayer(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerClickPlayer", "(III)Z");
    if (!jmid) return 0;

    int playerid = params[1], clickedplayerid = params[2], source = params[3];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, clickedplayerid, source);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerEditObject(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerEditObject", "(IIIIFFFFFF)I");
    if (!jmid) return 0;

    int playerid = params[1], playerobject = params[2], objectid = params[3], response = params[4];
    float fX = amx_ctof(params[5]), fY = amx_ctof(params[6]), fZ = amx_ctof(params[7]), fRotX = amx_ctof(
            params[8]), fRotY = amx_ctof(params[9]), fRotZ = amx_ctof(params[10]);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, playerobject, objectid, response,
                                  fX, fY, fZ,
                                  fRotX, fRotY, fRotZ);
    jni_jvm_printExceptionStack(env);
    return (bool) ret;
}

cell n_OnPlayerEditAttachedObject(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerEditAttachedObject",
                                             "(IIIIIFFFFFFFFF)Z");
    if (!jmid) return 0;

    int playerid = params[1], response = params[2], index = params[3], modelid = params[4], boneid = params[5];
    float fOffsetX = amx_ctof(params[6]), fOffsetY = amx_ctof(params[7]), fOffsetZ = amx_ctof(
            params[8]), fRotX = amx_ctof(params[9]), fRotY = amx_ctof(params[10]), fRotZ = amx_ctof(
            params[11]), fScaleX = amx_ctof(params[12]), fScaleY = amx_ctof(params[13]), fScaleZ = amx_ctof(params[14]);

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, response, index, modelid,
                                          boneid,
                                          fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerSelectObject(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerSelectObject", "(IIIIFFF)Z");
    if (!jmid) return 0;

    int playerid = params[1], type = params[2], objectid = params[3], modelid = params[4];
    float fX = amx_ctof(params[5]), fY = amx_ctof(params[6]), fZ = amx_ctof(params[7]);

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, type, objectid, modelid,
                                          fX, fY, fZ);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerWeaponShot(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerWeaponShot", "(IIIIFFF)Z");
    if (!jmid) return 0;

    int playerid = params[1], weaponid = params[2], hittype = params[3], hitid = params[4];
    float fX = amx_ctof(params[5]), fY = amx_ctof(params[6]), fZ = amx_ctof(params[7]);

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, weaponid, hittype, hitid,
                                          fX, fY, fZ);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnIncomingConnection(AMX *amx, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onIncomingConnection",
                                             "(ILjava/lang/String;I)I");
    if (!jmid) return 0;

    int playerid = params[1], port = params[3];
    char ip_address[24];
    amx_GetString(amx, params[2], ip_address, sizeof ip_address);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, env->NewStringUTF(ip_address),
                                  port);
    jni_jvm_printExceptionStack(env);
    return (bool) ret;
}

cell n_OnTrailerUpdate(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onTrailerUpdate", "(II)Z");
    if (!jmid) return 0;

    int playerid = params[1], vehicleid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, vehicleid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnActorStreamIn(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;
    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onActorStreamIn", "(II)Z");
    if (!jmid) return 0;

    int actorid = params[1], forplayerid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, actorid, forplayerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnActorStreamOut(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;
    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onActorStreamOut", "(II)Z");
    if (!jmid) return 0;

    int actorid = params[1], forplayerid = params[2];

    jboolean ret = env->CallBooleanMethod(shoebill.getCallbackHandlerObject(), jmid, actorid, forplayerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

cell n_OnPlayerGiveDamageActor(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;
    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onPlayerGiveDamageActor", "(IIIII)I");
    if (!jmid) return 0;

    int playerid = params[1], damaged_actor = params[2], amount = (int) amx_ctof(
            params[3]), weapon = params[4], bodypart = params[5];

    int ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, damaged_actor, amount, weapon,
                                 bodypart);
    jni_jvm_printExceptionStack(env);
    return (bool) ret;
}

cell n_OnVehicleSirenStateChange(AMX *, cell *params)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;
    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onVehicleSirenStateChange", "(III)I");
    if (!jmid) return 0;

    int playerid = params[1], vehicleid = params[2], newstate = params[3];

    int ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, vehicleid, newstate);
    jni_jvm_printExceptionStack(env);
    return (bool) ret;
}

int OnAmxVehicleCreated(int vehicleid, int modelid, float x, float y, float z, float angle, int interiorid, int worldid,
                        int color1, int color2, int respawn_delay, int addsiren)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxVehicleCreated",
                                             "(IIFFFFIIIIII)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, vehicleid, modelid, x, y, z, angle,
                                  interiorid, worldid,
                                  color1, color2, respawn_delay, addsiren);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxDestroyVehicle(int vehicleid)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxDestroyVehicle", "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, vehicleid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxSampObjectCreated(int objectId, int modelid, float x, float y, float z, float rX, float rY, float rZ,
                           int worldid, int interiorid, float render_distance)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxSampObjectCreated",
                                             "(IIFFFFFFIIF)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, objectId, modelid, x, y, z, rX, rY, rZ,
                                  worldid,
                                  interiorid, render_distance);
    jni_jvm_printExceptionStack(env);

    return ret;
}

int OnAmxSampObjectDestroyed(int objectId)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxSampObjectDestroyed", "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, objectId);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxAttachObjectToVehicle(int objectId, int vehicleId, float x, float y, float z, float rX, float rY, float rZ)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxAttachObjectToVehicle",
                                             "(IIFFFFFF)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, objectId, vehicleId, x, y, z, rX, rY, rZ);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxAttachObjectToPlayer(int objectId, int playerId, float x, float y, float z, float rX, float rY, float rZ)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxAttachObjectToPlayer",
                                             "(IIFFFFFF)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, objectId, playerId, x, y, z, rX, rY, rZ);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxAttachObjectToObject(int objectId, int other_object_Id, float x, float y, float z, float rX, float rY,
                              float rZ)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxAttachObjectToObject",
                                             "(IIFFFFFF)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, objectId, other_object_Id, x, y, z, rX, rY,
                                  rZ);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxCreatePlayerObject(int playerid, int modelid, float x, float y, float z, float rX, float rY, float rZ,
                            float drawdistance, int worldid, int interiorid, int returnedValue)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxCreatePlayerObject",
                                             "(IIFFFFFFIIFI)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, modelid, x, y, z, rX, rY, rZ,
                                  drawdistance,
                                  worldid, interiorid, returnedValue);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxDestroyPlayerObject(int playerid, int objectid)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxDestroyPlayerObject", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, objectid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxSetPlayerAttachedObject(int playerid, int index, int modelid, int bone, float offsetX, float offsetY,
                                 float offsetZ, float rotX, float rotY, float rotZ, float scaleX, float scaleY,
                                 float scaleZ, int materialcolor1, int materialcolor2)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxSetPlayerAttachedObject",
                                             "(IIIIFFFFFFFFFII)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, index, modelid, bone, offsetX,
                                  offsetY,
                                  offsetZ, rotX, rotY, rotZ, scaleX, scaleY, scaleZ, materialcolor1, materialcolor2);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxRemovePlayerAttachedObject(int playerid, int index)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxRemovePlayerAttachedObject",
                                             "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, index);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxDestroyPickup(int pickupid)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxDestroyPickup", "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, pickupid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxCreatePickup(int model, int type, float posX, float posY, float posZ, int virtualworld, int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxCreatePickup", "(IIFFFII)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, model, type, posX, posY, posZ,
                                  virtualworld, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxAddStaticPickup(int model, int type, float posX, float posY, float posZ, int virtualworld)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxAddStaticPickup", "(IIFFFI)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, model, type, posX, posY, posZ,
                                  virtualworld);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxCreateLabel(char *text, int color, float posX, float posY, float posZ, float drawDistance, int virtualworld,
                     int testLOS, int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxCreateLabel",
                                             "(Ljava/lang/String;IFFFFIII)I");
    if (!jmid) return 0;

    jchar wtext[1024];
    int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), text, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

    jstring str = env->NewString(wtext, len);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, str, color, posX, posY, posZ, drawDistance,
                                  virtualworld,
                                  testLOS, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxDeleteLabel(int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxDeleteLabel", "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxAttachLabelToPlayer(int id, int playerid, float offsetX, float offsetY, float offsetZ)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxAttachLabelToPlayer",
                                             "(IIFFF)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, id, playerid, offsetX, offsetY, offsetZ);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxAttachLabelToVehicle(int id, int vehicleid, float offsetX, float offsetY, float offsetZ)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxAttachLabelToVehicle",
                                             "(IIFFF)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, id, vehicleid, offsetX, offsetY, offsetZ);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxUpdateLabel(int id, int color, char *text)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxUpdateLabel",
                                             "(IILjava/lang/String;)I");
    if (!jmid) return 0;

    jchar wtext[1024];
    int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), text, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

    jstring str = env->NewString(wtext, len);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, id, color, str);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxCreatePlayerLabel(int playerid, char *text, int color, float posX, float posY, float posZ, float drawDistance,
                           int attachedPlayer, int attachedVehicle, int testLOS, int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxCreatePlayerLabel",
                                             "(ILjava/lang/String;IFFFFIIII)I");
    if (!jmid) return 0;

    jchar wtext[1024];
    int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), text, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

    jstring str = env->NewString(wtext, len);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, str, color, posX, posY, posZ,
                                  drawDistance,
                                  attachedPlayer, attachedVehicle, testLOS, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxDeletePlayerLabel(int playerid, int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxDeletePlayerLabel", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxUpdatePlayerLabel(int playerid, int id, int color, char *text)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxUpdatePlayerLabel",
                                             "(IIILjava/lang/String;)I");
    if (!jmid) return 0;

    jchar wtext[1024];
    int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), text, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

    jstring str = env->NewString(wtext, len);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, id, color, str);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxCreateMenu(char *title, int columns, float x, float y, float col1Width, float col2Width, int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxCreateMenu",
                                             "(Ljava/lang/String;IFFFFI)I");
    if (!jmid) return 0;

    jchar wtext[32];
    int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), title, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

    jstring str = env->NewString(wtext, len);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, str, columns, x, y, col1Width, col2Width,
                                  id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxSetMenuColumnHeader(int id, int column, char *text)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxSetMenuColumnHeader",
                                             "(IILjava/lang/String;)I");
    if (!jmid) return 0;

    jchar wtext[32];
    int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), text, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

    jstring str = env->NewString(wtext, len);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, id, column, str);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxDestroyMenu(int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxDestroyMenu", "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxGangZoneCreate(float minX, float minY, float maxX, float maxY, int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxGangZoneCreate", "(FFFFI)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, minX, minY, maxX, maxY, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxGangZoneDestroy(int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxGangZoneDestroy", "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxGangZoneShowForPlayer(int playerid, int zone, int color)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxGangZoneShowForPlayer",
                                             "(III)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, zone, color);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxGangZoneShowForAll(int zone, int color)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxGangZoneShowForAll", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, zone, color);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxGangZoneHideForPlayer(int playerid, int zone)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxGangZoneHideForPlayer", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, zone);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxGangZoneHideForAll(int zone)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxGangZoneHideForAll", "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, zone);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxGangZoneFlashForPlayer(int playerid, int zone, int flashColor)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxGangZoneFlashForPlayer",
                                             "(III)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, zone, flashColor);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxGangZoneFlashForAll(int zone, int flashColor)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxGangZoneFlashForAll", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, zone, flashColor);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxGangZoneStopFlashForPlayer(int playerid, int zone)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxGangZoneStopFlashForPlayer",
                                             "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, zone);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxGangZoneStopFlashForAll(int zone)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxGangZoneStopFlashForAll",
                                             "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, zone);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxSetSkillLevel(int playerid, int skill, int level)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxSetSkillLevel", "(III)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, skill, level);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxSetPlayerMapIcon(int playerid, int iconid, float x, float y, float z, int markertype, int color, int style)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxSetPlayerMapIcon",
                                             "(IIFFFIII)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, iconid, x, y, z, markertype,
                                  color, style);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxRemovePlayerMapIcon(int playerid, int iconid)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxRemovePlayerMapIcon", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, iconid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxShowPlayerDialog(int playerid, int dialogid, int style, char *caption, char *info, char *button1,
                          char *button2)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxShowPlayerDialog",
                                             "(IIILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
    if (!jmid) return 0;

    unsigned int serverCodepage = (unsigned int) shoebill.getServerCodepage();

    jchar wCaption[64];
    int len = mbs2wcs(serverCodepage, caption, -1, wCaption,
                      sizeof(wCaption) / sizeof(wCaption[0]));
    jstring jCaption = env->NewString(wCaption, len);

    jchar wInfo[1024];
    int len2 = mbs2wcs(serverCodepage, info, -1, wInfo, sizeof(wInfo) / sizeof(wInfo[0]));
    jstring jInfo = env->NewString(wInfo, len2);

    jchar wButton1[32];
    int len3 = mbs2wcs(serverCodepage, button1, -1, wButton1,
                       sizeof(wButton1) / sizeof(wButton1[0]));
    jstring jButton1 = env->NewString(wButton1, len3);

    jchar wButton2[32];
    int len4 = mbs2wcs(serverCodepage, button2, -1, wButton2,
                       sizeof(wButton2) / sizeof(wButton2[0]));
    jstring jButton2 = env->NewString(wButton2, len4);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, dialogid, style, jCaption, jInfo,
                                  jButton1,
                                  jButton2);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxSetPlayerWorldBounds(int playerid, float minX, float minY, float maxX, float maxY)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxSetPlayerWorldBounds",
                                             "(IFFFF)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, minX, minY, maxX, maxY);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxSetPlayerWeather(int playerid, int weatherid)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxSetPlayerWeather", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, weatherid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxSetPlayerCheckpoint(int playerid, float x, float y, float z, float size)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxSetPlayerCheckpoint",
                                             "(IFFFF)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, x, y, z, size);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxDisablePlayerCheckpoint(int playerid)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxDisablePlayerCheckpoint",
                                             "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxSetPlayerRaceCheckpoint(int playerid, int type, float x, float y, float z, float nextX, float nextY,
                                 float nextZ, float size)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxSetPlayerRaceCheckpoint",
                                             "(IIFFFFFFF)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, type, x, y, z, nextX, nextY,
                                  nextZ, size);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxDisablePlayerRaceCheckpoint(int playerid)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxDisablePlayerRaceCheckpoint",
                                             "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxTogglePlayerSpectating(int playerid, int toggle)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxTogglePlayerSpectating",
                                             "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, toggle);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxPlayerSpectatePlayer(int playerid, int target, int mode)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxPlayerSpectatePlayer", "(III)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, target, mode);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxPlayerSpectateVehicle(int playerid, int target, int mode)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxPlayerSpectateVehicle",
                                             "(III)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, target, mode);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxEnableStuntBonusForPlayer(int playerid, int toggle)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxEnableStuntBonusForPlayer",
                                             "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, toggle);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxStartRecording(int playerid, int type, char *recordName)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxStartRecording",
                                             "(IILjava/lang/String;)I");
    if (!jmid) return 0;

    jchar wstring[128];
    int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), recordName, -1, wstring,
                      sizeof(wstring) / sizeof(wstring[0]));
    jstring str = env->NewString(wstring, len);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, type, str);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxStopRecording(int playerid)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxStopRecording", "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxToggleControllabel(int playerid, int toggle)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxToggleControllable", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, toggle);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxTextDrawCreate(float x, float y, char *text, int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxTextDrawCreate",
                                             "(FFLjava/lang/String;I)I");
    if (!jmid) return 0;

    jchar wstring[1024];
    int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), text, -1, wstring,
                      sizeof(wstring) / sizeof(wstring[0]));
    jstring str = env->NewString(wstring, len);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, x, y, str, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxTextDrawDestroy(int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxTextDrawDestroy", "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxTextDrawSetString(int id, char *text)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxTextDrawSetString",
                                             "(ILjava/lang/String;)I");
    if (!jmid) return 0;

    jchar wstring[1024];
    int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), text, -1, wstring,
                      sizeof(wstring) / sizeof(wstring[0]));
    jstring str = env->NewString(wstring, len);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, id, str);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxTextDrawShowForPlayer(int playerid, int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxTextDrawShowForPlayer", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxTextDrawHideForPlayer(int playerid, int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxTextDrawHideForPlayer", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxTextDrawShowForAll(int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxTextDrawShowForAll", "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxTextDrawHideForAll(int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxTextDrawHideForAll", "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxCreatePlayerTextDraw(int playerid, float x, float y, char *text, int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxCreatePlayerTextDraw",
                                             "(IFFLjava/lang/String;I)I");
    if (!jmid) return 0;

    jchar wstring[1024];
    int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), text, -1, wstring,
                      sizeof(wstring) / sizeof(wstring[0]));
    jstring str = env->NewString(wstring, len);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, x, y, str, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxPlayerTextDrawDestroy(int playerid, int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxPlayerTextDrawDestroy", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxPlayerTextDrawSetString(int playerid, int id, char *text)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxPlayerTextDrawSetString",
                                             "(IILjava/lang/String;)I");
    if (!jmid) return 0;

    jchar wstring[1024];
    int len = mbs2wcs((unsigned int) shoebill.getServerCodepage(), text, -1, wstring,
                      sizeof(wstring) / sizeof(wstring[0]));
    jstring str = env->NewString(wstring, len);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, id, str);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxPlayerTextDrawShow(int playerid, int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxPlayerTextDrawShow", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxPlayerTextDrawHide(int playerid, int id)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxPlayerTextDrawHide", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, id);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxAddVehicleComponent(int vehicleid, int componentid)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxAddVehicleComponent", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, vehicleid, componentid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxLinkVehicleToInterior(int vehicleid, int interiorid)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxLinkVehicleToInterior", "(II)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, vehicleid, interiorid);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxChangeVehicleColor(int vehicleid, int color1, int color2)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxChangeVehicleColor", "(III)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, vehicleid, color1, color2);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxCreateActor(int actorid, int modelid, float x, float y, float z, float rotation)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxCreateActor", "(IIFFFF)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, actorid, modelid, x, y, z, rotation);
    jni_jvm_printExceptionStack(env);
    return ret;
}

int OnAmxDestroyActor(int actorid)
{
    if (!shoebill.getCallbackHandlerObject()) return 0;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onAmxDestroyActor", "(I)I");
    if (!jmid) return 0;

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, actorid);
    jni_jvm_printExceptionStack(env);
    return ret;
}