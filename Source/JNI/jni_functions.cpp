/**
 * Copyright (C) 2011-2013 MK124
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

#include <string.h>

#include "jni_functions.h"
#include "samp_core.h"
#include "encoding.h"
#include "../Wrapper/a_samp.h"


/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setServerCodepage
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setServerCodepage
  (JNIEnv *env, jclass jcls, jint codepage)
{
	setServerCodepage(codepage);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getServerCodepage
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getServerCodepage
  (JNIEnv *env, jclass jcls)
{
	return getServerCodepage();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerCodepage
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerCodepage
  (JNIEnv *env, jclass jcls, jint playerid, jint codepage)
{
	setPlayerCodepage(playerid, codepage);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerCodepage
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerCodepage
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return getPlayerCodepage(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createObject
 * Signature: (IFFFFFFF)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createObject
  (JNIEnv *env, jclass jcls, jint modelid, jfloat x, jfloat y, jfloat z, jfloat rX, jfloat rY, jfloat rZ, jfloat drawDistance)
{
	return CreateObject( modelid, x, y, z, rX, rY, rZ, drawDistance );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachObjectToVehicle
 * Signature: (IIFFFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachObjectToVehicle
  (JNIEnv *env, jclass jcls, jint objectid, jint vehicleid, jfloat x, jfloat y, jfloat z, jfloat rX, jfloat rY, jfloat rZ)
{
	AttachObjectToVehicle( objectid, vehicleid, x, y, z, rX, rY, rZ );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachObjectToObject
 * Signature: (IIFFFFFFI)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachObjectToObject
  (JNIEnv *env, jclass jcls, jint objectid, jint attachtoid, jfloat offsetX, jfloat offsetY, jfloat offsetZ, jfloat rotX, jfloat rotY, jfloat rotZ, jint syncRotation)
{
	AttachObjectToObject( objectid, attachtoid, offsetX, offsetY, offsetZ, rotX, rotY, rotZ, syncRotation );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachObjectToPlayer
 * Signature: (IIFFFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachObjectToPlayer
  (JNIEnv *env, jclass jcls, jint objectid, jint playerid,
  jfloat offsetX, jfloat offsetY, jfloat offsetZ, jfloat rX, jfloat rY, jfloat rZ)
{
	AttachObjectToPlayer( objectid, playerid, offsetX, offsetY, offsetZ, rX, rY, rZ );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setObjectPos
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setObjectPos
  (JNIEnv *env, jclass jcls, jint objectid, jfloat x, jfloat y, jfloat z)
{
	SetObjectPos( objectid, x, y, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getObjectPos
 * Signature: (ILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getObjectPos
  (JNIEnv *env, jclass jcls, jint objectid, jobject vector3d)
{
	static jclass cls = env->GetObjectClass(vector3d);
	static jfieldID fidX = env->GetFieldID(cls, "x", "F");
	static jfieldID fidY = env->GetFieldID(cls, "y", "F");
	static jfieldID fidZ = env->GetFieldID(cls, "z", "F");

	float x, y, z;
	GetObjectPos( objectid, x, y, z );

	env->SetFloatField( vector3d, fidX, x );
	env->SetFloatField( vector3d, fidY, y );
	env->SetFloatField( vector3d, fidZ, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setObjectRot
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setObjectRot
  (JNIEnv *env, jclass jcls, jint objectid, jfloat rotX, jfloat rotY, jfloat rotZ)
{
	SetObjectRot( objectid, rotX, rotY, rotZ );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getObjectRot
 * Signature: (ILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getObjectRot
  (JNIEnv *env, jclass jcls, jint objectid, jobject rotate)
{
	static jclass cls = env->GetObjectClass(rotate);
	static jfieldID fidX = env->GetFieldID(cls, "x", "F");
	static jfieldID fidY = env->GetFieldID(cls, "y", "F");
	static jfieldID fidZ = env->GetFieldID(cls, "z", "F");

	float rotX, rotY, rotZ;
	GetObjectPos( objectid, rotX, rotY, rotZ );

	env->SetFloatField( rotate, fidX, rotX );
	env->SetFloatField( rotate, fidY, rotY );
	env->SetFloatField( rotate, fidZ, rotZ );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isValidObject
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isValidObject
  (JNIEnv *env, jclass jcls, jint objectid)
{
	return IsValidObject(objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    destroyObject
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_destroyObject
  (JNIEnv *env, jclass jcls, jint objectid)
{
	DestroyObject( objectid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    moveObject
 * Signature: (IFFFFFFF)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_moveObject
  (JNIEnv *env, jclass jcls, jint objectid, jfloat x, jfloat y, jfloat z, jfloat speed, jfloat rotX, jfloat rotY, jfloat rotZ)
{
	return MoveObject( objectid, x, y, z, speed, rotX, rotY, rotZ );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    stopObject
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_stopObject
  (JNIEnv *env, jclass jcls, jint objectid)
{
	StopObject( objectid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isObjectMoving
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isObjectMoving
  (JNIEnv *env, jclass jcls, jint objectid)
{
	return IsObjectMoving(objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    editObject
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_editObject
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid)
{
	return EditObject(playerid, objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    editPlayerObject
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_editPlayerObject
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid)
{
	return EditPlayerObject(playerid, objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    selectObject
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_selectObject
  (JNIEnv *env, jclass jcls, jint playerid)
{
	SelectObject(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    cancelEdit
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_cancelEdit
  (JNIEnv *env, jclass jcls, jint playerid)
{
	CancelEdit(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createPlayerObject
 * Signature: (IIFFFFFFF)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createPlayerObject
  (JNIEnv *env, jclass jcls, jint playerid, jint modelid, jfloat x, jfloat y, jfloat z, jfloat rX, jfloat rY, jfloat rZ, jfloat drawDistance)
{
	return CreatePlayerObject(playerid, modelid, x, y, z, rX, rY, rZ, drawDistance);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachPlayerObjectToVehicle
 * Signature: (IIIFFFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachPlayerObjectToVehicle
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid, jint vehicleid, jfloat fOffsetX, jfloat fOffsetY, jfloat fOffsetZ, jfloat fRotX, jfloat fRotY, jfloat fRotZ)
{
	AttachPlayerObjectToVehicle(playerid, objectid, vehicleid, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerObjectPos
 * Signature: (IIFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerObjectPos
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid, jfloat x, jfloat y, jfloat z)
{
	SetPlayerObjectPos( playerid, objectid, x, y, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerObjectPos
 * Signature: (IILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerObjectPos
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid, jobject vector3d)
{
	static jclass cls = env->GetObjectClass(vector3d);
	static jfieldID fidX = env->GetFieldID(cls, "x", "F");
	static jfieldID fidY = env->GetFieldID(cls, "y", "F");
	static jfieldID fidZ = env->GetFieldID(cls, "z", "F");

	float x, y, z;
	GetPlayerObjectPos( playerid, objectid, x, y, z );

	env->SetFloatField( vector3d, fidX, x );
	env->SetFloatField( vector3d, fidY, y );
	env->SetFloatField( vector3d, fidZ, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerObjectRot
 * Signature: (IIFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerObjectRot
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid, jfloat rotX, jfloat rotY, jfloat rotZ)
{
	SetPlayerObjectRot( playerid, objectid, rotX, rotY, rotZ );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerObjectRot
 * Signature: (IILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerObjectRot
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid, jobject rotate)
{
	static jclass cls = env->GetObjectClass(rotate);
	static jfieldID fidX = env->GetFieldID(cls, "x", "F");
	static jfieldID fidY = env->GetFieldID(cls, "y", "F");
	static jfieldID fidZ = env->GetFieldID(cls, "z", "F");

	float rotX, rotY, rotZ;
	GetPlayerObjectRot( playerid, objectid, rotX, rotY, rotZ );

	env->SetFloatField( rotate, fidX, rotX );
	env->SetFloatField( rotate, fidY, rotY );
	env->SetFloatField( rotate, fidZ, rotZ );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isValidPlayerObject
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isValidPlayerObject
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid)
{
	return IsValidPlayerObject( playerid, objectid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    destroyPlayerObject
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_destroyPlayerObject
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid)
{
	DestroyPlayerObject( playerid, objectid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    movePlayerObject
 * Signature: (IIFFFFFFF)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_movePlayerObject
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid, jfloat x, jfloat y, jfloat z, jfloat speed, jfloat rotX, jfloat rotY, jfloat rotZ)
{
	return MovePlayerObject( playerid, objectid, x, y, z, speed, rotX, rotY, rotZ );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    stopPlayerObject
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_stopPlayerObject
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid)
{
	StopPlayerObject( playerid, objectid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerObjectMoving
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerObjectMoving
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid)
{
	return IsPlayerObjectMoving(playerid, objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachPlayerObjectToPlayer
 * Signature: (IIIFFFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachPlayerObjectToPlayer
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid, jint attachplayerid,
  jfloat offsetX, jfloat offsetY, jfloat offsetZ, jfloat rX, jfloat rY, jfloat rZ)
{
	AttachPlayerObjectToPlayer( playerid, objectid, attachplayerid, offsetX, offsetY, offsetZ, rX, rY, rZ );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setObjectMaterial
 * Signature: (IIILjava/lang/String;Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setObjectMaterial
  (JNIEnv *env, jclass jcls, jint objectid, jint materialindex, jint modelid, jstring txdname, jstring texturename, jint materialcolor)
{
	const jchar* wmsg = env->GetStringChars(txdname, NULL);
	int len = env->GetStringLength(txdname);

	char _txdname[1024];
	wcs2mbs( getServerCodepage(), wmsg, len, _txdname, sizeof(_txdname) );
	env->ReleaseStringChars(txdname, wmsg);

	const jchar* wmsg2 = env->GetStringChars(texturename, NULL);
	len = env->GetStringLength(texturename);

	char _texturename[1024];
	wcs2mbs( getServerCodepage(), wmsg2, len, _texturename, sizeof(_texturename) );
	env->ReleaseStringChars(texturename, wmsg2);

	SetObjectMaterial(objectid, materialindex, modelid, _txdname, _texturename, materialcolor);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerObjectMaterial
 * Signature: (IIIILjava/lang/String;Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerObjectMaterial
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid, jint materialindex, jint modelid, jstring txdname, jstring texturename, jint materialcolor)
{
	const jchar* wmsg = env->GetStringChars(txdname, NULL);
	int len = env->GetStringLength(txdname);

	char _txdname[1024];
	wcs2mbs( getServerCodepage(), wmsg, len, _txdname, sizeof(_txdname) );
	env->ReleaseStringChars(txdname, wmsg);

	wmsg = env->GetStringChars(texturename, NULL);
	len = env->GetStringLength(texturename);

	char _texturename[1024];
	wcs2mbs( getServerCodepage(), wmsg, len, _texturename, sizeof(_texturename) );
	env->ReleaseStringChars(texturename, wmsg);

	SetPlayerObjectMaterial(playerid, objectid, materialindex, modelid, _txdname, _texturename, materialcolor);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setObjectMaterialText
 * Signature: (ILjava/lang/String;IILjava/lang/String;IIIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setObjectMaterialText
  (JNIEnv *env, jclass jcls, jint objectid, jstring text, jint materialindex, jint materialsize, jstring fontface, jint fontsize, jint bold, jint fontcolor, jint backcolor, jint textalignment)
{
	const jchar* wmsg = env->GetStringChars(text, NULL);
	int len = env->GetStringLength(text);

	char _text[1024];
	wcs2mbs( getServerCodepage(), wmsg, len, _text, sizeof(_text) );
	env->ReleaseStringChars(text, wmsg);

	const jchar* wmsg2 = env->GetStringChars(fontface, NULL);
	len = env->GetStringLength(fontface);

	char _fontface[1024];
	wcs2mbs( getServerCodepage(), wmsg2, len, _fontface, sizeof(_fontface) );
	env->ReleaseStringChars(fontface, wmsg2);

	SetObjectMaterialText(objectid, _text, materialindex, materialsize, _fontface, fontsize, bold, fontcolor, backcolor, textalignment);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerObjectMaterialText
 * Signature: (IILjava/lang/String;IILjava/lang/String;IIIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerObjectMaterialText
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid, jstring text, jint materialindex, jint materialsize, jstring fontface, jint fontsize, jint bold, jint fontcolor, jint backcolor, jint textalignment)
{
	const jchar* wmsg = env->GetStringChars(text, NULL);
	int len = env->GetStringLength(text);

	char _text[1024];
	wcs2mbs( getPlayerCodepage(playerid), wmsg, len, _text, sizeof(_text) );
	env->ReleaseStringChars(text, wmsg);

	const jchar* wmsg2 = env->GetStringChars(fontface, NULL);
	len = env->GetStringLength(fontface);

	char _fontface[1024];
	wcs2mbs( getPlayerCodepage(playerid), wmsg2, len, _fontface, sizeof(_fontface) );
	env->ReleaseStringChars(fontface, wmsg2);

	SetPlayerObjectMaterialText(playerid, objectid, _text, materialindex, materialsize, _fontface, fontsize, bold, fontcolor, backcolor, textalignment);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setSpawnInfo
 * Signature: (IIIFFFFIIIIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setSpawnInfo
  (JNIEnv *env, jclass jcls, jint playerid, jint teamid, jint skinid, jfloat x, jfloat y, jfloat z,
  jfloat rotation, jint weapon1, jint ammo1, jint weapon2, jint ammo2, jint weapon3, jint ammo3)
{
	SetSpawnInfo( playerid, teamid, skinid, x, y, z, rotation, weapon1, ammo1, weapon2, ammo2, weapon3, ammo3 );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    spawnPlayer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_spawnPlayer
  (JNIEnv *env, jclass jcls, jint playerid)
{
	SpawnPlayer( playerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerPos
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerPos
  (JNIEnv *env, jclass jcls, jint playerid, jfloat x, jfloat y, jfloat z)
{
	SetPlayerPos( playerid, x, y, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerPosFindZ
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerPosFindZ
  (JNIEnv *env, jclass jcls, jint playerid, jfloat x, jfloat y, jfloat z)
{
	SetPlayerPosFindZ( playerid, x, y, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerPos
 * Signature: (ILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerPos
  (JNIEnv *env, jclass jcls, jint playerid, jobject vector3d)
{
	static jclass cls = env->GetObjectClass(vector3d);
	static jfieldID fidX = env->GetFieldID(cls, "x", "F");
	static jfieldID fidY = env->GetFieldID(cls, "y", "F");
	static jfieldID fidZ = env->GetFieldID(cls, "z", "F");

	float x, y, z;
	GetPlayerPos( playerid, x, y, z );

	env->SetFloatField( vector3d, fidX, x );
	env->SetFloatField( vector3d, fidY, y );
	env->SetFloatField( vector3d, fidZ, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerFacingAngle
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerFacingAngle
  (JNIEnv *env, jclass jcls, jint playerid, jfloat angle)
{
	SetPlayerFacingAngle( playerid, angle );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerFacingAngle
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerFacingAngle
  (JNIEnv *env, jclass jcls, jint playerid)
{
	float angle;
	GetPlayerFacingAngle( playerid, angle );

	return angle;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerInRangeOfPoint
 * Signature: (IFFFF)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerInRangeOfPoint
  (JNIEnv *env, jclass jcls, jint playerid, jfloat range, jfloat x, jfloat y, jfloat z)
{
	return IsPlayerInRangeOfPoint(playerid, range, x, y, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerStreamedIn
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerStreamedIn
  (JNIEnv *env, jclass jcls, jint playerid, jint forplayerid)
{
	return IsPlayerStreamedIn(playerid, forplayerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerInterior
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerInterior
  (JNIEnv *env, jclass jcls, jint playerid, jint interiorid)
{
	SetPlayerInterior( playerid, interiorid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerInterior
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerInterior
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerInterior(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerHealth
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerHealth
  (JNIEnv *env, jclass jcls, jint playerid, jfloat health)
{
	SetPlayerHealth( playerid, health );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerHealth
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerHealth
  (JNIEnv *env, jclass jcls, jint playerid)
{
	float health;
	GetPlayerHealth( playerid, health );

	return health;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerArmour
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerArmour
  (JNIEnv *env, jclass jcls, jint playerid, jfloat armour)
{
	SetPlayerArmour( playerid, armour );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerArmour
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerArmour
  (JNIEnv *env, jclass jcls, jint playerid)
{
	float armour;
	GetPlayerArmour( playerid, armour );

	return armour;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerAmmo
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerAmmo
  (JNIEnv *env, jclass jcls, jint playerid, jint weaponslot, jint ammo)
{
	SetPlayerAmmo( playerid, weaponslot, ammo );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerAmmo
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerAmmo
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerAmmo(playerid);;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerWeaponState
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerWeaponState
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerWeaponState(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerTargetPlayer
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerTargetPlayer
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerTargetPlayer(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerTeam
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerTeam
  (JNIEnv *env, jclass jcls, jint playerid, jint teamid)
{
	SetPlayerTeam( playerid, teamid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerTeam
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerTeam
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerTeam(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerScore
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerScore
  (JNIEnv *env, jclass jcls, jint playerid, jint score)
{
	SetPlayerScore( playerid, score );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerScore
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerScore
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerScore(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerDrunkLevel
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerDrunkLevel
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerDrunkLevel(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerDrunkLevel
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerDrunkLevel
  (JNIEnv *env, jclass jcls, jint playerid, jint level)
{
	SetPlayerDrunkLevel( playerid, level );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerColor
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerColor
  (JNIEnv *env, jclass jcls, jint playerid, jint color)
{
	SetPlayerColor( playerid, color );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerColor
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerColor
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerColor(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerSkin
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerSkin
  (JNIEnv *env, jclass jcls, jint playerid, jint skinid)
{
	SetPlayerSkin( playerid, skinid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerSkin
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerSkin
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerSkin(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    givePlayerWeapon
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_givePlayerWeapon
  (JNIEnv *env, jclass jcls, jint playerid, jint weaponid, jint ammo)
{
	GivePlayerWeapon( playerid, weaponid, ammo );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    resetPlayerWeapons
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_resetPlayerWeapons
  (JNIEnv *env, jclass jcls, jint playerid)
{
	ResetPlayerWeapons( playerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerArmedWeapon
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerArmedWeapon
  (JNIEnv *env, jclass jcls, jint playerid, jint weaponid)
{
	SetPlayerArmedWeapon( playerid, weaponid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerWeaponData
 * Signature: (IILnet/gtaun/shoebill/data/WeaponData;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerWeaponData
  (JNIEnv *env, jclass jcls, jint playerid, jint slot, jobject weapondata)
{
	static jclass cls = env->GetObjectClass(weapondata);
	static jfieldID fidId = env->GetFieldID(cls, "id", "I");
	static jfieldID fidAmmo = env->GetFieldID(cls, "ammo", "I");

	int weaponid, ammo;
	GetPlayerWeaponData( playerid, slot, weaponid, ammo );

	env->SetIntField( weapondata, fidId, weaponid );
	env->SetIntField( weapondata, fidAmmo, ammo );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    givePlayerMoney
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_givePlayerMoney
  (JNIEnv *env, jclass jcls, jint playerid, jint money)
{
	GivePlayerMoney( playerid, money );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    resetPlayerMoney
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_resetPlayerMoney
  (JNIEnv *env, jclass jcls, jint playerid)
{
	ResetPlayerMoney( playerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerName
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerName
  (JNIEnv *env, jclass jcls, jint playerid, jstring name)
{
	const jchar* wmsg = env->GetStringChars(name, NULL);
	int len = env->GetStringLength(name);

	char str[1024];
	wcs2mbs( getServerCodepage(), wmsg, len, str, sizeof(str) );
	env->ReleaseStringChars(name, wmsg);

	return SetPlayerName(playerid, str);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerMoney
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerMoney
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerMoney(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerState
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerState
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerState(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerIp
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerIp
  (JNIEnv *env, jclass jcls, jint playerid)
{
	char ip[16];
	GetPlayerIp( playerid, ip, sizeof(ip) );

	return env->NewStringUTF(ip);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerPing
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerPing
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerPing(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerWeapon
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerWeapon
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerWeapon(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerKeys
 * Signature: (ILnet/gtaun/shoebill/object/impl/PlayerKeyStateImpl;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerKeys
  (JNIEnv *env, jclass jcls, jint playerid, jobject keystate)
{
	static jclass cls = env->GetObjectClass(keystate);
	static jfieldID fidKeys = env->GetFieldID(cls, "keys", "I");
	static jfieldID fidUpdown = env->GetFieldID(cls, "updownValue", "I");
	static jfieldID fidLeftright = env->GetFieldID(cls, "leftrightValue", "I");

	int keys, updown, leftright;
	GetPlayerKeys( playerid, keys, updown, leftright );

	env->SetIntField( keystate, fidKeys, keys );
	env->SetIntField( keystate, fidUpdown, updown );
	env->SetIntField( keystate, fidLeftright, leftright );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerName
  (JNIEnv *env, jclass jcls, jint playerid)
{
	char name[MAX_PLAYER_NAME];
	GetPlayerName( playerid, name, sizeof(name) );

	jchar wstr[MAX_PLAYER_NAME];
	int len = mbs2wcs( getServerCodepage(), name, -1, wstr, sizeof(wstr)/sizeof(wstr[0]) );

	return env->NewString(wstr, len);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerTime
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerTime
  (JNIEnv *env, jclass jcls, jint playerid, jint hour, jint minute)
{
	SetPlayerTime( playerid, hour, minute );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerTime
 * Signature: (ILjava/sql/Time;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerTime
  (JNIEnv *env, jclass jcls, jint playerid, jobject time)
{
	static jclass cls = env->GetObjectClass(time);
	static jfieldID fidHour = env->GetFieldID(cls, "hour", "I");
	static jfieldID fidMinute = env->GetFieldID(cls, "minute", "I");

	int hour, minute;
	GetPlayerTime( playerid, hour, minute );

	env->SetIntField( time, fidHour, hour );
	env->SetIntField( time, fidMinute, minute );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    togglePlayerClock
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_togglePlayerClock
  (JNIEnv *env, jclass jcls, jint playerid, jboolean toggle)
{
	TogglePlayerClock( playerid, toggle );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerWeather
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerWeather
  (JNIEnv *env, jclass jcls, jint playerid, jint weather)
{
	SetPlayerWeather( playerid, weather );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    forceClassSelection
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_forceClassSelection
  (JNIEnv *env, jclass jcls, jint playerid)
{
	ForceClassSelection( playerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerWantedLevel
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerWantedLevel
  (JNIEnv *env, jclass jcls, jint playerid, jint level)
{
	SetPlayerWantedLevel( playerid, level );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerWantedLevel
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerWantedLevel
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerWantedLevel(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerFightingStyle
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerFightingStyle
  (JNIEnv *env, jclass jcls, jint playerid, jint style)
{
	SetPlayerFightingStyle( playerid, style );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerFightingStyle
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerFightingStyle
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerFightingStyle(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerVelocity
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerVelocity
  (JNIEnv *env, jclass jcls, jint playerid, jfloat x, jfloat y, jfloat z)
{
	SetPlayerVelocity( playerid, x, y, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerVelocity
 * Signature: (ILnet/gtaun/shoebill/data/Velocity;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerVelocity
  (JNIEnv *env, jclass jcls, jint playerid, jobject velocity)
{
	static jclass cls = env->GetObjectClass(velocity);
	static jfieldID fidX = env->GetFieldID(cls, "x", "F");
	static jfieldID fidY = env->GetFieldID(cls, "y", "F");
	static jfieldID fidZ = env->GetFieldID(cls, "z", "F");

	float x, y, z;
	GetPlayerVelocity( playerid, x, y, z );

	env->SetFloatField( velocity, fidX, x );
	env->SetFloatField( velocity, fidY, y );
	env->SetFloatField( velocity, fidZ, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playCrimeReportForPlayer
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playCrimeReportForPlayer
  (JNIEnv *env, jclass jcls, jint playerid, jint suspectid, jint crime)
{
	PlayCrimeReportForPlayer( playerid, suspectid, crime );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playAudioStreamForPlayer
 * Signature: (ILjava/lang/String;FFFFI)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playAudioStreamForPlayer
  (JNIEnv *env, jclass jcls, jint player, jstring url, jfloat posX, jfloat posY, jfloat posZ, jfloat distance, jint usepos)
{
	const jchar* wurl = env->GetStringChars(url, NULL);
	int len = env->GetStringLength(url);

	char str[1024];
	wcs2mbs( getServerCodepage(), wurl, len, str, sizeof(str) );
	env->ReleaseStringChars(url, wurl);

	PlayAudioStreamForPlayer( player, str, posX, posY, posZ, distance, usepos );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    stopAudioStreamForPlayer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_stopAudioStreamForPlayer
  (JNIEnv *env, jclass jcls, jint playerid)
{
	StopAudioStreamForPlayer( playerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerShopName
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerShopName
  (JNIEnv *env, jclass jcls, jint playerid, jstring name)
{
	const jchar* wmsg = env->GetStringChars(name, NULL);
	int len = env->GetStringLength(name);

	char str[1024];
	wcs2mbs( getPlayerCodepage(playerid), wmsg, len, str, sizeof(str) );
	env->ReleaseStringChars(name, wmsg);

	SetPlayerShopName( playerid, str );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerSkillLevel
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerSkillLevel
  (JNIEnv *env, jclass jcls, jint playerid, jint skill, jint level)
{
	SetPlayerSkillLevel( playerid, skill, level );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerSurfingVehicleID
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerSurfingVehicleID
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerSurfingVehicleID(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerSurfingObjectID
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerSurfingObjectID
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerSurfingObjectID(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    removeBuildingForPlayer
 * Signature: (IIFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_removeBuildingForPlayer
  (JNIEnv *env, jclass jcls, jint player, jint modelid, jfloat x, jfloat y, jfloat z, jfloat radius)
{
	RemoveBuildingForPlayer( player, modelid, x, y, z, radius );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerAttachedObject
 * Signature: (IIIIFFFFFFFFF)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerAttachedObject
  (JNIEnv *env, jclass jcls, jint playerid, jint index, jint modelid, jint bone, jfloat offsetX, jfloat offsetY,
  jfloat offsetZ, jfloat rotX, jfloat rotY, jfloat rotZ, jfloat scaleX, jfloat scaleY, jfloat scaleZ, jint materialcolor1, jint materialcolor2)
{
	return SetPlayerAttachedObject( playerid, index, modelid, bone,
		offsetX, offsetY, offsetZ, rotX, rotY, rotZ, scaleX, scaleY, scaleZ, materialcolor1, materialcolor2 );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    removePlayerAttachedObject
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_removePlayerAttachedObject
  (JNIEnv *env, jclass jcls, jint playerid, jint index)
{
	return RemovePlayerAttachedObject(playerid, index);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerAttachedObjectSlotUsed
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerAttachedObjectSlotUsed
  (JNIEnv *env, jclass jcls, jint playerid, jint index)
{
	return IsPlayerAttachedObjectSlotUsed(playerid, index);
}
/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    editAttachedObject
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_editAttachedObject
  (JNIEnv *env, jclass jcls, jint playerid, jint index)
{
	return EditAttachedObject(playerid, index);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createPlayerTextDraw
 * Signature: (IFFLjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createPlayerTextDraw
  (JNIEnv *env, jclass jcls, jint playerid, jfloat x, jfloat y, jstring text)
{
	const jchar* wmsg = env->GetStringChars(text, NULL);
	int len = env->GetStringLength(text);

	char _text[1024];
	wcs2mbs( getPlayerCodepage(playerid), wmsg, len, _text, sizeof(_text) );
	env->ReleaseStringChars(text, wmsg);

	return CreatePlayerTextDraw(playerid, x, y, _text);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawDestroy
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawDestroy
  (JNIEnv *env, jclass jcls, jint playerid, jint textid)
{
	PlayerTextDrawDestroy(playerid, textid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawLetterSize
 * Signature: (IIFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawLetterSize
  (JNIEnv *env, jclass jcls, jint playerid, jint textid, jfloat x, jfloat y)
{
	PlayerTextDrawLetterSize(playerid, textid, x, y);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawTextSize
 * Signature: (IIFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawTextSize
  (JNIEnv *env, jclass jcls, jint playerid, jint textid, jfloat x, jfloat y)
{
	PlayerTextDrawTextSize(playerid, textid, x, y);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawAlignment
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawAlignment
  (JNIEnv *env, jclass jcls, jint playerid, jint textid, jint alignment)
{
	PlayerTextDrawAlignment(playerid, textid, alignment);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawColor
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawColor
  (JNIEnv *env, jclass jcls, jint playerid, jint textid, jint color)
{
	PlayerTextDrawColor(playerid, textid, color);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawUseBox
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawUseBox
  (JNIEnv *env, jclass jcls, jint playerid, jint textid, jint use)
{
	PlayerTextDrawUseBox(playerid, textid, use);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawBoxColor
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawBoxColor
  (JNIEnv *env, jclass jcls, jint playerid, jint textid, jint color)
{
	PlayerTextDrawBoxColor(playerid, textid, color);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawSetShadow
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetShadow
  (JNIEnv *env, jclass jcls, jint playerid, jint textid, jint size)
{
	PlayerTextDrawSetShadow(playerid, textid, size);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawSetOutline
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetOutline
  (JNIEnv *env, jclass jcls, jint playerid, jint textid, jint size)
{
	PlayerTextDrawSetOutline(playerid, textid, size);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawBackgroundColor
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawBackgroundColor
  (JNIEnv *env, jclass jcls, jint playerid, jint textid, jint color)
{
	PlayerTextDrawBackgroundColor(playerid, textid, color);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawFont
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawFont
  (JNIEnv *env, jclass jcls, jint playerid, jint textid, jint font)
{
	PlayerTextDrawFont(playerid, textid, font);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawSetProportional
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetProportional
  (JNIEnv *env, jclass jcls, jint playerid, jint textid, jint set)
{
	PlayerTextDrawSetProportional(playerid, textid, set);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawSetSelectable
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetSelectable
  (JNIEnv *env, jclass jcls, jint playerid, jint textid, jint set)
{
	PlayerTextDrawSetSelectable(playerid, textid, set);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawShow
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawShow
  (JNIEnv *env, jclass jcls, jint playerid, jint textid)
{
	PlayerTextDrawShow(playerid, textid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawHide
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawHide
  (JNIEnv *env, jclass jcls, jint playerid, jint textid)
{
	PlayerTextDrawHide(playerid, textid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawSetString
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetString
  (JNIEnv *env, jclass jcls, jint playerid, jint textid, jstring string)
{
	const jchar* wmsg = env->GetStringChars(string, NULL);
	int len = env->GetStringLength(string);

	char _string[1024];
	wcs2mbs( getPlayerCodepage(playerid), wmsg, len, _string, sizeof(_string) );
	env->ReleaseStringChars(string, wmsg);

	PlayerTextDrawSetString(playerid, textid, _string);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    playerTextDrawSetPreviewModel
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetPreviewModel
  (JNIEnv *env, jclass jcls, jint playerid, jint textId, jint modelindex)
{
	PlayerTextDrawSetPreviewModel(playerid, textId, modelindex);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    playerTextDrawSetPreviewRot
 * Signature: (IIFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetPreviewRot
  (JNIEnv *env, jclass jcls, jint playerid, jint textId, jfloat fRotX, jfloat fRotY, jfloat fRotZ, jfloat fZoom)
{
	PlayerTextDrawSetPreviewRot(playerid, textId, fRotX, fRotY, fRotZ, fZoom);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    playerTextDrawSetPreviewVehCol
 * Signature: (IIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetPreviewVehCol
  (JNIEnv *env, jclass jcls, jint playerid, jint textId, jint color1, jint color2)
{
	PlayerTextDrawSetPreviewVehCol(playerid, textId, color1, color2);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerChatBubble
 * Signature: (ILjava/lang/String;IFI)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerChatBubble
  (JNIEnv *env, jclass jcls, jint playerid, jstring text, jint color, jfloat drawdistance, jint expiretime)
{
	const jchar* wmsg = env->GetStringChars(text, NULL);
	int len = env->GetStringLength(text);

	char str[1024];
	wcs2mbs( getPlayerCodepage(playerid), wmsg, len, str, sizeof(str) );
	env->ReleaseStringChars(text, wmsg);

	SetPlayerChatBubble( playerid, str, color, drawdistance, expiretime );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    putPlayerInVehicle
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_putPlayerInVehicle
  (JNIEnv *env, jclass jcls, jint playerid, jint vehicleid, jint seatid)
{
	PutPlayerInVehicle( playerid, vehicleid, seatid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerVehicleID
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerVehicleID
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerVehicleID(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerVehicleSeat
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerVehicleSeat
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerVehicleSeat(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    removePlayerFromVehicle
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_removePlayerFromVehicle
  (JNIEnv *env, jclass jcls, jint playerid)
{
	RemovePlayerFromVehicle(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    togglePlayerControllable
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_togglePlayerControllable
  (JNIEnv *env, jclass jcls, jint playerid, jboolean toggle)
{
	TogglePlayerControllable( playerid, toggle );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerPlaySound
 * Signature: (IIFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerPlaySound
  (JNIEnv *env, jclass jcls, jint playerid, jint soundid, jfloat x, jfloat y, jfloat z)
{
	PlayerPlaySound( playerid, soundid, x, y, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    applyAnimation
 * Signature: (ILjava/lang/String;Ljava/lang/String;FIIIIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_applyAnimation
  (JNIEnv *env, jclass jcls, jint playerid, jstring animlib, jstring animname,
  jfloat delta, jint loop, jint lockX, jint lockY, jint freeze, jint time, jint forcesync)
{
	const char* str_animlib = env->GetStringUTFChars(animlib, NULL);
	const char* str_animname = env->GetStringUTFChars(animname, NULL);

	ApplyAnimation( playerid, str_animlib, str_animname, delta, loop, lockX, lockY, freeze, time, forcesync );

	env->ReleaseStringUTFChars(animlib, str_animlib);
	env->ReleaseStringUTFChars(animname, str_animname);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    clearAnimations
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_clearAnimations
  (JNIEnv *env, jclass jcls, jint playerid, jint forcesync)
{
	ClearAnimations( playerid, forcesync );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerAnimationIndex
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerAnimationIndex
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerAnimationIndex(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerSpecialAction
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerSpecialAction
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerSpecialAction(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerSpecialAction
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerSpecialAction
  (JNIEnv *env, jclass jcls, jint playerid, jint actionid)
{
	SetPlayerSpecialAction( playerid, actionid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerCheckpoint
 * Signature: (IFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerCheckpoint
  (JNIEnv *env, jclass jcls, jint playerid, jfloat x, jfloat y, jfloat z, jfloat size)
{
	SetPlayerCheckpoint( playerid, x, y, z, size );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    disablePlayerCheckpoint
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_disablePlayerCheckpoint
  (JNIEnv *env, jclass jcls, jint playerid)
{
	DisablePlayerCheckpoint( playerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerRaceCheckpoint
 * Signature: (IIFFFFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerRaceCheckpoint
  (JNIEnv *env, jclass jcls, jint playerid, jint type, jfloat x, jfloat y, jfloat z,
  jfloat nextX, jfloat nextY, jfloat nextZ, jfloat size)
{
	SetPlayerRaceCheckpoint( playerid, type, x,  y,  z, nextX, nextY, nextZ, size );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    disablePlayerRaceCheckpoint
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_disablePlayerRaceCheckpoint
  (JNIEnv *env, jclass jcls, jint playerid)
{
	DisablePlayerRaceCheckpoint( playerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerWorldBounds
 * Signature: (IFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerWorldBounds
  (JNIEnv *env, jclass jcls, jint playerid, jfloat x_max, jfloat x_min, jfloat y_max, jfloat y_min)
{
	SetPlayerWorldBounds( playerid, x_max, x_min, y_max, y_min );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerMarkerForPlayer
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerMarkerForPlayer
  (JNIEnv *env, jclass jcls, jint playerid, jint showplayerid, jint color)
{
	SetPlayerMarkerForPlayer( playerid, showplayerid, color );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    showPlayerNameTagForPlayer
 * Signature: (IIZ)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_showPlayerNameTagForPlayer
  (JNIEnv *env, jclass jcls, jint playerid, jint showplayerid, jboolean show)
{
	ShowPlayerNameTagForPlayer( playerid, showplayerid, show );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerMapIcon
 * Signature: (IIFFFIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerMapIcon
  (JNIEnv *env, jclass jcls, jint playerid, jint iconid, jfloat x, jfloat y, jfloat z, jint markertype, jint color, jint style)
{
	SetPlayerMapIcon( playerid, iconid, x, y, z, markertype, color, style );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    removePlayerMapIcon
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_removePlayerMapIcon
  (JNIEnv *env, jclass jcls, jint playerid, jint iconid)
{
	RemovePlayerMapIcon( playerid, iconid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerCameraPos
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerCameraPos
  (JNIEnv *env, jclass jcls, jint playerid, jfloat x, jfloat y, jfloat z)
{
	SetPlayerCameraPos( playerid, x, y, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerCameraLookAt
 * Signature: (IFFFI)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerCameraLookAt
  (JNIEnv *env, jclass jcls, jint playerid, jfloat x, jfloat y, jfloat z, jint cut)
{
	SetPlayerCameraLookAt( playerid, x, y, z, cut );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setCameraBehindPlayer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setCameraBehindPlayer
  (JNIEnv *env, jclass jcls, jint playerid)
{
	SetCameraBehindPlayer( playerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerCameraPos
 * Signature: (ILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerCameraPos
  (JNIEnv *env, jclass jcls, jint playerid, jobject vector3d)
{
	static jclass cls = env->GetObjectClass(vector3d);
	static jfieldID fidX = env->GetFieldID(cls, "x", "F");
	static jfieldID fidY = env->GetFieldID(cls, "y", "F");
	static jfieldID fidZ = env->GetFieldID(cls, "z", "F");

	float x, y, z;
	GetPlayerCameraPos( playerid, x, y, z );

	env->SetFloatField( vector3d, fidX, x );
	env->SetFloatField( vector3d, fidY, y );
	env->SetFloatField( vector3d, fidZ, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerCameraFrontVector
 * Signature: (ILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerCameraFrontVector
  (JNIEnv *env, jclass jcls, jint playerid, jobject vector3d)
{
	static jclass cls = env->GetObjectClass(vector3d);
	static jfieldID fidX = env->GetFieldID(cls, "x", "F");
	static jfieldID fidY = env->GetFieldID(cls, "y", "F");
	static jfieldID fidZ = env->GetFieldID(cls, "z", "F");

	float x, y, z;
	GetPlayerCameraFrontVector( playerid, x, y, z );

	env->SetFloatField( vector3d, fidX, x );
	env->SetFloatField( vector3d, fidY, y );
	env->SetFloatField( vector3d, fidZ, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerCameraMode
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerCameraMode
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerCameraMode(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachCameraToObject
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachCameraToObject
  (JNIEnv *env, jclass jcls, jint playerid, jint objectid)
{
	AttachCameraToObject(playerid, objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachCameraToPlayerObject
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachCameraToPlayerObject
  (JNIEnv *env, jclass jcls, jint playerid, jint playerobjectid)
{
	AttachCameraToPlayerObject(playerid, playerobjectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    interpolateCameraPos
 * Signature: (IFFFFFFII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_interpolateCameraPos
  (JNIEnv *env, jclass jcls, jint playerid, jfloat FromX, jfloat FromY, jfloat FromZ, jfloat ToX, jfloat ToY, jfloat ToZ, jint time, jint cut)
{
	InterpolateCameraPos(playerid, FromX, FromY, FromZ, ToX, ToY, ToZ, time, cut);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    interpolateCameraLookAt
 * Signature: (IFFFFFFII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_interpolateCameraLookAt
  (JNIEnv *env, jclass jcls, jint playerid, jfloat FromX, jfloat FromY, jfloat FromZ, jfloat ToX, jfloat ToY, jfloat ToZ, jint time, jint cut)
{
	InterpolateCameraLookAt(playerid, FromX, FromY, FromZ, ToX, ToY, ToZ, time, cut);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerConnected
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerConnected
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return IsPlayerConnected(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerInVehicle
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerInVehicle
  (JNIEnv *env, jclass jcls, jint playerid, jint vehicleid)
{
	return IsPlayerInVehicle(playerid, vehicleid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerInAnyVehicle
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerInAnyVehicle
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return IsPlayerInAnyVehicle(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerInCheckpoint
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerInCheckpoint
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return IsPlayerInCheckpoint(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerInRaceCheckpoint
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerInRaceCheckpoint
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return IsPlayerInRaceCheckpoint(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerVirtualWorld
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerVirtualWorld
  (JNIEnv *env, jclass jcls, jint playerid, jint worldid)
{
	SetPlayerVirtualWorld( playerid, worldid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerVirtualWorld
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerVirtualWorld
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerVirtualWorld(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    enableStuntBonusForPlayer
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_enableStuntBonusForPlayer
  (JNIEnv *env, jclass jcls, jint playerid, jint enabled)
{
	EnableStuntBonusForPlayer( playerid, enabled );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    enableStuntBonusForAll
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_enableStuntBonusForAll
  (JNIEnv *env, jclass jcls, jboolean enabled)
{
	EnableStuntBonusForAll( enabled );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    togglePlayerSpectating
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_togglePlayerSpectating
  (JNIEnv *env, jclass jcls, jint playerid, jboolean toggle)
{
	TogglePlayerSpectating( playerid, toggle );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerSpectatePlayer
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerSpectatePlayer
  (JNIEnv *env, jclass jcls, jint playerid, jint targetplayerid, jint mode)
{
	PlayerSpectatePlayer( playerid, targetplayerid, mode );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerSpectateVehicle
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerSpectateVehicle
  (JNIEnv *env, jclass jcls, jint playerid, jint targetvehicleid, jint mode)
{
	PlayerSpectateVehicle( playerid, targetvehicleid, mode );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    startRecordingPlayerData
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_startRecordingPlayerData
  (JNIEnv *env, jclass jcls, jint playerid, jint recordtype, jstring recordname)
{
	const char* str_recordname = env->GetStringUTFChars(recordname, NULL);

	StartRecordingPlayerData( playerid, recordtype, str_recordname );

	env->ReleaseStringUTFChars(recordname, str_recordname);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    stopRecordingPlayerData
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_stopRecordingPlayerData
  (JNIEnv *env, jclass jcls, jint playerid)
{
	StopRecordingPlayerData( playerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    selectTextDraw
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_selectTextDraw
  (JNIEnv *env, jclass jcls, jint playerid, jint hovercolor)
{
	SelectTextDraw(playerid, hovercolor);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    cancelSelectTextDraw
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_cancelSelectTextDraw
  (JNIEnv *env, jclass jcls, jint playerid)
{
	CancelSelectTextDraw(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    sendClientMessage
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_sendClientMessage
  (JNIEnv *env, jclass jcls, jint playerid, jint color, jstring message)
{
	const jchar* wmsg = env->GetStringChars(message, NULL);
	int len = env->GetStringLength(message);
	
	char msg[1024];
	wcs2mbs( getPlayerCodepage(playerid), wmsg, len, msg, sizeof(msg) );
	env->ReleaseStringChars(message, wmsg);

	SendClientMessage( playerid, color, msg );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    sendClientMessageToAll
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_sendClientMessageToAll
  (JNIEnv *env, jclass jcls, jint color, jstring message)
{
	const jchar* wmsg = env->GetStringChars(message, NULL);
	int len = env->GetStringLength(message);

	char msg[1024];
	wcs2mbs( getServerCodepage(), wmsg, len, msg, sizeof(msg) );
	env->ReleaseStringChars(message, wmsg);

	SendClientMessageToAll( color, msg );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    sendPlayerMessageToPlayer
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_sendPlayerMessageToPlayer
  (JNIEnv *env, jclass jcls, jint playerid, jint senderid, jstring message)
{
	const jchar* wmsg = env->GetStringChars(message, NULL);
	int len = env->GetStringLength(message);

	char msg[1024];
	wcs2mbs( getPlayerCodepage(playerid), wmsg, len, msg, sizeof(msg) );
	env->ReleaseStringChars(message, wmsg);

	SendPlayerMessageToPlayer( playerid, senderid, msg );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    sendPlayerMessageToAll
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_sendPlayerMessageToAll
  (JNIEnv *env, jclass jcls, jint senderid, jstring message)
{
	const jchar* wmsg = env->GetStringChars(message, NULL);
	int len = env->GetStringLength(message);

	char msg[1024];
	wcs2mbs( getServerCodepage(), wmsg, len, msg, sizeof(msg) );
	env->ReleaseStringChars(message, wmsg);
	
	SendPlayerMessageToAll( senderid, msg );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    sendDeathMessage
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_sendDeathMessage
  (JNIEnv *env, jclass jcls, jint killerid, jint victimid, jint reason)
{
	SendDeathMessage( killerid, victimid, reason );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gameTextForAll
 * Signature: (Ljava/lang/String;II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gameTextForAll
  (JNIEnv *env, jclass jcls, jstring string, jint time, jint style)
{
	const jchar* wmsg = env->GetStringChars(string, NULL);
	int len = env->GetStringLength(string);

	char str[1024];
	wcs2mbs( getServerCodepage(), wmsg, len, str, sizeof(str) );
	env->ReleaseStringChars(string, wmsg);

	GameTextForAll( str, time, style );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gameTextForPlayer
 * Signature: (ILjava/lang/String;II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gameTextForPlayer
  (JNIEnv *env, jclass jcls, jint playerid, jstring string, jint time, jint style)
{
	const jchar* wmsg = env->GetStringChars(string, NULL);
	int len = env->GetStringLength(string);

	char str[1024];
	wcs2mbs( getPlayerCodepage(playerid), wmsg, len, str, sizeof(str) );
	env->ReleaseStringChars(string, wmsg);

	GameTextForPlayer( playerid, str, time, style );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setTimer
 * Signature: (III)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setTimer
  (JNIEnv *env, jclass jcls, jint index, jint interval, jint repeating)
{
	return SetTimer( index, interval, repeating );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    killTimer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_killTimer
  (JNIEnv *env, jclass jcls, jint timerid)
{
	KillTimer( timerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getMaxPlayers
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getMaxPlayers
  (JNIEnv *env, jclass jcls)
{
	return GetMaxPlayers();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setGameModeText
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setGameModeText
  (JNIEnv *env, jclass jcls, jstring string)
{
	const jchar* wmsg = env->GetStringChars(string, NULL);
	int len = env->GetStringLength(string);

	char str[1024];
	wcs2mbs( getServerCodepage(), wmsg, len, str, sizeof(str) );
	env->ReleaseStringChars(string, wmsg);

	SetGameModeText( str );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setTeamCount
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setTeamCount
  (JNIEnv *env, jclass jcls, jint count)
{
	SetTeamCount( count );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    addPlayerClass
 * Signature: (IFFFFIIIIII)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_addPlayerClass
  (JNIEnv *env, jclass jcls, jint modelid, jfloat spawn_x, jfloat spawn_y, jfloat spawn_z,
  jfloat z_angle, jint weapon1, jint weapon1_ammo, jint weapon2, jint weapon2_ammo, jint weapon3, jint weapon3_ammo)
{
	return AddPlayerClass( modelid, spawn_x, spawn_y, spawn_z, z_angle, 
		weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    addPlayerClassEx
 * Signature: (IIFFFFIIIIII)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_addPlayerClassEx
  (JNIEnv *env, jclass jcls, int teamid, jint modelid, jfloat spawn_x, jfloat spawn_y, jfloat spawn_z,
  jfloat z_angle, jint weapon1, jint weapon1_ammo, jint weapon2, jint weapon2_ammo, jint weapon3, jint weapon3_ammo)
{
	return AddPlayerClassEx( teamid, modelid, spawn_x, spawn_y, spawn_z, z_angle, 
		weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    addStaticVehicle
 * Signature: (IFFFFII)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_addStaticVehicle
  (JNIEnv *env, jclass jcls, jint modelid, jfloat spawn_x, jfloat spawn_y, jfloat spawn_z, 
  jfloat z_angle, jint color1, jint color2)
{
	return AddStaticVehicle( modelid, spawn_x, spawn_x, spawn_z, z_angle, color1, color2 );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    addStaticVehicleEx
 * Signature: (IFFFFIII)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_addStaticVehicleEx
  (JNIEnv *env, jclass jcls,  jint modelid, jfloat spawn_x, jfloat spawn_y, jfloat spawn_z, 
  jfloat z_angle, jint color1, jint color2, jint respawn_delay)
{
	return AddStaticVehicleEx( modelid, spawn_x, spawn_y, spawn_z, z_angle, color1, color2, respawn_delay );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    addStaticPickup
 * Signature: (IIFFFI)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_addStaticPickup
  (JNIEnv *env, jclass jcls, jint model, jint type, jfloat x, jfloat y, jfloat z, jint virtualworld)
{
	return AddStaticPickup( model, type, x, y, z, virtualworld );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createPickup
 * Signature: (IIFFFI)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createPickup
  (JNIEnv *env, jclass jcls, jint model, jint type, jfloat x, jfloat y, jfloat z, jint virtualworld)
{
	return CreatePickup( model, type, x, y, z, virtualworld );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    destroyPickup
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_destroyPickup
  (JNIEnv *env, jclass jcls, jint pickup)
{
	DestroyPickup( pickup );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    showNameTags
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_showNameTags
  (JNIEnv *env, jclass jcls, jboolean enabled)
{
	ShowNameTags( enabled );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    showPlayerMarkers
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_showPlayerMarkers
  (JNIEnv *env, jclass jcls, jint mode)
{
	ShowPlayerMarkers( mode );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gameModeExit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gameModeExit
  (JNIEnv *env, jclass jcls)
{
	GameModeExit();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setWorldTime
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setWorldTime
  (JNIEnv *env, jclass jcls, jint hour)
{
	SetWorldTime( hour );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getWeaponName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getWeaponName
  (JNIEnv *env, jclass jcls, jint weaponid)
{
	char name[32];
	GetWeaponName( weaponid, name, sizeof(name) );

	return env->NewStringUTF(name);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    enableTirePopping
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_enableTirePopping
  (JNIEnv *env, jclass jcls, jboolean enabled)
{
	EnableTirePopping( enabled );
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    enableVehicleFriendlyFire
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_enableVehicleFriendlyFire
  (JNIEnv *env, jclass jcls)
{
	EnableVehicleFriendlyFire();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    allowInteriorWeapons
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_allowInteriorWeapons
  (JNIEnv *env, jclass jcls, jboolean allow)
{
	AllowInteriorWeapons( allow );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setWeather
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setWeather
  (JNIEnv *env, jclass jcls, jint weatherid)
{
	SetWeather( weatherid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setGravity
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setGravity
  (JNIEnv *env, jclass jcls, jfloat gravity)
{
	SetGravity( gravity );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setDeathDropAmount
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setDeathDropAmount
  (JNIEnv *env, jclass jcls, jint amount)
{
	SetDeathDropAmount( amount );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createExplosion
 * Signature: (FFFIF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createExplosion
  (JNIEnv *env, jclass jcls, jfloat x, jfloat y, jfloat z, jint type, jfloat radius)
{
	CreateExplosion( x, y, z, type, radius );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    enableZoneNames
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_enableZoneNames
  (JNIEnv *env, jclass jcls, jboolean enabled)
{
	EnableZoneNames( enabled );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    usePlayerPedAnims
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_usePlayerPedAnims
  (JNIEnv *env, jclass jcls)
{
	UsePlayerPedAnims();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    disableInteriorEnterExits
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_disableInteriorEnterExits
  (JNIEnv *env, jclass jcls)
{
	DisableInteriorEnterExits();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setNameTagDrawDistance
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setNameTagDrawDistance
  (JNIEnv *env, jclass jcls, jfloat distance)
{
	SetNameTagDrawDistance( distance );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    disableNameTagLOS
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_disableNameTagLOS
  (JNIEnv *env, jclass jcls)
{
	DisableNameTagLOS();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    limitGlobalChatRadius
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_limitGlobalChatRadius
  (JNIEnv *env, jclass jcls, jfloat chat_radius)
{
	LimitGlobalChatRadius( chat_radius );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    limitPlayerMarkerRadius
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_limitPlayerMarkerRadius
  (JNIEnv *env, jclass jcls, jfloat chat_radius)
{
	LimitPlayerMarkerRadius( chat_radius );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    connectNPC
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_connectNPC
  (JNIEnv *env, jclass jcls, jstring name, jstring script)
{
	const char* str_name = env->GetStringUTFChars(name, NULL);
	const char* str_script = env->GetStringUTFChars(script, NULL);

	ConnectNPC( str_name, str_script );

	env->ReleaseStringUTFChars(name, str_name);
	env->ReleaseStringUTFChars(name, str_script);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerNPC
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerNPC
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return IsPlayerNPC(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerAdmin
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerAdmin
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return IsPlayerAdmin(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    kick
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_kick
  (JNIEnv *env, jclass jcls, jint playerid)
{
	Kick( playerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    ban
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_ban
  (JNIEnv *env, jclass jcls, jint playerid)
{
	Ban( playerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    banEx
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_banEx
  (JNIEnv *env, jclass jcls, jint playerid, jstring reason)
{
	const jchar* wmsg = env->GetStringChars(reason, NULL);
	int len = env->GetStringLength(reason);

	char str[1024];
	wcs2mbs( getServerCodepage(), wmsg, len, str, sizeof(str) );
	env->ReleaseStringChars(reason, wmsg);

	BanEx( playerid, str );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    sendRconCommand
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_sendRconCommand
  (JNIEnv *env, jclass jcls, jstring cmd)
{
	const jchar* wmsg = env->GetStringChars(cmd, NULL);
	int len = env->GetStringLength(cmd);

	char str[1024];
	wcs2mbs( getServerCodepage(), wmsg, len, str, sizeof(str) );
	env->ReleaseStringChars(cmd, wmsg);

	SendRconCommand( str );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getServerVarAsString
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getServerVarAsString
  (JNIEnv *env, jclass jcls, jstring varname)
{
	const char* str_varname = env->GetStringUTFChars(varname, NULL);

	char var[256];
	GetServerVarAsString( str_varname, var, sizeof(var) );
	env->ReleaseStringUTFChars(varname, str_varname);

	jchar wstr[256];
	int len = mbs2wcs( getServerCodepage(), var, -1, wstr, sizeof(wstr)/sizeof(wstr[0]) );
	return env->NewString(wstr, len);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getServerVarAsInt
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getServerVarAsInt
  (JNIEnv *env, jclass jcls, jstring varname)
{
	const char* str_varname = env->GetStringUTFChars(varname, NULL);

	int ret = GetServerVarAsInt(str_varname);

	env->ReleaseStringUTFChars(varname, str_varname);
	return ret;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getServerVarAsBool
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getServerVarAsBool
  (JNIEnv *env, jclass jcls, jstring varname)
{
	const char* str_varname = env->GetStringUTFChars(varname, NULL);

	int ret = GetServerVarAsBool(str_varname);

	env->ReleaseStringUTFChars(varname, str_varname);
	return ret;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerNetworkStats
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerNetworkStats
  (JNIEnv *env, jclass jcls, jint playerid)
{
	char retstr[2048];
	GetPlayerNetworkStats( playerid, retstr, sizeof(retstr) );

	jchar wstr[2048];
	int len = mbs2wcs( getServerCodepage(), retstr, -1, wstr, sizeof(wstr)/sizeof(wstr[0]) );

	return env->NewString(wstr, len);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getNetworkStats
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getNetworkStats
  (JNIEnv *env, jclass jcls)
{
	char retstr[2048];
	GetNetworkStats( retstr, sizeof(retstr) );

	jchar wstr[2048];
	int len = mbs2wcs( getServerCodepage(), retstr, -1, wstr, sizeof(wstr)/sizeof(wstr[0]) );

	return env->NewString(wstr, len);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerVersion
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerVersion
  (JNIEnv *env, jclass jcls, jint playerid)
{
	char str[128];
	GetPlayerVersion(playerid, str, sizeof(str));

	return env->NewStringUTF(str);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createMenu
 * Signature: (Ljava/lang/String;IFFFF)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createMenu
  (JNIEnv *env, jclass jcls, jstring title, jint columns, jfloat x, jfloat y, jfloat col1width, jfloat col2width)
{
	const char* str_title = env->GetStringUTFChars(title, NULL);

	int ret = CreateMenu(str_title, columns, x, y, col1width, col2width);

	env->ReleaseStringUTFChars(title, str_title);
	return ret;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    destroyMenu
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_destroyMenu
  (JNIEnv *env, jclass jcls, jint menuid)
{
	DestroyMenu( menuid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    addMenuItem
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_addMenuItem
  (JNIEnv *env, jclass jcls, jint menuid, jint column, jstring menutext)
{
	const char* str_menutext = env->GetStringUTFChars(menutext, NULL);

	AddMenuItem( menuid, column, str_menutext );

	env->ReleaseStringUTFChars(menutext, str_menutext);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setMenuColumnHeader
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setMenuColumnHeader
  (JNIEnv *env, jclass jcls, jint menuid, jint column, jstring columnheader)
{
	const char* str_columnheader = env->GetStringUTFChars(columnheader, NULL);

	AddMenuItem( menuid, column, str_columnheader );

	env->ReleaseStringUTFChars(columnheader, str_columnheader);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    showMenuForPlayer
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_showMenuForPlayer
  (JNIEnv *env, jclass jcls, jint menuid, jint playerid)
{
	ShowMenuForPlayer( menuid, playerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    hideMenuForPlayer
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_hideMenuForPlayer
  (JNIEnv *env, jclass jcls, jint menuid, jint playerid)
{
	HideMenuForPlayer( menuid, playerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isValidMenu
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isValidMenu
  (JNIEnv *env, jclass jcls, jint menuid)
{
	return IsValidMenu( menuid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    disableMenu
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_disableMenu
  (JNIEnv *env, jclass jcls, jint menuid)
{
	DisableMenu( menuid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    disableMenuRow
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_disableMenuRow
  (JNIEnv *env, jclass jcls, jint menuid, jint row)
{
	DisableMenuRow( menuid, row );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerMenu
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerMenu
  (JNIEnv *env, jclass jcls, jint playerid)
{
	return GetPlayerMenu(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawCreate
 * Signature: (FFLjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawCreate
  (JNIEnv *env, jclass jcls, jfloat x, jfloat y, jstring text)
{
	const char* str = env->GetStringUTFChars(text, NULL);

	int ret = TextDrawCreate( x, y, str );

	env->ReleaseStringUTFChars(text, str);
	return ret;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawDestroy
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawDestroy
  (JNIEnv *env, jclass jcls, jint textid)
{
	TextDrawDestroy( textid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawLetterSize
 * Signature: (IFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawLetterSize
  (JNIEnv *env, jclass jcls, jint textid, jfloat x, jfloat y)
{
	TextDrawLetterSize( textid, x, y );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawTextSize
 * Signature: (IFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawTextSize
  (JNIEnv *env, jclass jcls, jint textid, jfloat x, jfloat y)
{
	TextDrawTextSize( textid, x, y );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawAlignment
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawAlignment
  (JNIEnv *env, jclass jcls, jint textid, jint alignment)
{
	TextDrawAlignment( textid, alignment );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawColor
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawColor
  (JNIEnv *env, jclass jcls, jint textid, jint color)
{
	TextDrawColor( textid, color );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawUseBox
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawUseBox
  (JNIEnv *env, jclass jcls, jint textid, jboolean use)
{
	TextDrawUseBox( textid, use );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawBoxColor
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawBoxColor
  (JNIEnv *env, jclass jcls, jint textid, jint color)
{
	TextDrawBoxColor( textid, color );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawSetShadow
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetShadow
  (JNIEnv *env, jclass jcls, jint textid, jint size)
{
	TextDrawSetShadow( textid, size );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawSetOutline
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetOutline
  (JNIEnv *env, jclass jcls, jint textid, jint size)
{
	TextDrawSetOutline( textid, size );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawBackgroundColor
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawBackgroundColor
  (JNIEnv *env, jclass jcls, jint textid, jint color)
{
	TextDrawBackgroundColor( textid, color );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawFont
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawFont
  (JNIEnv *env, jclass jcls, jint textid, jint font)
{
	TextDrawFont( textid, font );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawSetProportional
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetProportional
  (JNIEnv *env, jclass jcls, jint textid, jint set)
{
	TextDrawSetProportional( textid, set );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawSetSelectable
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetSelectable
  (JNIEnv *env, jclass jcls, jint textid, jint set)
{
	TextDrawSetSelectable(textid, set);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawShowForPlayer
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawShowForPlayer
  (JNIEnv *env, jclass jcls, jint playerid, jint textid)
{
	TextDrawShowForPlayer( playerid, textid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawHideForPlayer
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawHideForPlayer
  (JNIEnv *env, jclass jcls, jint playerid, jint textid)
{
	TextDrawHideForPlayer( playerid, textid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawShowForAll
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawShowForAll
  (JNIEnv *env, jclass jcls, jint textid)
{
	TextDrawShowForAll( textid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawHideForAll
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawHideForAll
  (JNIEnv *env, jclass jcls, jint textid)
{
	TextDrawHideForAll( textid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawSetString
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetString
  (JNIEnv *env, jclass jcls, jint textid, jstring string)
{
	const char* str = env->GetStringUTFChars(string, NULL);

	TextDrawSetString( textid, str );

	env->ReleaseStringUTFChars(string, str);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    textDrawSetPreviewModel
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetPreviewModel
  (JNIEnv *env, jclass jcls, jint textid, jint modelindex)
{
	TextDrawSetPreviewModel(textid, modelindex);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    textDrawSetPreviewRot
 * Signature: (IFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetPreviewRot
  (JNIEnv *env, jclass jcls, jint textid, jfloat fRotX, jfloat fRotY, jfloat fRotZ, jfloat fZoom)
{
	TextDrawSetPreviewRot(textid, fRotX, fRotY, fRotZ, fZoom);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    textDrawSetPreviewVehCol
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetPreviewVehCol
  (JNIEnv *env, jclass jcls, jint textid, jint color1, jint color2)
{
	TextDrawSetPreviewVehCol(textid, color1, color2);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneCreate
 * Signature: (FFFF)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneCreate
  (JNIEnv *env, jclass jcls, jfloat minx, jfloat miny, jfloat maxx, jfloat maxy)
{
	return GangZoneCreate(minx, miny, maxx, maxy);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneDestroy
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneDestroy
  (JNIEnv *env, jclass jcls, jint zoneid)
{
	GangZoneDestroy( zoneid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneShowForPlayer
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneShowForPlayer
  (JNIEnv *env, jclass jcls, jint playerid, jint zoneid, jint color)
{
	GangZoneShowForPlayer( playerid, zoneid, color );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneShowForAll
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneShowForAll
  (JNIEnv *env, jclass jcls, jint zoneid, jint color)
{
	GangZoneShowForAll( zoneid, color );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneHideForPlayer
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneHideForPlayer
  (JNIEnv *env, jclass jcls, jint playerid, jint zoneid)
{
	GangZoneHideForPlayer( playerid, zoneid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneHideForAll
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneHideForAll
  (JNIEnv *env, jclass jcls, jint zoneid)
{
	GangZoneHideForAll( zoneid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneFlashForPlayer
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneFlashForPlayer
  (JNIEnv *env, jclass jcls, jint playerid, jint zoneid, jint flashcolor)
{
	GangZoneFlashForPlayer( playerid, zoneid, flashcolor );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneFlashForAll
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneFlashForAll
  (JNIEnv *env, jclass jcls, jint zoneid, jint flashcolor)
{
	GangZoneFlashForAll( zoneid, flashcolor );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneStopFlashForPlayer
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneStopFlashForPlayer
  (JNIEnv *env, jclass jcls, jint playerid, jint zoneid)
{
	GangZoneStopFlashForPlayer( playerid, zoneid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneStopFlashForAll
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneStopFlashForAll
  (JNIEnv *env, jclass jcls, jint zoneid)
{
	GangZoneStopFlashForAll( zoneid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    create3DTextLabel
 * Signature: (Ljava/lang/String;IFFFFIZ)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_create3DTextLabel
  (JNIEnv *env, jclass jcls, jstring text, jint color, jfloat x, jfloat y , jfloat z,
  jfloat drawDistance, jint worldid, jboolean testLOS)
{
	const jchar* wmsg = env->GetStringChars(text, NULL);
	int len = env->GetStringLength(text);

	char str[1024];
	wcs2mbs( getServerCodepage(), wmsg, len, str, sizeof(str) );
	env->ReleaseStringChars(text, wmsg);

	return Create3DTextLabel(str, color, x, y, z, drawDistance, worldid, testLOS);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    delete3DTextLabel
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_delete3DTextLabel
  (JNIEnv *env, jclass jcls, jint id)
{
	Delete3DTextLabel( id );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attach3DTextLabelToPlayer
 * Signature: (IIFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attach3DTextLabelToPlayer
  (JNIEnv *env, jclass jcls, jint id, jint playerid, jfloat offsetX, jfloat offsetY, jfloat offsetZ)
{
	Attach3DTextLabelToPlayer( id, playerid, offsetX, offsetY, offsetZ );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attach3DTextLabelToVehicle
 * Signature: (IIFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attach3DTextLabelToVehicle
  (JNIEnv *env, jclass jcls, jint id, jint vehicleid, jfloat offsetX, jfloat offsetY, jfloat offsetZ)
{
	Attach3DTextLabelToVehicle( id, vehicleid, offsetX, offsetY, offsetZ );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    update3DTextLabelText
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_update3DTextLabelText
  (JNIEnv *env, jclass jcls, jint id, jint color, jstring text)
{
	const jchar* wmsg = env->GetStringChars(text, NULL);
	int len = env->GetStringLength(text);

	char str[1024];
	wcs2mbs( getServerCodepage(), wmsg, len, str, sizeof(str) );
	env->ReleaseStringChars(text, wmsg);

	Update3DTextLabelText( id, color, str );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createPlayer3DTextLabel
 * Signature: (ILjava/lang/String;IFFFFIIZ)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createPlayer3DTextLabel
  (JNIEnv *env, jclass jcls, jint playerid, jstring text, jint color, jfloat x, jfloat y, jfloat z,
  jfloat drawDistance, jint attachedplayerid, jint attachedvehicleid, jboolean testLOS)
{
	const jchar* wmsg = env->GetStringChars(text, NULL);
	int len = env->GetStringLength(text);

	char str[1024];
	wcs2mbs( getPlayerCodepage(playerid), wmsg, len, str, sizeof(str) );
	env->ReleaseStringChars(text, wmsg);

	return CreatePlayer3DTextLabel( playerid, str, color, x, y, z, drawDistance, attachedplayerid, attachedvehicleid, testLOS );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    deletePlayer3DTextLabel
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_deletePlayer3DTextLabel
  (JNIEnv *env, jclass jcls, jint playerid, jint id)
{
	DeletePlayer3DTextLabel( playerid, id );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    updatePlayer3DTextLabelText
 * Signature: (IIILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_updatePlayer3DTextLabelText
  (JNIEnv *env, jclass jcls, jint playerid, jint id, jint color, jstring text)
{
	const jchar* wmsg = env->GetStringChars(text, NULL);
	int len = env->GetStringLength(text);

	char str[1024];
	wcs2mbs( getPlayerCodepage(playerid), wmsg, len, str, sizeof(str) );
	env->ReleaseStringChars(text, wmsg);

	UpdatePlayer3DTextLabelText( playerid, id, color, str );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    showPlayerDialog
 * Signature: (IIILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_showPlayerDialog
  (JNIEnv *env, jclass jcls, jint playerid, jint dialogid, jint style,
  jstring caption, jstring info, jstring button1, jstring button2)
{
	char str_caption[64], str_info[2048], str_button1[32], str_button2[32];
	int len;
	
	const jchar* wmsg1 = env->GetStringChars(caption, NULL);
	len = env->GetStringLength(caption);
	wcs2mbs( getPlayerCodepage(playerid), wmsg1, len, str_caption, sizeof(str_caption) );
	env->ReleaseStringChars(caption, wmsg1);

	const jchar* wmsg2 = env->GetStringChars(info, NULL);
	len = env->GetStringLength(info);
	wcs2mbs( getPlayerCodepage(playerid), wmsg2, len, str_info, sizeof(str_info) );
	env->ReleaseStringChars(info, wmsg2);

	const jchar* wmsg3 = env->GetStringChars(button1, NULL);
	len = env->GetStringLength(button1);
	wcs2mbs( getPlayerCodepage(playerid), wmsg3, len, str_button1, sizeof(str_button1) );
	env->ReleaseStringChars(button1, wmsg3);

	const jchar* wmsg4 = env->GetStringChars(button2, NULL);
	len = env->GetStringLength(button2);
	wcs2mbs( getPlayerCodepage(playerid), wmsg4, len, str_button2, sizeof(str_button2) );
	env->ReleaseStringChars(button2, wmsg4);

	return ShowPlayerDialog(playerid, dialogid, style, str_caption, str_info, str_button1, str_button2);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createVehicle
 * Signature: (IFFFFIII)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createVehicle
  (JNIEnv *env, jclass jcls, jint model, jfloat x, jfloat y, jfloat z, jfloat rotation, jint color1, jint color2, jint respawnDelay)
{
	return CreateVehicle(model, x, y, z, rotation, color1, color2, respawnDelay);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    destroyVehicle
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_destroyVehicle
  (JNIEnv *env, jclass jcls, jint vehicleid)
{
	DestroyVehicle( vehicleid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isVehicleStreamedIn
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isVehicleStreamedIn
  (JNIEnv *env, jclass jcls, jint vehicleid, jint forplayerid)
{
	return IsVehicleStreamedIn(vehicleid, forplayerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehiclePos
 * Signature: (ILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehiclePos
  (JNIEnv *env, jclass jcls, jint vehicleid, jobject vector3d)
{
	static jclass cls = env->GetObjectClass(vector3d);
	static jfieldID fidX = env->GetFieldID(cls, "x", "F");
	static jfieldID fidY = env->GetFieldID(cls, "y", "F");
	static jfieldID fidZ = env->GetFieldID(cls, "z", "F");

	float x, y, z;
	GetVehiclePos( vehicleid, x, y, z );

	env->SetFloatField( vector3d, fidX, x );
	env->SetFloatField( vector3d, fidY, y );
	env->SetFloatField( vector3d, fidZ, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehiclePos
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehiclePos
  (JNIEnv *env, jclass jcls, jint vehicleid, jfloat x, jfloat y, jfloat z)
{
	SetVehiclePos( vehicleid, x, y, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleZAngle
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleZAngle
  (JNIEnv *env, jclass jcls, jint vehicleid)
{
	float angle;
	GetVehicleZAngle( vehicleid, angle );

	return angle;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleRotationQuat
 * Signature: (ILnet/gtaun/shoebill/data/Quaternion;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleRotationQuat
  (JNIEnv *env, jclass jcls, jint vehicleid, jobject quat)
{
	static jclass cls = env->GetObjectClass(quat);
	static jfieldID fidW = env->GetFieldID(cls, "w", "F");
	static jfieldID fidX = env->GetFieldID(cls, "x", "F");
	static jfieldID fidY = env->GetFieldID(cls, "y", "F");
	static jfieldID fidZ = env->GetFieldID(cls, "z", "F");

	float w, x, y, z;
	GetVehicleRotationQuat( vehicleid, w, x, y, z );

	env->SetFloatField( quat, fidW, w );
	env->SetFloatField( quat, fidX, x );
	env->SetFloatField( quat, fidY, y );
	env->SetFloatField( quat, fidZ, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleZAngle
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleZAngle
  (JNIEnv *env, jclass jcls, jint vehicleid, jfloat angle)
{
	SetVehicleZAngle( vehicleid, angle );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleParamsForPlayer
 * Signature: (IIZZ)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleParamsForPlayer
  (JNIEnv *env, jclass jcls, jint vehicleid, jint playerid, jboolean objective, jboolean doorslocked)
{
	SetVehicleParamsForPlayer( vehicleid, playerid, objective, doorslocked );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    manualVehicleEngineAndLights
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_manualVehicleEngineAndLights
  (JNIEnv *env, jclass jcls)
{
	ManualVehicleEngineAndLights();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleParamsEx
 * Signature: (IZZZZZZZ)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleParamsEx
  (JNIEnv *env, jclass jcls, jint vehicleid, jint engine, jint lights,
  jint alarm, jint doors, jint bonnet, jint boot, jint objective)
{
	SetVehicleParamsEx( vehicleid, engine, lights, alarm, doors, bonnet, boot, objective );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleParamsEx
 * Signature: (ILnet/gtaun/shoebill/object/impl/VehicleParamImpl;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleParamsEx
  (JNIEnv *env, jclass jcls, jint vehicleid, jobject state)
{
	static jclass cls = env->GetObjectClass(state);
	static jfieldID fidEngine = env->GetFieldID(cls, "engine", "I");
	static jfieldID fidLights = env->GetFieldID(cls, "lights", "I");
	static jfieldID fidAlarm = env->GetFieldID(cls, "alarm", "I");
	static jfieldID fidDoors = env->GetFieldID(cls, "doors", "I");
	static jfieldID fidBonnet = env->GetFieldID(cls, "bonnet", "I");
	static jfieldID fidBoot = env->GetFieldID(cls, "boot", "I");
	static jfieldID fidObjective = env->GetFieldID(cls, "objective", "I");

	int engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx( vehicleid, engine, lights, alarm, doors, bonnet, boot, objective );

	env->SetIntField( state, fidEngine, engine );
	env->SetIntField( state, fidLights, lights );
	env->SetIntField( state, fidAlarm, alarm );
	env->SetIntField( state, fidDoors, doors );
	env->SetIntField( state, fidBonnet, bonnet );
	env->SetIntField( state, fidBoot, boot );
	env->SetIntField( state, fidObjective, objective );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleToRespawn
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleToRespawn
  (JNIEnv *env, jclass jcls, jint vehicleid)
{
	SetVehicleToRespawn( vehicleid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    linkVehicleToInterior
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_linkVehicleToInterior
  (JNIEnv *env, jclass jcls, jint vehicleid, jint interiorid)
{
	LinkVehicleToInterior( vehicleid, interiorid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    addVehicleComponent
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_addVehicleComponent
  (JNIEnv *env, jclass jcls, jint vehicleid, jint componentid)
{
	AddVehicleComponent( vehicleid, componentid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    removeVehicleComponent
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_removeVehicleComponent
  (JNIEnv *env, jclass jcls, jint vehicleid, jint componentid)
{
	RemoveVehicleComponent( vehicleid, componentid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    changeVehicleColor
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_changeVehicleColor
  (JNIEnv *env, jclass jcls, jint vehicleid, jint color1, jint color2)
{
	ChangeVehicleColor( vehicleid, color1, color2 );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    changeVehiclePaintjob
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_changeVehiclePaintjob
  (JNIEnv *env, jclass jcls, jint vehicleid, jint paintjobid)
{
	ChangeVehiclePaintjob( vehicleid, paintjobid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleHealth
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleHealth
  (JNIEnv *env, jclass jcls, jint vehicleid, jfloat health)
{
	SetVehicleHealth( vehicleid, health );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleHealth
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleHealth
  (JNIEnv *env, jclass jcls, jint vehicleid)
{
	float health;
	GetVehicleHealth( vehicleid, health );

	return health;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachTrailerToVehicle
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachTrailerToVehicle
  (JNIEnv *env, jclass jcls, jint trailerid, jint vehicleid)
{
	AttachTrailerToVehicle( trailerid, vehicleid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    detachTrailerFromVehicle
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_detachTrailerFromVehicle
  (JNIEnv *env, jclass jcls, jint trailerid)
{
	DetachTrailerFromVehicle( trailerid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isTrailerAttachedToVehicle
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isTrailerAttachedToVehicle
  (JNIEnv *env, jclass jcls, jint vehicleid)
{
	return IsTrailerAttachedToVehicle(vehicleid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleTrailer
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleTrailer
  (JNIEnv *env, jclass jcls, jint vehicleid)
{
	return GetVehicleTrailer(vehicleid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleNumberPlate
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleNumberPlate
  (JNIEnv *env, jclass jcls, jint vehicleid, jstring numberplate)
{
	const char* str_numberplate = env->GetStringUTFChars(numberplate, NULL);

	SetVehicleNumberPlate( vehicleid, str_numberplate );

	env->ReleaseStringUTFChars(numberplate, str_numberplate);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleModel
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleModel
  (JNIEnv *env, jclass jcls, jint vehicleid)
{
	return 0;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleComponentInSlot
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleComponentInSlot
  (JNIEnv *env, jclass jcls, jint vehicleid, jint slot)
{
	return GetVehicleComponentInSlot(vehicleid, slot);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleComponentType
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleComponentType
  (JNIEnv *env, jclass jcls, jint component)
{
	return GetVehicleComponentType(component);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    repairVehicle
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_repairVehicle
  (JNIEnv *env, jclass jcls, jint vehicleid)
{
	RepairVehicle( vehicleid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleVelocity
 * Signature: (ILnet/gtaun/shoebill/data/Velocity;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleVelocity
  (JNIEnv *env, jclass jcls, jint vehicleid, jobject velocity)
{
	static jclass cls = env->GetObjectClass(velocity);
	static jfieldID fidX = env->GetFieldID(cls, "x", "F");
	static jfieldID fidY = env->GetFieldID(cls, "y", "F");
	static jfieldID fidZ = env->GetFieldID(cls, "z", "F");

	float x, y, z;
	GetVehicleVelocity( vehicleid, x, y, z );

	env->SetFloatField( velocity, fidX, x );
	env->SetFloatField( velocity, fidY, y );
	env->SetFloatField( velocity, fidZ, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleVelocity
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleVelocity
  (JNIEnv *env, jclass jcls, jint vehicleid, jfloat x, jfloat y, jfloat z)
{
	SetVehicleVelocity( vehicleid, x, y, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleAngularVelocity
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleAngularVelocity
  (JNIEnv *env, jclass jcls, jint vehicleid, jfloat x, jfloat y, jfloat z)
{
	SetVehicleAngularVelocity( vehicleid, x, y, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleDamageStatus
 * Signature: (ILnet/gtaun/shoebill/object/impl/VehicleDamageImpl;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleDamageStatus
  (JNIEnv *env, jclass jcls, jint vehicleid, jobject damage )
{
	static jclass cls = env->GetObjectClass(damage);
	static jfieldID fidPanels = env->GetFieldID(cls, "panels", "I");
	static jfieldID fidDoors = env->GetFieldID(cls, "doors", "I");
	static jfieldID fidLights = env->GetFieldID(cls, "lights", "I");
	static jfieldID fidTires = env->GetFieldID(cls, "lights", "I");

	int panels, doors, lights, tires;
	GetVehicleDamageStatus( vehicleid, panels, doors, lights, tires );

	env->SetIntField( damage, fidPanels, panels );
	env->SetIntField( damage, fidDoors, doors );
	env->SetIntField( damage, fidLights, lights );
	env->SetIntField( damage, fidTires, tires );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    updateVehicleDamageStatus
 * Signature: (IIIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_updateVehicleDamageStatus
  (JNIEnv *env, jclass jcls, jint vehicleid, jint panels, jint doors, jint lights, jint tires)
{
	UpdateVehicleDamageStatus( vehicleid, panels, doors, lights, tires );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleModelInfo
 * Signature: (IILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleModelInfo
  (JNIEnv *env, jclass jcls, jint vehiclemodel, jint infotype, jobject vector3d)
{
	static jclass cls = env->GetObjectClass(vector3d);
	static jfieldID fidX = env->GetFieldID(cls, "x", "F");
	static jfieldID fidY = env->GetFieldID(cls, "y", "F");
	static jfieldID fidZ = env->GetFieldID(cls, "z", "F");

	float x, y, z;
	GetVehicleModelInfo(vehiclemodel, infotype, x, y, z);

	env->SetFloatField( vector3d, fidX, x );
	env->SetFloatField( vector3d, fidY, y );
	env->SetFloatField( vector3d, fidZ, z );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleVirtualWorld
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleVirtualWorld
  (JNIEnv *env, jclass jcls, jint vehicleid, jint worldid)
{
	SetVehicleVirtualWorld( vehicleid, worldid );
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleVirtualWorld
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleVirtualWorld
  (JNIEnv *env, jclass jcls, jint vehicleid)
{
	return GetVehicleVirtualWorld(vehicleid);
}
