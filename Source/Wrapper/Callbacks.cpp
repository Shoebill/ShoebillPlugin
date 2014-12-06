/**
 * Copyright (C) 2011-2014 MK124
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

#include "AmxHelper.h"
#include "samp.h"

static cell AMX_NATIVE_CALL n_OnGameModeInit(AMX* amx, cell* params)
{
	return OnGameModeInit();
}

static cell AMX_NATIVE_CALL n_OnGameModeExit(AMX* amx, cell* params)
{
	return OnGameModeExit();
}

static cell AMX_NATIVE_CALL n_OnFilterScriptInit(AMX* amx, cell* params)
{
	return OnFilterScriptInit();
}

static cell AMX_NATIVE_CALL n_OnFilterScriptExit(AMX* amx, cell* params)
{
	return OnFilterScriptExit();
}

static cell AMX_NATIVE_CALL n_OnPlayerConnect(AMX* amx, cell* params)
{
	return OnPlayerConnect(params[1]);
}

static cell AMX_NATIVE_CALL n_OnPlayerDisconnect(AMX* amx, cell* params)
{
	return OnPlayerDisconnect(params[1], params[2]);
}

static cell AMX_NATIVE_CALL n_OnPlayerSpawn(AMX* amx, cell* params)
{
	return OnPlayerSpawn(params[1]);
}

static cell AMX_NATIVE_CALL n_OnPlayerDeath(AMX* amx, cell* params)
{
	return OnPlayerDeath(params[1], params[2], params[3]);
}

static cell AMX_NATIVE_CALL n_OnVehicleSpawn(AMX* amx, cell* params)
{
	return OnVehicleSpawn(params[1]);
}

static cell AMX_NATIVE_CALL n_OnVehicleDeath(AMX* amx, cell* params)
{
	return OnVehicleDeath(params[1], params[2]);
}

static cell AMX_NATIVE_CALL n_OnPlayerText(AMX* amx, cell* params)
{
	char text[1024];
	amx_GetString(amx, params[2], text, sizeof(text));

	return OnPlayerText(params[1], text);
}

static cell AMX_NATIVE_CALL n_OnPlayerCommandText(AMX* amx, cell* params)
{
	char text[1024];
	amx_GetString(amx, params[2], text, sizeof(text));

	return OnPlayerCommandText(params[1], text);
}

static cell AMX_NATIVE_CALL n_OnPlayerRequestClass(AMX* amx, cell* params)
{
	return OnPlayerRequestClass(params[1], params[2]);
}

static cell AMX_NATIVE_CALL n_OnPlayerEnterVehicle(AMX* amx, cell* params)
{
	return OnPlayerEnterVehicle(params[1], params[2], params[3]);
}

static cell AMX_NATIVE_CALL n_OnPlayerExitVehicle(AMX* amx, cell* params)
{
	return OnPlayerExitVehicle(params[1], params[2]);
}

static cell AMX_NATIVE_CALL n_OnPlayerStateChange(AMX* amx, cell* params)
{
	return OnPlayerStateChange(params[1], params[2], params[3]);
}

static cell AMX_NATIVE_CALL n_OnPlayerEnterCheckpoint(AMX* amx, cell* params)
{
	return OnPlayerEnterCheckpoint(params[1]);
}

static cell AMX_NATIVE_CALL n_OnPlayerLeaveCheckpoint(AMX* amx, cell* params)
{
	return OnPlayerLeaveCheckpoint(params[1]);
}

static cell AMX_NATIVE_CALL n_OnPlayerEnterRaceCheckpoint(AMX* amx, cell* params)
{
	return OnPlayerEnterRaceCheckpoint(params[1]);
}

static cell AMX_NATIVE_CALL n_OnPlayerLeaveRaceCheckpoint(AMX* amx, cell* params)
{
	return OnPlayerLeaveRaceCheckpoint(params[1]);
}

static cell AMX_NATIVE_CALL n_OnRconCommand(AMX* amx, cell* params)
{
	char text[1024];
	amx_GetString(amx, params[1], text, sizeof(text));

	return OnRconCommand(text);
}

static cell AMX_NATIVE_CALL n_OnPlayerRequestSpawn(AMX* amx, cell* params)
{
	return OnPlayerRequestSpawn(params[1]);
}

static cell AMX_NATIVE_CALL n_OnObjectMoved(AMX* amx, cell* params)
{
	return OnObjectMoved(params[1]);
}

static cell AMX_NATIVE_CALL n_OnPlayerObjectMoved(AMX* amx, cell* params)
{
	return OnPlayerObjectMoved(params[1], params[2]);
}

static cell AMX_NATIVE_CALL n_OnPlayerPickUpPickup(AMX* amx, cell* params)
{
	return OnPlayerPickUpPickup(params[1], params[2]);
}

static cell AMX_NATIVE_CALL n_OnVehicleMod(AMX* amx, cell* params)
{
	return OnVehicleMod(params[1], params[2], params[3]);
}

static cell AMX_NATIVE_CALL n_OnEnterExitModShop(AMX* amx, cell* params)
{
	return OnEnterExitModShop(params[1], params[2], params[3]);
}

static cell AMX_NATIVE_CALL n_OnVehiclePaintjob(AMX* amx, cell* params)
{
	return OnVehiclePaintjob(params[1], params[2], params[3]);
}

static cell AMX_NATIVE_CALL n_OnVehicleRespray(AMX* amx, cell* params)
{
	return OnVehicleRespray(params[1], params[2], params[3], params[4]);
}

static cell AMX_NATIVE_CALL n_OnVehicleDamageStatusUpdate(AMX* amx, cell* params)
{
	return OnVehicleDamageStatusUpdate(params[1], params[2]);
}

static cell AMX_NATIVE_CALL n_OnUnoccupiedVehicleUpdate(AMX* amx, cell* params)
{
	return OnUnoccupiedVehicleUpdate(params[1], params[2], params[3], amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));
}

static cell AMX_NATIVE_CALL n_OnPlayerSelectedMenuRow(AMX* amx, cell* params)
{
	return OnPlayerSelectedMenuRow(params[1], params[2]);
}

static cell AMX_NATIVE_CALL n_OnPlayerExitedMenu(AMX* amx, cell* params)
{
	return OnPlayerExitedMenu(params[1]);
}

static cell AMX_NATIVE_CALL n_OnPlayerInteriorChange(AMX* amx, cell* params)
{
	return OnPlayerInteriorChange(params[1], params[2], params[3]);
}

static cell AMX_NATIVE_CALL n_OnPlayerKeyStateChange(AMX* amx, cell* params)
{
	return OnPlayerKeyStateChange(params[1], params[2], params[3]);
}

static cell AMX_NATIVE_CALL n_OnRconLoginAttempt(AMX* amx, cell* params)
{
	char ip[16], password[64];
	amx_GetString(amx, params[1], ip, sizeof(ip));
	amx_GetString(amx, params[2], password, sizeof(password));

	return OnRconLoginAttempt(ip, password, params[3]);
}

static cell AMX_NATIVE_CALL n_OnPlayerUpdate(AMX* amx, cell* params)
{
	return OnPlayerUpdate(params[1]);
}

static cell AMX_NATIVE_CALL n_OnPlayerStreamIn(AMX* amx, cell* params)
{
	return OnPlayerStreamIn(params[1], params[2]);
}

static cell AMX_NATIVE_CALL n_OnPlayerStreamOut(AMX* amx, cell* params)
{
	return OnPlayerStreamOut(params[1], params[2]);
}

static cell AMX_NATIVE_CALL n_OnVehicleStreamIn(AMX* amx, cell* params)
{
	return OnVehicleStreamIn(params[1], params[2]);
}

static cell AMX_NATIVE_CALL n_OnVehicleStreamOut(AMX* amx, cell* params)
{
	return OnVehicleStreamOut(params[1], params[2]);
}

static cell AMX_NATIVE_CALL n_OnDialogResponse(AMX* amx, cell* params)
{
	char text[1024];
	amx_GetString(amx, params[5], text, sizeof(text));

	return OnDialogResponse(params[1], params[2], params[3], params[4], text);
}

static cell AMX_NATIVE_CALL n_OnPlayerTakeDamage(AMX* amx, cell* params)
{
	return OnPlayerTakeDamage(params[1], params[2], amx_ctof(params[3]), params[4], params[5]);
}

static cell AMX_NATIVE_CALL n_OnPlayerGiveDamage(AMX* amx, cell* params)
{
	return OnPlayerGiveDamage(params[1], params[2], amx_ctof(params[3]), params[4], params[5]);
}

static cell AMX_NATIVE_CALL n_OnPlayerClickMap(AMX* amx, cell* params)
{
	return OnPlayerClickMap(params[1], amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]));
}

static cell AMX_NATIVE_CALL n_OnPlayerClickTextDraw(AMX* amx, cell* params)
{
	return OnPlayerClickTextDraw(params[1], params[2]);
}

static cell AMX_NATIVE_CALL n_OnPlayerClickPlayerTextDraw(AMX* amx, cell* params)
{
	return OnPlayerClickPlayerTextDraw(params[1], params[2]);
}

static cell AMX_NATIVE_CALL n_OnPlayerClickPlayer(AMX* amx, cell* params)
{
	return OnPlayerClickPlayer(params[1], params[2], params[3]);
}

static cell AMX_NATIVE_CALL n_OnPlayerEditObject(AMX* amx, cell* params)
{
	return OnPlayerEditObject(params[1], params[2], params[3], params[4], amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]), amx_ctof(params[8]), amx_ctof(params[9]), amx_ctof(params[10]));
}

static cell AMX_NATIVE_CALL n_OnPlayerEditAttachedObject(AMX* amx, cell* params)
{
	return OnPlayerEditAttachedObject(params[1], params[2], params[3], params[4], params[5], amx_ctof(params[6]), amx_ctof(params[7]), amx_ctof(params[8]), amx_ctof(params[9]), amx_ctof(params[10]), amx_ctof(params[11]), amx_ctof(params[12]), amx_ctof(params[13]), amx_ctof(params[14]));
}

static cell AMX_NATIVE_CALL n_OnPlayerSelectObject(AMX* amx, cell* params)
{
	return OnPlayerSelectObject(params[1], params[2], params[3], params[4], amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]));
}

static cell AMX_NATIVE_CALL n_OnPlayerWeaponShot(AMX* amx, cell* params)
{
	return OnPlayerWeaponShot(params[1], params[2], params[3], params[4], amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]));
}

static cell AMX_NATIVE_CALL n_OnIncomingConnection(AMX* amx, cell* params)
{
	char str[24];
	amx_GetString(amx, params[2], str, sizeof(str));
	return OnIncomingConnection(params[1], str, params[3]);
}

AMX_NATIVE_INFO CallbackNatives[] =
{
	{ "OnGameModeInit",					n_OnGameModeInit },
	{ "OnGameModeExit",					n_OnGameModeExit },
	{ "OnFilterScriptInit",				n_OnFilterScriptInit },
	{ "OnFilterScriptExit",				n_OnFilterScriptExit },
	{ "OnPlayerConnect",				n_OnPlayerConnect },
	{ "OnPlayerDisconnect",				n_OnPlayerDisconnect },
	{ "OnPlayerSpawn",					n_OnPlayerSpawn },
	{ "OnPlayerDeath",					n_OnPlayerDeath },
	{ "OnVehicleSpawn",					n_OnVehicleSpawn },
	{ "OnVehicleDeath",					n_OnVehicleDeath },
	{ "OnPlayerText",					n_OnPlayerText },
	{ "OnPlayerCommandText",			n_OnPlayerCommandText },
	{ "OnPlayerRequestClass",			n_OnPlayerRequestClass },
	{ "OnPlayerEnterVehicle",			n_OnPlayerEnterVehicle },
	{ "OnPlayerExitVehicle",			n_OnPlayerExitVehicle },
	{ "OnPlayerStateChange",			n_OnPlayerStateChange },
	{ "OnPlayerEnterCheckpoint",		n_OnPlayerEnterCheckpoint },
	{ "OnPlayerLeaveCheckpoint",		n_OnPlayerLeaveCheckpoint },
	{ "OnPlayerEnterRaceCheckpoint",	n_OnPlayerEnterRaceCheckpoint },
	{ "OnPlayerLeaveRaceCheckpoint",	n_OnPlayerLeaveRaceCheckpoint },
	{ "OnRconCommand",					n_OnRconCommand },
	{ "OnPlayerRequestSpawn",			n_OnPlayerRequestSpawn },
	{ "OnObjectMoved",					n_OnObjectMoved },
	{ "OnPlayerObjectMoved",			n_OnPlayerObjectMoved },
	{ "OnPlayerPickUpPickup",			n_OnPlayerPickUpPickup },
	{ "OnVehicleMod",					n_OnVehicleMod },
	{ "OnEnterExitModShop",				n_OnEnterExitModShop },
	{ "OnVehiclePaintjob",				n_OnVehiclePaintjob },
	{ "OnVehicleRespray",				n_OnVehicleRespray },
	{ "OnVehicleDamageStatusUpdate",	n_OnVehicleDamageStatusUpdate },
	{ "OnUnoccupiedVehicleUpdate",		n_OnUnoccupiedVehicleUpdate },
	{ "OnPlayerSelectedMenuRow",		n_OnPlayerSelectedMenuRow },
	{ "OnPlayerExitedMenu",				n_OnPlayerExitedMenu },
	{ "OnPlayerInteriorChange",			n_OnPlayerInteriorChange },
	{ "OnPlayerKeyStateChange",			n_OnPlayerKeyStateChange },
	{ "OnRconLoginAttempt",				n_OnRconLoginAttempt },
	{ "OnPlayerUpdate",					n_OnPlayerUpdate },
	{ "OnPlayerStreamIn",				n_OnPlayerStreamIn },
	{ "OnPlayerStreamOut",				n_OnPlayerStreamOut },
	{ "OnVehicleStreamIn",				n_OnVehicleStreamIn },
	{ "OnVehicleStreamOut",				n_OnVehicleStreamOut },
	{ "OnDialogResponse",				n_OnDialogResponse },
	{ "OnPlayerTakeDamage",				n_OnPlayerTakeDamage },
	{ "OnPlayerGiveDamage",				n_OnPlayerGiveDamage },
	{ "OnPlayerClickMap",				n_OnPlayerClickMap },
	{ "OnPlayerClickTextDraw",			n_OnPlayerClickTextDraw },
	{ "OnPlayerClickPlayerTextDraw",	n_OnPlayerClickPlayerTextDraw },
	{ "OnPlayerClickPlayer",			n_OnPlayerClickPlayer },
	{ "OnPlayerEditObject",				n_OnPlayerEditObject },
	{ "OnPlayerEditAttachedObject",		n_OnPlayerEditAttachedObject },
	{ "OnPlayerSelectObject",			n_OnPlayerSelectObject },
	{ "OnPlayerWeaponShot",				n_OnPlayerWeaponShot },
	{ "OnIncomingConnection",			n_OnIncomingConnection },
	{ 0, 0 }
};
