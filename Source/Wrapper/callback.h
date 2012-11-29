/*  SA-MP Callbacks
 *
 *  (c) Copyright 2005-2010, SA-MP Team
 *
 */


// Plugin Callbacks
bool OnLoadPlugin();
void OnUnloadPlugin();
void OnProcessTick();


// SA:MP Callbacks
int OnGameModeInit();
int OnGameModeExit();
int OnFilterScriptInit();
int OnFilterScriptExit();
int OnPlayerConnect(int playerid);
int OnPlayerDisconnect(int playerid, int reason);
int OnPlayerSpawn(int playerid);
int OnPlayerDeath(int playerid, int killerid, int reason);
int OnVehicleSpawn(int vehicleid);
int OnVehicleDeath(int vehicleid, int killerid);
int OnPlayerText(int playerid, char* text);
int OnPlayerCommandText(int playerid, char* cmdtext);
int OnPlayerRequestClass(int playerid, int classid);
int OnPlayerEnterVehicle(int playerid, int vehicleid, int ispassenger);
int OnPlayerExitVehicle(int playerid, int vehicleid);
int OnPlayerStateChange(int playerid, int newstate, int oldstate);
int OnPlayerEnterCheckpoint(int playerid);
int OnPlayerLeaveCheckpoint(int playerid);
int OnPlayerEnterRaceCheckpoint(int playerid);
int OnPlayerLeaveRaceCheckpoint(int playerid);
int OnRconCommand(char* cmd);
int OnPlayerRequestSpawn(int playerid);
int OnObjectMoved(int objectid);
int OnPlayerObjectMoved(int playerid, int objectid);
int OnPlayerPickUpPickup(int playerid, int pickupid);
int OnVehicleMod(int playerid, int vehicleid, int componentid);
int OnEnterExitModShop(int playerid, int enterexit, int interiorid);
int OnVehiclePaintjob(int playerid, int vehicleid, int paintjobid);
int OnVehicleRespray(int playerid, int vehicleid, int color1, int color2);
int OnVehicleDamageStatusUpdate(int vehicleid, int playerid);
int OnUnoccupiedVehicleUpdate(int vehicleid, int playerid, int passenger_seat);
int OnPlayerSelectedMenuRow(int playerid, int row);
int OnPlayerExitedMenu(int playerid);
int OnPlayerInteriorChange(int playerid, int newinteriorid, int oldinteriorid);
int OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys);
int OnRconLoginAttempt( char* ip, char* password, int success );
int OnPlayerUpdate(int playerid);
int OnPlayerStreamIn(int playerid, int forplayerid);
int OnPlayerStreamOut(int playerid, int forplayerid);
int OnVehicleStreamIn(int vehicleid, int forplayerid);
int OnVehicleStreamOut(int vehicleid, int forplayerid);
int OnDialogResponse(int playerid, int dialogid, int response, int listitem, char *inputtext);
int OnPlayerTakeDamage(int playerid, int issuerid, float amount, int weaponid);
int OnPlayerGiveDamage(int playerid, int damagedid, float amount, int weaponid);
int OnPlayerClickMap(int playerid, float fX, float fY, float fZ);
int OnPlayerClickTextDraw(int playerid, int clickedid);
int OnPlayerClickPlayerTextDraw(int playerid, int playertextid);
int OnPlayerClickPlayer(int playerid, int clickedplayerid, int source);
int OnPlayerEditObject(int playerid, int playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ);
int OnPlayerEditAttachedObject(int playerid, int response, int index, int modelid, int boneid, float fOffsetX, float fOffsetY, float fOffsetZ, float fRotX, float fRotY, float fRotZ, float fScaleX, float fScaleY, float fScaleZ);
int OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float fX, float fY, float fZ);
