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
const char classpath[] = "net/gtaun/shoebill/Shoebill";

jclass shoebill_cls = NULL;
jobject shoebill_obj = NULL;

jclass event_handler_cls = NULL;
jobject event_handler_obj = NULL;

int server_codepage = 0;
int player_codepage[MAX_PLAYERS] = {0};


int Initialize();
int Uninitialize();


bool OnLoadPlugin()
{
	logprintf( "  > ShoebillPlugin Milestone 2 for SA-MP 0.3D by MK124 & JoJLlmAn" );

	if( jni_jvm_create(jarpath) < 0 )
	{
		logprintf( "  > Error: Can't create Java VM." );
		return false;
	}

	logprintf( "  > Java VM has been created." );

	int ret = Initialize();
	return ret >= 0;
}

void OnUnloadPlugin()
{
	Uninitialize();
	if( jni_jvm_destroy() >= 0 ) logprintf( "Java VM destroyed." );
}

int Initialize()
{
	shoebill_cls = env->FindClass(classpath);
	if( !shoebill_cls )
	{
		logprintf( "  > Error: Can't find main class [%s].", classpath );
		return -1;
	}

	if( jni_jvm_newobject(shoebill_cls, &shoebill_obj) < 0 )
	{
		logprintf( "  > Error: Can't create main object [%s].", classpath );
		return -2;
	}

	static jmethodID jmid = env->GetMethodID(shoebill_cls, "getCallbackHandler", "()Lnet/gtaun/shoebill/samp/ISampCallbackHandler;");
	if( !jmid )
	{
		logprintf( "  > Error: Can't find method getCallbackHandler()." );
		return -3;
	}

	event_handler_obj = env->CallObjectMethod(shoebill_obj, jmid);
	if( event_handler_obj == NULL )
	{
		logprintf( "  > Error: Can't find main EventHandler." );
		return -4;
	}
	
	shoebill_obj = env->NewGlobalRef(shoebill_obj);
	shoebill_cls = (jclass)( env->NewGlobalRef(shoebill_cls) );
	event_handler_obj = env->NewGlobalRef(event_handler_obj);
	event_handler_cls = env->NewGlobalRef(env->GetObjectClass(event_handler_obj));

	logprintf( "  > Shoebill has been initialized." );
	return 0;
}

int Uninitialize()
{
	env->DeleteGlobalRef( shoebill_obj );
	env->DeleteGlobalRef( shoebill_cls );
	env->DeleteGlobalRef( event_handler_obj );
	env->DeleteGlobalRef( event_handler_cls );
	return 0;
}

void OnProcessTick()
{
	if( !event_handler_obj ) return;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onProcessTick", "()V");
	if( !jmid ) return;

	env->CallVoidMethod(event_handler_obj, jmid);
}

int OnGameModeInit()
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onGameModeInit", "()I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid);
}

int OnGameModeExit()
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onGameModeExit", "()I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid);
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
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerConnect", "(I)I");
	if( !jmid ) return 0;
	
	return env->CallIntMethod(event_handler_obj, jmid, playerid);
}

int OnPlayerDisconnect( int playerid, int reason )
{
	if( !event_handler_obj ) return 0;

	player_codepage[playerid] = 0;
	
	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerDisconnect", "(II)I");
	if( !jmid ) return 0;
	
	return env->CallIntMethod(event_handler_obj, jmid, playerid, reason);
}

int OnPlayerSpawn( int playerid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerSpawn", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid);
}

int OnPlayerDeath( int playerid, int killerid, int reason )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerDeath", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, killerid, reason);
}

int OnVehicleSpawn( int vehicleid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onVehicleSpawn", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, vehicleid);
}

int OnVehicleDeath( int vehicleid, int killerid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onVehicleDeath", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, vehicleid, killerid);
}

int OnPlayerText( int playerid, char* text )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerText", "(ILjava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(player_codepage[playerid], text, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len-1);
	return env->CallIntMethod(event_handler_obj, jmid, playerid, str);
}

int OnPlayerCommandText( int playerid, char* cmdtext )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerCommandText", "(ILjava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(player_codepage[playerid], cmdtext, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len-1);
	return env->CallIntMethod(event_handler_obj, jmid, playerid, str);
}

int OnPlayerRequestClass( int playerid, int classid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerRequestClass", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, classid);
}

int OnPlayerEnterVehicle( int playerid, int vehicleid, int ispassenger )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerEnterVehicle", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, vehicleid, ispassenger);
}

int OnPlayerExitVehicle( int playerid, int vehicleid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerExitVehicle", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, vehicleid);
}

int OnPlayerStateChange( int playerid, int newstate, int oldstate )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerStateChange", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, newstate, oldstate);
}

int OnPlayerEnterCheckpoint( int playerid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerEnterCheckpoint", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid);
}

int OnPlayerLeaveCheckpoint( int playerid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerLeaveCheckpoint", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid);
}

int OnPlayerEnterRaceCheckpoint( int playerid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerEnterRaceCheckpoint", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid);
}

int OnPlayerLeaveRaceCheckpoint( int playerid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerLeaveRaceCheckpoint", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid);
}

int OnRconCommand( char* cmd )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onServerRconCommand", "(Ljava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(server_codepage, cmd, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len-1);
	return env->CallIntMethod(event_handler_obj, jmid, str);
}

int OnPlayerRequestSpawn( int playerid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerRequestSpawn", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid);
}

int OnObjectMoved( int objectid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onObjectMoved", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, objectid);
}

int OnPlayerObjectMoved( int playerid, int objectid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerObjectMoved", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, objectid);
}

int OnPlayerPickUpPickup( int playerid, int pickupid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerPickUpPickup", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, pickupid);
}

int OnVehicleMod( int playerid, int vehicleid, int componentid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onVehicleMod", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, vehicleid, componentid);
}

int OnEnterExitModShop( int playerid, int enterexit, int interiorid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onEnterExitModShop", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, enterexit, interiorid);
}

int OnVehiclePaintjob( int playerid, int vehicleid, int paintjobid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onVehiclePaintjob", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, vehicleid, paintjobid);
}

int OnVehicleRespray( int playerid, int vehicleid, int color1, int color2 )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onVehicleRespray", "(IIII)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, vehicleid, color1, color2);
}

int OnVehicleDamageStatusUpdate( int vehicleid, int playerid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onVehicleDamageStatusUpdate", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, vehicleid, playerid);
}

int OnUnoccupiedVehicleUpdate(int vehicleid, int playerid, int passenger_seat)
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onUnoccupiedVehicleUpdate", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, vehicleid, playerid, passenger_seat);
}

int OnPlayerSelectedMenuRow( int playerid, int row )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerSelectedMenuRow", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, row);
}

int OnPlayerExitedMenu( int playerid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerExitedMenu", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid);
}

int OnPlayerInteriorChange( int playerid, int newinteriorid, int oldinteriorid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerInteriorChange", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, newinteriorid, oldinteriorid);
}

int OnPlayerKeyStateChange( int playerid, int newkeys, int oldkeys )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerKeyStateChange", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, newkeys, oldkeys);
}

int OnRconLoginAttempt( char* ip, char* password, int success )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onServerRconLoginAttempt", "(Ljava/lang/String;Ljava/lang/String;I)I");
	if( !jmid ) return 0;

	jstring iptext = env->NewStringUTF(ip);

	jchar wtext[1024];
	int len = mbs2wcs(server_codepage, password, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));
	jstring str = env->NewString(wtext, len-1);

	return env->CallIntMethod(event_handler_obj, jmid, iptext, str, success);
}

int OnPlayerUpdate( int playerid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerUpdate", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid);
}

int OnPlayerStreamIn( int playerid, int forplayerid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerStreamIn", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, forplayerid);
}

int OnPlayerStreamOut( int playerid, int forplayerid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerStreamOut", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, forplayerid);
}

int OnVehicleStreamIn( int vehicleid, int forplayerid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onVehicleStreamIn", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, vehicleid, forplayerid);
}

int OnVehicleStreamOut( int vehicleid, int forplayerid )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onVehicleStreamOut", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, vehicleid, forplayerid);
}

int OnDialogResponse( int playerid, int dialogid, int response, int listitem, char *inputtext )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onDialogResponse", "(IIIILjava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(player_codepage[playerid], inputtext, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len-1);
	return env->CallIntMethod(event_handler_obj, jmid, playerid, dialogid, response, listitem, str);
}

int OnPlayerTakeDamage(int playerid, int issuerid, float amount, int weaponid)
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerTakeDamage", "(IIFI)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, issuerid, amount, weaponid);
}

int OnPlayerGiveDamage(int playerid, int damagedid, float amount, int weaponid)
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerGiveDamage", "(IIFI)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, damagedid, amount, weaponid);
}

int OnPlayerClickMap(int playerid, float x, float y, float z)
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerClickMap", "(IFFF)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, x, y, z);
}

int OnPlayerClickPlayer( int playerid, int clickedplayerid, int source )
{
	if( !event_handler_obj ) return 0;

	static jmethodID jmid = env->GetMethodID(event_handler_cls, "onPlayerClickPlayer", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(event_handler_obj, jmid, playerid, clickedplayerid, source);
}
