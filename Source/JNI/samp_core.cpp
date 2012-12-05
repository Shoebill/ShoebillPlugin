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

#include <jni.h>
#include <string.h>

#include "encoding.h"
#include "jni_core.h"
#include "samp_core.h"
#include "../Wrapper/a_samp.h"

#if defined(LINUX)
#include "linux.h"
#include <map>
#include <fstream>
extern std::map<int, std::string> codepages;
#endif

const char CODEPAGE_FILE_PATH[] = "./shoebill/codepages.txt";

const char JVM_CLASSPATH_SEARCH_PATH[] = "./shoebill/bootstrap/shoebill-launcher*.jar";
const char LAUNCHER_CLASS_NAME[] = "net/gtaun/shoebill/launcher/ShoebillLauncher";

const char RESOLVE_DEPENDENCIES_METHOD_NAME[] = "resolveDependencies";
const char RESOLVE_DEPENDENCIES_METHOD_SIGN[] = "()Ljava/lang/Object;";

const char CREATE_SHOEBILL_METHOD_NAME[] = "createShoebill";
const char CREATE_SHOEBILL_METHOD_SIGN[] = "(Ljava/lang/Object;)Ljava/lang/Object;";

jclass shoebillLauncherClass = NULL;

jclass shoebillClass = NULL;
jobject shoebillObject = NULL;

jclass callbackHandlerClass = NULL;
jobject callbackHandlerObject = NULL;

int serverCodepage = 1252;
int playerCodepage[MAX_PLAYERS] = {0};


int Initialize( JNIEnv *env );
int Uninitialize( JNIEnv *env );


bool OnLoadPlugin()
{
	logprintf( "  > ShoebillPlugin Milestone 2 for SA-MP 0.3E by MK124 & JoJLlmAn" );

	char classpath[2048] = {0};
	findAndGenerateClassPath(JVM_CLASSPATH_SEARCH_PATH, classpath);

	JNIEnv *env;
	if( jni_jvm_create(&env, classpath) < 0 )
	{
		logprintf( "  > Error: Can't create Java VM." );
		return false;
	}

	logprintf( "  > Java VM has been created." );

	int ret = Initialize( env );
	return ret >= 0;
}

void OnUnloadPlugin()
{
	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	Uninitialize( env );
	if( jni_jvm_destroy(env) >= 0 ) logprintf( "Java VM destroyed." );
}

int Initialize( JNIEnv *env )
{
	shoebillLauncherClass = env->FindClass(LAUNCHER_CLASS_NAME);
	if( !shoebillLauncherClass )
	{
		logprintf( "  > Error: Can't find launcher class [%s].", LAUNCHER_CLASS_NAME );
		return -1;
	}

	jobject files = NULL;
	static jmethodID resolveDependenciesMethodID = env->GetStaticMethodID(shoebillLauncherClass, RESOLVE_DEPENDENCIES_METHOD_NAME, RESOLVE_DEPENDENCIES_METHOD_SIGN);
	if( !resolveDependenciesMethodID )
	{
		logprintf( "  > Error: Can't find launcher method [%s::%s%s], ignore.", LAUNCHER_CLASS_NAME, RESOLVE_DEPENDENCIES_METHOD_NAME, RESOLVE_DEPENDENCIES_METHOD_SIGN );
	}
	else
	{
		files = env->CallStaticObjectMethod(shoebillLauncherClass, resolveDependenciesMethodID);
		if( !files )
		{
			jni_jvm_printExceptionStack( env );
			logprintf( "  > Error: Can't resolve dependencies." );
			return -2;
		}
	}

	static jmethodID createShoebillMethodID = env->GetStaticMethodID(shoebillLauncherClass, CREATE_SHOEBILL_METHOD_NAME, CREATE_SHOEBILL_METHOD_SIGN);
	if( !createShoebillMethodID )
	{
		logprintf( "  > Error: Can't find launcher method [%s::%s%s].", LAUNCHER_CLASS_NAME, CREATE_SHOEBILL_METHOD_NAME, CREATE_SHOEBILL_METHOD_SIGN );
		return -3;
	}

	shoebillObject = env->CallStaticObjectMethod(shoebillLauncherClass, createShoebillMethodID, files);
	if( !shoebillObject )
	{
		jni_jvm_printExceptionStack( env );
		logprintf( "  > Error: Can't create shoebill object." );
		return -4;
	}

	shoebillClass = env->GetObjectClass(shoebillObject);
	if( !shoebillClass )
	{
		logprintf( "  > Error: Can't get shoebill class." );
		return -5;
	}
	
	shoebillLauncherClass = (jclass)( env->NewGlobalRef(shoebillLauncherClass) );
	shoebillObject = env->NewGlobalRef(shoebillObject);
	shoebillClass = (jclass)( env->NewGlobalRef(shoebillClass) );

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

	logprintf( "  > Shoebill has been initialized." );
	return 0;
}

int Uninitialize( JNIEnv *env )
{
	env->DeleteGlobalRef( shoebillObject );
	env->DeleteGlobalRef( shoebillClass );
	env->DeleteGlobalRef( shoebillLauncherClass );
	return 0;
}

void OnProcessTick()
{
	if( !callbackHandlerObject ) return;

	JNIEnv *env = NULL;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onProcessTick", "()V");
	if( !jmid ) return;

	env->CallVoidMethod(callbackHandlerObject, jmid);
	jni_jvm_printExceptionStack( env );
}

int OnGameModeInit()
{
	if( !shoebillObject ) return 1;

	JNIEnv *env = NULL;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid_gch = env->GetMethodID(shoebillClass, "getCallbackHandler", "()Lnet/gtaun/shoebill/samp/SampCallbackHandler;");
	if( !jmid_gch )
	{
		logprintf( "ShoebillPlugin Error: Can't find method getCallbackHandler()." );
		return 1;
	}

	callbackHandlerObject = env->CallObjectMethod(shoebillObject, jmid_gch);
	if( callbackHandlerObject == NULL )
	{
		logprintf( "ShoebillPlugin Error: Can't find main EventHandler." );
		return 1;
	}

	callbackHandlerObject = env->NewGlobalRef(callbackHandlerObject);
	callbackHandlerClass = (jclass)( env->NewGlobalRef(env->GetObjectClass(callbackHandlerObject)) );

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onGameModeInit", "()I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnGameModeExit()
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onGameModeExit", "()I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid);
	jni_jvm_printExceptionStack( env );

	env->DeleteGlobalRef( callbackHandlerObject );
	env->DeleteGlobalRef( callbackHandlerClass );
	callbackHandlerObject = NULL;
	callbackHandlerClass = NULL;

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

int OnPlayerConnect( int playerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerConnect", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerDisconnect( int playerid, int reason )
{
	if( !callbackHandlerObject ) return 0;

	playerCodepage[playerid] = 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerDisconnect", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, reason);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerSpawn( int playerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerSpawn", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerDeath( int playerid, int killerid, int reason )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerDeath", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, killerid, reason);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehicleSpawn( int vehicleid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleSpawn", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehicleDeath( int vehicleid, int killerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleDeath", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid, killerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerText( int playerid, char* text )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerText", "(ILjava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(getPlayerCodepage(playerid), text, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len);
	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, str);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerCommandText( int playerid, char* cmdtext )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerCommandText", "(ILjava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(getPlayerCodepage(playerid), cmdtext, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len);
	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, str);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerRequestClass( int playerid, int classid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerRequestClass", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, classid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerEnterVehicle( int playerid, int vehicleid, int ispassenger )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerEnterVehicle", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, vehicleid, ispassenger);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerExitVehicle( int playerid, int vehicleid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerExitVehicle", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, vehicleid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerStateChange( int playerid, int newstate, int oldstate )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerStateChange", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, newstate, oldstate);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerEnterCheckpoint( int playerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerEnterCheckpoint", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerLeaveCheckpoint( int playerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerLeaveCheckpoint", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerEnterRaceCheckpoint( int playerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerEnterRaceCheckpoint", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerLeaveRaceCheckpoint( int playerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerLeaveRaceCheckpoint", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnRconCommand( char* cmd )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onRconCommand", "(Ljava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(getServerCodepage(), cmd, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len);
	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, str);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerRequestSpawn( int playerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerRequestSpawn", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnObjectMoved( int objectid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onObjectMoved", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, objectid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerObjectMoved( int playerid, int objectid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerObjectMoved", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, objectid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerPickUpPickup( int playerid, int pickupid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerPickUpPickup", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, pickupid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehicleMod( int playerid, int vehicleid, int componentid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleMod", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, vehicleid, componentid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnEnterExitModShop( int playerid, int enterexit, int interiorid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onEnterExitModShop", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, enterexit, interiorid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehiclePaintjob( int playerid, int vehicleid, int paintjobid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehiclePaintjob", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, vehicleid, paintjobid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehicleRespray( int playerid, int vehicleid, int color1, int color2 )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleRespray", "(IIII)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, vehicleid, color1, color2);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehicleDamageStatusUpdate( int vehicleid, int playerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleDamageStatusUpdate", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnUnoccupiedVehicleUpdate(int vehicleid, int playerid, int passenger_seat)
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onUnoccupiedVehicleUpdate", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid, playerid, passenger_seat);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerSelectedMenuRow( int playerid, int row )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerSelectedMenuRow", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, row);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerExitedMenu( int playerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerExitedMenu", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerInteriorChange( int playerid, int newinteriorid, int oldinteriorid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerInteriorChange", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, newinteriorid, oldinteriorid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerKeyStateChange( int playerid, int newkeys, int oldkeys )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerKeyStateChange", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, newkeys, oldkeys);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnRconLoginAttempt( char* ip, char* password, int success )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onRconLoginAttempt", "(Ljava/lang/String;Ljava/lang/String;I)I");
	if( !jmid ) return 0;

	jstring iptext = env->NewStringUTF(ip);

	jchar wtext[1024];
	int len = mbs2wcs(getServerCodepage(), password, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));
	jstring str = env->NewString(wtext, len);

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, iptext, str, success);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerUpdate( int playerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerUpdate", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerStreamIn( int playerid, int forplayerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerStreamIn", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, forplayerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerStreamOut( int playerid, int forplayerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerStreamOut", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, forplayerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehicleStreamIn( int vehicleid, int forplayerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleStreamIn", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid, forplayerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehicleStreamOut( int vehicleid, int forplayerid )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onVehicleStreamOut", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, vehicleid, forplayerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnDialogResponse( int playerid, int dialogid, int response, int listitem, char *inputtext )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onDialogResponse", "(IIIILjava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(getPlayerCodepage(playerid), inputtext, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len);
	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, dialogid, response, listitem, str);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerTakeDamage(int playerid, int issuerid, float amount, int weaponid)
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerTakeDamage", "(IIFI)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, issuerid, amount, weaponid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerGiveDamage(int playerid, int damagedid, float amount, int weaponid)
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerGiveDamage", "(IIFI)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, damagedid, amount, weaponid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerClickMap(int playerid, float x, float y, float z)
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerClickMap", "(IFFF)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, x, y, z);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerClickTextDraw(int playerid, int clickedid)
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerClickTextDraw", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, clickedid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerClickPlayerTextDraw(int playerid, int playertextid)
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerClickPlayerTextDraw", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, playertextid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerClickPlayer( int playerid, int clickedplayerid, int source )
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerClickPlayer", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, clickedplayerid, source);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerEditObject(int playerid, int playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ)
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerEditObject", "(IIIIFFFFFF)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, playerobject, objectid, response, fX, fY, fZ, fRotX, fRotY, fRotZ);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerEditAttachedObject(int playerid, int response, int index, int modelid, int boneid, float fOffsetX, float fOffsetY, float fOffsetZ, float fRotX, float fRotY, float fRotZ, float fScaleX, float fScaleY, float fScaleZ)
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerEditAttachedObject", "(IIIIIFFFFFFFFF)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, response, index, modelid, boneid, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float fX, float fY, float fZ)
{
	if( !callbackHandlerObject ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callbackHandlerClass, "onPlayerSelectObject", "(IIIIFFF)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callbackHandlerObject, jmid, playerid, type, objectid, modelid, fX, fY, fZ);
	jni_jvm_printExceptionStack( env );
	return ret;
}
