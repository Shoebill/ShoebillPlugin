/**
 * Copyright (C) 2011 MK124
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

#ifndef __JNIUTILS_H__
#define __JNIUTILS_H__

#include <sampplugin/amx/amx.h>
#include <map>
#include <vector>
#include "AmxHelper.h"

#define assertNotNull(obj, param_name, function_name); \
    if(!obj) { jni_jvm_throwNullPointerException(env,param_name,function_name); return; }

extern JavaVM *jvm;

int jni_jvm_create(JNIEnv **env, const char *clspath, const char *jvmOptionPath);

int jni_jvm_printExceptionStack(JNIEnv *env);

void jni_jvm_throwNullPointerException(JNIEnv *env, const char *paramName, const char *functionName);

int jni_jvm_constructObject(JNIEnv *env, jclass jcls, jobject *pjobj);

int jni_jvm_destroy(JNIEnv *env);

int findAndGenerateClassPath(const char *searchPath, char *classPath);

jobject makeJavaInteger(JNIEnv *env, int value);

jobject makeJavaFloat(JNIEnv *env, float value);

jobjectArray makeJavaObjectArray(JNIEnv *env, int len);

jintArray makeJavaIntArray(JNIEnv *env, int len);

jfloatArray makeJavaFloatArray(JNIEnv *env, int len);

int getIntegerFromObject(JNIEnv *env, jobject integer);

float getFloatFromObject(JNIEnv *env, jobject object);

jobject makeObjectFromReturnType(JNIEnv *env, jint returnType, AMX *amx, cell retval);

#endif