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

#define _CRT_SECURE_NO_WARNINGS
#include <io.h>

#endif

#if defined(LINUX)
#include <dirent.h>
#include "linux.h"
#endif


#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>

#include "jni_core.h"


JavaVM *jvm = NULL;


int jni_jvm_create( JNIEnv** env, const char* clspath, const char* jvmOptionPath )
{
	if( jvm != NULL ) return -1;

	char clspathOpt[2048] = "-Djava.class.path=";
	strcat( clspathOpt, clspath );

	std::vector<char*> optionStrings;
	std::ifstream optionStream( jvmOptionPath, std::ifstream::in );
	if( optionStream.is_open() )
	{
		optionStrings.reserve( 20 );
		while( optionStream.good() )
		{
			char* option = new char[128];
			optionStream.getline( option, 128 );
			optionStrings.push_back( option );
		}
		optionStream.close();
	}

	JavaVMOption* options = new JavaVMOption[optionStrings.size()+2];
	options[0].optionString = clspathOpt;
	options[1].optionString = "-Djava.library.path=./plugins";
	for(unsigned int i=0;i<optionStrings.size();i++)
	{
		options[i+2].optionString = optionStrings[i];
	}

	JavaVMInitArgs vm_args;
	vm_args.version = JNI_VERSION_1_6;
	vm_args.options = options;
	vm_args.nOptions = optionStrings.size()+2;
	vm_args.ignoreUnrecognized = JNI_FALSE;

	jint res = JNI_CreateJavaVM(&jvm, (void**)env, &vm_args);
	for(unsigned int i=0;i<optionStrings.size();i++)
	{
		delete[] optionStrings.at(i);
	}
	delete[] options;

	if (res < 0) return -3;

	return 0;
}

int jni_jvm_printExceptionStack( JNIEnv *env )
{
	if( !env->ExceptionCheck() ) return 1;

	jthrowable throwable = env->ExceptionOccurred();
	jclass throwableCls = env->GetObjectClass(throwable);
	jmethodID printStackTraceId = env->GetMethodID(throwableCls, "printStackTrace", "()V");
	if( !printStackTraceId ) return -4;

	env->CallVoidMethod( throwable, printStackTraceId );
	env->ExceptionClear();

	return 0;
}

int jni_jvm_constructObject( JNIEnv *env, jclass jcls, jobject *pjobj )
{
	if( !jvm ) return -1;

	jmethodID jmid = env->GetMethodID(jcls, "<init>", "()V");
	if( !jmid ) return -2;

	*pjobj = env->NewObject( jcls, jmid );
	if( !*pjobj )
	{
		jni_jvm_printExceptionStack( env );
		return -3;
	}

	return 0;
}

int jni_jvm_destroy( JNIEnv *env )
{
	if( !jvm ) return -1;

	if ( env->ExceptionOccurred() ) env->ExceptionDescribe();
	jvm->DestroyJavaVM();

	env = NULL;
	jvm = NULL;
	return 0;
}

int findAndGenerateClassPath(const char* searchPath, char* classPath)
{
	char basepath[512];

	strcpy( basepath, searchPath );
	for( int i=strlen(basepath)-1; i>=0; i-- )
		if( basepath[i] == '/' ) { basepath[i+1] = 0; break; }

#if defined(WIN32)
		_finddata_t finddata;
		int hfind = _findfirst(searchPath, &finddata);
		if( hfind < 0 ) return -2;

		do 
		{
			strcat( classPath, basepath );
			strcat( classPath, finddata.name );
			strcat( classPath, ";" );
		} while ( !_findnext(hfind, &finddata) );
		_findclose( hfind );
#else
		DIR *dir = opendir(basepath);
		struct dirent entry;
		struct dirent* entryPtr = NULL;

		readdir_r(dir, &entry, &entryPtr);
		if(entryPtr == NULL) return -2;

		while(entryPtr != NULL)
		{
			if(strncmp(entry.d_name, ".", PATH_MAX) ==0 ||
				strncmp(entry.d_name, "..", PATH_MAX) ==0)
			{
				readdir_r(dir, &entry, &entryPtr);
				continue;
			}

			int len = strlen(entry.d_name);

			if(strcmp(strlwr(&(entry.d_name[len-4])), ".jar") != 0)
			{
				readdir_r(dir, &entry, &entryPtr);
				continue;
			}

			strcat( classPath, basepath );
			strcat( classPath, entry.d_name );
			strcat( classPath, ":" );

			readdir_r(dir, &entry, &entryPtr);
		}

		closedir(dir);
#endif

		classPath[ strlen(classPath)-1 ] = 0;
		return 0;
}
