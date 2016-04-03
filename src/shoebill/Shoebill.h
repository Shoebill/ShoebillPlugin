//
// Created by marvin on 03.04.16.
//

#ifndef SHOEBILLPLUGIN_SHOEBILL_H
#define SHOEBILLPLUGIN_SHOEBILL_H

#include <jni.h>
#include <set>
#include "AmxInstanceManager.hpp"
#include "AmxHelper.h"
#include "NativeFunctionManager.h"

#include "EncodingUtils.h"
#include "JniUtils.h"
#include <Callbacks.h>

#if defined(LINUX)

#include <map>
#include <fstream>

extern std::map<int, std::string> codepages;
#endif //LINUX

#ifndef SHOEBILL_DEBUG
#define LOG(x)
#else
#define LOG(x) sampgdk_logprintf(x)
#endif

const char CODEPAGE_FILE_PATH[] = "./shoebill/codepages.txt";
const char JVM_OPTION_FILE_PATH[] = "./shoebill/jvm_options.txt";

const char JVM_CLASSPATH_SEARCH_PATH[] = "./shoebill/bootstrap/shoebill-launcher-1.3*.jar";
const char LAUNCHER_CLASS_NAME[] = "net/gtaun/shoebill/ShoebillLauncher";

const char RESOLVE_DEPENDENCIES_METHOD_NAME[] = "resolveDependencies";
const char RESOLVE_DEPENDENCIES_METHOD_SIGN[] = "()Ljava/lang/Object;";

const char LOAD_NATIVE_LIBRARY_METHOD_NAME[] = "loadNativeLibrary";
const char LOAD_NATIVE_LIBRARY_METHOD_SIGN[] = "()V";

const char CREATE_SHOEBILL_METHOD_NAME[] = "createShoebill";
const char CREATE_SHOEBILL_METHOD_SIGN[] = "(Ljava/lang/Object;[I)Ljava/lang/Object;";

class Shoebill
{
public:
    static Shoebill &GetInstance();

    Shoebill();

    ~Shoebill();

    int Initialize(JNIEnv *);

    int Uninitialize(JNIEnv *);

    int CreateShoebillObject(JNIEnv *);

    int ReleaseShoebillObject(JNIEnv *);

    int StartShoebill();

    int RestartShoebill();

    void OnShoebillLoad();

    bool OnPluginLoad();

    bool OnPluginUnload();

    void OnProcessTick();

    void OnAmxLoad(AMX *amx);

    void OnAmxUnload(AMX *amx);

    int CallRegisteredFunction(AMX *, std::string, jobjectArray);

    void setPlayerCodepage(int playerid, int codepage)
    {
        playerCodepage[playerid] = codepage;
    }

    int getPlayerCodepage(int playerid)
    {
        int codepage = playerCodepage[playerid];
        return codepage == 0 ? serverCodepage : codepage;
    }

    void setServerCodepage(int codepage)
    {
        serverCodepage = codepage;
    }

    int getServerCodepage()
    {
        return serverCodepage;
    }

    jobject getCallbackHandlerObject()
    { return callbackHandlerObject; }

    jclass getCallbackHandlerClass()
    { return callbackHandlerClass; }

private:
    bool initialized;
    int serverCodepage = 1252;
    int playerCodepage[MAX_PLAYERS] = {0};

    jclass shoebillLauncherClass = NULL;
    jclass callbackHandlerClass = NULL;
    jobject callbackHandlerObject = NULL;

    jclass shoebillClass = NULL;
    jobject shoebillObject = NULL;
};


#endif //SHOEBILLPLUGIN_SHOEBILL_H
