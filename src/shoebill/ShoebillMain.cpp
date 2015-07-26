/**
 * Copyright (C) 2011-2013 MK124
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
#include "ShoebillMain.h"

const char CODEPAGE_FILE_PATH[] = "./shoebill/codepages.txt";
const char JVM_OPTION_FILE_PATH[] = "./shoebill/jvm_options.txt";

const char JVM_CLASSPATH_SEARCH_PATH[] = "./shoebill/bootstrap/shoebill-launcher-1.1*.jar";
const char LAUNCHER_CLASS_NAME[] = "net/gtaun/shoebill/ShoebillLauncher";

const char RESOLVE_DEPENDENCIES_METHOD_NAME[] = "resolveDependencies";
const char RESOLVE_DEPENDENCIES_METHOD_SIGN[] = "()Ljava/lang/Object;";

const char LOAD_NATIVE_LIBRARY_METHOD_NAME[] = "loadNativeLibrary";
const char LOAD_NATIVE_LIBRARY_METHOD_SIGN[] = "()V";

const char CREATE_SHOEBILL_METHOD_NAME[] = "createShoebill";
const char CREATE_SHOEBILL_METHOD_SIGN[] = "(Ljava/lang/Object;[I)Ljava/lang/Object;";
jclass shoebillLauncherClass = NULL;

jclass shoebillClass = NULL;
jobject shoebillObject = NULL;

jclass callbackHandlerClass = NULL;
jobject callbackHandlerObject = NULL;

jclass integerClass = NULL;
jclass floatClass = NULL;
jclass stringClass = NULL;

int serverCodepage = 1252;
int playerCodepage[MAX_PLAYERS] = { 0 };

int Initialize(JNIEnv *env);
int Uninitialize(JNIEnv *env);

int CreateShoebillObject(JNIEnv *env);
int ReleaseShoebillObject(JNIEnv *env);
bool initialized = false;

cell AMX_NATIVE_CALL CallShoebillFunction(AMX* amx, cell* params)
{
	int parameterCount = params[0] / sizeof(cell);
	char functionName[128];
	amx_GetString(amx, params[1], functionName, sizeof(functionName));
	if (!AmxInstanceManager::get().registeredFunctionExists(amx, std::string(functionName)))
	{
		logprintf("[SHOEBILL] Function %s is not registered.", functionName);
		return -1;
	}
	auto definedParameters = AmxInstanceManager::get().getRegisteredParamters(amx, std::string(functionName));
	if (parameterCount-1 != definedParameters.size())
	{
		logprintf("[SHOEBILL] Calling %s with %i parameters, but was expecting %i parameters.", functionName, parameterCount-1, definedParameters.size());
		return -1;
	}
	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);
	static auto objectClass = env->FindClass("java/lang/Object");
	jobjectArray objectArray = env->NewObjectArray(definedParameters.size(), objectClass, NULL);
	env->NewGlobalRef(objectArray);
	std::vector<std::pair<cell*, std::string>> referenceValues;
	std::map<cell*, int> arrayLengths;
	int lastArrayIndex = 0;
	for (auto i = 0; i < definedParameters.size(); i++)
	{
		cell iterationCell = params[2 + i];
		if (definedParameters[i] == "java.lang.String")
		{
			char parameterString[1024];
			cell* phys_addr = NULL;
			amx_GetString(amx, iterationCell, parameterString, sizeof(parameterString));
			amx_GetAddr(amx, iterationCell, &phys_addr);
			auto string = env->NewStringUTF(parameterString);
			env->SetObjectArrayElement(objectArray, i, string);
			referenceValues.push_back(std::pair<cell*, std::string>(phys_addr, definedParameters[i]));
		}
		else if (definedParameters[i] == "java.lang.Integer")
		{
			static auto methodId = env->GetMethodID(integerClass, "<init>", "(I)V");
			cell* integerValue;
			amx_GetAddr(amx, iterationCell, &integerValue);
			auto integerObject = env->NewObject(integerClass, methodId, *integerValue);
			env->SetObjectArrayElement(objectArray, i, integerObject);
			referenceValues.push_back(std::pair<cell*, std::string>(integerValue, definedParameters[i]));
		}
		else if (definedParameters[i] == "java.lang.Float")
		{
			static auto methodId = env->GetMethodID(floatClass, "<init>", "(F)V");
			cell* floatValue;
			amx_GetAddr(amx, iterationCell, &floatValue);
			auto floatObject = env->NewObject(floatClass, methodId, amx_ctof(*floatValue));
			env->SetObjectArrayElement(objectArray, i, floatObject);
			referenceValues.push_back(std::pair<cell*, std::string>(floatValue, definedParameters[i]));
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
		else if (definedParameters[i] == "[Ljava.lang.Integer;")
		{
			cell* phys_addr = NULL;
			amx_GetAddr(amx, params[2 + i + 1], &phys_addr);
			auto arrayLength = *phys_addr;
			cell* array = NULL;
			amx_GetAddr(amx, iterationCell, &array);
			arrayLengths[array] = arrayLength;
			auto javaArray = env->NewObjectArray(arrayLength, integerClass, NULL);
			static auto methodId = env->GetMethodID(integerClass, "<init>", "(I)V");
			for (auto a = 0; a < arrayLength; a++)
			{
				env->SetObjectArrayElement(javaArray, a, env->NewObject(integerClass, methodId, *(array + a)));
			}
			env->SetObjectArrayElement(objectArray, lastArrayIndex, javaArray);
			referenceValues.push_back(std::pair<cell*, std::string>(array, definedParameters[i]));
			i += 1; //skip next iteration because of length
			lastArrayIndex += 1;
		}
		else if (definedParameters[i] == "[Ljava.lang.Float;")
		{
			cell* phys_addr = NULL;
			amx_GetAddr(amx, params[2 + i + 1], &phys_addr);
			auto arrayLength = *phys_addr;
			cell* array = NULL;
			amx_GetAddr(amx, iterationCell, &array);
			arrayLengths[array] = arrayLength;
			auto javaArray = env->NewObjectArray(arrayLength, floatClass, NULL);
			static auto methodId = env->GetMethodID(floatClass, "<init>", "(F)V");
			for (auto a = 0; a < arrayLength; a++)
			{
				env->SetObjectArrayElement(javaArray, a, env->NewObject(floatClass, methodId, amx_ctof(*(array + a))));
			}
			env->SetObjectArrayElement(objectArray, lastArrayIndex, javaArray);
			referenceValues.push_back(std::pair<cell*, std::string>(array, definedParameters[i]));
			i += 1; //skip next iteration because of length
			lastArrayIndex += 1;
		}
	}
	auto result = CallRegisteredFunction(std::string(functionName), objectArray);
	for (auto i = 0; i < referenceValues.size(); i++)
	{
		if (referenceValues[i].second == "java.lang.String")
		{
			auto string = (jstring)env->GetObjectArrayElement(objectArray, i);
			auto stringObject = env->GetStringUTFChars(string, NULL);
			amx_SetString(referenceValues[i].first, stringObject, NULL, NULL, strlen(stringObject)+1);
			env->ReleaseStringUTFChars(string, stringObject);
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
		else if (referenceValues[i].second == "[Ljava.lang.String;")
		{
			auto stringArrayObject = static_cast<jobjectArray>(env->GetObjectArrayElement(objectArray, i));
			int newArrayLength = env->GetArrayLength(stringArrayObject);
			if (newArrayLength > params[2 + i + 1])
				logprintf("[SHOEBILL] %s has a bigger array than pawn. New values are ignored.", functionName);
			cell* pawnArray = referenceValues[i].first;
			auto arrayLength = arrayLengths[pawnArray];
			for (int a = 0; a < arrayLength; a++) {
				amx_Release(amx, *(pawnArray + a));
				auto stringObject = static_cast<jstring>(env->GetObjectArrayElement(stringArrayObject, a));
				auto string = env->GetStringUTFChars(stringObject, false);
				*(pawnArray + a) = amx_NewString(amx, string, strlen(string) + 1);
				env->ReleaseStringUTFChars(stringObject, string);
			}
		}
		else if (referenceValues[i].second == "[Ljava.lang.Integer;")
		{
			auto intArrayObject = static_cast<jobjectArray>(env->GetObjectArrayElement(objectArray, i));
			int newArrayLength = env->GetArrayLength(intArrayObject);
			if (newArrayLength > params[2 + i + 1])
				logprintf("[SHOEBILL] %s has a bigger array than pawn. New values are ignored.", functionName);
			cell* pawnArray = referenceValues[i].first;
			auto arrayLength = arrayLengths[pawnArray];
			static auto methodId = env->GetMethodID(integerClass, "intValue", "()I");
			for (int a = 0; a < arrayLength; a++)
				*(pawnArray + a) = env->CallIntMethod(env->GetObjectArrayElement(intArrayObject, a), methodId);
		}
		else if (referenceValues[i].second == "[Ljava.lang.Float;")
		{
			auto floatArrayObject = static_cast<jobjectArray>(env->GetObjectArrayElement(objectArray, i));
			int newArrayLength = env->GetArrayLength(floatArrayObject);
			if (newArrayLength > params[2 + i + 1])
				logprintf("[SHOEBILL] %s has a bigger array than pawn. New values are ignored.", functionName);
			cell* pawnArray = referenceValues[i].first;
			auto arrayLength = arrayLengths[pawnArray];
			static auto methodId = env->GetMethodID(floatClass, "floatValue", "()F");
			for (int a = 0; a < arrayLength; a++) {
				auto newValue = env->CallFloatMethod(env->GetObjectArrayElement(floatArrayObject, a), methodId);
				*(pawnArray + a) = amx_ftoc(newValue);
			}
		}
	}
	env->DeleteGlobalRef(objectArray);
	return result;
}

AMX_NATIVE_INFO PluginExports[]
{
	{"CallShoebillFunction", CallShoebillFunction},
	{0, 0}
};

void OnShoebillLoad()
{
	if (!callbackHandlerObject) return;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onShoebillLoad", "()V");
	if (!jmid) return;
	env->CallVoidMethod(callbackHandlerObject, jmid);
	jni_jvm_printExceptionStack(env);
}

bool OnLoadPlugin()
{
	logprintf("  > Shoebill 1.1 NativePlugin for SA-MP 0.3.7 by MK124, JoJLlmAn & 123marvin123");

	char classpath[2048] = { 0 };
	if (findAndGenerateClassPath(JVM_CLASSPATH_SEARCH_PATH, classpath) < 0)
	{
		logprintf("  > Error: Can't find launcher library.");
		return false;
	}
	JNIEnv *env;
	if (jni_jvm_create(&env, classpath, JVM_OPTION_FILE_PATH) < 0)
	{
		logprintf("  > Error: Can't create Java VM.");
		return false;
	}

	logprintf("  > Java VM has been created.");

	jvm->AttachCurrentThread((void**)&env, NULL);
	int ret = Initialize(env);
	return ret >= 0;
}

void OnUnloadPlugin()
{
	if (!callbackHandlerObject) return;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onShoebillUnload", "()V");
	if (!jmid) return;

	env->CallVoidMethod(callbackHandlerObject, jmid);
	jni_jvm_printExceptionStack(env);
	Uninitialize(env);
	if (jni_jvm_destroy(env) >= 0) {
		logprintf("Java VM destroyed.");
	}
}

void OnAmxLoad(AMX *amx)
{
	amx_Register(amx, PluginExports, -1);
	AmxInstanceManager::get().registerAmx(amx);
	if (!callbackHandlerObject) return;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxLoad", "(I)V");
	if (!jmid) return;

	env->CallVoidMethod(callbackHandlerObject, jmid, amx);
	jni_jvm_printExceptionStack(env);
}

void OnAmxUnload(AMX *amx)
{
	AmxInstanceManager::get().unregisterAmx(amx);
	if (!callbackHandlerObject) return;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxUnload", "(I)V");
	if (!jmid) return;

	env->CallVoidMethod(callbackHandlerObject, jmid, amx);
	jni_jvm_printExceptionStack(env);
}


int StartShoebill()
{
	if (initialized) return 0;
	JNIEnv *env = NULL;
	jvm->AttachCurrentThread((void**)&env, NULL);
	return CreateShoebillObject(env);
}

int Initialize(JNIEnv *env)
{
	shoebillLauncherClass = env->FindClass(LAUNCHER_CLASS_NAME);
	if (!shoebillLauncherClass)
	{
		logprintf("  > Error: Can't find launcher class [%s].", LAUNCHER_CLASS_NAME);
		return -1;
	}
	
	static jmethodID loadNativeLibraryMethodID = env->GetStaticMethodID(shoebillLauncherClass, LOAD_NATIVE_LIBRARY_METHOD_NAME, LOAD_NATIVE_LIBRARY_METHOD_SIGN);
	if (!loadNativeLibraryMethodID)
	{
		logprintf("  > Error: Can't find launcher method [%s::%s%s].", LAUNCHER_CLASS_NAME, LOAD_NATIVE_LIBRARY_METHOD_NAME, LOAD_NATIVE_LIBRARY_METHOD_SIGN);
		return -6;
	}

	env->CallStaticVoidMethod(shoebillLauncherClass, loadNativeLibraryMethodID);
	if (env->ExceptionCheck())
	{
		jni_jvm_printExceptionStack(env);
		logprintf("  > Error: Can't load Shoebill JNI library.");
		return -7;
	}
	shoebillLauncherClass = (jclass)(env->NewGlobalRef(shoebillLauncherClass));
	jvm->AttachCurrentThread((void**)&env, NULL);

	integerClass = env->FindClass("java/lang/Integer");
	floatClass = env->FindClass("java/lang/Float");
	stringClass = env->FindClass("java/lang/String");

	StartShoebill();
	initialized = true;
	return 0;
}

int Uninitialize(JNIEnv *env)
{
	ReleaseShoebillObject(env);
	if (shoebillLauncherClass) env->DeleteGlobalRef(shoebillLauncherClass);
	shoebillLauncherClass = NULL;
	initialized = false;
	return 0;
}

int CreateShoebillObject(JNIEnv *env)
{
	static jmethodID resolveDependenciesMethodID = env->GetStaticMethodID(shoebillLauncherClass, RESOLVE_DEPENDENCIES_METHOD_NAME, RESOLVE_DEPENDENCIES_METHOD_SIGN);
	if (!resolveDependenciesMethodID)
	{
		logprintf("ShoebillPlugin Error: Can't find launcher method [%s::%s%s].", LAUNCHER_CLASS_NAME, RESOLVE_DEPENDENCIES_METHOD_NAME, RESOLVE_DEPENDENCIES_METHOD_SIGN);
		return -8;
	}

	jobject context = env->CallStaticObjectMethod(shoebillLauncherClass, resolveDependenciesMethodID);
	if (!context)
	{
		jni_jvm_printExceptionStack(env);
		logprintf("ShoebillPlugin Error: Can't resolve dependencies.");
		return -2;
	}

	static jmethodID createShoebillMethodID = env->GetStaticMethodID(shoebillLauncherClass, CREATE_SHOEBILL_METHOD_NAME, CREATE_SHOEBILL_METHOD_SIGN);
	if (!createShoebillMethodID)
	{
		logprintf("ShoebillPlugin Error: Can't find launcher method [%s::%s%s], Maybe the launcher library is outdated.", LAUNCHER_CLASS_NAME, CREATE_SHOEBILL_METHOD_NAME, CREATE_SHOEBILL_METHOD_SIGN);
		return -3;
	}

	std::unordered_set<AMX*> amxInstances = AmxInstanceManager::get().getInstances();
	jsize size = amxInstances.size();
	jint* array = new jint[size];
	{
		int i = 0;
		for (std::unordered_set<AMX*>::iterator it = amxInstances.begin(); it != amxInstances.end() && i < size; it++, i++)
		{
			array[i] = (jint)(*it);
		}
	}
	jintArray amxHandleArray = env->NewIntArray(size);
	env->SetIntArrayRegion(amxHandleArray, 0, size, array);
	delete array;

	shoebillObject = env->CallStaticObjectMethod(shoebillLauncherClass, createShoebillMethodID, context, amxHandleArray);
	if (!shoebillObject)
	{
		jni_jvm_printExceptionStack(env);
		logprintf("ShoebillPlugin Error: Can't create shoebill object.");
		return -4;
	}

	shoebillClass = env->GetObjectClass(shoebillObject);
	if (!shoebillClass)
	{
		logprintf("ShoebillPlugin Error: Can't get shoebill class.");
		return -5;
	}

#if defined(LINUX)
	std::ifstream codepageFile( CODEPAGE_FILE_PATH, std::ifstream::in);
	if(codepageFile.is_open())
	{
		char input[256], charset[256];
		unsigned int code = 0;
		while ( codepageFile.good() )
		{
			codepageFile.getline( input, 256 );
			sscanf( input, "%u %s", &code, charset);
			if( code && charset[0] )
			{
				if( codepages.find( code ) != codepages.end() )
					logprintf( "  > Error: Codepage already in use, %d=%s", code, codepages[code].c_str() );
				else
				{
					codepages[code] = std::string( charset );
				}
			}
			code = 0;
			charset[0] = 0;
		}
		codepageFile.close();
	}
	else
	{
		logprintf( "  > Error: Can't open %s.", CODEPAGE_FILE_PATH );
	}
#endif

	static jmethodID getCallbackHandlerMethodID = env->GetMethodID(shoebillClass, "getCallbackHandler", "()Lnet/gtaun/shoebill/samp/SampCallbackHandler;");
	if (!getCallbackHandlerMethodID)
	{
		logprintf("ShoebillPlugin Error: Can't find method getCallbackHandler().");
		return 1;
	}

	callbackHandlerObject = env->CallObjectMethod(shoebillObject, getCallbackHandlerMethodID);
	if (callbackHandlerObject == NULL)
	{
		logprintf("ShoebillPlugin Error: Can't find main EventHandler.");
		return 1;
	}

	shoebillObject = env->NewGlobalRef(shoebillObject);
	shoebillClass = (jclass)(env->NewGlobalRef(shoebillClass));
	callbackHandlerObject = env->NewGlobalRef(callbackHandlerObject);
	callbackHandlerClass = (jclass)(env->NewGlobalRef(env->GetObjectClass(callbackHandlerObject)));
	initialized = true;
	OnShoebillLoad();
	logprintf("  > Shoebill has been initialized.");
	return 0;
}

int ReleaseShoebillObject(JNIEnv *env)
{
	if (shoebillObject) env->DeleteGlobalRef(shoebillObject);
	shoebillObject = NULL;

	if (shoebillClass) env->DeleteGlobalRef(shoebillClass);
	shoebillClass = NULL;

	if (callbackHandlerObject) env->DeleteGlobalRef(callbackHandlerObject);
	callbackHandlerObject = NULL;

	if (callbackHandlerClass) env->DeleteGlobalRef(callbackHandlerClass);
	callbackHandlerClass = NULL;

	return 0;
}

void OnProcessTick()
{
	if (!callbackHandlerObject) return;

	JNIEnv *env = NULL;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onProcessTick", "()V");
	if (!jmid) return;

	env->CallVoidMethod(callbackHandlerObject, jmid);
	jni_jvm_printExceptionStack(env);
}

int CallRegisteredFunction(std::string functionName, jobjectArray parameters)
{
	if (!callbackHandlerObject) return -1;

	JNIEnv *env = NULL;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onRegisteredFunctionCall", "(Ljava/lang/String;[Ljava/lang/Object;)I");
	if (!jmid) return -1;

	auto jstr = env->NewStringUTF(functionName.c_str());

	int result = env->CallIntMethod(callbackHandlerObject, jmid, jstr, parameters);
	jni_jvm_printExceptionStack(env);
	return result;
}

int OnGameModeInit()
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env = NULL;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onGameModeInit", "()I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnGameModeExit()
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onGameModeExit", "()I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnFilterScriptInit()
{
	return 0;
}

int OnFilterScriptExit()
{
	return 0;
}

int OnPlayerConnect(int playerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerConnect", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerDisconnect(int playerid, int reason)
{
	if (!callbackHandlerObject) return 0;

	playerCodepage[playerid] = 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerDisconnect", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, reason);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerSpawn(int playerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerSpawn", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerDeath(int playerid, int killerid, int reason)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerDeath", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, killerid, reason);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnVehicleSpawn(int vehicleid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleSpawn", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnVehicleDeath(int vehicleid, int killerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleDeath", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid, killerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerText(int playerid, char* text)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerText", "(ILjava/lang/String;)I");
	if (!jmid) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(getPlayerCodepage(playerid), text, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len);
	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, str);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerCommandText(int playerid, char* cmdtext)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerCommandText", "(ILjava/lang/String;)I");
	if (!jmid) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(getPlayerCodepage(playerid), cmdtext, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len);
	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, str);
	jni_jvm_printExceptionStack(env);
	
	return ret;
}

int OnPlayerRequestClass(int playerid, int classid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerRequestClass", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, classid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerEnterVehicle(int playerid, int vehicleid, int ispassenger)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerEnterVehicle", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, vehicleid, ispassenger);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerExitVehicle(int playerid, int vehicleid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerExitVehicle", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, vehicleid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerStateChange(int playerid, int newstate, int oldstate)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerStateChange", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, newstate, oldstate);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerEnterCheckpoint(int playerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerEnterCheckpoint", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerLeaveCheckpoint(int playerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerLeaveCheckpoint", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerEnterRaceCheckpoint(int playerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerEnterRaceCheckpoint", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerLeaveRaceCheckpoint(int playerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerLeaveRaceCheckpoint", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnRconCommand(char* cmd)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onRconCommand", "(Ljava/lang/String;)I");
	if (!jmid) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(getServerCodepage(), cmd, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len);
	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, str);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerRequestSpawn(int playerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerRequestSpawn", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnObjectMoved(int objectid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onObjectMoved", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, objectid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerObjectMoved(int playerid, int objectid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerObjectMoved", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, objectid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerPickUpPickup(int playerid, int pickupid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerPickUpPickup", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, pickupid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnVehicleMod(int playerid, int vehicleid, int componentid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleMod", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, vehicleid, componentid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnEnterExitModShop(int playerid, int enterexit, int interiorid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onEnterExitModShop", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, enterexit, interiorid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnVehiclePaintjob(int playerid, int vehicleid, int paintjobid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehiclePaintjob", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, vehicleid, paintjobid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnVehicleRespray(int playerid, int vehicleid, int color1, int color2)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleRespray", "(IIII)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, vehicleid, color1, color2);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnVehicleDamageStatusUpdate(int vehicleid, int playerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleDamageStatusUpdate", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid, playerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnUnoccupiedVehicleUpdate(int vehicleid, int playerid, int passenger_seat, float new_x, float new_y, float new_z, float vel_x, float vel_y, float vel_z)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onUnoccupiedVehicleUpdate", "(IIIFFFFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid, playerid, passenger_seat, new_x, new_y, new_z, vel_x, vel_y, vel_z);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerSelectedMenuRow(int playerid, int row)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerSelectedMenuRow", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, row);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerExitedMenu(int playerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerExitedMenu", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerInteriorChange(int playerid, int newinteriorid, int oldinteriorid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerInteriorChange", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, newinteriorid, oldinteriorid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerKeyStateChange", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, newkeys, oldkeys);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnRconLoginAttempt(char* ip, char* password, int success)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onRconLoginAttempt", "(Ljava/lang/String;Ljava/lang/String;I)I");
	if (!jmid) return 0;

	jstring iptext = env->NewStringUTF(ip);

	jchar wtext[1024];
	int len = mbs2wcs(getServerCodepage(), password, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));
	jstring str = env->NewString(wtext, len);

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, iptext, str, success);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerUpdate(int playerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerUpdate", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerStreamIn(int playerid, int forplayerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerStreamIn", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, forplayerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerStreamOut(int playerid, int forplayerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerStreamOut", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, forplayerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnVehicleStreamIn(int vehicleid, int forplayerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleStreamIn", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid, forplayerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnVehicleStreamOut(int vehicleid, int forplayerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleStreamOut", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid, forplayerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnDialogResponse(int playerid, int dialogid, int response, int listitem, char *inputtext)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onDialogResponse", "(IIIILjava/lang/String;)I");
	if (!jmid) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(getPlayerCodepage(playerid), inputtext, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len);
	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, dialogid, response, listitem, str);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerTakeDamage(int playerid, int issuerid, float amount, int weaponid, int bodypart)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerTakeDamage", "(IIFII)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, issuerid, amount, weaponid, bodypart);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerGiveDamage(int playerid, int damagedid, float amount, int weaponid, int bodypart)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerGiveDamage", "(IIFII)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, damagedid, amount, weaponid, bodypart);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerClickMap(int playerid, float x, float y, float z)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerClickMap", "(IFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, x, y, z);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerClickTextDraw(int playerid, int clickedid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerClickTextDraw", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, clickedid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerClickPlayerTextDraw(int playerid, int playertextid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerClickPlayerTextDraw", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, playertextid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerClickPlayer(int playerid, int clickedplayerid, int source)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerClickPlayer", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, clickedplayerid, source);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerEditObject(int playerid, int playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerEditObject", "(IIIIFFFFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, playerobject, objectid, response, fX, fY, fZ, fRotX, fRotY, fRotZ);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerEditAttachedObject(int playerid, int response, int index, int modelid, int boneid, float fOffsetX, float fOffsetY, float fOffsetZ, float fRotX, float fRotY, float fRotZ, float fScaleX, float fScaleY, float fScaleZ)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerEditAttachedObject", "(IIIIIFFFFFFFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, response, index, modelid, boneid, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float fX, float fY, float fZ)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerSelectObject", "(IIIIFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, type, objectid, modelid, fX, fY, fZ);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float fX, float fY, float fZ)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

		static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerWeaponShot", "(IIIIFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, weaponid, hittype, hitid, fX, fY, fZ);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnIncomingConnection(int playerid, char* ip_address, int port)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onIncomingConnection", "(ILjava/lang/String;I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, env->NewStringUTF(ip_address), port);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnTrailerUpdate(int playerid, int vehicleid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onTrailerUpdate", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, vehicleid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnActorStreamIn(int actorid, int forplayerid)
{
	if (!callbackHandlerObject) return 0;
	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onActorStreamIn", "(II)I");
	if (!jmid) return 0;

	int ret = env->CallIntMethod(callbackHandlerObject, jmid, actorid, forplayerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnActorStreamOut(int actorid, int forplayerid)
{
	if (!callbackHandlerObject) return 0;
	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onActorStreamOut", "(II)I");
	if (!jmid) return 0;

	int ret = env->CallIntMethod(callbackHandlerObject, jmid, actorid, forplayerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnPlayerGiveDamageActor(int playerid, int damaged_actor, int amount, int weapon, int bodypart)
{
	if (!callbackHandlerObject) return 0;
	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerGiveDamageActor", "(IIIII)I");
	if (!jmid) return 0;

	int ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, damaged_actor, amount, weapon, bodypart);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnVehicleSirenStateChange(int playerid, int vehicleid, int newstate)
{
	if (!callbackHandlerObject) return 0;
	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleSirenStateChange", "(III)I");
	if (!jmid) return 0;

	int ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, vehicleid, newstate);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxVehicleCreated(int vehicleid, int modelid, float x, float y, float z, float angle, int interiorid, int worldid, int color1, int color2, int respawn_delay, int addsiren) 
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxVehicleCreated", "(IIFFFFIIIIII)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid, modelid, x, y, z, angle, interiorid, worldid, color1, color2, respawn_delay, addsiren);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxDestroyVehicle(int vehicleid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxDestroyVehicle", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxSampObjectCreated(int objectId, int modelid, float x, float y, float z, float rX, float rY, float rZ, int worldid, int interiorid, float render_distance)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxSampObjectCreated", "(IIFFFFFFIIF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, objectId, modelid, x, y, z, rX, rY, rZ, worldid, interiorid, render_distance);
	jni_jvm_printExceptionStack(env);

	return ret;
}

int OnAmxSampObjectDestroyed(int objectId)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxSampObjectDestroyed", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, objectId);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxAttachObjectToVehicle(int objectId, int vehicleId, float x, float y, float z, float rX, float rY, float rZ)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxAttachObjectToVehicle", "(IIFFFFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, objectId, vehicleId, x, y, z, rX, rY, rZ);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxAttachObjectToPlayer(int objectId, int playerId, float x, float y, float z, float rX, float rY, float rZ)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxAttachObjectToPlayer", "(IIFFFFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, objectId, playerId, x, y, z, rX, rY, rZ);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxAttachObjectToObject(int objectId, int other_object_Id, float x, float y, float z, float rX, float rY, float rZ)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxAttachObjectToObject", "(IIFFFFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, objectId, other_object_Id, x, y, z, rX, rY, rZ);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxCreatePlayerObject(int playerid, int modelid, float x, float y, float z, float rX, float rY, float rZ, float drawdistance, int worldid, int interiorid, int returnedValue)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);
	
	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxCreatePlayerObject", "(IIFFFFFFIIFI)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, modelid, x, y, z, rX, rY, rZ, drawdistance, worldid, interiorid, returnedValue);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxDestroyPlayerObject(int playerid, int objectid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxDestroyPlayerObject", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, objectid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxSetPlayerAttachedObject(int playerid, int index, int modelid, int bone, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, int materialcolor1, int materialcolor2) 
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxSetPlayerAttachedObject", "(IIIIFFFFFFFFFII)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, index, modelid, bone, offsetX, offsetY, offsetZ, rotX, rotY, rotZ, scaleX, scaleY, scaleZ, materialcolor1, materialcolor2);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxRemovePlayerAttachedObject(int playerid, int index)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxRemovePlayerAttachedObject", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, index);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxDestroyPickup(int pickupid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxDestroyPickup", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, pickupid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxCreatePickup(int model, int type, float posX, float posY, float posZ, int virtualworld, int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxCreatePickup", "(IIFFFII)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, model, type, posX, posY, posZ, virtualworld, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxAddStaticPickup(int model, int type, float posX, float posY, float posZ, int virtualworld)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxAddStaticPickup", "(IIFFFI)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, model, type, posX, posY, posZ, virtualworld);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxCreateLabel(char* text, int color, float posX, float posY, float posZ, float drawDistance, int virtualworld, int testLOS, int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxCreateLabel", "(Ljava/lang/String;IFFFFIII)I");
	if (!jmid) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(getServerCodepage(), text, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len);

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, str, color, posX, posY, posZ, drawDistance, virtualworld, testLOS, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxDeleteLabel(int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxDeleteLabel", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxAttachLabelToPlayer(int id, int playerid, float offsetX, float offsetY, float offsetZ)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxAttachLabelToPlayer", "(IIFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, id, playerid, offsetX, offsetY, offsetZ);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxAttachLabelToVehicle(int id, int vehicleid, float offsetX, float offsetY, float offsetZ)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxAttachLabelToVehicle", "(IIFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, id, vehicleid, offsetX, offsetY, offsetZ);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxUpdateLabel(int id, int color, char* text)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxUpdateLabel", "(IILjava/lang/String;)I");
	if (!jmid) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(getServerCodepage(), text, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len);

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, id, color, str);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxCreatePlayerLabel(int playerid, char* text, int color, float posX, float posY, float posZ, float drawDistance, int attachedPlayer, int attachedVehicle, int testLOS, int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxCreatePlayerLabel", "(ILjava/lang/String;IFFFFIIII)I");
	if (!jmid) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(getServerCodepage(), text, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len);

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, str, color, posX, posY, posZ, drawDistance, attachedPlayer, attachedVehicle, testLOS, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxDeletePlayerLabel(int playerid, int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxDeletePlayerLabel", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxUpdatePlayerLabel(int playerid, int id, int color, char* text)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxUpdatePlayerLabel", "(IIILjava/lang/String;)I");
	if (!jmid) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(getServerCodepage(), text, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len);

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, id, color, str);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxCreateMenu(char* title, int columns, float x, float y, float col1Width, float col2Width, int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxCreateMenu", "(Ljava/lang/String;IFFFFI)I");
	if (!jmid) return 0;

	jchar wtext[32];
	int len = mbs2wcs(getServerCodepage(), title, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len);

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, str, columns, x, y, col1Width, col2Width, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxSetMenuColumnHeader(int id, int column, char* text)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxSetMenuColumnHeader", "(IILjava/lang/String;)I");
	if (!jmid) return 0;

	jchar wtext[32];
	int len = mbs2wcs(getServerCodepage(), text, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len);

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, id, column, str);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxDestroyMenu(int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxDestroyMenu", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxGangZoneCreate(float minX, float minY, float maxX, float maxY, int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxGangZoneCreate", "(FFFFI)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, minX, minY, maxX, maxY, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxGangZoneDestroy(int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxGangZoneDestroy", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxGangZoneShowForPlayer(int playerid, int zone, int color)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxGangZoneShowForPlayer", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, zone, color);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxGangZoneShowForAll(int zone, int color)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxGangZoneShowForAll", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, zone, color);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxGangZoneHideForPlayer(int playerid, int zone)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxGangZoneHideForPlayer", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, zone);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxGangZoneHideForAll(int zone)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxGangZoneHideForAll", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, zone);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxGangZoneFlashForPlayer(int playerid, int zone, int flashColor)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxGangZoneFlashForPlayer", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, zone, flashColor);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxGangZoneFlashForAll(int zone, int flashColor)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxGangZoneFlashForAll", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, zone, flashColor);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxGangZoneStopFlashForPlayer(int playerid, int zone)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxGangZoneStopFlashForPlayer", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, zone);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxGangZoneStopFlashForAll(int zone)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxGangZoneStopFlashForAll", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, zone);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxSetSkillLevel(int playerid, int skill, int level)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxSetSkillLevel", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, skill, level);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxSetPlayerMapIcon(int playerid, int iconid, float x, float y, float z, int markertype, int color, int style)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxSetPlayerMapIcon", "(IIFFFIII)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, iconid, x, y, z, markertype, color, style);
	jni_jvm_printExceptionStack(env);
	return ret;
}
 
int OnAmxRemovePlayerMapIcon(int playerid, int iconid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxRemovePlayerMapIcon", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, iconid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxShowPlayerDialog(int playerid, int dialogid, int style, char* caption, char* info, char* button1, char* button2)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxShowPlayerDialog", "(IIILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
	if (!jmid) return 0;

	jchar wCaption[64];
	int len = mbs2wcs(getServerCodepage(), caption, -1, wCaption, sizeof(wCaption) / sizeof(wCaption[0]));
	jstring jCaption = env->NewString(wCaption, len);

	jchar wInfo[1024];
	int len2 = mbs2wcs(getServerCodepage(), info, -1, wInfo, sizeof(wInfo) / sizeof(wInfo[0]));
	jstring jInfo = env->NewString(wInfo, len2);

	jchar wButton1[32];
	int len3 = mbs2wcs(getServerCodepage(), button1, -1, wButton1, sizeof(wButton1) / sizeof(wButton1[0]));
	jstring jButton1 = env->NewString(wButton1, len3);

	jchar wButton2[32];
	int len4 = mbs2wcs(getServerCodepage(), button2, -1, wButton2, sizeof(wButton2) / sizeof(wButton2[0]));
	jstring jButton2 = env->NewString(wButton2, len4);

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, dialogid, style, jCaption, jInfo, jButton1, jButton2);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxSetPlayerWorldBounds(int playerid, float minX, float minY, float maxX, float maxY)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxSetPlayerWorldBounds", "(IFFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, minX, minY, maxX, maxY);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxSetPlayerWeather(int playerid, int weatherid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxSetPlayerWeather", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, weatherid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxSetPlayerCheckpoint(int playerid, float x, float y, float z, float size)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxSetPlayerCheckpoint", "(IFFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, x, y, z, size);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxDisablePlayerCheckpoint(int playerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxDisablePlayerCheckpoint", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxSetPlayerRaceCheckpoint(int playerid, int type, float x, float y, float z, float nextX, float nextY, float nextZ, float size)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxSetPlayerRaceCheckpoint", "(IIFFFFFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, type, x, y, z, nextX, nextY, nextZ, size);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxDisablePlayerRaceCheckpoint(int playerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxDisablePlayerRaceCheckpoint", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxTogglePlayerSpectating(int playerid, int toggle)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxTogglePlayerSpectating", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, toggle);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxPlayerSpectatePlayer(int playerid, int target, int mode)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxPlayerSpectatePlayer", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, target, mode);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxPlayerSpectateVehicle(int playerid, int target, int mode)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxPlayerSpectateVehicle", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, target, mode);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxEnableStuntBonusForPlayer(int playerid, int toggle)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxEnableStuntBonusForPlayer", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, toggle);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxStartRecording(int playerid, int type, char* recordName)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxStartRecording", "(IILjava/lang/String;)I");
	if (!jmid) return 0;

	jchar wstring[128];
	int len = mbs2wcs(getServerCodepage(), recordName, -1, wstring, sizeof(wstring) / sizeof(wstring[0]));
	jstring str = env->NewString(wstring, len);

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, type, str);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxStopRecording(int playerid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxStopRecording", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxToggleControllabel(int playerid, int toggle)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxToggleControllable", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, toggle);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxTextDrawCreate(float x, float y, char* text, int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxTextDrawCreate", "(FFLjava/lang/String;I)I");
	if (!jmid) return 0;

	jchar wstring[1024];
	int len = mbs2wcs(getServerCodepage(), text, -1, wstring, sizeof(wstring) / sizeof(wstring[0]));
	jstring str = env->NewString(wstring, len);

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, x, y, str, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxTextDrawDestroy(int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxTextDrawDestroy", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxTextDrawSetString(int id, char* text)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxTextDrawSetString", "(ILjava/lang/String;)I");
	if (!jmid) return 0;

	jchar wstring[1024];
	int len = mbs2wcs(getServerCodepage(), text, -1, wstring, sizeof(wstring) / sizeof(wstring[0]));
	jstring str = env->NewString(wstring, len);

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, id, str);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxTextDrawShowForPlayer(int playerid, int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxTextDrawShowForPlayer", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxTextDrawHideForPlayer(int playerid, int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxTextDrawHideForPlayer", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxTextDrawShowForAll(int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxTextDrawShowForAll", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxTextDrawHideForAll(int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxTextDrawHideForAll", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxCreatePlayerTextDraw(int playerid, float x, float y, char* text, int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxCreatePlayerTextDraw", "(IFFLjava/lang/String;I)I");
	if (!jmid) return 0;

	jchar wstring[1024];
	int len = mbs2wcs(getServerCodepage(), text, -1, wstring, sizeof(wstring) / sizeof(wstring[0]));
	jstring str = env->NewString(wstring, len);

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, x, y, str, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxPlayerTextDrawDestroy(int playerid, int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxPlayerTextDrawDestroy", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxPlayerTextDrawSetString(int playerid, int id, char* text)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxPlayerTextDrawSetString", "(IILjava/lang/String;)I");
	if (!jmid) return 0;

	jchar wstring[1024];
	int len = mbs2wcs(getServerCodepage(), text, -1, wstring, sizeof(wstring) / sizeof(wstring[0]));
	jstring str = env->NewString(wstring, len);

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, id, str);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxPlayerTextDrawShow(int playerid, int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxPlayerTextDrawShow", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxPlayerTextDrawHide(int playerid, int id)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxPlayerTextDrawHide", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, id);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxAddVehicleComponent(int vehicleid, int componentid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxAddVehicleComponent", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid, componentid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxLinkVehicleToInterior(int vehicleid, int interiorid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxLinkVehicleToInterior", "(II)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid, interiorid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxChangeVehicleColor(int vehicleid, int color1, int color2)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxChangeVehicleColor", "(III)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid, color1, color2);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxCreateActor(int actorid, int modelid, float x, float y, float z, float rotation)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxCreateActor", "(IIFFFF)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, actorid, modelid, x, y, z, rotation);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int OnAmxDestroyActor(int actorid)
{
	if (!callbackHandlerObject) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onAmxDestroyActor", "(I)I");
	if (!jmid) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, actorid);
	jni_jvm_printExceptionStack(env);
	return ret;
}

int RestartShoebill()
{
	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);
	if (!env) return 0;
	Uninitialize(env);
	Initialize(env);
	return 1;
}

int* callHookedCallback(AMX *amx, std::string name, cell* params)
{
	auto it = getIterator(name);
	if (it != getEnd()) {
		JNIEnv *env;
		jvm->AttachCurrentThread((void**)&env, NULL);
		if (!env) return 0;
		std::string types = it->second;
		int count = params[0] / sizeof(cell);
		if (count != types.length()) {
			logprintf("%s did not equal count! Correct: %i wrong: %i", name.c_str(), count, types.length());
			return nullptr;
		}
		jclass objectClass = env->FindClass("java/lang/Object");
		jclass integerClass = env->FindClass("java/lang/Integer");
		jclass floatClass = env->FindClass("java/lang/Float");
		jmethodID integerMethodID = env->GetMethodID(integerClass, "<init>", "(I)V");
		jmethodID floatMethodID = env->GetMethodID(floatClass, "<init>", "(F)V");
		jobjectArray objectArray = (jobjectArray)env->NewObjectArray(count, objectClass, 0);
		for (std::string::size_type i = 0; i < types.size(); ++i)
		{
			char paramType = types[i];
			if (paramType == 's') {
				char text[1024];
				amx_GetString(amx, params[i + 1], text, sizeof(text));
				jchar wtext[1024];
				int len = mbs2wcs(getServerCodepage(), text, -1, wtext, sizeof(wtext) / sizeof(wtext[0]));
				env->SetObjectArrayElement(objectArray, i, env->NewString(wtext, len));
			}
			else if (paramType == 'i') {
				env->SetObjectArrayElement(objectArray, i, env->NewObject(integerClass, integerMethodID, params[i + 1]));
			}
			else if (paramType == 'f') {
				float value = amx_ctof(params[i + 1]);
				env->SetObjectArrayElement(objectArray, i, env->NewObject(floatClass, floatMethodID, value));
			}
		}

		jchar wtext[256];
		int len = mbs2wcs(getServerCodepage(), name.c_str(), -1, wtext, sizeof(wtext) / sizeof(wtext[0]));

		static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onHookCall", "(Ljava/lang/String;[Ljava/lang/Object;)[I");
		if (!jmid) return 0;
		jintArray event = (jintArray)env->CallObjectMethod(callbackHandlerObject, jmid, env->NewString(wtext, len), objectArray);
		jni_jvm_printExceptionStack(env);
		jint* values = env->GetIntArrayElements(event, false);
		int* returnObject = new int [2] { values[0], values[1] };
		env->ReleaseIntArrayElements(event, values, 0);
		return returnObject;
	}
	else return nullptr;
}
