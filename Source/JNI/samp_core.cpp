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

#include <jni.h>
#include <string.h>

#include "encoding.h"
#include "jni_core.h"
#include "../Wrapper/a_samp.h"

#if defined(LINUX)
#include "linux.h"
#endif

const char jarpath[] = "./shoebill/libraries/*.jar";
const char classpath[] = "net/gtaun/shoebill/ShoebillImpl";

jclass shoebill_cls = NULL;
jobject shoebill_obj = NULL;

jclass callback_handler_cls = NULL;
jobject callback_handler_obj = NULL;

int server_codepage = 0;
int player_codepage[MAX_PLAYERS] = {0};


int Initialize( JNIEnv *env );
int Uninitialize( JNIEnv *env );


bool OnLoadPlugin()
{
	logprintf( "  > ShoebillPlugin Milestone 2 for SA-MP 0.3D by MK124 & JoJLlmAn" );

	JNIEnv *env;
	if( jni_jvm_create(&env, jarpath) < 0 )
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
	shoebill_cls = env->FindClass(classpath);
	if( !shoebill_cls )
	{
		logprintf( "  > Error: Can't find main class [%s].", classpath );
		return -1;
	}

	if( jni_jvm_constructObject(env, shoebill_cls, &shoebill_obj) < 0 )
	{
		logprintf( "  > Error: Can't create main object [%s].", classpath );
		return -2;
	}
	
	shoebill_obj = env->NewGlobalRef(shoebill_obj);
	shoebill_cls = (jclass)( env->NewGlobalRef(shoebill_cls) );

	logprintf( "  > Shoebill has been initialized." );
	return 0;
}

int Uninitialize( JNIEnv *env )
{
	env->DeleteGlobalRef( shoebill_obj );
	env->DeleteGlobalRef( shoebill_cls );
	return 0;
}

void OnProcessTick()
{
	if( !callback_handler_obj ) return;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onProcessTick", "()V");
	if( !jmid ) return;

	env->CallVoidMethod(callback_handler_obj, jmid);
	jni_jvm_printExceptionStack( env );
}

int OnGameModeInit()
{
	if( !shoebill_obj ) return 1;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid_gch = env->GetMethodID(shoebill_cls, "getCallbackHandler", "()Lnet/gtaun/shoebill/samp/SampCallbackHandler;");
	if( !jmid_gch )
	{
		logprintf( "ShoebillPlugin Error: Can't find method getCallbackHandler()." );
		return 1;
	}

	callback_handler_obj = env->CallObjectMethod(shoebill_obj, jmid_gch);
	if( callback_handler_obj == NULL )
	{
		logprintf( "ShoebillPlugin Error: Can't find main EventHandler." );
		return 1;
	}

	callback_handler_obj = env->NewGlobalRef(callback_handler_obj);
	callback_handler_cls = (jclass)( env->NewGlobalRef(env->GetObjectClass(callback_handler_obj)) );

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onGameModeInit", "()I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnGameModeExit()
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onGameModeExit", "()I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid);
	jni_jvm_printExceptionStack( env );

	env->DeleteGlobalRef( callback_handler_obj );
	env->DeleteGlobalRef( callback_handler_cls );
	callback_handler_obj = NULL;
	callback_handler_cls = NULL;

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
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerConnect", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerDisconnect( int playerid, int reason )
{
	if( !callback_handler_obj ) return 0;

	player_codepage[playerid] = 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerDisconnect", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, reason);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerSpawn( int playerid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerSpawn", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerDeath( int playerid, int killerid, int reason )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerDeath", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, killerid, reason);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehicleSpawn( int vehicleid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onVehicleSpawn", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, vehicleid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehicleDeath( int vehicleid, int killerid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onVehicleDeath", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, vehicleid, killerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerText( int playerid, char* text )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerText", "(ILjava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(player_codepage[playerid], text, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len-1);
	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, str);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerCommandText( int playerid, char* cmdtext )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerCommandText", "(ILjava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(player_codepage[playerid], cmdtext, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len-1);
	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, str);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerRequestClass( int playerid, int classid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerRequestClass", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, classid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerEnterVehicle( int playerid, int vehicleid, int ispassenger )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerEnterVehicle", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, vehicleid, ispassenger);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerExitVehicle( int playerid, int vehicleid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerExitVehicle", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, vehicleid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerStateChange( int playerid, int newstate, int oldstate )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerStateChange", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, newstate, oldstate);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerEnterCheckpoint( int playerid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerEnterCheckpoint", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerLeaveCheckpoint( int playerid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerLeaveCheckpoint", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerEnterRaceCheckpoint( int playerid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerEnterRaceCheckpoint", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerLeaveRaceCheckpoint( int playerid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerLeaveRaceCheckpoint", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnRconCommand( char* cmd )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onRconCommand", "(Ljava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(server_codepage, cmd, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len-1);
	jint ret = env->CallIntMethod(callback_handler_obj, jmid, str);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerRequestSpawn( int playerid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerRequestSpawn", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnObjectMoved( int objectid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onObjectMoved", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, objectid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerObjectMoved( int playerid, int objectid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerObjectMoved", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, objectid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerPickUpPickup( int playerid, int pickupid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerPickUpPickup", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, pickupid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehicleMod( int playerid, int vehicleid, int componentid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onVehicleMod", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, vehicleid, componentid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnEnterExitModShop( int playerid, int enterexit, int interiorid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onEnterExitModShop", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, enterexit, interiorid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehiclePaintjob( int playerid, int vehicleid, int paintjobid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onVehiclePaintjob", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, vehicleid, paintjobid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehicleRespray( int playerid, int vehicleid, int color1, int color2 )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onVehicleRespray", "(IIII)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, vehicleid, color1, color2);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehicleDamageStatusUpdate( int vehicleid, int playerid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onVehicleDamageStatusUpdate", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, vehicleid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnUnoccupiedVehicleUpdate(int vehicleid, int playerid, int passenger_seat)
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onUnoccupiedVehicleUpdate", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, vehicleid, playerid, passenger_seat);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerSelectedMenuRow( int playerid, int row )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerSelectedMenuRow", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, row);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerExitedMenu( int playerid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerExitedMenu", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerInteriorChange( int playerid, int newinteriorid, int oldinteriorid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerInteriorChange", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, newinteriorid, oldinteriorid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerKeyStateChange( int playerid, int newkeys, int oldkeys )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerKeyStateChange", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, newkeys, oldkeys);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnRconLoginAttempt( char* ip, char* password, int success )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onRconLoginAttempt", "(Ljava/lang/String;Ljava/lang/String;I)I");
	if( !jmid ) return 0;

	jstring iptext = env->NewStringUTF(ip);

	jchar wtext[1024];
	int len = mbs2wcs(server_codepage, password, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));
	jstring str = env->NewString(wtext, len-1);

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, iptext, str, success);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerUpdate( int playerid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerUpdate", "(I)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerStreamIn( int playerid, int forplayerid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerStreamIn", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, forplayerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerStreamOut( int playerid, int forplayerid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerStreamOut", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, forplayerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehicleStreamIn( int vehicleid, int forplayerid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onVehicleStreamIn", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, vehicleid, forplayerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnVehicleStreamOut( int vehicleid, int forplayerid )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onVehicleStreamOut", "(II)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, vehicleid, forplayerid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnDialogResponse( int playerid, int dialogid, int response, int listitem, char *inputtext )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onDialogResponse", "(IIIILjava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(player_codepage[playerid], inputtext, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len-1);
	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, dialogid, response, listitem, str);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerTakeDamage(int playerid, int issuerid, float amount, int weaponid)
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerTakeDamage", "(IIFI)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, issuerid, amount, weaponid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerGiveDamage(int playerid, int damagedid, float amount, int weaponid)
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerGiveDamage", "(IIFI)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, damagedid, amount, weaponid);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerClickMap(int playerid, float x, float y, float z)
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerClickMap", "(IFFF)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, x, y, z);
	jni_jvm_printExceptionStack( env );
	return ret;
}

int OnPlayerClickPlayer( int playerid, int clickedplayerid, int source )
{
	if( !callback_handler_obj ) return 0;

	JNIEnv *env;
	jvm->AttachCurrentThread((void**)&env, NULL);

	static jmethodID jmid = env->GetMethodID(callback_handler_cls, "onPlayerClickPlayer", "(III)I");
	if( !jmid ) return 0;

	jint ret = env->CallIntMethod(callback_handler_obj, jmid, playerid, clickedplayerid, source);
	jni_jvm_printExceptionStack( env );
	return ret;
}
