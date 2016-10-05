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
    if (it != callbackMap.end()) {
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
    if (it != callbackCancelValues.end()) {
        return it->second == returnValue;
    }
    return false;
}

std::map<std::string, std::vector<std::string>> hookedCallbacks;
Shoebill &shoebill = Shoebill::GetInstance();

bool HookCallback(std::string name, std::vector<std::string> types)
{
    hookedCallbacks[name] = types;
    return true;
}

bool UnhookCallback(std::string name)
{
    auto it = hookedCallbacks.find(name);
    if (it == hookedCallbacks.end())
        return false;
    hookedCallbacks.erase(name);
    return true;
}

cell AMX_NATIVE_CALL CallShoebillFunction(AMX *amx, cell *params)
{
    char function[64];
    amx_GetString(amx, params[1], function, 64);
    std::string functionName = std::string(function);
    if (!AmxInstanceManager::GetInstance().RegisteredFunctionExists(amx, functionName)) {
        sampgdk_logprintf("[SHOEBILL] Function %s is not registered.", function);
        return -1;
    }
    auto types = AmxInstanceManager::GetInstance().GetParameterTypes(amx, functionName);
    signed int shouldBeCount = 0;
    int parameterCount = (int) (params[0] / sizeof(cell));
    for (unsigned int i = 0; i < types.size(); i++) {
        auto type = types[i];
        if (type.find("[") != std::string::npos) shouldBeCount += 2;
        else shouldBeCount += 1;
    }
    if (parameterCount - 1 != shouldBeCount) {
        sampgdk_logprintf("[SHOEBILL] Calling %s with %i parameters, but was expecting %i parameters.", function,
                          parameterCount - 1, shouldBeCount);
        return -1;
    }

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    auto objectArray = makeJavaObjectArray(env, shouldBeCount);
    auto globalObjectArray = env->NewGlobalRef(objectArray);

    std::vector<cell *> referenceValues;

    for (unsigned int i = 0, p = 2; i < types.size(); i++) {
        cell *phys_addr;
        amx_GetAddr(amx, params[p], &phys_addr);
        referenceValues.push_back(phys_addr);

        if (types[i] == "s") {
            char parameterString[1024];
            amx_GetString(amx, params[p], parameterString, 1024);
            env->SetObjectArrayElement(objectArray, i,
                                       mbs2wcs(env, shoebill.getServerCodepage(), parameterString, 1024));
            p += 1;
        } else if (types[i] == "i") {
            env->SetObjectArrayElement(objectArray, i, makeJavaInteger(env, *phys_addr));
            p += 1;
        } else if (types[i] == "f") {
            env->SetObjectArrayElement(objectArray, i, makeJavaFloat(env, amx_ctof(*phys_addr)));
            p += 1;
        }
            /*else if (types[i] == "s[]")
            {
                cell* length_addr;
                amx_GetAddr(amx, params[p + 1], &length_addr);

                auto arrayLength = *length_addr;
                auto javaArray = makeJavaObjectArray(env, arrayLength);

                char parameterString[1024];

                for (auto a = 0; a < arrayLength; a++)
                {
                    sampgdk_logprintf("Getting string for #%i of %i", a, arrayLength);

                    amx_GetString(parameterString, (phys_addr + a * 128 + 4), 0, 1024);
                    sampgdk_logprintf("Got string for #%i: %s", a, parameterString);
                    env->SetObjectArrayElement(javaArray, a, mbs2wcs(env, shoebill.getServerCodepage(), parameterString, 1024));
                }
                env->SetObjectArrayElement(objectArray, i, javaArray);
                p += 2;
            }*/
        else if (types[i] == "i[]") {
            cell *length_addr;
            amx_GetAddr(amx, params[p + 1], &length_addr);

            int arrayLength = *length_addr;
            auto javaArray = makeJavaIntArray(env, arrayLength);

            for (auto a = 0; a < arrayLength; a++) {
                jint elements[] = {*(phys_addr + a)};
                env->SetIntArrayRegion(javaArray, a, 1, elements);
            }
            env->SetObjectArrayElement(objectArray, i, javaArray);
            p += 2;
        } else if (types[i] == "f[]") {
            cell *length_addr;
            amx_GetAddr(amx, params[p + 1], &length_addr);

            auto arrayLength = *length_addr;
            auto javaArray = makeJavaFloatArray(env, arrayLength);

            for (auto a = 0; a < arrayLength; a++) {
                jfloat elements[] = {amx_ctof(*(phys_addr + a))};
                env->SetFloatArrayRegion(javaArray, a, 1, elements);
            }

            env->SetObjectArrayElement(objectArray, i, javaArray);
            p += 2;
        }
    }
    auto result = shoebill.CallRegisteredFunction(amx, std::string(function), objectArray);
    for (unsigned int i = 0, p = 2; i < referenceValues.size(); i++) {
        if (types[i] == "s") {
            auto string = static_cast<jstring>(env->GetObjectArrayElement(objectArray, i));

            char *str = wcs2mbs(env, shoebill.getServerCodepage(), string, 1024);
            //TODO: check if delete is needed.
            amx_SetString(referenceValues[i], str, 0, 0, 1024);
            p += 1;
        } else if (types[i] == "i") {
            auto integer = getIntegerFromObject(env, env->GetObjectArrayElement(objectArray, i));
            *referenceValues[i] = integer;
            p += 1;
        } else if (types[i] == "f") {
            auto value = getFloatFromObject(env, env->GetObjectArrayElement(objectArray, i));
            *referenceValues[i] = amx_ftoc(value);
            p += 1;
        }
            /*else if (types[i] == "s[]")
            {
                auto stringArrayObject = static_cast<jobjectArray>(env->GetObjectArrayElement(objectArray, i));
                int arrayLength = env->GetArrayLength(stringArrayObject);
                if (arrayLength > params[p + 1])
                    sampgdk_logprintf("[SHOEBILL] %s has a bigger array than pawn. New values are ignored.", function);

                cell* pawnArray = referenceValues[i];
                for (int a = 0; a < arrayLength; a++) {
                    auto stringObject = static_cast<jstring>(env->GetObjectArrayElement(stringArrayObject, a));
                    auto string = wcs2mbs(env, shoebill.getServerCodepage(), stringObject, 1024);
                    amx_SetString(pawnArray + a, string, 0, 0, 1024);
                    //TODO: check if it needs a delete.
                }

                p += 2;
            }*/
        else if (types[i] == "i[]") {
            auto intArrayObject = static_cast<jintArray>(env->GetObjectArrayElement(objectArray, i));
            int arrayLength = env->GetArrayLength(intArrayObject);
            if (arrayLength > params[p + 1])
                sampgdk_logprintf("[SHOEBILL] %s has a bigger array than pawn. New values are ignored.", function);

            cell *pawnArray = referenceValues[i];
            jint *elements = env->GetIntArrayElements(intArrayObject, nullptr);
            for (int a = 0; a < arrayLength; a++)
                *(pawnArray + a) = elements[a];

            env->ReleaseIntArrayElements(intArrayObject, elements, 0);
            p += 2;
        } else if (types[i] == "f[]") {
            auto floatArrayObject = static_cast<jfloatArray>(env->GetObjectArrayElement(objectArray, i));
            int arrayLength = env->GetArrayLength(floatArrayObject);
            if (arrayLength > params[p + 1])
                sampgdk_logprintf("[SHOEBILL] %s has a bigger array than pawn. New values are ignored.", function);

            cell *pawnArray = referenceValues[i];
            jfloat *elements = env->GetFloatArrayElements(floatArrayObject, nullptr);
            for (int a = 0; a < arrayLength; a++) {
                *(pawnArray + a) = amx_ftoc(elements[a]);
            }
            env->ReleaseFloatArrayElements(floatArrayObject, elements, 0);
            p += 2;
        }
    }
    env->DeleteGlobalRef(globalObjectArray);
    return result;
}

std::array<int, 2> CallHookedCallback(AMX *amx, std::string name, cell *params, bool &success)
{
    auto it = hookedCallbacks.find(name);
    auto result = std::array<int, 2>();
    success = false;
    if (it == hookedCallbacks.end()) return result;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);
    if (!env) return result;

    std::vector<std::string> types = it->second;
    unsigned int shouldBeCount = 0;
    for (unsigned int i = 0; i < types.size(); i++) {
        auto type = types[i];
        if (type.find("[") != std::string::npos) shouldBeCount += 2;
        else shouldBeCount += 1;
    }
    unsigned int count = params[0] / sizeof(cell);
    if (count != shouldBeCount) {
        sampgdk_logprintf("[SHOEBILL] %s did not equal parameter count! Actual: %i | Expected: %i", name.c_str(), count,
                          shouldBeCount);
        return result;
    }

    auto objectArray = makeJavaObjectArray(env, count);

    for (unsigned int i = 0, p = 1; i < types.size(); ++i) {
        std::string paramType = types[i];
        if (params[p] == 0)
            sampgdk_logprintf("[SHOEBILL] %s doesn't contain a value for parameter %i which should be of type '%s'.",
                              name.c_str(), i, paramType.c_str());

        if (paramType == "s") {
            char text[1024];
            amx_GetString(amx, params[p], text, sizeof(text));
            env->SetObjectArrayElement(objectArray, i, mbs2wcs(env, shoebill.getServerCodepage(), text, 1024));
            p += 1;
        } else if (paramType == "i") {
            env->SetObjectArrayElement(objectArray, i, makeJavaInteger(env, params[p]));
            p += 1;
        } else if (paramType == "f") {
            float value = amx_ctof(params[p]);
            env->SetObjectArrayElement(objectArray, i, makeJavaFloat(env, value));
            p += 1;
        } else if (paramType.find("[]") != std::string::npos) {
            cell *address = NULL;
            amx_GetAddr(amx, params[p], &address);
            int arrayLength = params[p + 1];
            auto array = makeJavaObjectArray(env, arrayLength);
            for (int a = 0; a < arrayLength; a++) {
                if (paramType == "i[]")
                    env->SetObjectArrayElement(array, a, makeJavaInteger(env, *(address + a)));
                else if (paramType == "f[]")
                    env->SetObjectArrayElement(array, a, makeJavaFloat(env, amx_ctof(*(address + a))));
                    /*else if(paramType == "s[]")
                    {
                        char text[1024];
                        amx_GetString(amx, *(address + a), text, sizeof(text));
                        env->SetObjectArrayElement(array, a, mbs2wcs(env, shoebill.getServerCodepage(), text, 1024));
                    }*/
                else {
                    sampgdk_logprintf("[SHOEBILL] Unknown parameter type '%s' for callback %s.", paramType.c_str(),
                                      name.c_str());
                    return result;
                }
            }
            env->SetObjectArrayElement(objectArray, i, array);
            p += 2;
        } else {
            sampgdk_logprintf("[SHOEBILL] Unknown parameter type '%s' for callback %s.", paramType.c_str(),
                              name.c_str());
            return result;
        }
    }

    static jmethodID jmid = env->GetMethodID(shoebill.getCallbackHandlerClass(), "onHookCall",
                                             "(Ljava/lang/String;[Ljava/lang/Object;)[I");
    if (!jmid) return result;
    jintArray event = (jintArray) env->CallObjectMethod(shoebill.getCallbackHandlerObject(), jmid,
                                                        mbs2wcs(env, shoebill.getServerCodepage(), name.c_str(), 64),
                                                        objectArray);
    jni_jvm_printExceptionStack(env);
    jint *values = env->GetIntArrayElements(event, NULL);
    result[0] = values[0];
    result[1] = values[1];
    env->ReleaseIntArrayElements(event, values, 0);
    success = true;
    return result;
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
    int len = mbs2wcs(shoebill.getPlayerCodepage(playerid), text, -1, wtext,
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

    jstring cmd = mbs2wcs(env, shoebill.getPlayerCodepage(playerid), cmdtext, 1024);

    jint ret = env->CallIntMethod(shoebill.getCallbackHandlerObject(), jmid, playerid, cmd);
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
    int len = mbs2wcs(shoebill.getServerCodepage(), cmd, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

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
    int len = mbs2wcs(shoebill.getServerCodepage(), password, -1, wtext,
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
    int len = mbs2wcs(shoebill.getPlayerCodepage(playerid), inputtext, -1, wtext,
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