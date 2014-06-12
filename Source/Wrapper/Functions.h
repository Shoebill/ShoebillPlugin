/**
* Copyright (C) 2005-2014 SA-MP Team
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

typedef void(*logprintf_t)(const char* format, ...);
extern logprintf_t logprintf;

// Custom functions
int CallNative(const char* func, const char* paramTypes, ...);

//----------------------------------------------------------
// a_objects.inc

int CreateObject(int modelid, float X, float Y, float Z, float rX, float rY, float rZ, float drawDistance);
int AttachObjectToVehicle(int objectid, int vehicleid, float OffsetX, float OffsetY, float OffsetZ, float RotX, float RotY, float RotZ);
int AttachObjectToObject(int objectid, int attachtoid, float offsetX, float offsetY, float offsetZ, float rotX, float rotY, float rotZ, int syncRotation);
int AttachObjectToPlayer(int objectid, int playerid, float OffsetX, float OffsetY, float OffsetZ, float rX, float rY, float rZ);
int SetObjectPos(int objectid, float X, float Y, float Z);
int GetObjectPos(int objectid, float &X, float &Y, float &Z);
int SetObjectRot(int objectid, float RotX, float RotY, float RotZ);
int GetObjectRot(int objectid, float &RotX, float &RotY, float &RotZ);
int IsValidObject(int objectid);
int DestroyObject(int objectid);
int MoveObject(int objectid, float X, float Y, float Z, float Speed, float rotX, float rotY, float rotZ);
int StopObject(int objectid);
int IsObjectMoving(int objectid);
int EditObject(int playerid, int objectid);
int EditPlayerObject(int playerid, int objectid);
int SelectObject(int playerid);
int CancelEdit(int playerid);
int CreatePlayerObject(int playerid, int modelid, float X, float Y, float Z, float rX, float rY, float rZ, float drawDistance);
int AttachPlayerObjectToVehicle(int playerid, int objectid, int vehicleid, float fOffsetX, float fOffsetY, float fOffsetZ, float fRotX, float fRotY, float fRotZ);
int SetPlayerObjectPos(int playerid, int objectid, float X, float Y, float Z);
int GetPlayerObjectPos(int playerid, int objectid, float &X, float &Y, float &Z);
int SetPlayerObjectRot(int playerid, int objectid, float RotX, float RotY, float RotZ);
int GetPlayerObjectRot(int playerid, int objectid, float &RotX, float &RotY, float &RotZ);
int IsValidPlayerObject(int playerid, int objectid);
int DestroyPlayerObject(int playerid, int objectid);
int MovePlayerObject(int playerid, int objectid, float X, float Y, float Z, float Speed, float rotX, float rotY, float rotZ);
int StopPlayerObject(int playerid, int objectid);
int IsPlayerObjectMoving(int playerid, int objectid);
int AttachPlayerObjectToPlayer(int objectplayer, int objectid, int attachplayer, float OffsetX, float OffsetY, float OffsetZ, float rX, float rY, float rZ);

int SetObjectMaterial(int objectid, int materialindex, int modelid, char* txdname, char* texturename, int materialcolor);
int SetPlayerObjectMaterial(int playerid, int objectid, int materialindex, int modelid, char* txdname, char* texturename, int materialcolor);

int SetObjectMaterialText(int objectid, char* text, int materialindex, int materialsize, char* fontface, int fontsize, int bold, int fontcolor, int backcolor, int textalignment);
int SetPlayerObjectMaterialText(int playerid, int objectid, char* text, int materialindex, int materialsize, char* fontface, int fontsize, int bold, int fontcolor, int backcolor, int textalignment);


//----------------------------------------------------------
// a_players.inc

int SetSpawnInfo(int playerid, int team, int skin, float x, float y, float z, float rotation, int weapon1, int weapon1_ammo, int weapon2, int weapon2_ammo, int weapon3, int weapon3_ammo);
int SpawnPlayer(int playerid);

// Player info
int SetPlayerPos(int playerid, float x, float y, float z);
int SetPlayerPosFindZ(int playerid, float x, float y, float z);
int GetPlayerPos(int playerid, float& x, float& y, float& z);
int SetPlayerFacingAngle(int playerid, float ang);
int GetPlayerFacingAngle(int playerid, float& ang);
int IsPlayerInRangeOfPoint(int playerid, float range, float x, float y, float z);
float GetPlayerDistanceFromPoint(int playerid, float x, float y, float z); //0.3c r3
int IsPlayerStreamedIn(int playerid, int forplayerid);
int SetPlayerInterior(int playerid, int interiorid);
int GetPlayerInterior(int playerid);
int SetPlayerHealth(int playerid, float health);
int GetPlayerHealth(int playerid, float &health);
int SetPlayerArmour(int playerid, float armour);
int GetPlayerArmour(int playerid, float &armour);
int SetPlayerAmmo(int playerid, int weaponslot, int ammo);
int GetPlayerAmmo(int playerid);
int GetPlayerWeaponState(int playerid);
int GetPlayerTargetPlayer(int playerid);
int SetPlayerTeam(int playerid, int teamid);
int GetPlayerTeam(int playerid);
int SetPlayerScore(int playerid, int score);
int GetPlayerScore(int playerid);
int GetPlayerDrunkLevel(int playerid);
int SetPlayerDrunkLevel(int playerid, int level);
int SetPlayerColor(int playerid, int color);
int GetPlayerColor(int playerid);
int SetPlayerSkin(int playerid, int skinid);
int GetPlayerSkin(int playerid);
int GivePlayerWeapon(int playerid, int weaponid, int ammo);
int ResetPlayerWeapons(int playerid);
int SetPlayerArmedWeapon(int playerid, int weaponid);
int GetPlayerWeaponData(int playerid, int slot, int &weapons, int &ammo);
int GivePlayerMoney(int playerid, int money);
int ResetPlayerMoney(int playerid);
int SetPlayerName(int playerid, const char* name);
int GetPlayerMoney(int playerid);
int GetPlayerState(int playerid);
int GetPlayerIp(int playerid, char* ip, int len);
int GetPlayerPing(int playerid);
int GetPlayerWeapon(int playerid);
int GetPlayerKeys(int playerid, int &keys, int &updown, int &leftright);
int GetPlayerName(int playerid, char* name, int len);
int SetPlayerTime(int playerid, int hour, int minute);
int GetPlayerTime(int playerid, int &hour, int &minute);
int TogglePlayerClock(int playerid, int toggle);
int SetPlayerWeather(int playerid, int weather);
int ForceClassSelection(int playerid);
int SetPlayerWantedLevel(int playerid, int level);
int GetPlayerWantedLevel(int playerid);
int SetPlayerFightingStyle(int playerid, int style);
int GetPlayerFightingStyle(int playerid);
int SetPlayerVelocity(int playerid, float X, float Y, float Z);
int GetPlayerVelocity(int playerid, float &X, float &Y, float &Z);
int PlayCrimeReportForPlayer(int playerid, int suspectid, int crime);
int PlayAudioStreamForPlayer(int playerid, const char* url, float posX, float posY, float posZ, float distance, int usepos);
int StopAudioStreamForPlayer(int playerid);
int SetPlayerShopName(int playerid, const char* shopname);
int SetPlayerSkillLevel(int playerid, int skill, int level);
int GetPlayerSurfingVehicleID(int playerid);
int GetPlayerSurfingObjectID(int playerid);
int RemoveBuildingForPlayer(int playerid, int modelid, float x, float y, float z, float radius);
int GetPlayerLastShotVectors(int playerid, float &fOriginX, float &fOriginY, float &fOriginZ, float &fHitPosX, float &fHitPosY, float &fHitPosZ);

int SetPlayerAttachedObject(int playerid, int index, int modelid, int bone, float fOffsetX = 0.0, float fOffsetY = 0.0, float fOffsetZ = 0.0, float fRotX = 0.0, float fRotY = 0.0, float fRotZ = 0.0, float fScaleX = 1.0, float fScaleY = 1.0, float fScaleZ = 1.0, int materialcolor1 = 0, int materialcolor2 = 0);
int RemovePlayerAttachedObject(int playerid, int index);
int IsPlayerAttachedObjectSlotUsed(int playerid, int index);
int EditAttachedObject(int playerid, int index);

// Per-player TextDraws
int CreatePlayerTextDraw(int playerid, float x, float y, char* text);
int PlayerTextDrawDestroy(int playerid, int textId);
int PlayerTextDrawLetterSize(int playerid, int textId, float x, float y);
int PlayerTextDrawTextSize(int playerid, int textId, float x, float y);
int PlayerTextDrawAlignment(int playerid, int textId, int alignment);
int PlayerTextDrawColor(int playerid, int textId, int color);
int PlayerTextDrawUseBox(int playerid, int textId, int use);
int PlayerTextDrawBoxColor(int playerid, int textId, int color);
int PlayerTextDrawSetShadow(int playerid, int textId, int size);
int PlayerTextDrawSetOutline(int playerid, int textId, int size);
int PlayerTextDrawBackgroundColor(int playerid, int textId, int color);
int PlayerTextDrawFont(int playerid, int textId, int font);
int PlayerTextDrawSetProportional(int playerid, int textId, int set);
int PlayerTextDrawSetSelectable(int playerid, int textId, int set);
int PlayerTextDrawShow(int playerid, int textId);
int PlayerTextDrawHide(int playerid, int textId);
int PlayerTextDrawSetString(int playerid, int textId, char* string);
int PlayerTextDrawSetPreviewModel(int playerid, int textId, int modelindex);
int PlayerTextDrawSetPreviewRot(int playerid, int textId, float fRotX, float fRotY, float fRotZ, float fZoom);
int PlayerTextDrawSetPreviewVehCol(int playerid, int textId, int color1, int color2);

int SetPlayerChatBubble(int playerid, const char* text, int color, float drawdistance, int expiretime);

// Player controls
int PutPlayerInVehicle(int playerid, int vehicleid, int seatid);
int GetPlayerVehicleID(int playerid);
int GetPlayerVehicleSeat(int playerid);
int RemovePlayerFromVehicle(int playerid);
int TogglePlayerControllable(int playerid, int toggle);
int PlayerPlaySound(int playerid, int soundid, float x, float y, float z);
int ApplyAnimation(int playerid, const char* animlib, const char* animname, float fDelta, int loop, int lockx, int locky, int freeze, int time, int forcesync);
int ClearAnimations(int playerid, int forcesync = 0);
int GetPlayerAnimationIndex(int playerid);
int GetAnimationName(int index, char* animlib, int len1, char* animname, int len2);
int GetPlayerSpecialAction(int playerid);
int SetPlayerSpecialAction(int playerid, int actionid);

// Player world/map related
int SetPlayerCheckpoint(int playerid, float x, float y, float z, float size);
int DisablePlayerCheckpoint(int playerid);
int SetPlayerRaceCheckpoint(int playerid, int type, float x, float y, float z, float nextx, float nexty, float nextz, float size);
int DisablePlayerRaceCheckpoint(int playerid);
int SetPlayerWorldBounds(int playerid, float x_max, float x_min, float y_max, float y_min);
int SetPlayerMarkerForPlayer(int playerid, int showplayerid, int color);
int ShowPlayerNameTagForPlayer(int playerid, int showplayerid, int show);

int SetPlayerMapIcon(int playerid, int iconid, float x, float y, float z, int markertype, int color, int style);
int RemovePlayerMapIcon(int playerid, int iconid);

// Player camera
int SetPlayerCameraPos(int playerid, float x, float y, float z);
int SetPlayerCameraLookAt(int playerid, float x, float y, float z, int cut = CAMERA_CUT);
int SetCameraBehindPlayer(int playerid);
int GetPlayerCameraPos(int playerid, float &x, float &y, float &z);
int GetPlayerCameraFrontVector(int playerid, float &x, float &y, float &z);
int GetPlayerCameraMode(int playerid); //0.3c r3
float GetPlayerCameraAspectRatio(int playerid);
float GetPlayerCameraZoom(int playerid);
int AttachCameraToObject(int playerid, int objectid);
int AttachCameraToPlayerObject(int playerid, int playerobjectid);
int InterpolateCameraPos(int playerid, float FromX, float FromY, float FromZ, float ToX, float ToY, float ToZ, int time, int cut);
int InterpolateCameraLookAt(int playerid, float FromX, float FromY, float FromZ, float ToX, float ToY, float ToZ, int time, int cut);

// Player conditionals
int IsPlayerConnected(int playerid);
int IsPlayerInVehicle(int playerid, int vehicleid);
int IsPlayerInAnyVehicle(int playerid);
int IsPlayerInCheckpoint(int playerid);
int IsPlayerInRaceCheckpoint(int playerid);

// Virtual Worlds
int SetPlayerVirtualWorld(int playerid, int worldid);
int GetPlayerVirtualWorld(int playerid);

// Insane Stunts
int EnableStuntBonusForPlayer(int playerid, int enable);
int EnableStuntBonusForAll(int enable);

// Spectating
int TogglePlayerSpectating(int playerid, int toggle);
int PlayerSpectatePlayer(int playerid, int targetplayerid, int mode = SPECTATE_MODE_NORMAL);
int PlayerSpectateVehicle(int playerid, int targetvehicleid, int mode = SPECTATE_MODE_NORMAL);

// Npc Record
int StartRecordingPlayerData(int playerid, int recordtype, const char* recordname);
int StopRecordingPlayerData(int playerid);

int SelectTextDraw(int playerid, int hovercolor); // enables the mouse so the player can select a textdraw
int CancelSelectTextDraw(int playerid);	// cancel textdraw selection with the mouse

// Explosion
int CreateExplosionForPlayer(int playerid, float X, float Y, float Z, int type, float Radius);


//----------------------------------------------------------
// a_samp.inc

// Util
int SendClientMessage(int playerid, int color, const char* message);
int SendClientMessageToAll(int color, const char* message);
int SendPlayerMessageToPlayer(int playerid, int senderid, const char* message);
int SendPlayerMessageToAll(int senderid, const char* message);
int SendDeathMessage(int killer, int killee, int weapon);
int SendDeathMessageToPlayer(int playerid, int killer, int killee, int weapon);
int GameTextForAll(const char* string, int time, int style);
int GameTextForPlayer(int playerid, const char* string, int time, int style);
int SetTimer(int timerIndex, int interval, int repeating);
int KillTimer(int timerid);
int GetMaxPlayers();
int LimitGlobalChatRadius(float chat_radius);

// Game
int SetGameModeText(const char* string);
int SetTeamCount(int count);
int AddPlayerClass(int modelid, float spawn_x, float spawn_y, float spawn_z, float z_angle, int weapon1, int weapon1_ammo, int weapon2, int weapon2_ammo, int weapon3, int weapon3_ammo);
int AddPlayerClassEx(int teamid, int modelid, float spawn_x, float spawn_y, float spawn_z, float z_angle, int weapon1, int weapon1_ammo, int weapon2, int weapon2_ammo, int weapon3, int weapon3_ammo);
int AddStaticVehicle(int modelid, float spawn_x, float spawn_y, float spawn_z, float z_angle, int color1, int color2);
int AddStaticVehicleEx(int modelid, float spawn_x, float spawn_y, float spawn_z, float z_angle, int color1, int color2, int respawn_delay);
int AddStaticPickup(int model, int type, float X, float Y, float Z, int virtualworld);
int CreatePickup(int model, int type, float X, float Y, float Z, int virtualworld);
int DestroyPickup(int pickup);
int ShowNameTags(int show);
int ShowPlayerMarkers(int mode);
int GameModeExit();
int SetWorldTime(int hour);
int GetWeaponName(int weaponid, char* weapon, int len);
int EnableTirePopping(int enable);
int EnableVehicleFriendlyFire();
int AllowInteriorWeapons(int allow);
int SetWeather(int weatherid);
int SetGravity(float gravity);
int SetDeathDropAmount(int amount);
int CreateExplosion(float X, float Y, float Z, int type, float Radius);
int EnableZoneNames(int enable);
int UsePlayerPedAnims();
int DisableInteriorEnterExits();
int SetNameTagDrawDistance(float distance);
int DisableNameTagLOS();
int LimitGlobalChatRadius(float chat_radius);
int LimitPlayerMarkerRadius(float marker_radius);

// Npc
int ConnectNPC(const char* name, const char* script);
int IsPlayerNPC(int playerid);

// Admin
int IsPlayerAdmin(int playerid);
int Kick(int playerid);
int Ban(int playerid);
int BanEx(int playerid, const char* reason);
int SendRconCommand(const char* cmd);
int GetServerVarAsString(const char* varname, char* buffer, int len);
int GetServerVarAsInt(const char* varname);
int GetServerVarAsBool(const char* varname);
int GetPlayerNetworkStats(int playerid, char* retstr, int retstr_size);
int GetNetworkStats(char* retstr, int retstr_size);
int GetPlayerVersion(int playerid, char* version, int len); // Returns the SA-MP client revision as reported by the player
int BlockIpAddress(const char* ip_address, int timems);
int UnBlockIpAddress(const char* ip_address);

// Extended admin network stats
int GetServerTickRate();
int NetStats_GetConnectedTime(int playerid);
int NetStats_MessagesReceived(int playerid);
int NetStats_BytesReceived(int playerid);
int NetStats_MessagesSent(int playerid);
int NetStats_BytesSent(int playerid);
int NetStats_MessagesRecvPerSecond(int playerid);
float NetStats_PacketLossPercent(int playerid);
int NetStats_ConnectionStatus(int playerid);
int NetStats_GetIpPort(int playerid, char* ip_port, int ip_port_len);

// Menu
int CreateMenu(const char* title, int columns, float x, float y, float col1width, float col2width = 0.0);
int DestroyMenu(int menuid);
int AddMenuItem(int menuid, int column, const char* menutext);
int SetMenuColumnHeader(int menuid, int column, const char* columnheader);
int ShowMenuForPlayer(int menuid, int playerid);
int HideMenuForPlayer(int menuid, int playerid);
int IsValidMenu(int menuid);
int DisableMenu(int menuid);
int DisableMenuRow(int menuid, int row);
int GetPlayerMenu(int playerid);

// Text Draw
int TextDrawCreate(float x, float y, const char* text);
int TextDrawDestroy(int text);
int TextDrawLetterSize(int text, float x, float y);
int TextDrawTextSize(int text, float x, float y);
int TextDrawAlignment(int text, int alignment);
int TextDrawColor(int text, int color);
int TextDrawUseBox(int text, int use);
int TextDrawBoxColor(int text, int color);
int TextDrawSetShadow(int text, int size);
int TextDrawSetOutline(int text, int size);
int TextDrawBackgroundColor(int text, int color);
int TextDrawFont(int text, int font);
int TextDrawSetProportional(int text, int set);
int TextDrawSetSelectable(int textid, int set);
int TextDrawShowForPlayer(int playerid, int text);
int TextDrawHideForPlayer(int playerid, int text);
int TextDrawShowForAll(int text);
int TextDrawHideForAll(int text);
int TextDrawSetString(int text, const char* string);
int TextDrawSetPreviewModel(int textid, int modelindex);
int TextDrawSetPreviewRot(int textid, float fRotX, float fRotY, float fRotZ, float fZoom);
int TextDrawSetPreviewVehCol(int textid, int color1, int color2);

// Gang Zones
int GangZoneCreate(float minx, float miny, float maxx, float maxy);
int GangZoneDestroy(int zone);
int GangZoneShowForPlayer(int playerid, int zone, int color);
int GangZoneShowForAll(int zone, int color);
int GangZoneHideForPlayer(int playerid, int zone);
int GangZoneHideForAll(int zone);
int GangZoneFlashForPlayer(int playerid, int zone, int flashcolor);
int GangZoneFlashForAll(int zone, int flashcolor);
int GangZoneStopFlashForPlayer(int playerid, int zone);
int GangZoneStopFlashForAll(int zone);

// Global 3D Text Labels
int Create3DTextLabel(const char* text, int color, float X, float Y, float Z, float DrawDistance, int virtualworld, int testLOS = 0);
int Delete3DTextLabel(int id);
int Attach3DTextLabelToPlayer(int id, int playerid, float OffsetX, float OffsetY, float OffsetZ);
int Attach3DTextLabelToVehicle(int id, int vehicleid, float OffsetX, float OffsetY, float OffsetZ);
int Update3DTextLabelText(int id, int color, const char* text);

// Per-player 3D Text Labels
int CreatePlayer3DTextLabel(int playerid, const char* text, int color, float X, float Y, float Z, float DrawDistance, int attachedplayer = INVALID_PLAYER_ID, int attachedvehicle = INVALID_VEHICLE_ID, int testLOS = 0);
int DeletePlayer3DTextLabel(int playerid, int id);
int UpdatePlayer3DTextLabelText(int playerid, int id, int color, const char* text);

// Player GUI Dialog
int ShowPlayerDialog(int playerid, int dialogid, int style, const char* caption, const char* info, const char* button1, const char* button2);


//----------------------------------------------------------
// a_vehicles.inc

int CreateVehicle(int vehicletype, float x, float y, float z, float rotation, int color1, int color2, int respawn_delay);
int DestroyVehicle(int vehicleid);
int IsVehicleStreamedIn(int vehicleid, int forplayerid);
int GetVehiclePos(int vehicleid, float &x, float &y, float &z);
int SetVehiclePos(int vehicleid, float x, float y, float z);
int GetVehicleZAngle(int vehicleid, float &z_angle);
int GetVehicleRotationQuat(int vehicleid, float &w, float &x, float &y, float &z);
float GetVehicleDistanceFromPoint(int vehicleid, float x, float y, float z); //0.3c r3
int SetVehicleZAngle(int vehicleid, float z_angle);
int SetVehicleParamsForPlayer(int vehicleid, int playerid, int objective, int doorslocked);
int ManualVehicleEngineAndLights();
int SetVehicleParamsEx(int vehicleid, int engine, int lights, int alarm, int doors, int bonnet, int boot, int objective);
int GetVehicleParamsEx(int vehicleid, int &engine, int &lights, int &alarm, int &doors, int &bonnet, int &boot, int &objective);
int SetVehicleToRespawn(int vehicleid);
int LinkVehicleToInterior(int vehicleid, int interiorid);
int AddVehicleComponent(int vehicleid, int componentid);
int RemoveVehicleComponent(int vehicleid, int componentid);
int ChangeVehicleColor(int vehicleid, int color1, int color2);
int ChangeVehiclePaintjob(int vehicleid, int paintjobid);
int SetVehicleHealth(int vehicleid, float health);
int GetVehicleHealth(int vehicleid, float &health);
int AttachTrailerToVehicle(int trailerid, int vehicleid);
int DetachTrailerFromVehicle(int vehicleid);
int IsTrailerAttachedToVehicle(int vehicleid);
int GetVehicleTrailer(int vehicleid);
int SetVehicleNumberPlate(int vehicleid, const char* numberplate);
int GetVehicleModel(int vehicleid);
int GetVehicleComponentInSlot(int vehicleid, int slot);
int GetVehicleComponentType(int component);
int RepairVehicle(int vehicleid);
int GetVehicleVelocity(int vehicleid, float& X, float& Y, float& Z);
int SetVehicleVelocity(int vehicleid, float X, float Y, float Z);
int SetVehicleAngularVelocity(int vehicleid, float X, float Y, float Z);
int GetVehicleDamageStatus(int vehicleid, int &panels, int &doors, int &lights, int &tires);
int UpdateVehicleDamageStatus(int vehicleid, int panels, int doors, int lights, int tires);

int GetVehicleModelInfo(int vehiclemodel, int infotype, float &X, float &Y, float &Z);

// Virtual Worlds
int SetVehicleVirtualWorld(int vehicleid, int worldid);
int GetVehicleVirtualWorld(int vehicleid);
