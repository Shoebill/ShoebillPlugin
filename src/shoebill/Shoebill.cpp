//
// Created by marvin on 03.04.16.
//

#include "Shoebill.h"


Shoebill &Shoebill::GetInstance()
{
    static Shoebill instance;
    return instance;
}

Shoebill::Shoebill() : initialized(false) {}

Shoebill::~Shoebill() {}

int Shoebill::Initialize(JNIEnv *env)
{
    if (initialized)
    {
        LOG("[DEBUG SHOEBILL] Shoebill is already initialized.");
        return -2;
    }
    shoebillLauncherClass = env->FindClass(LAUNCHER_CLASS_NAME);
    if (!shoebillLauncherClass)
    {
        sampgdk_logprintf("  > Error: Can't find launcher class [%s].", LAUNCHER_CLASS_NAME);
        sampgdk_logprintf("  > Please make sure that you use the correct launcher for this plugin.");
        return -1;
    }

    static jmethodID loadNativeLibraryMethodID = env->GetStaticMethodID(shoebillLauncherClass,
                                                                        LOAD_NATIVE_LIBRARY_METHOD_NAME,
                                                                        LOAD_NATIVE_LIBRARY_METHOD_SIGN);
    if (!loadNativeLibraryMethodID)
    {
        sampgdk_logprintf("  > Error: Can't find launcher method [%s::%s%s].", LAUNCHER_CLASS_NAME,
                          LOAD_NATIVE_LIBRARY_METHOD_NAME, LOAD_NATIVE_LIBRARY_METHOD_SIGN);
        sampgdk_logprintf("  > Please make sure that you use the correct launcher for this plugin.");
        return -6;
    }

    env->CallStaticVoidMethod(shoebillLauncherClass, loadNativeLibraryMethodID);
    if (env->ExceptionCheck())
    {
        jni_jvm_printExceptionStack(env);
        sampgdk_logprintf("  > Error: Could not load the native library from the launcher class.");
        sampgdk_logprintf("  > Please make sure that you use the correct launcher for this plugin.");
        return -7;
    }
    shoebillLauncherClass = (jclass) (env->NewGlobalRef(shoebillLauncherClass));
    jvm->AttachCurrentThread((void **) &env, NULL);

    Start();
    initialized = true;
    return 0;
}

int Shoebill::CreateShoebillObject(JNIEnv *env)
{
    if (initialized)
    {
        LOG("[SHOEBILL DEBUG] Shoebill is already initialized.");
        return -9;
    }

    static jmethodID resolveDependenciesMethodID = env->GetStaticMethodID(shoebillLauncherClass,
                                                                          RESOLVE_DEPENDENCIES_METHOD_NAME,
                                                                          RESOLVE_DEPENDENCIES_METHOD_SIGN);
    if (!resolveDependenciesMethodID)
    {
        sampgdk_logprintf("  > Error: Can't find launcher method [%s::%s%s].", LAUNCHER_CLASS_NAME,
                          RESOLVE_DEPENDENCIES_METHOD_NAME, RESOLVE_DEPENDENCIES_METHOD_SIGN);
        sampgdk_logprintf("  > Please make sure that you use the correct launcher for this plugin.");
        return -8;
    }

    jobject context = env->CallStaticObjectMethod(shoebillLauncherClass, resolveDependenciesMethodID);
    if (!context)
    {
        jni_jvm_printExceptionStack(env);
        sampgdk_logprintf("  > Error: Shoebill couldn't resolve dependencies.");
        sampgdk_logprintf(
                "  > Please make sure that you use the correct launcher and dependency manager for this plugin.");
        return -2;
    }

    static jmethodID createShoebillMethodID = env->GetStaticMethodID(shoebillLauncherClass, CREATE_SHOEBILL_METHOD_NAME,
                                                                     CREATE_SHOEBILL_METHOD_SIGN);
    if (!createShoebillMethodID)
    {
        sampgdk_logprintf("  > Error: Can't find launcher method [%s::%s%s]", LAUNCHER_CLASS_NAME,
                          CREATE_SHOEBILL_METHOD_NAME,
                          CREATE_SHOEBILL_METHOD_SIGN);
        sampgdk_logprintf("  > Please make sure that you use the correct launcher for this plugin.");
        return -3;
    }

    std::unordered_set<AMX *> amxInstances = AmxInstanceManager::GetInstance().GetInstances();
    jsize size = amxInstances.size();
    jint *array = new jint[size];
    {
        int i = 0;
        for (std::unordered_set<AMX *>::iterator it = amxInstances.begin();
             it != amxInstances.end() && i < size; it++, i++)
        {
            array[i] = (jint) (*it);
        }
    }
    jintArray amxHandleArray = env->NewIntArray(size);
    env->SetIntArrayRegion(amxHandleArray, 0, size, array);
    delete[] array;

    shoebillObject = env->CallStaticObjectMethod(shoebillLauncherClass, createShoebillMethodID, context,
                                                 amxHandleArray);
    if (!shoebillObject)
    {
        jni_jvm_printExceptionStack(env);
        sampgdk_logprintf("  > Error: Couldn't create Shoebill object.");
        sampgdk_logprintf("  > Please make sure that you use the correct launcher for this plugin.");
        return -4;
    }

    shoebillClass = env->GetObjectClass(shoebillObject);
    if (!shoebillClass)
    {
        sampgdk_logprintf("  > Error: Couldn't get Shoebill class.");
        sampgdk_logprintf("  > Please make sure that you use the correct launcher for this plugin.");
        return -5;
    }

#if defined(LINUX)
    std::ifstream codepageFile(CODEPAGE_FILE_PATH, std::ifstream::in);
    if (codepageFile.is_open())
    {
        char input[256], charset[256];
        unsigned int code = 0;
        while (codepageFile.good())
        {
            codepageFile.getline(input, 256);
            sscanf(input, "%u %s", &code, charset);
            if (code && charset[0])
            {
                if (codepages.find(code) != codepages.end())
                    sampgdk_logprintf("  > Error: Codepage is already in use, %d=%s", code, codepages[code].c_str());
                else
                {
                    codepages[code] = std::string(charset);
                }
            }
            code = 0;
            charset[0] = 0;
        }
        codepageFile.close();
    }
    else
    {
        sampgdk_logprintf("  > Error: Can't open %s.", CODEPAGE_FILE_PATH);
    }
#endif

    static jmethodID getCallbackHandlerMethodID = env->GetMethodID(shoebillClass, "getCallbackHandler",
                                                                   "()Lnet/gtaun/shoebill/samp/SampCallbackHandler;");
    if (!getCallbackHandlerMethodID)
    {
        sampgdk_logprintf("  > Error: Couldn't find the main callbackHandler.");
        sampgdk_logprintf("  > Please make sure that you use the correct API and Runtime for this plugin.");
        return -6;
    }

    callbackHandlerObject = env->CallObjectMethod(shoebillObject, getCallbackHandlerMethodID);
    if (callbackHandlerObject == NULL)
    {
        sampgdk_logprintf("  > Error: Couldn't find the main callbackHandler.");
        sampgdk_logprintf("  > Please make sure that you use the correct API and Runtime for this plugin.");
        return -7;
    }

    shoebillObject = env->NewGlobalRef(shoebillObject);
    shoebillClass = (jclass) (env->NewGlobalRef(shoebillClass));
    callbackHandlerObject = env->NewGlobalRef(callbackHandlerObject);
    callbackHandlerClass = (jclass) (env->NewGlobalRef(env->GetObjectClass(callbackHandlerObject)));
    initialized = true;
    OnShoebillLoad();
    sampgdk_logprintf("  > Shoebill has been initialized successfully.");
    return 0;
}

int Shoebill::ReleaseShoebillObject(JNIEnv *env)
{
    if (!initialized)
    {
        LOG("[SHOEBILL DEBUG] Shoebill is not yet initialized!");
        return 0;
    }

    if (shoebillObject) env->DeleteGlobalRef(shoebillObject);
    shoebillObject = NULL;

    if (shoebillClass) env->DeleteGlobalRef(shoebillClass);
    shoebillClass = NULL;

    if (callbackHandlerObject) env->DeleteGlobalRef(callbackHandlerObject);
    callbackHandlerObject = NULL;

    if (callbackHandlerClass) env->DeleteGlobalRef(callbackHandlerClass);
    callbackHandlerClass = NULL;

    return 1;
}

int Shoebill::Uninitialize(JNIEnv *env)
{
    ReleaseShoebillObject(env);
    if (shoebillLauncherClass) env->DeleteGlobalRef(shoebillLauncherClass);
    shoebillLauncherClass = NULL;
    initialized = false;
#if defined(LINUX)
    codepages.clear(); //Clear Codepages
#endif
    return 0;
}

void Shoebill::OnShoebillLoad() const
{
    if (!callbackHandlerObject) return;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onShoebillLoad", "()V");
    if (!jmid) return;
    env->CallVoidMethod(callbackHandlerObject, jmid);
    jni_jvm_printExceptionStack(env);
}

bool Shoebill::OnPluginLoad()
{
    sampgdk_logprintf("  > Shoebill 2.0 NativePlugin for SA-MP 0.3.7 by MK124, JoJLlmAn & 123marvin123");

    char classpath[2048] = {0};
    if (findAndGenerateClassPath(JVM_CLASSPATH_SEARCH_PATH, classpath) < 0)
    {
        sampgdk_logprintf("  > Error: Can't find launcher library.");
        return false;
    }
    JNIEnv *env;
    auto createResult = jni_jvm_create(&env, classpath, JVM_OPTION_FILE_PATH);
    if (createResult < 0)
    {
        sampgdk_logprintf("  > Error: The Java VM could not be created.");
        switch (createResult) {
            case -1:
                sampgdk_logprintf("  > It looks like the JVM has already been initialized.");
                break;
            case -3:
                sampgdk_logprintf("  > Please make sure that you use Java 8 32 Bit (not 64 Bit) to run Shoebill.");
                break;
            default:
                sampgdk_logprintf("  > The reason is unknown (Error code %i).", createResult);
                break;
        }
        return false;
    }
    if (createResult == 0) {
        sampgdk_logprintf("  > The Java VM has been successfully created.");
        jvm->AttachCurrentThread((void **) &env, NULL);
        return Initialize(env) >= 0;
    }
    return false;
}

bool Shoebill::OnPluginUnload()
{
    if (!callbackHandlerObject) return false;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onShoebillUnload", "()V");
    if (!jmid) return false;

    env->CallVoidMethod(callbackHandlerObject, jmid);
    jni_jvm_printExceptionStack(env);
    Uninitialize(env);
    if (jni_jvm_destroy(env) >= 0)
    {
        sampgdk_logprintf("  > The Java VM has been successfully destroyed.");
        return true;
    }
    return false;
}

int Shoebill::Start()
{
    if (initialized)
    {
        LOG("[SHOEBILL DEBUG] Shoebill is already initialized.");
        return 0;
    }
    JNIEnv *env = NULL;
    jvm->AttachCurrentThread((void **) &env, NULL);
    return CreateShoebillObject(env);
}

void Shoebill::OnProcessTick() const
{
    if (!callbackHandlerObject) return;

    JNIEnv *env = NULL;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onProcessTick", "()V");
    if (!jmid) return;

    env->CallVoidMethod(callbackHandlerObject, jmid);
    jni_jvm_printExceptionStack(env);
}

int Shoebill::Restart()
{
    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);
    if (!env) return 0;
    Uninitialize(env);
    Initialize(env);
    return 1;
}

int Shoebill::CallRegisteredFunction(AMX *amx, std::string functionName, jobjectArray parameters) const
{
    if (!callbackHandlerObject) return -1;

    JNIEnv *env = NULL;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onRegisteredFunctionCall",
                                             "(ILjava/lang/String;[Ljava/lang/Object;)I");
    if (!jmid) return -1;

    auto jstr = env->NewStringUTF(functionName.c_str());

    int result = env->CallIntMethod(callbackHandlerObject, jmid, amx, jstr, parameters);
    jni_jvm_printExceptionStack(env);
    return result;
}

void Shoebill::OnAmxLoad(AMX *amx) const
{
    amx_Register(amx, PluginExports, -1);
    AmxInstanceManager::GetInstance().RegisterAmx(amx);
    if (!callbackHandlerObject) return;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxLoad", "(I)V");
    if (!jmid) return;

    env->CallVoidMethod(callbackHandlerObject, jmid, amx);
    jni_jvm_printExceptionStack(env);
}

void Shoebill::OnAmxUnload(AMX *amx) const
{
    AmxInstanceManager::GetInstance().UnregisterAmx(amx);
    if (!callbackHandlerObject) return;

    JNIEnv *env;
    jvm->AttachCurrentThread((void **) &env, NULL);

    static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxUnload", "(I)V");
    if (!jmid) return;

    env->CallVoidMethod(callbackHandlerObject, jmid, amx);
    jni_jvm_printExceptionStack(env);
}











