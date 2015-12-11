/**
 * Copyright (C) 2011-2015 MK124
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
#include "Callbacks.h"
#include "JniUtils.h"

cell invokeCallback(AMX *amx, std::string name, cell *params) {
    static std::map<std::string, AMX_NATIVE> callbackMap = {
        { "OnGameModeInit", n_OnGameModeInit },
        { "OnGameModeExit", n_OnGameModeExit },
        { "OnPlayerConnect", n_OnPlayerConnect },
        { "OnPlayerDisconnect", n_OnPlayerDisconnect },
        { "OnPlayerSpawn", n_OnPlayerSpawn },
        { "OnPlayerDeath", n_OnPlayerDeath },
        { "OnVehicleSpawn", n_OnVehicleSpawn },
        { "OnVehicleDeath", n_OnVehicleDeath },
        { "OnPlayerText", n_OnPlayerText },
        { "OnPlayerCommandText", n_OnPlayerCommandText },
        { "OnPlayerRequestClass", n_OnPlayerRequestClass },
        { "OnPlayerEnterVehicle", n_OnPlayerEnterVehicle },
        { "OnPlayerExitVehicle", n_OnPlayerExitVehicle },
        { "OnPlayerStateChange", n_OnPlayerStateChange },
        { "OnPlayerEnterCheckpoint", n_OnPlayerEnterCheckpoint },
        { "OnPlayerLeaveCheckpoint", n_OnPlayerLeaveCheckpoint },
        { "OnPlayerEnterRaceCheckpoint", n_OnPlayerEnterRaceCheckpoint },
        { "OnPlayerLeaveRaceCheckpoint", n_OnPlayerLeaveRaceCheckpoint },
        { "OnRconCommand", n_OnRconCommand },
        { "OnPlayerRequestSpawn", n_OnPlayerRequestSpawn },
        { "OnObjectMoved", n_OnObjectMoved },
        { "OnPlayerObjectMoved", n_OnPlayerObjectMoved },
        { "OnPlayerPickUpPickup", n_OnPlayerPickUpPickup },
        { "OnVehicleMod", n_OnVehicleMod },
        { "OnEnterExitModShop", n_OnEnterExitModShop },
        { "OnVehiclePaintjob", n_OnVehiclePaintjob },
        { "OnVehicleRespray", n_OnVehicleRespray },
        { "OnVehicleDamageStatusUpdate", n_OnVehicleDamageStatusUpdate },
        { "OnUnoccupiedVehicleUpdate", n_OnUnoccupiedVehicleUpdate },
        { "OnPlayerSelectedMenuRow", n_OnPlayerSelectedMenuRow },
        { "OnPlayerExitedMenu", n_OnPlayerExitedMenu },
        { "OnPlayerInteriorChange", n_OnPlayerInteriorChange },
        { "OnPlayerKeyStateChange", n_OnPlayerKeyStateChange },
        { "OnRconLoginAttempt", n_OnRconLoginAttempt },
        { "OnPlayerUpdate", n_OnPlayerUpdate },
        { "OnPlayerStreamIn", n_OnPlayerStreamIn },
        { "OnPlayerStreamOut", n_OnPlayerStreamOut },
        { "OnVehicleStreamIn", n_OnVehicleStreamIn },
        { "OnVehicleStreamOut", n_OnVehicleStreamOut },
        { "OnDialogResponse", n_OnDialogResponse },
        { "OnPlayerTakeDamage", n_OnPlayerTakeDamage },
        { "OnPlayerGiveDamage", n_OnPlayerGiveDamage },
        { "OnPlayerClickMap", n_OnPlayerClickMap },
        { "OnPlayerClickTextDraw", n_OnPlayerClickTextDraw },
        { "OnPlayerClickPlayerTextDraw", n_OnPlayerClickPlayerTextDraw },
        { "OnPlayerClickPlayer", n_OnPlayerClickPlayer },
        { "OnPlayerEditObject", n_OnPlayerEditObject },
        { "OnPlayerEditAttachedObject", n_OnPlayerEditAttachedObject },
        { "OnPlayerSelectObject", n_OnPlayerSelectObject },
        { "OnPlayerWeaponShot", n_OnPlayerWeaponShot },
        { "OnIncomingConnection", n_OnIncomingConnection },
        { "OnTrailerUpdate", n_OnTrailerUpdate },
        { "OnActorStreamIn", n_OnActorStreamIn },
        { "OnActorStreamOut", n_OnActorStreamOut },
        { "OnPlayerGiveDamageActor", n_OnPlayerGiveDamageActor },
        { "OnVehicleSirenStateChange", n_OnVehicleSirenStateChange }
    };
    auto it = callbackMap.find(name);
    if (it != callbackMap.end()) {
		return it->second(amx, params);
    }
    return 0;
}

bool shouldCancelCallback(std::string callbackName, cell returnValue) {
	static std::map<std::string, int> callbackCancelValues = {
		{ "OnPlayerText", 0 },
		{ "OnPlayerCommandText", 1 },
		{ "OnRconCommand", 1 },
		{ "OnPlayerRequestClass", 0 },
		{ "OnDialogResponse", 1 },
		{ "OnPlayerClickPlayerTextDraw", 1 },
		{ "OnPlayerClickTextDraw", 1 },
		{ "OnPlayerConnect", 0 },
		{ "OnPlayerDisconnect", 0 },
		{ "OnPlayerKeyStateChange", 0 },
		{ "OnPlayerRequestSpawn", 0 },
		{ "OnPlayerSpawn", 0 },
		{ "OnPlayerTakeDamage", 0 },
		{ "OnPlayerUpdate", 0 },
		{ "OnPlayerWeaponShot", 0 },
		{ "OnTrailerUpdate", 0 },
		{ "OnUnoccupiedVehicleUpdate", 0 },
		{ "OnVehicleMod", 0 },
		{ "OnVehicleRespray", 0 } 
	};
	auto it = callbackCancelValues.find(callbackName);
	if (it != callbackCancelValues.end()) {
		return it->second == returnValue;
	}
	return false;
}

std::map<std::string, std::string> shoebillHookedCallbacks;

std::map<std::string, std::string>::iterator getIterator(std::string name)
{
	return shoebillHookedCallbacks.find(name);
}

std::map<std::string, std::string>::const_iterator getEnd()
{
	return shoebillHookedCallbacks.end();
}

bool hookCallback(std::string name, std::string types)
{
	shoebillHookedCallbacks[name] = types;
	return true;
}

bool unhookCallback(std::string name)
{
	auto it = shoebillHookedCallbacks.find(name);
	if (it == shoebillHookedCallbacks.end())
		return false;
	shoebillHookedCallbacks.erase(name);
	return true;
}