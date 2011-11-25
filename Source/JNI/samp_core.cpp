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

const char classpath[] = "./shoebill/libraries/*.jar";
const char mainclass[] = "net/gtaun/shoebill/Shoebill";

jclass jmaincls = NULL;
jobject jmainobj = NULL;

int server_codepage = 0;
int player_codepage[MAX_PLAYERS] = {0};


int Initialize();


bool OnLoadPlugin()
{
	logprintf( "  > ShoebillPlugin Milestone 2 (20111125) for SA-MP 0.3C R5 by MK124 & JoJLlmAn" );

	if( jni_jvm_create(classpath) < 0 )
	{
		logprintf( "  > Error: Can't create Java VM." );
		return false;
	}

	logprintf( "  > Java VM has been created." );

	int ret = Initialize();
	return ret >= 0;
}

int Initialize()
{
	jmaincls = env->FindClass(mainclass);
	if( !jmaincls )
	{
		logprintf( "  > Error: Can't find main class [%s].", mainclass );
		return -1;
	}

	if( jni_jvm_newobject(jmaincls, &jmainobj) < 0 )
	{
		logprintf( "  > Error: Can't create main object [%s].", mainclass );
		return -2;
	}

	logprintf( "  > Shoebill has been initialized." );
	return 0;
}

void OnUnloadPlugin()
{
	if( jni_jvm_destroy() >= 0 ) logprintf( "Java VM destroyed." );
}

void OnProcessTick()
{
	if( !jmainobj ) return;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onProcessTick", "()V");
	if( !jmid ) return;

	env->CallVoidMethod(jmainobj, jmid);
}

int OnGameModeInit()
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onGameModeInit", "()I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid);
}

int OnGameModeExit()
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onGameModeExit", "()I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid);
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
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerConnect", "(I)I");
	if( !jmid ) return 0;
	
	return env->CallIntMethod(jmainobj, jmid, playerid);
}

int OnPlayerDisconnect( int playerid, int reason )
{
	if( !jmainobj ) return 0;

	player_codepage[playerid] = 0;
	
	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerDisconnect", "(II)I");
	if( !jmid ) return 0;
	
	return env->CallIntMethod(jmainobj, jmid, playerid, reason);
}

int OnPlayerSpawn( int playerid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerSpawn", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid);
}

int OnPlayerDeath( int playerid, int killerid, int reason )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerDeath", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, killerid, reason);
}

int OnVehicleSpawn( int vehicleid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onVehicleSpawn", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, vehicleid);
}

int OnVehicleDeath( int vehicleid, int killerid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onVehicleDeath", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, vehicleid, killerid);
}

int OnPlayerText( int playerid, char* text )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerText", "(ILjava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(player_codepage[playerid], text, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len-1);
	return env->CallIntMethod(jmainobj, jmid, playerid, str);
}

int OnPlayerCommandText( int playerid, char* cmdtext )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerCommandText", "(ILjava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(player_codepage[playerid], cmdtext, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len-1);
	return env->CallIntMethod(jmainobj, jmid, playerid, str);
}

int OnPlayerRequestClass( int playerid, int classid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerRequestClass", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, classid);
}

int OnPlayerEnterVehicle( int playerid, int vehicleid, int ispassenger )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerEnterVehicle", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, vehicleid, ispassenger);
}

int OnPlayerExitVehicle( int playerid, int vehicleid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerExitVehicle", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, vehicleid);
}

int OnPlayerStateChange( int playerid, int newstate, int oldstate )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerStateChange", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, newstate, oldstate);
}

int OnPlayerEnterCheckpoint( int playerid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerEnterCheckpoint", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid);
}

int OnPlayerLeaveCheckpoint( int playerid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerLeaveCheckpoint", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid);
}

int OnPlayerEnterRaceCheckpoint( int playerid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerEnterRaceCheckpoint", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid);
}

int OnPlayerLeaveRaceCheckpoint( int playerid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerLeaveRaceCheckpoint", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid);
}

int OnRconCommand( char* cmd )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onServerRconCommand", "(Ljava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(server_codepage, cmd, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len-1);
	return env->CallIntMethod(jmainobj, jmid, str);
}

int OnPlayerRequestSpawn( int playerid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerRequestSpawn", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid);
}

int OnObjectMoved( int objectid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onObjectMoved", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, objectid);
}

int OnPlayerObjectMoved( int playerid, int objectid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerObjectMoved", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, objectid);
}

int OnPlayerPickUpPickup( int playerid, int pickupid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerPickUpPickup", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, pickupid);
}

int OnVehicleMod( int playerid, int vehicleid, int componentid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onVehicleMod", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, vehicleid, componentid);
}

int OnEnterExitModShop( int playerid, int enterexit, int interiorid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onEnterExitModShop", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, enterexit, interiorid);
}

int OnVehiclePaintjob( int playerid, int vehicleid, int paintjobid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onVehiclePaintjob", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, vehicleid, paintjobid);
}

int OnVehicleRespray( int playerid, int vehicleid, int color1, int color2 )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onVehicleRespray", "(IIII)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, vehicleid, color1, color2);
}

int OnVehicleDamageStatusUpdate( int vehicleid, int playerid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onVehicleDamageStatusUpdate", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, vehicleid, playerid);
}

int OnPlayerSelectedMenuRow( int playerid, int row )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerSelectedMenuRow", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, row);
}

int OnPlayerExitedMenu( int playerid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerExitedMenu", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid);
}

int OnPlayerInteriorChange( int playerid, int newinteriorid, int oldinteriorid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerInteriorChange", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, newinteriorid, oldinteriorid);
}

int OnPlayerKeyStateChange( int playerid, int newkeys, int oldkeys )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerKeyStateChange", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, newkeys, oldkeys);
}

int OnRconLoginAttempt( char* ip, char* password, int success )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onServerRconLoginAttempt", "(Ljava/lang/String;Ljava/lang/String;I)I");
	if( !jmid ) return 0;

	jstring iptext = env->NewStringUTF(ip);

	jchar wtext[1024];
	int len = mbs2wcs(server_codepage, password, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));
	jstring str = env->NewString(wtext, len-1);

	return env->CallIntMethod(jmainobj, jmid, iptext, str, success);
}

int OnPlayerUpdate( int playerid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerUpdate", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid);
}

int OnPlayerStreamIn( int playerid, int forplayerid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerStreamIn", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, forplayerid);
}

int OnPlayerStreamOut( int playerid, int forplayerid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerStreamOut", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, forplayerid);
}

int OnVehicleStreamIn( int vehicleid, int forplayerid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onVehicleStreamIn", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, vehicleid, forplayerid);
}

int OnVehicleStreamOut( int vehicleid, int forplayerid )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onVehicleStreamOut", "(II)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, vehicleid, forplayerid);
}

int OnDialogResponse( int playerid, int dialogid, int response, int listitem, char *inputtext )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onDialogResponse", "(IIIILjava/lang/String;)I");
	if( !jmid ) return 0;

	jchar wtext[1024];
	int len = mbs2wcs(player_codepage[playerid], inputtext, -1, wtext, sizeof(wtext)/sizeof(wtext[0]));

	jstring str = env->NewString(wtext, len-1);
	return env->CallIntMethod(jmainobj, jmid, playerid, dialogid, response, listitem, str);
}

int OnPlayerClickPlayer( int playerid, int clickedplayerid, int source )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onPlayerClickPlayer", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, playerid, clickedplayerid, source);
}

int OnTimer( int TimerIndex )
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onTimer", "(I)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, TimerIndex);
}

int OnUnoccupiedVehicleUpdate(int vehicleid, int playerid, int passenger_seat)
{
	if( !jmainobj ) return 0;

	static jmethodID jmid = env->GetMethodID(jmaincls, "onUnoccupiedVehicleUpdate", "(III)I");
	if( !jmid ) return 0;

	return env->CallIntMethod(jmainobj, jmid, vehicleid, playerid, passenger_seat);
}
