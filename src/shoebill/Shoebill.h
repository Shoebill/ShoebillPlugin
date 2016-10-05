//
// Created by marvin on 03.04.16.
//

#ifndef SHOEBILLPLUGIN_SHOEBILL_H
#define SHOEBILLPLUGIN_SHOEBILL_H

#include <jni.h>
#include <set>
#include "AmxInstanceManager.hpp"
#include "AmxHelper.h"

#include "EncodingUtils.h"
#include "JniUtils.h"
#include <Callbacks.h>
#include <sampgdk.h>

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

const char JVM_CLASSPATH_SEARCH_PATH[] = "./shoebill/bootstrap/shoebill-launcher-2.0*.jar";
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

    int Start();

    int Restart();

    void OnShoebillLoad() const;

    bool OnPluginLoad();

    bool OnPluginUnload();

    void OnProcessTick() const;

    void OnAmxLoad(AMX *amx) const;

    void OnAmxUnload(AMX *amx) const;

    int CallRegisteredFunction(AMX *, std::string, jobjectArray) const;

    void setPlayerCodepage(int playerid, unsigned int codepage)
    {
        playerCodepage[playerid] = codepage;
    }

    unsigned int getPlayerCodepage(int playerid)
    {
        unsigned int codepage = playerCodepage[playerid];
        return codepage == 0 ? serverCodepage : codepage;
    }

    void setServerCodepage(unsigned int codepage)
    {
        serverCodepage = codepage;
    }

    unsigned int getServerCodepage() const
    {
        return serverCodepage;
    }

    jobject getCallbackHandlerObject() const
    { return callbackHandlerObject; }

    jclass getCallbackHandlerClass() const
    { return callbackHandlerClass; }

private:
    bool initialized;
    unsigned int serverCodepage = 1252;
    unsigned int playerCodepage[MAX_PLAYERS] = {0};

    jclass shoebillLauncherClass = NULL;
    jclass callbackHandlerClass = NULL;
    jobject callbackHandlerObject = NULL;

    jclass shoebillClass = NULL;
    jobject shoebillObject = NULL;
};


#endif //SHOEBILLPLUGIN_SHOEBILL_H
