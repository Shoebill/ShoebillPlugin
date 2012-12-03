#include <a_samp>

native n_OnGameModeInit();
native n_OnGameModeExit();
native n_OnFilterScriptInit();
native n_OnFilterScriptExit();
native n_OnPlayerConnect(playerid);
native n_OnPlayerDisconnect(playerid, reason);
native n_OnPlayerSpawn(playerid);
native n_OnPlayerDeath(playerid, killerid, reason);
native n_OnVehicleSpawn(vehicleid);
native n_OnVehicleDeath(vehicleid, killerid);
native n_OnPlayerText(playerid, text[]);
native n_OnPlayerCommandText(playerid, cmdtext[]);
native n_OnPlayerRequestClass(playerid, classid);
native n_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
native n_OnPlayerExitVehicle(playerid, vehicleid);
native n_OnPlayerStateChange(playerid, newstate, oldstate);
native n_OnPlayerEnterCheckpoint(playerid);
native n_OnPlayerLeaveCheckpoint(playerid);
native n_OnPlayerEnterRaceCheckpoint(playerid);
native n_OnPlayerLeaveRaceCheckpoint(playerid);
native n_OnRconCommand(cmd[]);
native n_OnPlayerRequestSpawn(playerid);
native n_OnObjectMoved(objectid);
native n_OnPlayerObjectMoved(playerid, objectid);
native n_OnPlayerPickUpPickup(playerid, pickupid);
native n_OnVehicleMod(playerid, vehicleid, componentid);
native n_OnEnterExitModShop(playerid, enterexit, interiorid);
native n_OnVehiclePaintjob(playerid, vehicleid, paintjobid);
native n_OnVehicleRespray(playerid, vehicleid, color1, color2);
native n_OnVehicleDamageStatusUpdate(vehicleid, playerid);
native n_OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat);
native n_OnPlayerSelectedMenuRow(playerid, row);
native n_OnPlayerExitedMenu(playerid);
native n_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
native n_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
native n_OnRconLoginAttempt(ip[], password[], success);
native n_OnPlayerUpdate(playerid);
native n_OnPlayerStreamIn(playerid, forplayerid);
native n_OnPlayerStreamOut(playerid, forplayerid);
native n_OnVehicleStreamIn(vehicleid, forplayerid);
native n_OnVehicleStreamOut(vehicleid, forplayerid);
native n_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
native n_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid);
native n_OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid);
native n_OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ);
native n_OnPlayerClickTextDraw(playerid, Text:clickedid);
native n_OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid);
native n_OnPlayerClickPlayer(playerid, clickedplayerid, source);
native n_OnPlayerEditObject( playerid, playerobject, objectid, response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ );
native n_OnPlayerEditAttachedObject( playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ );
native n_OnPlayerSelectObject(playerid, type, objectid, modelid, Float:fX, Float:fY, Float:fZ);


forward Shoebill_Oops();


main()
{
	SendRconCommand("loadfs ShoebillFS");
}

public OnGameModeInit()
{
	return n_OnGameModeInit();
}

public OnGameModeExit()
{
	new ret = n_OnGameModeExit();
	SendRconCommand("unloadfs ShoebillFS");
	return ret;
}

public OnFilterScriptInit()
{
	return n_OnFilterScriptInit();
}

public OnFilterScriptExit()
{
	return n_OnFilterScriptExit();
}

public OnPlayerConnect(playerid)
{
	return n_OnPlayerConnect(playerid);
}

public OnPlayerDisconnect(playerid, reason)
{
	return n_OnPlayerDisconnect(playerid, reason);
}

public OnPlayerSpawn(playerid)
{
	return n_OnPlayerSpawn(playerid);
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return n_OnPlayerDeath(playerid, killerid, reason);
}

public OnVehicleSpawn(vehicleid)
{
	return n_OnVehicleSpawn(vehicleid);
}

public OnVehicleDeath(vehicleid, killerid)
{
	return n_OnVehicleDeath(vehicleid, killerid);
}

public OnPlayerText(playerid, text[])
{
	return n_OnPlayerText(playerid, text);
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	return n_OnPlayerCommandText(playerid, cmdtext);
}

public OnPlayerRequestClass(playerid, classid)
{
	return n_OnPlayerRequestClass(playerid, classid);
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return n_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return n_OnPlayerExitVehicle(playerid, vehicleid);
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return n_OnPlayerStateChange(playerid, newstate, oldstate);
}

public OnPlayerEnterCheckpoint(playerid)
{
	return n_OnPlayerEnterCheckpoint(playerid);
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return n_OnPlayerLeaveCheckpoint(playerid);
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return n_OnPlayerEnterRaceCheckpoint(playerid);
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return n_OnPlayerLeaveRaceCheckpoint(playerid);
}

public OnRconCommand(cmd[])
{
	return n_OnRconCommand(cmd);
}

public OnPlayerRequestSpawn(playerid)
{
	return n_OnPlayerRequestSpawn(playerid);
}

public OnObjectMoved(objectid)
{
	return n_OnObjectMoved(objectid);
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return n_OnPlayerObjectMoved(playerid, objectid);
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return n_OnPlayerPickUpPickup(playerid, pickupid);
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return n_OnVehicleMod(playerid, vehicleid, componentid);
}

public OnEnterExitModShop(playerid, enterexit, interiorid)
{
	return n_OnEnterExitModShop(playerid, enterexit, interiorid);
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return n_OnVehiclePaintjob(playerid, vehicleid, paintjobid);
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return n_OnVehicleRespray(playerid, vehicleid, color1, color2);
}

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{
	return n_OnVehicleDamageStatusUpdate(vehicleid, playerid);
}

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat)
{
	return n_OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat);
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return n_OnPlayerSelectedMenuRow(playerid, row);
}

public OnPlayerExitedMenu(playerid)
{
	return n_OnPlayerExitedMenu(playerid);
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
    return n_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    return n_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return n_OnRconLoginAttempt(ip, password, success);
}

public OnPlayerUpdate(playerid)
{
	return n_OnPlayerUpdate(playerid);
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return n_OnPlayerStreamIn(playerid, forplayerid);
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return n_OnPlayerStreamOut(playerid, forplayerid);
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return n_OnVehicleStreamIn(vehicleid, forplayerid);
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return n_OnVehicleStreamOut(vehicleid, forplayerid);
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	return n_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid)
{
	return n_OnPlayerTakeDamage(playerid, issuerid, amount, weaponid);
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid)
{
	return n_OnPlayerGiveDamage(playerid, damagedid, amount, weaponid);
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	return n_OnPlayerClickMap(playerid, fX, fY, fZ);
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	return n_OnPlayerClickTextDraw(playerid, clickedid);
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	return n_OnPlayerClickPlayerTextDraw(playerid, playertextid);
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return n_OnPlayerClickPlayer(playerid, clickedplayerid, source);
}

public OnPlayerEditObject( playerid, playerobject, objectid, response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ )
{
	return n_OnPlayerEditObject(playerid, playerobject, objectid, response, fX, fY, fZ, fRotX, fRotY, fRotZ);
}

public OnPlayerEditAttachedObject( playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ )
{
	return n_OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ);
}

public OnPlayerSelectObject(playerid, type, objectid, modelid, Float:fX, Float:fY, Float:fZ)
{
	return n_OnPlayerSelectObject(playerid, type, objectid, modelid, fX, fY, fZ);
}


public Shoebill_Oops()
{
	new n, a[2], Float:f;

// a_object.inc
	CreateObject(0, 0, 0, 0, 0, 0, 0);
	AttachObjectToVehicle(0, 0, f, f, f, f, f, f);
	AttachObjectToObject(0, 0, 0, 0, 0, 0, 0, 0, 0);
	AttachObjectToPlayer(0, 0, 0, 0, 0, 0, 0, 0);
	SetObjectPos(0, 0, 0, 0);
	GetObjectPos(0, f, f, f);
	SetObjectRot(0, 0, 0, 0);
	GetObjectRot(0, f, f, f);
	IsValidObject(0);
	DestroyObject(0);
	MoveObject(0, 0, 0, 0, 0);
	StopObject(0);
	IsObjectMoving(0);
	EditObject(0, 0);
	EditPlayerObject(0, 0);
	SelectObject(0);
	CancelEdit(0);
	CreatePlayerObject(0, 0, 0, 0, 0, 0, 0, 0);
	AttachPlayerObjectToVehicle(0, 0, 0, 0, 0, 0, 0, 0, 0);
	SetPlayerObjectPos(0, 0, 0, 0, 0);
	GetPlayerObjectPos(0, 0, f, f, f);
	SetPlayerObjectRot(0, 0, 0, 0, 0);
	GetPlayerObjectRot(0, 0, f, f, f);
	IsValidPlayerObject(0, 0);
	DestroyPlayerObject(0, 0);
	MovePlayerObject(0, 0, 0, 0, 0, 0);
	StopPlayerObject(0, 0);
	IsPlayerObjectMoving(0, 0);
	AttachPlayerObjectToPlayer(0, 0, 0, 0, 0, 0, 0, 0, 0);
	
	SetObjectMaterial(0, 0, 0, a, a, 0);
	SetPlayerObjectMaterial(0, 0, 0, 0, a, a, 0);

	SetObjectMaterialText(0, a, 0, 0, a, 0, 0, 0, 0, 0);
	SetPlayerObjectMaterialText(0, 0, a, 0, 0, a, 0, 0, 0, 0, 0);

// a_player.inc

	SetSpawnInfo(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	SpawnPlayer(0);

	// Player info
	SetPlayerPos(0, 0, 0, 0);
	SetPlayerPosFindZ(0, 0, 0, 0);
	GetPlayerPos(0, f, f, f);
	SetPlayerFacingAngle(0, 0);
	GetPlayerFacingAngle(0, f);
	IsPlayerInRangeOfPoint(0, 0, 0, 0, 0);
	GetPlayerDistanceFromPoint(0, f, f, f); //0.3c r3
	IsPlayerStreamedIn(0, 0);
	SetPlayerInterior(0, 0);
	GetPlayerInterior(0);
	SetPlayerHealth(0, 0);
	GetPlayerHealth(0, f);
	SetPlayerArmour(0, 0);
	GetPlayerArmour(0, f);
	SetPlayerAmmo(0, 0, 0);
	GetPlayerAmmo(0);
	GetPlayerWeaponState(0);
	GetPlayerTargetPlayer(0);
	SetPlayerTeam(0, 0);
	GetPlayerTeam(0);
	SetPlayerScore(0, 0);
	GetPlayerScore(0);
	GetPlayerDrunkLevel(0);
	SetPlayerDrunkLevel(0, 0);
	SetPlayerColor(0, 0);
	GetPlayerColor(0);
	SetPlayerSkin(0, 0);
	GetPlayerSkin(0);
	GivePlayerWeapon(0, 0, 0);
	ResetPlayerWeapons(0);
	SetPlayerArmedWeapon(0, 0);
	GetPlayerWeaponData(0, 0, n, n);
	GivePlayerMoney(0, 0);
	ResetPlayerMoney(0);
	SetPlayerName(0, a);
	GetPlayerMoney(0);
	GetPlayerState(0);
	GetPlayerIp(0, a, 0);
	GetPlayerPing(0);
	GetPlayerWeapon(0);
	GetPlayerKeys(0, n, n, n);
	GetPlayerName(0, a, 0);
	SetPlayerTime(0, 0, 0);
	GetPlayerTime(0, n, n);
	TogglePlayerClock(0, 0);
	SetPlayerWeather(0, 0);
	ForceClassSelection(0);
	SetPlayerWantedLevel(0, 0);
	GetPlayerWantedLevel(0);
	SetPlayerFightingStyle(0, 0);
	GetPlayerFightingStyle(0);
	SetPlayerVelocity(0, 0, 0, 0);
	GetPlayerVelocity(0, f, f, f);
	PlayCrimeReportForPlayer(0, 0, 0);
	PlayAudioStreamForPlayer(0, a, 0, 0, 0, 0, 0);
	StopAudioStreamForPlayer(0);
	SetPlayerShopName(0, a);
	SetPlayerSkillLevel(0, 0, 0);
	GetPlayerSurfingVehicleID(0);
	GetPlayerSurfingObjectID(0);
	RemoveBuildingForPlayer(0, 0, 0, 0, 0, 0);

	// Attached to bone objects
	SetPlayerAttachedObject(0, 0, 0, 0, 0, 0, 0, 0, 0);
	RemovePlayerAttachedObject(0, 0);
	IsPlayerAttachedObjectSlotUsed(0, 0);
	EditAttachedObject(0, 0);
	
	// Per-player TextDraws
	CreatePlayerTextDraw(0, 0, 0, a);
	PlayerTextDrawDestroy(0, PlayerText:0);
	PlayerTextDrawLetterSize(0, PlayerText:0, 0, 0);
	PlayerTextDrawTextSize(0, PlayerText:0, 0, 0);
	PlayerTextDrawAlignment(0, PlayerText:0, 0);
	PlayerTextDrawColor(0, PlayerText:0, 0);
	PlayerTextDrawUseBox(0, PlayerText:0, 0);
	PlayerTextDrawBoxColor(0, PlayerText:0, 0);
	PlayerTextDrawSetShadow(0, PlayerText:0, 0);
	PlayerTextDrawSetOutline(0, PlayerText:0, 0);
	PlayerTextDrawBackgroundColor(0, PlayerText:0, 0);
	PlayerTextDrawFont(0, PlayerText:0, 0);
	PlayerTextDrawSetProportional(0, PlayerText:0, 0);
	PlayerTextDrawSetSelectable(0, PlayerText:0, 0);
	PlayerTextDrawShow(0, PlayerText:0);
	PlayerTextDrawHide(0, PlayerText:0);
	PlayerTextDrawSetString(0, PlayerText:0, a);

	SetPVarInt(0, a, 0);
	GetPVarInt(0, a);
	SetPVarString(0, a, a);
	GetPVarString(0, a, a, 0);
	SetPVarFloat(0, a, 0);
	GetPVarFloat(0, a);
	DeletePVar(0, a);
	GetPVarsUpperIndex(0);
	GetPVarNameAtIndex(0, 0, a, 0);
	GetPVarType(0, a);
	SetPlayerChatBubble(0, a, 0, 0, 0);

	// Player controls
	PutPlayerInVehicle(0, 0, 0);
	GetPlayerVehicleID(0);
	GetPlayerVehicleSeat(0);
	RemovePlayerFromVehicle(0);
	TogglePlayerControllable(0, 0);
	PlayerPlaySound(0, 0, 0, 0, 0);
	ApplyAnimation(0, a, a, 0, 0, 0, 0, 0, 0, 0);
	ClearAnimations(0, 0);
	GetPlayerAnimationIndex(0);
	GetAnimationName(0, a, 0, a, 0);

	GetPlayerSpecialAction(0);
	SetPlayerSpecialAction(0, 0);

	// Player map commands
	SetPlayerCheckpoint(0, 0, 0, 0, 0);
	DisablePlayerCheckpoint(0);
	SetPlayerRaceCheckpoint(0, 0, 0, 0, 0, 0, 0, 0, 0);
	DisablePlayerRaceCheckpoint(0);
	SetPlayerWorldBounds(0, 0, 0, 0, 0);
	SetPlayerMarkerForPlayer(0, 0, 0);
	ShowPlayerNameTagForPlayer(0, 0, 0);
	SetPlayerMapIcon(0, 0, 0, 0, 0, 0, 0, 0);
	RemovePlayerMapIcon(0, 0);
	AllowPlayerTeleport(0, 0);

	SetPlayerCameraPos(0, 0, 0, 0);
	SetPlayerCameraLookAt(0, 0, 0, 0);
	SetCameraBehindPlayer(0);
	GetPlayerCameraPos(0, f, f, f);
	GetPlayerCameraFrontVector(0, f, f, f);
	GetPlayerCameraMode(0); //0.3c r3
	AttachCameraToObject(0, 0);
	AttachCameraToPlayerObject(0, 0);
	InterpolateCameraPos(0, 0, 0, 0, 0, 0, 0, 0, 0);
	InterpolateCameraLookAt(0, 0, 0, 0, 0, 0, 0, 0, 0);

	// Player conditionals
	IsPlayerConnected(0);
	IsPlayerInVehicle(0, 0);
	IsPlayerInAnyVehicle(0);
	IsPlayerInCheckpoint(0);
	IsPlayerInRaceCheckpoint(0);

	// Virtual Worlds
	SetPlayerVirtualWorld(0, 0);
	GetPlayerVirtualWorld(0);

	// Insane Stunts
	EnableStuntBonusForPlayer(0, 0);
	EnableStuntBonusForAll(0);

	TogglePlayerSpectating(0, 0);
	PlayerSpectatePlayer(0, 0, 0);
	PlayerSpectateVehicle(0, 0, 0);
	
	// Recording for NPC playback
	StartRecordingPlayerData(0, 0, a);
	StopRecordingPlayerData(0);
	
	SelectTextDraw(0, 0); 		// enables the mouse so the player can select a textdraw
	CancelSelectTextDraw(0);	// cancel textdraw selection with the mouse

// a_samp.inc

	// Util
	print(a);
	printf(a);
	format(a, 0, a);
	SendClientMessage(0, 0, a);
	SendClientMessageToAll(0, a);
	SendPlayerMessageToPlayer(0, 0, a);
	SendPlayerMessageToAll(0, a);
	SendDeathMessage(0, 0, 0);
	GameTextForAll(a, 0, 0);
	GameTextForPlayer(0, a, 0, 0);
	SetTimer(a, 0, 0);
	SetTimerEx(a, 0, 0, a);
	KillTimer(0);
	GetTickCount();
	GetMaxPlayers();
	LimitGlobalChatRadius(0);
	CallRemoteFunction(a, a);
	CallLocalFunction(a, a);

	// Game
	SetGameModeText(a);
	SetTeamCount(0);
	AddPlayerClass(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddPlayerClassEx(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	AddStaticVehicle(0, 0, 0, 0, 0, 0, 0);
	AddStaticVehicleEx(0, 0, 0, 0, 0, 0, 0, 0);
	AddStaticPickup(0, 0, 0, 0, 0, 0);
	CreatePickup(0, 0, 0, 0, 0, 0);
	DestroyPickup(0);
	ShowNameTags(0);
	ShowPlayerMarkers(0);
	GameModeExit();
	SetWorldTime(0);
	GetWeaponName(0, a, 0);
	EnableTirePopping(0);
	AllowInteriorWeapons(0);
	SetWeather(0);
	SetGravity(0);
	AllowAdminTeleport(0);
	SetDeathDropAmount(0);
	CreateExplosion(0, 0, 0, 0, 0);
	EnableZoneNames(0);
	UsePlayerPedAnims();
	DisableInteriorEnterExits();
	SetNameTagDrawDistance(0);
	DisableNameTagLOS();
	LimitGlobalChatRadius(0);
	LimitPlayerMarkerRadius(0);

	ConnectNPC(a, a);
	IsPlayerNPC(0);

	// Admin
	IsPlayerAdmin(0);
	Kick(0);
	Ban(0);
	BanEx(0, a);
	SendRconCommand(a);
	GetServerVarAsString(a, a, 0);
	GetServerVarAsInt(a);
	GetServerVarAsBool(a);
	GetPlayerNetworkStats(0, a, 0);
	GetNetworkStats(a, 0);
	GetPlayerVersion(0, a, 0); // Returns the SA-MP client revision as reported by the player

	// Menu
	CreateMenu(a, 0, 0, 0, 0, 0);
	DestroyMenu(Menu:0);
	AddMenuItem(Menu:0, 0, a);
	SetMenuColumnHeader(Menu:0, 0, a);
	ShowMenuForPlayer(Menu:0, 0);
	HideMenuForPlayer(Menu:0, 0);
	IsValidMenu(Menu:0);
	DisableMenu(Menu:0);
	DisableMenuRow(Menu:0, 0);
	GetPlayerMenu(0);

	// Text Draw
	TextDrawCreate(0, 0, a);
	TextDrawDestroy(Text:0);
	TextDrawLetterSize(Text:0, 0, 0);
	TextDrawTextSize(Text:0, 0, 0);
	TextDrawAlignment(Text:0, 0);
	TextDrawColor(Text:0, 0);
	TextDrawUseBox(Text:0, 0);
	TextDrawBoxColor(Text:0, 0);
	TextDrawSetShadow(Text:0, 0);
	TextDrawSetOutline(Text:0, 0);
	TextDrawBackgroundColor(Text:0, 0);
	TextDrawFont(Text:0, 0);
	TextDrawSetProportional(Text:0, 0);
	TextDrawSetSelectable(Text:0, 0);
	TextDrawShowForPlayer(0, Text:0);
	TextDrawHideForPlayer(0, Text:0);
	TextDrawShowForAll(Text:0);
	TextDrawHideForAll(Text:0);
	TextDrawSetString(Text:0, a);

	// Gang Zones

	GangZoneCreate(0, 0, 0, 0);
	GangZoneDestroy(0);
	GangZoneShowForPlayer(0, 0, 0);
	GangZoneShowForAll(0, 0);
	GangZoneHideForPlayer(0, 0);
	GangZoneHideForAll(0);
	GangZoneFlashForPlayer(0, 0, 0);
	GangZoneFlashForAll(0, 0);
	GangZoneStopFlashForPlayer(0, 0);
	GangZoneStopFlashForAll(0);

	Create3DTextLabel(a, 0, 0, 0, 0, 0, 0, 0);
	Delete3DTextLabel(Text3D:0);
	Attach3DTextLabelToPlayer(Text3D:0, 0, 0, 0, 0);
	Attach3DTextLabelToVehicle(Text3D:0, 0, 0, 0, 0);
	Update3DTextLabelText(Text3D:0, 0, a);
	CreatePlayer3DTextLabel(0, a, 0, 0, 0, 0, 0);
	DeletePlayer3DTextLabel(0, PlayerText3D:0);
	UpdatePlayer3DTextLabelText(0, PlayerText3D:0, 0, a);

	ShowPlayerDialog(0, 0, 0, a, a, a, a);

// a_vehicles.inc
	// Vehicle
	CreateVehicle(0, 0, 0, 0, 0, 0, 0, 0);
	DestroyVehicle(0);
	IsVehicleStreamedIn(0, 0);
	GetVehiclePos(0, f, f, f);
	SetVehiclePos(0, 0, 0, 0);
	GetVehicleZAngle(0, f);
	GetVehicleRotationQuat(0, f, f, f, f);
	GetVehicleDistanceFromPoint(0, f, f, f); //0.3c r3
	SetVehicleZAngle(0, 0);
	SetVehicleParamsForPlayer(0, 0, 0, 0);
	ManualVehicleEngineAndLights();
	SetVehicleParamsEx(0, 0, 0, 0, 0, 0, 0, 0);
	GetVehicleParamsEx(0, n, n, n, n, n, n, n);
	SetVehicleToRespawn(0);
	LinkVehicleToInterior(0, 0);
	AddVehicleComponent(0, 0);
	RemoveVehicleComponent(0, 0);
	ChangeVehicleColor(0, 0, 0);
	ChangeVehiclePaintjob(0, 0);
	SetVehicleHealth(0, 0);
	GetVehicleHealth(0, f);
	AttachTrailerToVehicle(0, 0);
	DetachTrailerFromVehicle(0);
	IsTrailerAttachedToVehicle(0);
	GetVehicleTrailer(0);
	SetVehicleNumberPlate(0, a);
	GetVehicleModel(0);
	GetVehicleComponentInSlot(0, 0);
	GetVehicleComponentType(0);
	RepairVehicle(0);
	GetVehicleVelocity(0, f, f, f);
	SetVehicleVelocity(0, 0, 0, 0);
	SetVehicleAngularVelocity(0, 0, 0, 0);
	GetVehicleDamageStatus(0, n, n, n, n);
	UpdateVehicleDamageStatus(0, 0, 0, 0, 0);
	
	GetVehicleModelInfo(0, 0, f, f, f);

	// Virtual Worlds
	SetVehicleVirtualWorld(0, 0);
	GetVehicleVirtualWorld(0);
}


