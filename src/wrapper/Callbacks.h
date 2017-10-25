/**
* Copyright (C) 2005-2014 SA-MP Team
* Copyright (C) 2011-2015 MK124 & 123marvin123
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
#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include "Shoebill.h"

int *CallHookedCallback(AMX *amx, std::string name, cell *params);

cell AMX_NATIVE_CALL CallShoebillFunction(AMX *amx, cell *params);

const static AMX_NATIVE_INFO PluginExports[]
        {
                {"CallShoebillFunction", CallShoebillFunction},
                {NULL, NULL}
        };

// SA:MP Callbacks
cell n_OnGameModeInit(AMX *amx, cell *params);

cell n_OnGameModeExit(AMX *amx, cell *params);

cell n_OnPlayerConnect(AMX *amx, cell *params);

cell n_OnPlayerDisconnect(AMX *amx, cell *params);

cell n_OnPlayerSpawn(AMX *amx, cell *params);

cell n_OnPlayerDeath(AMX *amx, cell *params);

cell n_OnVehicleSpawn(AMX *amx, cell *params);

cell n_OnVehicleDeath(AMX *amx, cell *params);

cell n_OnPlayerText(AMX *amx, cell *params);

cell n_OnPlayerCommandText(AMX *amx, cell *params);

cell n_OnPlayerRequestClass(AMX *amx, cell *params);

cell n_OnPlayerEnterVehicle(AMX *amx, cell *params);

cell n_OnPlayerExitVehicle(AMX *amx, cell *params);

cell n_OnPlayerStateChange(AMX *amx, cell *params);

cell n_OnPlayerEnterCheckpoint(AMX *amx, cell *params);

cell n_OnPlayerLeaveCheckpoint(AMX *amx, cell *params);

cell n_OnPlayerEnterRaceCheckpoint(AMX *amx, cell *params);

cell n_OnPlayerLeaveRaceCheckpoint(AMX *amx, cell *params);

cell n_OnRconCommand(AMX *amx, cell *params);

cell n_OnPlayerRequestSpawn(AMX *amx, cell *params);

cell n_OnObjectMoved(AMX *amx, cell *params);

cell n_OnPlayerObjectMoved(AMX *amx, cell *params);

cell n_OnPlayerPickUpPickup(AMX *amx, cell *params);

cell n_OnVehicleMod(AMX *amx, cell *params);

cell n_OnEnterExitModShop(AMX *amx, cell *params);

cell n_OnVehiclePaintjob(AMX *amx, cell *params);

cell n_OnVehicleRespray(AMX *amx, cell *params);

cell n_OnVehicleDamageStatusUpdate(AMX *amx, cell *params);

cell n_OnUnoccupiedVehicleUpdate(AMX *amx, cell *params);

cell n_OnPlayerSelectedMenuRow(AMX *amx, cell *params);

cell n_OnPlayerExitedMenu(AMX *amx, cell *params);

cell n_OnPlayerInteriorChange(AMX *amx, cell *params);

cell n_OnPlayerKeyStateChange(AMX *amx, cell *params);

cell n_OnRconLoginAttempt(AMX *amx, cell *params);

cell n_OnPlayerUpdate(AMX *amx, cell *params);

cell n_OnPlayerStreamIn(AMX *amx, cell *params);

cell n_OnPlayerStreamOut(AMX *amx, cell *params);

cell n_OnVehicleStreamIn(AMX *amx, cell *params);

cell n_OnVehicleStreamOut(AMX *amx, cell *params);

cell n_OnDialogResponse(AMX *amx, cell *params);

cell n_OnPlayerTakeDamage(AMX *amx, cell *params);

cell n_OnPlayerGiveDamage(AMX *amx, cell *params);

cell n_OnPlayerClickMap(AMX *amx, cell *params);

cell n_OnPlayerClickTextDraw(AMX *amx, cell *params);

cell n_OnPlayerClickPlayerTextDraw(AMX *amx, cell *params);

cell n_OnPlayerClickPlayer(AMX *amx, cell *params);

cell n_OnPlayerEditObject(AMX *amx, cell *params);

cell n_OnPlayerEditAttachedObject(AMX *amx, cell *params);

cell n_OnPlayerSelectObject(AMX *amx, cell *params);

cell n_OnPlayerWeaponShot(AMX *amx, cell *params);

cell n_OnIncomingConnection(AMX *amx, cell *params);

cell n_OnTrailerUpdate(AMX *amx, cell *params);

cell n_OnActorStreamIn(AMX *amx, cell *params);

cell n_OnActorStreamOut(AMX *amx, cell *params);

cell n_OnPlayerGiveDamageActor(AMX *amx, cell *params);

cell n_OnVehicleSirenStateChange(AMX *amx, cell *params);

cell InvokeCallback(AMX *, std::string, cell *, bool &);

bool ShouldCancelCallback(std::string, cell);

bool HookCallback(std::string name, std::string types);

bool UnhookCallback(std::string name);

std::map<std::string, std::string>::iterator getIterator(std::string name);

std::map<std::string, std::string>::const_iterator getEnd();

#endif
