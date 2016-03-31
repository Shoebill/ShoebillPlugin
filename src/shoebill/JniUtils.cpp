/**
 * Copyright (C) 2011-2012 MK124
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

#if defined(WIN32)

#pragma comment (lib, "jvm.lib")
#pragma comment (lib, "jawt.lib")
#endif


#include <jni.h>
#include <string.h>
#include <fstream>
#include <vector>

#include "JniUtils.h"

JavaVM *jvm = NULL;

int jni_jvm_create(JNIEnv **env, const char *clspath, const char *jvmOptionPath)
{
	if (jvm != NULL) return -1;

	char classPathOptions[2048] = "-Djava.class.path=";
	strcat(classPathOptions, clspath);

	std::vector<char *> optionStrings;
	std::ifstream optionStream(jvmOptionPath, std::ifstream::in);
	if (optionStream.is_open())
	{
		optionStrings.reserve(20);
		while (optionStream.good())
		{
			char *option = new char[128];
			optionStream.getline(option, 128);
			char *p = option + strlen(option) - 1;
			if (p >= option && *p == '\r') *p = 0;
			if (strlen(option) < 1) continue;
			optionStrings.push_back(option);
		}
		optionStream.close();
	}

	JavaVMOption *options = new JavaVMOption[optionStrings.size() + 2];
	options[0].optionString = classPathOptions;
	options[1].optionString = "-Djava.library.path=./plugins";
	for (unsigned int i = 0; i < optionStrings.size(); i++)
	{
		options[i + 2].optionString = optionStrings[i];
	}

	JavaVMInitArgs vm_args;
	vm_args.version = JNI_VERSION_1_8;
	vm_args.options = options;
	vm_args.nOptions = (jint) (optionStrings.size() + 2);
	vm_args.ignoreUnrecognized = JNI_FALSE;

	jint res = JNI_CreateJavaVM(&jvm, (void **) env, &vm_args);
	for (unsigned int i = 0; i < optionStrings.size(); i++)
	{
		delete[] optionStrings.at(i);
	}
	delete[] options;

	if (res < 0) return -3;

	return 0;
}

int jni_jvm_printExceptionStack(JNIEnv *env)
{
	if (!env->ExceptionCheck()) return 1;

	jthrowable throwable = env->ExceptionOccurred();
	jclass throwableCls = env->GetObjectClass(throwable);
	jmethodID printStackTraceId = env->GetMethodID(throwableCls, "printStackTrace", "()V");
	if (!printStackTraceId) return -4;

	env->CallVoidMethod(throwable, printStackTraceId);
	env->ExceptionClear();

	return 0;
}

int jni_jvm_constructObject(JNIEnv *env, jclass jcls, jobject *pjobj)
{
	if (!jvm) return -1;

	jmethodID jmid = env->GetMethodID(jcls, "<init>", "()V");
	if (!jmid) return -2;

	*pjobj = env->NewObject(jcls, jmid);
	if (!*pjobj)
	{
		jni_jvm_printExceptionStack(env);
		return -3;
	}

	return 0;
}

int jni_jvm_destroy(JNIEnv *env)
{
	if (!jvm) return -1;

	if (env->ExceptionOccurred()) env->ExceptionDescribe();
	jvm->DestroyJavaVM();

	env = NULL;
	jvm = NULL;
	return 0;
}

jobject makeObjectFromReturnType(JNIEnv *env, jint returnType, AMX *amx, cell retval)
{
	if (returnType == 0) //If returnType == Integer
	{
		auto cls = env->FindClass("java/lang/Integer");
		auto methodID = env->GetMethodID(cls, "<init>", "(I)V");
		return env->NewObject(cls, methodID, retval);
	}
	else if (returnType == 1) //If returnType == Float
	{
		auto cls = env->FindClass("java/lang/Float");
		auto methodID = env->GetMethodID(cls, "<init>", "(F)V");
		return env->NewObject(cls, methodID, amx_ctof(retval));
	}
	else if (returnType == 2) //If returnType == String
	{
		char result[1024];
		amx_GetString(amx, retval, &result[0], sizeof result);
		auto cls = env->FindClass("java/lang/String");
		auto methodID = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;)V");
		//TODO: test
		return env->NewObject(cls, methodID, result);
	}
	return nullptr;
}

#if defined(WIN32)
#include <io.h>

int findAndGenerateClassPath(const char* searchPath, char* classPath)
{
	char basepath[512];

	strcpy(basepath, searchPath);
	char *lastPos = strrchr(basepath, '/');
	if (lastPos) *(lastPos + 1) = 0;

	_finddata_t finddata;
	int hfind = _findfirst(searchPath, &finddata);
	if (hfind < 0) return -1;

	do
	{
		strcat(classPath, basepath);
		strcat(classPath, finddata.name);
		strcat(classPath, ";");
	} while (!_findnext(hfind, &finddata));
	_findclose(hfind);

	classPath[strlen(classPath) - 1] = 0;
	return 0;
}

void pushJavaString(JNIEnv* env, AMX* amx, jobject object, std::vector<cell>& stringCells)
{
	auto string = static_cast<jstring>(object);
	auto content = env->GetStringUTFChars(string, false);
	cell strCell;
	amx_PushString(amx, &strCell, NULL, content, 0, 0);
	stringCells.push_back(strCell);
	env->ReleaseStringUTFChars(string, content);
}

void pushJavaInteger(JNIEnv* env, AMX* amx, jobject object)
{
	static auto integerClass = env->FindClass("java/lang/Integer");
	static auto methodId = env->GetMethodID(integerClass, "intValue", "()I");
	auto value = env->CallIntMethod(object, methodId);
	amx_Push(amx, static_cast<cell>(value));
}

void pushJavaFloat(JNIEnv* env, AMX* amx, jobject object)
{
	static auto floatClass = env->FindClass("java/lang/Float");
	static auto methodId = env->GetMethodID(floatClass, "floatValue", "()F");
	auto value = env->CallFloatMethod(object, methodId);
	amx_Push(amx, amx_ftoc(value));
}

void pushJavaReferenceFloatInt(JNIEnv* env, AMX* amx, jobject object, std::map<std::pair<jobject, std::string>, std::pair<cell*, cell>>& references, std::string className)
{
	cell amx_addr, *phys_addr;
	amx_Allot(amx, 1, &amx_addr, &phys_addr);
	amx_Push(amx, amx_addr);
	references[std::pair<jobject, std::string>(object, className)] = std::pair<cell *, cell>(
		phys_addr, amx_addr);
}

void pushJavaReferenceString(JNIEnv* env, AMX* amx, jobject object, std::map<std::pair<jobject, std::string>, std::pair<cell*, cell>>& references, std::string className)
{
	auto objectClass = env->GetObjectClass(object);
	auto lengthMethodId = env->GetMethodID(objectClass, "getLength", "()I");
	auto length = env->CallIntMethod(object, lengthMethodId);
	cell amx_str, *amx_str_phys;
	amx_Allot(amx, length, &amx_str, &amx_str_phys);
	amx_Push(amx, amx_str);
	references[std::pair<jobject, std::string>(object, className)] = std::pair<cell *, cell>(
		amx_str_phys, amx_str);
}
#else

#include <glob.h>

int findAndGenerateClassPath(const char *searchPath, char *classPath)
{
	glob_t globdata;
	glob(searchPath, 0, 0, &globdata);

	size_t files = globdata.gl_pathc;
	if (files < 1) return -1;

	char **p = globdata.gl_pathv;
	for (size_t i = 0; i < files; i++)
	{
		strcat(classPath, *p);
		strcat(classPath, ":");
		p++;
	}
	globfree(&globdata);

	classPath[strlen(classPath) - 1] = 0;
	return 0;
}

#endif
