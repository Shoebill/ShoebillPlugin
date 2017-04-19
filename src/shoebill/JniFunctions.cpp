/**
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

#include "JniFunctions.h"
#include "sampgdk.h"
#include "EncodingUtils.h"
#include "Shoebill.h"
#include "amx/amx.h"
#include "SimpleInlineHook.hpp"

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setServerCodepage
 * Signature: (I)V
 */

JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setServerCodepage
        (JNIEnv *, jclass, jint codepage) {
    Shoebill::GetInstance().setServerCodepage((unsigned int) codepage);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getServerCodepage
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getServerCodepage
        (JNIEnv *, jclass) {
    return (jint) Shoebill::GetInstance().getServerCodepage();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerCodepage
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerCodepage
        (JNIEnv *, jclass, jint playerid, jint codepage) {
    Shoebill::GetInstance().setPlayerCodepage(playerid, (unsigned int) codepage);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerCodepage
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerCodepage
        (JNIEnv *, jclass, jint playerid) {
    return (jint) Shoebill::GetInstance().getPlayerCodepage(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createObject
 * Signature: (IFFFFFFF)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createObject
        (JNIEnv *, jclass, jint modelid, jfloat x, jfloat y, jfloat z, jfloat rX, jfloat rY, jfloat rZ,
         jfloat drawDistance) {
    return CreateObject(modelid, x, y, z, rX, rY, rZ, drawDistance);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachObjectToVehicle
 * Signature: (IIFFFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachObjectToVehicle
        (JNIEnv *, jclass, jint objectid, jint vehicleid, jfloat x, jfloat y, jfloat z, jfloat rX, jfloat rY,
         jfloat rZ) {
    AttachObjectToVehicle(objectid, vehicleid, x, y, z, rX, rY, rZ);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachObjectToObject
 * Signature: (IIFFFFFFI)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachObjectToObject
        (JNIEnv *, jclass, jint objectid, jint attachtoid, jfloat offsetX, jfloat offsetY, jfloat offsetZ, jfloat rotX,
         jfloat rotY, jfloat rotZ, jint syncRotation) {
    AttachObjectToObject(objectid, attachtoid, offsetX, offsetY, offsetZ, rotX, rotY, rotZ, (bool) syncRotation);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachObjectToPlayer
 * Signature: (IIFFFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachObjectToPlayer
        (JNIEnv *, jclass, jint objectid, jint playerid,
         jfloat offsetX, jfloat offsetY, jfloat offsetZ, jfloat rX, jfloat rY, jfloat rZ) {
    AttachObjectToPlayer(objectid, playerid, offsetX, offsetY, offsetZ, rX, rY, rZ);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setObjectPos
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setObjectPos
        (JNIEnv *, jclass, jint objectid, jfloat x, jfloat y, jfloat z) {
    SetObjectPos(objectid, x, y, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getObjectPos
 * Signature: (ILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getObjectPos
        (JNIEnv *env, jclass, jint objectid, jobject vector3d) {
	assertNotNull(vector3d, "vector3d", __FUNCTION__);

    static auto cls = env->GetObjectClass(vector3d);
    static auto fidX = env->GetFieldID(cls, "x", "F");
    static auto fidY = env->GetFieldID(cls, "y", "F");
    static auto fidZ = env->GetFieldID(cls, "z", "F");

    float x, y, z;
    GetObjectPos(objectid, &x, &y, &z);

    env->SetFloatField(vector3d, fidX, x);
    env->SetFloatField(vector3d, fidY, y);
    env->SetFloatField(vector3d, fidZ, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setObjectRot
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setObjectRot
        (JNIEnv *, jclass, jint objectid, jfloat rotX, jfloat rotY, jfloat rotZ) {
    SetObjectRot(objectid, rotX, rotY, rotZ);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getObjectRot
 * Signature: (ILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getObjectRot
        (JNIEnv *env, jclass, jint objectid, jobject rotate) {
	assertNotNull(rotate, "rotate", __FUNCTION__);

    static auto cls = env->GetObjectClass(rotate);
    static auto fidX = env->GetFieldID(cls, "x", "F");
    static auto fidY = env->GetFieldID(cls, "y", "F");
    static auto fidZ = env->GetFieldID(cls, "z", "F");

    float rotX, rotY, rotZ;
    GetObjectRot(objectid, &rotX, &rotY, &rotZ);

    env->SetFloatField(rotate, fidX, rotX);
    env->SetFloatField(rotate, fidY, rotY);
    env->SetFloatField(rotate, fidZ, rotZ);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isValidObject
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isValidObject
        (JNIEnv *, jclass, jint objectid) {
    return (jboolean) IsValidObject(objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    destroyObject
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_destroyObject
        (JNIEnv *, jclass, jint objectid) {
    DestroyObject(objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    moveObject
 * Signature: (IFFFFFFF)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_moveObject
        (JNIEnv *, jclass, jint objectid, jfloat x, jfloat y, jfloat z, jfloat speed, jfloat rotX, jfloat rotY,
         jfloat rotZ) {
    return MoveObject(objectid, x, y, z, speed, rotX, rotY, rotZ);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    stopObject
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_stopObject
        (JNIEnv *, jclass, jint objectid) {
    StopObject(objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isObjectMoving
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isObjectMoving
        (JNIEnv *, jclass, jint objectid) {
    return (jboolean) IsObjectMoving(objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    editObject
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_editObject
        (JNIEnv *, jclass, jint playerid, jint objectid) {
    return (jboolean) EditObject(playerid, objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    editPlayerObject
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_editPlayerObject
        (JNIEnv *, jclass, jint playerid, jint objectid) {
    return (jboolean) EditPlayerObject(playerid, objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    selectObject
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_selectObject
        (JNIEnv *, jclass, jint playerid) {
    SelectObject(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    cancelEdit
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_cancelEdit
        (JNIEnv *, jclass, jint playerid) {
    CancelEdit(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createPlayerObject
 * Signature: (IIFFFFFFF)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createPlayerObject
        (JNIEnv *, jclass, jint playerid, jint modelid, jfloat x, jfloat y, jfloat z, jfloat rX, jfloat rY, jfloat rZ,
         jfloat drawDistance) {
    return CreatePlayerObject(playerid, modelid, x, y, z, rX, rY, rZ, drawDistance);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachPlayerObjectToVehicle
 * Signature: (IIIFFFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachPlayerObjectToVehicle
        (JNIEnv *, jclass, jint playerid, jint objectid, jint vehicleid, jfloat fOffsetX, jfloat fOffsetY,
         jfloat fOffsetZ, jfloat fRotX, jfloat fRotY, jfloat fRotZ) {
    AttachPlayerObjectToVehicle(playerid, objectid, vehicleid, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerObjectPos
 * Signature: (IIFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerObjectPos
        (JNIEnv *, jclass, jint playerid, jint objectid, jfloat x, jfloat y, jfloat z) {
    SetPlayerObjectPos(playerid, objectid, x, y, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerObjectPos
 * Signature: (IILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerObjectPos
        (JNIEnv *env, jclass, jint playerid, jint objectid, jobject vector3d) {
	assertNotNull(vector3d, "vector3d", __FUNCTION__);

    static auto cls = env->GetObjectClass(vector3d);
    static auto fidX = env->GetFieldID(cls, "x", "F");
    static auto fidY = env->GetFieldID(cls, "y", "F");
    static auto fidZ = env->GetFieldID(cls, "z", "F");

    float x, y, z;
    GetPlayerObjectPos(playerid, objectid, &x, &y, &z);

    env->SetFloatField(vector3d, fidX, x);
    env->SetFloatField(vector3d, fidY, y);
    env->SetFloatField(vector3d, fidZ, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerObjectRot
 * Signature: (IIFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerObjectRot
        (JNIEnv *, jclass, jint playerid, jint objectid, jfloat rotX, jfloat rotY, jfloat rotZ) {
    SetPlayerObjectRot(playerid, objectid, rotX, rotY, rotZ);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerObjectRot
 * Signature: (IILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerObjectRot
        (JNIEnv *env, jclass, jint playerid, jint objectid, jobject rotate) {
    assertNotNull(rotate, "rotate", __FUNCTION__);

    static auto cls = env->GetObjectClass(rotate);
    static auto fidX = env->GetFieldID(cls, "x", "F");
    static auto fidY = env->GetFieldID(cls, "y", "F");
    static auto fidZ = env->GetFieldID(cls, "z", "F");

    float rotX, rotY, rotZ;
    GetPlayerObjectRot(playerid, objectid, &rotX, &rotY, &rotZ);

    env->SetFloatField(rotate, fidX, rotX);
    env->SetFloatField(rotate, fidY, rotY);
    env->SetFloatField(rotate, fidZ, rotZ);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isValidPlayerObject
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isValidPlayerObject
        (JNIEnv *, jclass, jint playerid, jint objectid) {
    return (jboolean) IsValidPlayerObject(playerid, objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    destroyPlayerObject
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_destroyPlayerObject
        (JNIEnv *, jclass, jint playerid, jint objectid) {
    DestroyPlayerObject(playerid, objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    movePlayerObject
 * Signature: (IIFFFFFFF)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_movePlayerObject
        (JNIEnv *, jclass, jint playerid, jint objectid, jfloat x, jfloat y, jfloat z, jfloat speed, jfloat rotX,
         jfloat rotY, jfloat rotZ) {
    return MovePlayerObject(playerid, objectid, x, y, z, speed, rotX, rotY, rotZ);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    stopPlayerObject
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_stopPlayerObject
        (JNIEnv *, jclass, jint playerid, jint objectid) {
    StopPlayerObject(playerid, objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerObjectMoving
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerObjectMoving
        (JNIEnv *, jclass, jint playerid, jint objectid) {
    return (jboolean) IsPlayerObjectMoving(playerid, objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachPlayerObjectToPlayer
 * Signature: (IIIFFFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachPlayerObjectToPlayer
        (JNIEnv *, jclass, jint playerid, jint objectid, jint attachplayerid,
         jfloat offsetX, jfloat offsetY, jfloat offsetZ, jfloat rX, jfloat rY, jfloat rZ) {
    AttachPlayerObjectToPlayer(playerid, objectid, attachplayerid, offsetX, offsetY, offsetZ, rX, rY, rZ);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setObjectMaterial
 * Signature: (IIILjava/lang/String;Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setObjectMaterial
        (JNIEnv *env, jclass, jint objectid, jint materialindex, jint modelid, jstring txdname, jstring texturename,
         jint materialcolor) {
    assertNotNull(txdname, "txdname", __FUNCTION__);assertNotNull(texturename, "texturename", __FUNCTION__);

	auto codepage = Shoebill::GetInstance().getServerCodepage();

	auto str_txdname = wcs2mbs(env, codepage, txdname, 1024);
	auto str_texturename = wcs2mbs(env, codepage, texturename, 1024);

    SetObjectMaterial(objectid, materialindex, modelid, str_txdname, str_texturename, materialcolor);

    delete[] str_texturename;
    delete[] str_txdname;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerObjectMaterial
 * Signature: (IIIILjava/lang/String;Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerObjectMaterial
        (JNIEnv *env, jclass, jint playerid, jint objectid, jint materialindex, jint modelid, jstring txdname,
         jstring texturename, jint materialcolor) {
    assertNotNull(txdname, "txdname", __FUNCTION__);assertNotNull(texturename, "texturename", __FUNCTION__);

	auto codepage = Shoebill::GetInstance().getServerCodepage();

	auto str_txdname = wcs2mbs(env, codepage, txdname, 1024);
    auto str_texturename = wcs2mbs(env, codepage, texturename, 1024);

    SetPlayerObjectMaterial(playerid, objectid, materialindex, modelid, str_txdname, str_texturename, materialcolor);

    delete[] str_texturename;
    delete[] str_txdname;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setObjectMaterialText
 * Signature: (ILjava/lang/String;IILjava/lang/String;IIIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setObjectMaterialText
        (JNIEnv *env, jclass, jint objectid, jstring text, jint materialindex, jint materialsize, jstring fontface,
         jint fontsize, jint bold, jint fontcolor, jint backcolor, jint textalignment) {
    assertNotNull(text, "text", __FUNCTION__);assertNotNull(fontface, "fontface", __FUNCTION__);

	auto codepage = Shoebill::GetInstance().getServerCodepage();

	auto str_text = wcs2mbs(env, codepage, text, 1024);
	auto str_fontface = wcs2mbs(env, codepage, fontface, 128);

    SetObjectMaterialText(objectid, str_text, materialindex, materialsize, str_fontface, fontsize, (bool) bold,
                          fontcolor,
                          backcolor, textalignment);

    delete[] str_text;
    delete[] str_fontface;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerObjectMaterialText
 * Signature: (IILjava/lang/String;IILjava/lang/String;IIIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerObjectMaterialText
        (JNIEnv *env, jclass, jint playerid, jint objectid, jstring text, jint materialindex, jint materialsize,
         jstring fontface, jint fontsize, jint bold, jint fontcolor, jint backcolor, jint textalignment) {
    assertNotNull(text, "text", __FUNCTION__);assertNotNull(fontface, "fontface", __FUNCTION__);

	auto codepage = Shoebill::GetInstance().getPlayerCodepage(playerid);
	auto str_text = wcs2mbs(env, codepage, text, 1024);
	auto str_fontface = wcs2mbs(env, codepage, fontface, 128);

    SetPlayerObjectMaterialText(playerid, objectid, str_text, materialindex, materialsize, str_fontface, fontsize,
                                (bool) bold, fontcolor, backcolor, textalignment);

    delete[] str_text;
    delete[] str_fontface;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setSpawnInfo
 * Signature: (IIIFFFFIIIIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setSpawnInfo
        (JNIEnv *, jclass, jint playerid, jint teamid, jint skinid, jfloat x, jfloat y, jfloat z,
         jfloat rotation, jint weapon1, jint ammo1, jint weapon2, jint ammo2, jint weapon3, jint ammo3) {
    SetSpawnInfo(playerid, teamid, skinid, x, y, z, rotation, weapon1, ammo1, weapon2, ammo2, weapon3, ammo3);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    spawnPlayer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_spawnPlayer
        (JNIEnv *, jclass, jint playerid) {
    SpawnPlayer(playerid);
}

/*
* Class:     net_gtaun_shoebill_SampNativeFunction
* Method:    getAnimationName
* Signature: (I)[Ljava/lang/String;
*/
JNIEXPORT jobjectArray JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getAnimationName(JNIEnv *env, jclass,
                                                                                           jint animationIndex) {
    char animLib[32] = "None", animName[32] = "None";
    GetAnimationName(animationIndex, animLib, 32, animName, 32);
	auto objectArray = static_cast<jobjectArray>(env->NewObjectArray(2, env->FindClass("java/lang/String"),
                                                                             env->NewStringUTF("")));
    env->SetObjectArrayElement(objectArray, 0, env->NewStringUTF(animLib));
    env->SetObjectArrayElement(objectArray, 1, env->NewStringUTF(animName));
    return objectArray;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerPos
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerPos
        (JNIEnv *, jclass, jint playerid, jfloat x, jfloat y, jfloat z) {
    SetPlayerPos(playerid, x, y, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerPosFindZ
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerPosFindZ
        (JNIEnv *, jclass, jint playerid, jfloat x, jfloat y, jfloat z) {
    SetPlayerPosFindZ(playerid, x, y, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerPos
 * Signature: (ILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerPos
        (JNIEnv *env, jclass, jint playerid, jobject vector3d) {
    assertNotNull(vector3d, "vector3d", __FUNCTION__);

    static auto cls = env->GetObjectClass(vector3d);
    static auto fidX = env->GetFieldID(cls, "x", "F");
    static auto fidY = env->GetFieldID(cls, "y", "F");
    static auto fidZ = env->GetFieldID(cls, "z", "F");

    float x, y, z;
    GetPlayerPos(playerid, &x, &y, &z);

    env->SetFloatField(vector3d, fidX, x);
    env->SetFloatField(vector3d, fidY, y);
    env->SetFloatField(vector3d, fidZ, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerFacingAngle
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerFacingAngle
        (JNIEnv *, jclass, jint playerid, jfloat angle) {
    SetPlayerFacingAngle(playerid, angle);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerFacingAngle
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerFacingAngle
        (JNIEnv *, jclass, jint playerid) {
    float angle;
    GetPlayerFacingAngle(playerid, &angle);

    return angle;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerInRangeOfPoint
 * Signature: (IFFFF)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerInRangeOfPoint
        (JNIEnv *, jclass, jint playerid, jfloat range, jfloat x, jfloat y, jfloat z) {
    return (jboolean) IsPlayerInRangeOfPoint(playerid, range, x, y, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerStreamedIn
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerStreamedIn
        (JNIEnv *, jclass, jint playerid, jint forplayerid) {
    return (jboolean) IsPlayerStreamedIn(playerid, forplayerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerInterior
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerInterior
        (JNIEnv *, jclass, jint playerid, jint interiorid) {
    SetPlayerInterior(playerid, interiorid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerInterior
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerInterior
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerInterior(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerHealth
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerHealth
        (JNIEnv *, jclass, jint playerid, jfloat health) {
    SetPlayerHealth(playerid, health);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerHealth
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerHealth
        (JNIEnv *, jclass, jint playerid) {
    float health;
    GetPlayerHealth(playerid, &health);

    return health;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerArmour
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerArmour
        (JNIEnv *, jclass, jint playerid, jfloat armour) {
    SetPlayerArmour(playerid, armour);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerArmour
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerArmour
        (JNIEnv *, jclass, jint playerid) {
    float armour;
    GetPlayerArmour(playerid, &armour);

    return armour;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerAmmo
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerAmmo
        (JNIEnv *, jclass, jint playerid, jint weaponslot, jint ammo) {
    SetPlayerAmmo(playerid, weaponslot, ammo);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerAmmo
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerAmmo
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerAmmo(playerid);;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerWeaponState
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerWeaponState
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerWeaponState(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerTargetPlayer
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerTargetPlayer
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerTargetPlayer(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerTeam
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerTeam
        (JNIEnv *, jclass, jint playerid, jint teamid) {
    SetPlayerTeam(playerid, teamid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerTeam
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerTeam
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerTeam(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerScore
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerScore
        (JNIEnv *, jclass, jint playerid, jint score) {
    SetPlayerScore(playerid, score);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerScore
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerScore
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerScore(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerDrunkLevel
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerDrunkLevel
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerDrunkLevel(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerDrunkLevel
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerDrunkLevel
        (JNIEnv *, jclass, jint playerid, jint level) {
    SetPlayerDrunkLevel(playerid, level);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerColor
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerColor
        (JNIEnv *, jclass, jint playerid, jint color) {
    SetPlayerColor(playerid, color);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerColor
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerColor
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerColor(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerSkin
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerSkin
        (JNIEnv *, jclass, jint playerid, jint skinid) {
    SetPlayerSkin(playerid, skinid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerSkin
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerSkin
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerSkin(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    givePlayerWeapon
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_givePlayerWeapon
        (JNIEnv *, jclass, jint playerid, jint weaponid, jint ammo) {
    GivePlayerWeapon(playerid, weaponid, ammo);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    resetPlayerWeapons
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_resetPlayerWeapons
        (JNIEnv *, jclass, jint playerid) {
    ResetPlayerWeapons(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerArmedWeapon
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerArmedWeapon
        (JNIEnv *, jclass, jint playerid, jint weaponid) {
    SetPlayerArmedWeapon(playerid, weaponid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerWeaponData
 * Signature: (IILnet/gtaun/shoebill/data/WeaponData;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerWeaponData
        (JNIEnv *env, jclass, jint playerid, jint slot, jobject weapondata) {
	assertNotNull(weapondata, "weapondata", __FUNCTION__);

    static auto cls = env->GetObjectClass(weapondata);
    static auto fidAmmo = env->GetFieldID(cls, "ammo", "I");

    static auto setWeaponMethodId = env->GetMethodID(cls, "setModel", "(I)V");

    int weaponid, ammo;
    GetPlayerWeaponData(playerid, slot, &weaponid, &ammo);

    env->CallVoidMethod(weapondata, setWeaponMethodId, weaponid);
    env->SetIntField(weapondata, fidAmmo, ammo);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    givePlayerMoney
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_givePlayerMoney
        (JNIEnv *, jclass, jint playerid, jint money) {
    GivePlayerMoney(playerid, money);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    resetPlayerMoney
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_resetPlayerMoney
        (JNIEnv *, jclass, jint playerid) {
    ResetPlayerMoney(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerName
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerName
        (JNIEnv *env, jclass, jint playerid, jstring name) {
	assertNotNullReturn(name, "name", __FUNCTION__, 0);

	auto str = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), name, MAX_PLAYER_NAME);

	auto ret = SetPlayerName(playerid, str);
    
    delete[] str;
    return ret;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerMoney
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerMoney
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerMoney(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerState
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerState
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerState(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerIp
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerIp
        (JNIEnv *env, jclass, jint playerid) {
    char ip[20];
    GetPlayerIp(playerid, ip, sizeof(ip));

    return env->NewStringUTF(ip);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerPing
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerPing
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerPing(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerWeapon
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerWeapon
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerWeapon(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerKeys
 * Signature: (ILnet/gtaun/shoebill/object/impl/PlayerKeyStateImpl;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerKeys
        (JNIEnv *env, jclass, jint playerid, jobject keystate) {
	assertNotNull(keystate, "keystate", __FUNCTION__);

    static auto cls = env->GetObjectClass(keystate);
    static auto fidKeys = env->GetFieldID(cls, "keys", "I");
    static auto fidUpdown = env->GetFieldID(cls, "updownValue", "I");
    static auto fidLeftright = env->GetFieldID(cls, "leftrightValue", "I");

    int keys, updown, leftright;
    GetPlayerKeys(playerid, &keys, &updown, &leftright);

    env->SetIntField(keystate, fidKeys, keys);
    env->SetIntField(keystate, fidUpdown, updown);
    env->SetIntField(keystate, fidLeftright, leftright);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerName
        (JNIEnv *env, jclass, jint playerid) {
    char name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    return mbs2wcs(env, Shoebill::GetInstance().getServerCodepage(), name, MAX_PLAYER_NAME);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerTime
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerTime
        (JNIEnv *, jclass, jint playerid, jint hour, jint minute) {
    SetPlayerTime(playerid, hour, minute);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerTime
 * Signature: (ILjava/sql/Time;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerTime
        (JNIEnv *env, jclass, jint playerid, jobject time) {
	assertNotNull(time, "time", __FUNCTION__);

    static auto cls = env->GetObjectClass(time);
    static auto fidHour = env->GetFieldID(cls, "hour", "I");
    static auto fidMinute = env->GetFieldID(cls, "minute", "I");

    int hour, minute;
    GetPlayerTime(playerid, &hour, &minute);

    env->SetIntField(time, fidHour, hour);
    env->SetIntField(time, fidMinute, minute);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    togglePlayerClock
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_togglePlayerClock
        (JNIEnv *, jclass, jint playerid, jboolean toggle) {
    TogglePlayerClock(playerid, toggle);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerWeather
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerWeather
        (JNIEnv *, jclass, jint playerid, jint weather) {
    SetPlayerWeather(playerid, weather);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    forceClassSelection
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_forceClassSelection
        (JNIEnv *, jclass, jint playerid) {
    ForceClassSelection(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerWantedLevel
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerWantedLevel
        (JNIEnv *, jclass, jint playerid, jint level) {
    SetPlayerWantedLevel(playerid, level);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerWantedLevel
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerWantedLevel
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerWantedLevel(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerFightingStyle
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerFightingStyle
        (JNIEnv *, jclass, jint playerid, jint style) {
    SetPlayerFightingStyle(playerid, style);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerFightingStyle
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerFightingStyle
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerFightingStyle(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerVelocity
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerVelocity
        (JNIEnv *, jclass, jint playerid, jfloat x, jfloat y, jfloat z) {
    SetPlayerVelocity(playerid, x, y, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerVelocity
 * Signature: (ILnet/gtaun/shoebill/data/Velocity;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerVelocity
        (JNIEnv *env, jclass, jint playerid, jobject velocity) {
	assertNotNull(velocity, "velocity", __FUNCTION__);

    static auto cls = env->GetObjectClass(velocity);
    static auto fidX = env->GetFieldID(cls, "x", "F");
    static auto fidY = env->GetFieldID(cls, "y", "F");
    static auto fidZ = env->GetFieldID(cls, "z", "F");

    float x, y, z;
    GetPlayerVelocity(playerid, &x, &y, &z);

    env->SetFloatField(velocity, fidX, x);
    env->SetFloatField(velocity, fidY, y);
    env->SetFloatField(velocity, fidZ, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playCrimeReportForPlayer
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playCrimeReportForPlayer
        (JNIEnv *, jclass, jint playerid, jint suspectid, jint crime) {
    PlayCrimeReportForPlayer(playerid, suspectid, crime);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playAudioStreamForPlayer
 * Signature: (ILjava/lang/String;FFFFI)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playAudioStreamForPlayer
        (JNIEnv *env, jclass, jint player, jstring url, jfloat posX, jfloat posY, jfloat posZ, jfloat distance,
         jint usepos) {
    assertNotNull(url, "url", __FUNCTION__);
	auto str = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(player), url, 1024);
    PlayAudioStreamForPlayer(player, str, posX, posY, posZ, distance, (bool) usepos);
    delete[] str;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    stopAudioStreamForPlayer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_stopAudioStreamForPlayer
        (JNIEnv *, jclass, jint playerid) {
    StopAudioStreamForPlayer(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerShopName
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerShopName
        (JNIEnv *env, jclass, jint playerid, jstring name) {
	assertNotNull(name, "name", __FUNCTION__);
	auto str = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(playerid), name, 64);
    SetPlayerShopName(playerid, str);
    delete[] str;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerSkillLevel
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerSkillLevel
        (JNIEnv *, jclass, jint playerid, jint skill, jint level) {
    SetPlayerSkillLevel(playerid, skill, level);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerSurfingVehicleID
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerSurfingVehicleID
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerSurfingVehicleID(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerSurfingObjectID
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerSurfingObjectID
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerSurfingObjectID(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    removeBuildingForPlayer
 * Signature: (IIFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_removeBuildingForPlayer
        (JNIEnv *, jclass, jint player, jint modelid, jfloat x, jfloat y, jfloat z, jfloat radius) {
    RemoveBuildingForPlayer(player, modelid, x, y, z, radius);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    getPlayerLastShotVectors
 * Signature: (ILjava/lang/Object;Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerLastShotVectors
        (JNIEnv *env, jclass, jint playerid, jobject origin, jobject hitpos) {
	assertNotNull(origin, "origin", __FUNCTION__); assertNotNull(hitpos, "hitpos", __FUNCTION__);

    static auto cls = env->GetObjectClass(origin);
    static auto fidX = env->GetFieldID(cls, "x", "F");
    static auto fidY = env->GetFieldID(cls, "y", "F");
    static auto fidZ = env->GetFieldID(cls, "z", "F");

    float x, y, z, hx, hy, hz;
    GetPlayerLastShotVectors(playerid, &x, &y, &z, &hx, &hy, &hz);

    env->SetFloatField(origin, fidX, x);
    env->SetFloatField(origin, fidY, y);
    env->SetFloatField(origin, fidZ, z);

    env->SetFloatField(hitpos, fidX, hx);
    env->SetFloatField(hitpos, fidY, hy);
    env->SetFloatField(hitpos, fidZ, hz);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerAttachedObject
 * Signature: (IIIIFFFFFFFFF)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerAttachedObject
        (JNIEnv *, jclass, jint playerid, jint index, jint modelid, jint bone, jfloat offsetX, jfloat offsetY,
         jfloat offsetZ, jfloat rotX, jfloat rotY, jfloat rotZ, jfloat scaleX, jfloat scaleY, jfloat scaleZ,
         jint materialcolor1, jint materialcolor2) {
    return (jboolean) SetPlayerAttachedObject(playerid, index, modelid, bone,
                                              offsetX, offsetY, offsetZ, rotX, rotY, rotZ, scaleX, scaleY, scaleZ,
                                              materialcolor1, materialcolor2);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    removePlayerAttachedObject
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_removePlayerAttachedObject
        (JNIEnv *, jclass, jint playerid, jint index) {
    return (jboolean) RemovePlayerAttachedObject(playerid, index);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerAttachedObjectSlotUsed
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerAttachedObjectSlotUsed
        (JNIEnv *, jclass, jint playerid, jint index) {
    return (jboolean) IsPlayerAttachedObjectSlotUsed(playerid, index);
}
/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    editAttachedObject
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_editAttachedObject
        (JNIEnv *, jclass, jint playerid, jint index) {
    return (jboolean) EditAttachedObject(playerid, index);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createPlayerTextDraw
 * Signature: (IFFLjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createPlayerTextDraw
        (JNIEnv *env, jclass, jint playerid, jfloat x, jfloat y, jstring text) {
	assertNotNullReturn(text, "text", __FUNCTION__, 0);
	auto str = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(playerid), text, 2048);
	auto ret = CreatePlayerTextDraw(playerid, x, y, str);
    delete[] str;
    return ret;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawDestroy
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawDestroy
        (JNIEnv *, jclass, jint playerid, jint textid) {
    PlayerTextDrawDestroy(playerid, textid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawLetterSize
 * Signature: (IIFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawLetterSize
        (JNIEnv *, jclass, jint playerid, jint textid, jfloat x, jfloat y) {
    PlayerTextDrawLetterSize(playerid, textid, x, y);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawTextSize
 * Signature: (IIFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawTextSize
        (JNIEnv *, jclass, jint playerid, jint textid, jfloat x, jfloat y) {
    PlayerTextDrawTextSize(playerid, textid, x, y);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawAlignment
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawAlignment
        (JNIEnv *, jclass, jint playerid, jint textid, jint alignment) {
    PlayerTextDrawAlignment(playerid, textid, alignment);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawColor
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawColor
        (JNIEnv *, jclass, jint playerid, jint textid, jint color) {
    PlayerTextDrawColor(playerid, textid, color);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawUseBox
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawUseBox
        (JNIEnv *, jclass, jint playerid, jint textid, jint use) {
    PlayerTextDrawUseBox(playerid, textid, (bool) use);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawBoxColor
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawBoxColor
        (JNIEnv *, jclass, jint playerid, jint textid, jint color) {
    PlayerTextDrawBoxColor(playerid, textid, color);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawSetShadow
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetShadow
        (JNIEnv *, jclass, jint playerid, jint textid, jint size) {
    PlayerTextDrawSetShadow(playerid, textid, size);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawSetOutline
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetOutline
        (JNIEnv *, jclass, jint playerid, jint textid, jint size) {
    PlayerTextDrawSetOutline(playerid, textid, size);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawBackgroundColor
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawBackgroundColor
        (JNIEnv *, jclass, jint playerid, jint textid, jint color) {
    PlayerTextDrawBackgroundColor(playerid, textid, color);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawFont
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawFont
        (JNIEnv *, jclass, jint playerid, jint textid, jint font) {
    PlayerTextDrawFont(playerid, textid, font);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawSetProportional
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetProportional
        (JNIEnv *, jclass, jint playerid, jint textid, jint set) {
    PlayerTextDrawSetProportional(playerid, textid, (bool) set);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawSetSelectable
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetSelectable
        (JNIEnv *, jclass, jint playerid, jint textid, jint set) {
    PlayerTextDrawSetSelectable(playerid, textid, (bool) set);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawShow
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawShow
        (JNIEnv *, jclass, jint playerid, jint textid) {
    PlayerTextDrawShow(playerid, textid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawHide
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawHide
        (JNIEnv *, jclass, jint playerid, jint textid) {
    PlayerTextDrawHide(playerid, textid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerTextDrawSetString
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetString
        (JNIEnv *env, jclass, jint playerid, jint textid, jstring string) {
	assertNotNull(string, "string", __FUNCTION__);
	auto str = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(playerid), string, 2048);
    PlayerTextDrawSetString(playerid, textid, str);
    delete[] str;
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    playerTextDrawSetPreviewModel
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetPreviewModel
        (JNIEnv *, jclass, jint playerid, jint textId, jint modelindex) {
    PlayerTextDrawSetPreviewModel(playerid, textId, modelindex);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    playerTextDrawSetPreviewRot
 * Signature: (IIFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetPreviewRot
        (JNIEnv *, jclass, jint playerid, jint textId, jfloat fRotX, jfloat fRotY, jfloat fRotZ, jfloat fZoom) {
    PlayerTextDrawSetPreviewRot(playerid, textId, fRotX, fRotY, fRotZ, fZoom);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    playerTextDrawSetPreviewVehCol
 * Signature: (IIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerTextDrawSetPreviewVehCol
        (JNIEnv *, jclass, jint playerid, jint textId, jint color1, jint color2) {
    PlayerTextDrawSetPreviewVehCol(playerid, textId, color1, color2);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerChatBubble
 * Signature: (ILjava/lang/String;IFI)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerChatBubble
        (JNIEnv *env, jclass, jint playerid, jstring text, jint color, jfloat drawdistance, jint expiretime) {
	assertNotNull(text, "text", __FUNCTION__);
	auto str = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(playerid), text, 256);
    SetPlayerChatBubble(playerid, str, color, drawdistance, expiretime);
    delete[] str;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    putPlayerInVehicle
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_putPlayerInVehicle
        (JNIEnv *, jclass, jint playerid, jint vehicleid, jint seatid) {
    PutPlayerInVehicle(playerid, vehicleid, seatid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerVehicleID
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerVehicleID
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerVehicleID(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerVehicleSeat
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerVehicleSeat
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerVehicleSeat(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    removePlayerFromVehicle
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_removePlayerFromVehicle
        (JNIEnv *, jclass, jint playerid) {
    RemovePlayerFromVehicle(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    togglePlayerControllable
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_togglePlayerControllable
        (JNIEnv *, jclass, jint playerid, jboolean toggle) {
    TogglePlayerControllable(playerid, toggle);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerPlaySound
 * Signature: (IIFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerPlaySound
        (JNIEnv *, jclass, jint playerid, jint soundid, jfloat x, jfloat y, jfloat z) {
    PlayerPlaySound(playerid, soundid, x, y, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    applyAnimation
 * Signature: (ILjava/lang/String;Ljava/lang/String;FIIIIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_applyAnimation
        (JNIEnv *env, jclass, jint playerid, jstring animlib, jstring animname,
         jfloat delta, jint loop, jint lockX, jint lockY, jint freeze, jint time, jint forcesync) {

	assertNotNull(animlib, "animlib", __FUNCTION__); assertNotNull(animlib, "animlib", __FUNCTION__);

    auto str_animlib = env->GetStringUTFChars(animlib, nullptr);
    auto str_animname = env->GetStringUTFChars(animname, nullptr);

    ApplyAnimation(playerid, str_animlib, str_animname, delta, (bool) loop, (bool) lockX, (bool) lockY, (bool) freeze,
                   time, (bool) forcesync);

    env->ReleaseStringUTFChars(animlib, str_animlib);
    env->ReleaseStringUTFChars(animname, str_animname);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    clearAnimations
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_clearAnimations
        (JNIEnv *, jclass, jint playerid, jint forcesync) {
    ClearAnimations(playerid, (bool) forcesync);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerAnimationIndex
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerAnimationIndex
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerAnimationIndex(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerSpecialAction
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerSpecialAction
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerSpecialAction(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerSpecialAction
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerSpecialAction
        (JNIEnv *, jclass, jint playerid, jint actionid) {
    SetPlayerSpecialAction(playerid, actionid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerCheckpoint
 * Signature: (IFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerCheckpoint
        (JNIEnv *, jclass, jint playerid, jfloat x, jfloat y, jfloat z, jfloat size) {
    SetPlayerCheckpoint(playerid, x, y, z, size);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    disablePlayerCheckpoint
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_disablePlayerCheckpoint
        (JNIEnv *, jclass, jint playerid) {
    DisablePlayerCheckpoint(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerRaceCheckpoint
 * Signature: (IIFFFFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerRaceCheckpoint
        (JNIEnv *, jclass, jint playerid, jint type, jfloat x, jfloat y, jfloat z,
         jfloat nextX, jfloat nextY, jfloat nextZ, jfloat size) {
    SetPlayerRaceCheckpoint(playerid, type, x, y, z, nextX, nextY, nextZ, size);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    disablePlayerRaceCheckpoint
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_disablePlayerRaceCheckpoint
        (JNIEnv *, jclass, jint playerid) {
    DisablePlayerRaceCheckpoint(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerWorldBounds
 * Signature: (IFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerWorldBounds
        (JNIEnv *, jclass, jint playerid, jfloat x_max, jfloat x_min, jfloat y_max, jfloat y_min) {
    SetPlayerWorldBounds(playerid, x_max, x_min, y_max, y_min);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerMarkerForPlayer
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerMarkerForPlayer
        (JNIEnv *, jclass, jint playerid, jint showplayerid, jint color) {
    SetPlayerMarkerForPlayer(playerid, showplayerid, color);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    showPlayerNameTagForPlayer
 * Signature: (IIZ)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_showPlayerNameTagForPlayer
        (JNIEnv *, jclass, jint playerid, jint showplayerid, jboolean show) {
    ShowPlayerNameTagForPlayer(playerid, showplayerid, show);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerMapIcon
 * Signature: (IIFFFIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerMapIcon
        (JNIEnv *, jclass, jint playerid, jint iconid, jfloat x, jfloat y, jfloat z, jint markertype, jint color,
         jint style) {
    SetPlayerMapIcon(playerid, iconid, x, y, z, markertype, color, style);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    removePlayerMapIcon
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_removePlayerMapIcon
        (JNIEnv *, jclass, jint playerid, jint iconid) {
    RemovePlayerMapIcon(playerid, iconid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerCameraPos
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerCameraPos
        (JNIEnv *, jclass, jint playerid, jfloat x, jfloat y, jfloat z) {
    SetPlayerCameraPos(playerid, x, y, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerCameraLookAt
 * Signature: (IFFFI)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerCameraLookAt
        (JNIEnv *, jclass, jint playerid, jfloat x, jfloat y, jfloat z, jint cut) {
    SetPlayerCameraLookAt(playerid, x, y, z, cut);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setCameraBehindPlayer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setCameraBehindPlayer
        (JNIEnv *, jclass, jint playerid) {
    SetCameraBehindPlayer(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerCameraPos
 * Signature: (ILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerCameraPos
        (JNIEnv *env, jclass, jint playerid, jobject vector3d) {
	assertNotNull(vector3d, "vector3d", __FUNCTION__);

    static auto cls = env->GetObjectClass(vector3d);
    static auto fidX = env->GetFieldID(cls, "x", "F");
    static auto fidY = env->GetFieldID(cls, "y", "F");
    static auto fidZ = env->GetFieldID(cls, "z", "F");

    float x, y, z;
    GetPlayerCameraPos(playerid, &x, &y, &z);

    env->SetFloatField(vector3d, fidX, x);
    env->SetFloatField(vector3d, fidY, y);
    env->SetFloatField(vector3d, fidZ, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerCameraFrontVector
 * Signature: (ILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerCameraFrontVector
        (JNIEnv *env, jclass, jint playerid, jobject vector3d) {
	assertNotNull(vector3d, "vector3d", __FUNCTION__);

    static auto cls = env->GetObjectClass(vector3d);
    static auto fidX = env->GetFieldID(cls, "x", "F");
    static auto fidY = env->GetFieldID(cls, "y", "F");
    static auto fidZ = env->GetFieldID(cls, "z", "F");

    float x, y, z;
    GetPlayerCameraFrontVector(playerid, &x, &y, &z);

    env->SetFloatField(vector3d, fidX, x);
    env->SetFloatField(vector3d, fidY, y);
    env->SetFloatField(vector3d, fidZ, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerCameraMode
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerCameraMode
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerCameraMode(playerid);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    getPlayerCameraAspectRatio
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerCameraAspectRatio
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerCameraAspectRatio(playerid);
}
/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    getPlayerCameraZoom
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerCameraZoom
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerCameraZoom(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachCameraToObject
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachCameraToObject
        (JNIEnv *, jclass, jint playerid, jint objectid) {
    AttachCameraToObject(playerid, objectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachCameraToPlayerObject
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachCameraToPlayerObject
        (JNIEnv *, jclass, jint playerid, jint playerobjectid) {
    AttachCameraToPlayerObject(playerid, playerobjectid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    interpolateCameraPos
 * Signature: (IFFFFFFII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_interpolateCameraPos
        (JNIEnv *, jclass, jint playerid, jfloat FromX, jfloat FromY, jfloat FromZ, jfloat ToX, jfloat ToY, jfloat ToZ,
         jint time, jint cut) {
    InterpolateCameraPos(playerid, FromX, FromY, FromZ, ToX, ToY, ToZ, time, cut);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    interpolateCameraLookAt
 * Signature: (IFFFFFFII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_interpolateCameraLookAt
        (JNIEnv *, jclass, jint playerid, jfloat FromX, jfloat FromY, jfloat FromZ, jfloat ToX, jfloat ToY, jfloat ToZ,
         jint time, jint cut) {
    InterpolateCameraLookAt(playerid, FromX, FromY, FromZ, ToX, ToY, ToZ, time, cut);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerConnected
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerConnected
        (JNIEnv *, jclass, jint playerid) {
    return (jboolean) IsPlayerConnected(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerInVehicle
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerInVehicle
        (JNIEnv *, jclass, jint playerid, jint vehicleid) {
    return (jboolean) IsPlayerInVehicle(playerid, vehicleid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerInAnyVehicle
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerInAnyVehicle
        (JNIEnv *, jclass, jint playerid) {
    return (jboolean) IsPlayerInAnyVehicle(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerInCheckpoint
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerInCheckpoint
        (JNIEnv *, jclass, jint playerid) {
    return (jboolean) IsPlayerInCheckpoint(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerInRaceCheckpoint
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerInRaceCheckpoint
        (JNIEnv *, jclass, jint playerid) {
    return (jboolean) IsPlayerInRaceCheckpoint(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setPlayerVirtualWorld
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerVirtualWorld
        (JNIEnv *, jclass, jint playerid, jint worldid) {
    SetPlayerVirtualWorld(playerid, worldid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerVirtualWorld
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerVirtualWorld
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerVirtualWorld(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    enableStuntBonusForPlayer
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_enableStuntBonusForPlayer
        (JNIEnv *, jclass, jint playerid, jint enabled) {
    EnableStuntBonusForPlayer(playerid, (bool) enabled);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    enableStuntBonusForAll
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_enableStuntBonusForAll
        (JNIEnv *, jclass, jboolean enabled) {
    EnableStuntBonusForAll(enabled);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    togglePlayerSpectating
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_togglePlayerSpectating
        (JNIEnv *, jclass, jint playerid, jboolean toggle) {
    TogglePlayerSpectating(playerid, toggle);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerSpectatePlayer
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerSpectatePlayer
        (JNIEnv *, jclass, jint playerid, jint targetplayerid, jint mode) {
    PlayerSpectatePlayer(playerid, targetplayerid, mode);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    playerSpectateVehicle
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_playerSpectateVehicle
        (JNIEnv *, jclass, jint playerid, jint targetvehicleid, jint mode) {
    PlayerSpectateVehicle(playerid, targetvehicleid, mode);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    startRecordingPlayerData
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_startRecordingPlayerData
        (JNIEnv *env, jclass, jint playerid, jint recordtype, jstring recordname) {
	assertNotNull(recordname, "recordname", __FUNCTION__);
	auto str_recordname = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), recordname, 64);

    StartRecordingPlayerData(playerid, recordtype, str_recordname);

    delete[] str_recordname;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    stopRecordingPlayerData
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_stopRecordingPlayerData
        (JNIEnv *, jclass, jint playerid) {
    StopRecordingPlayerData(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    selectTextDraw
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_selectTextDraw
        (JNIEnv *, jclass, jint playerid, jint hovercolor) {
    SelectTextDraw(playerid, hovercolor);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    cancelSelectTextDraw
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_cancelSelectTextDraw
        (JNIEnv *, jclass, jint playerid) {
    CancelSelectTextDraw(playerid);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    createExplosionForPlayer
 * Signature: (IFFFIF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createExplosionForPlayer
        (JNIEnv *, jclass, jint playerid, jfloat x, jfloat y, jfloat z, jint type, jfloat radius) {
    CreateExplosionForPlayer(playerid, x, y, z, type, radius);
}


JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getServerVarNameAtIndex
        (JNIEnv *env, jclass, jint index) {
    char varname[64];
	auto len = 0;
    GetSVarNameAtIndex(index, varname, len);

    return mbs2wcs(env, Shoebill::GetInstance().getServerCodepage(), varname, len);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    sendClientMessage
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_sendClientMessage
        (JNIEnv *env, jclass, jint playerid, jint color, jstring message) {
	assertNotNull(message, "message", __FUNCTION__);
	auto msg = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(playerid), message, 144);
    SendClientMessage(playerid, color, msg);
    delete[] msg;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    sendClientMessageToAll
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_sendClientMessageToAll
        (JNIEnv *env, jclass, jint color, jstring message) {
	assertNotNull(message, "message", __FUNCTION__);
	auto msg = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), message, 144);
    SendClientMessageToAll(color, msg);
    delete[] msg;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    sendPlayerMessageToPlayer
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_sendPlayerMessageToPlayer
        (JNIEnv *env, jclass, jint playerid, jint senderid, jstring message) {
	assertNotNull(message, "message", __FUNCTION__);
	auto msg = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(senderid), message, 144);
    SendPlayerMessageToPlayer(playerid, senderid, msg);
    delete[] msg;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    sendPlayerMessageToAll
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_sendPlayerMessageToAll
        (JNIEnv *env, jclass, jint senderid, jstring message) {
	assertNotNull(message, "message", __FUNCTION__);
	auto msg = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), message, 144);
    SendPlayerMessageToAll(senderid, msg);
    delete[] msg;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    sendDeathMessage
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_sendDeathMessage
        (JNIEnv *, jclass, jint killerid, jint victimid, jint reason) {
    SendDeathMessage(killerid, victimid, reason);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    sendDeathMessageToPlayer
 * Signature: (IIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_sendDeathMessageToPlayer
        (JNIEnv *, jclass, jint playerid, jint killer, jint killee, jint weapon) {
    SendDeathMessageToPlayer(playerid, killer, killee, weapon);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gameTextForAll
 * Signature: (Ljava/lang/String;II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gameTextForAll
        (JNIEnv *env, jclass, jstring string, jint time, jint style) {
	assertNotNull(string, "string", __FUNCTION__);
	auto str = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), string, 256);
    GameTextForAll(str, time, style);
    delete[] str;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gameTextForPlayer
 * Signature: (ILjava/lang/String;II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gameTextForPlayer
        (JNIEnv *env, jclass, jint playerid, jstring string, jint time, jint style) {
	assertNotNull(string, "string", __FUNCTION__);
	auto str = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(playerid), string, 256);
    GameTextForPlayer(playerid, str, time, style);
    delete[] str;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setTimer
 * Signature: (III)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setTimer
        (JNIEnv *, jclass, jint /* index */, jint interval, jint repeating) {
    return SetTimer(interval, repeating > 0, nullptr, nullptr);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    killTimer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_killTimer
        (JNIEnv *, jclass, jint timerid) {
    KillTimer(timerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getMaxPlayers
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getMaxPlayers
        (JNIEnv *, jclass) {
    return GetMaxPlayers();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setGameModeText
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setGameModeText
        (JNIEnv *env, jclass, jstring string) {
	assertNotNull(string, "string", __FUNCTION__);
	auto str = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), string, 128);
    SetGameModeText(str);
    delete[] str;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setTeamCount
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setTeamCount
        (JNIEnv *, jclass, jint count) {
    SetTeamCount(count);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    addPlayerClass
 * Signature: (IFFFFIIIIII)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_addPlayerClass
        (JNIEnv *, jclass, jint modelid, jfloat spawn_x, jfloat spawn_y, jfloat spawn_z,
         jfloat z_angle, jint weapon1, jint weapon1_ammo, jint weapon2, jint weapon2_ammo, jint weapon3,
         jint weapon3_ammo) {
    return AddPlayerClass(modelid, spawn_x, spawn_y, spawn_z, z_angle,
                          weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    addPlayerClassEx
 * Signature: (IIFFFFIIIIII)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_addPlayerClassEx
        (JNIEnv *, jclass, jint teamid, jint modelid, jfloat spawn_x, jfloat spawn_y, jfloat spawn_z,
         jfloat z_angle, jint weapon1, jint weapon1_ammo, jint weapon2, jint weapon2_ammo, jint weapon3,
         jint weapon3_ammo) {
    return AddPlayerClassEx(teamid, modelid, spawn_x, spawn_y, spawn_z, z_angle,
                            weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    addStaticVehicle
 * Signature: (IFFFFII)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_addStaticVehicle
        (JNIEnv *, jclass, jint modelid, jfloat spawn_x, jfloat spawn_y, jfloat spawn_z,
         jfloat z_angle, jint color1, jint color2) {
    return AddStaticVehicle(modelid, spawn_x, spawn_y, spawn_z, z_angle, color1, color2);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    addStaticVehicleEx
 * Signature: (IFFFFIII)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_addStaticVehicleEx
        (JNIEnv *, jclass, jint modelid, jfloat spawn_x, jfloat spawn_y, jfloat spawn_z,
         jfloat z_angle, jint color1, jint color2, jint respawn_delay, jboolean addsiren) {
    return AddStaticVehicleEx(modelid, spawn_x, spawn_y, spawn_z, z_angle, color1, color2, respawn_delay, addsiren);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    addStaticPickup
 * Signature: (IIFFFI)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_addStaticPickup
        (JNIEnv *, jclass, jint model, jint type, jfloat x, jfloat y, jfloat z, jint virtualworld) {
    return AddStaticPickup(model, type, x, y, z, virtualworld);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createPickup
 * Signature: (IIFFFI)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createPickup
        (JNIEnv *, jclass, jint model, jint type, jfloat x, jfloat y, jfloat z, jint virtualworld) {
    return CreatePickup(model, type, x, y, z, virtualworld);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    destroyPickup
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_destroyPickup
        (JNIEnv *, jclass, jint pickup) {
    DestroyPickup(pickup);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    showNameTags
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_showNameTags
        (JNIEnv *, jclass, jboolean enabled) {
    ShowNameTags(enabled);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    showPlayerMarkers
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_showPlayerMarkers
        (JNIEnv *, jclass, jint mode) {
    ShowPlayerMarkers(mode);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gameModeExit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gameModeExit
        (JNIEnv *, jclass) {
    GameModeExit();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setWorldTime
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setWorldTime
        (JNIEnv *, jclass, jint hour) {
    SetWorldTime(hour);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getWeaponName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getWeaponName
        (JNIEnv *env, jclass, jint weaponid) {
    char name[32];
    GetWeaponName(weaponid, name, sizeof(name));

    return env->NewStringUTF(name);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    enableTirePopping
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_enableTirePopping
        (JNIEnv *, jclass, jboolean enabled) {
    EnableTirePopping(enabled);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    enableVehicleFriendlyFire
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_enableVehicleFriendlyFire
        (JNIEnv *, jclass) {
    EnableVehicleFriendlyFire();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    allowInteriorWeapons
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_allowInteriorWeapons
        (JNIEnv *, jclass, jboolean allow) {
    AllowInteriorWeapons(allow);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setWeather
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setWeather
        (JNIEnv *, jclass, jint weatherid) {
    SetWeather(weatherid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setGravity
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setGravity
        (JNIEnv *, jclass, jfloat gravity) {
    SetGravity(gravity);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setDeathDropAmount
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setDeathDropAmount
        (JNIEnv *, jclass, jint amount) {
    SetDeathDropAmount(amount);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createExplosion
 * Signature: (FFFIF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createExplosion
        (JNIEnv *, jclass, jfloat x, jfloat y, jfloat z, jint type, jfloat radius) {
    CreateExplosion(x, y, z, type, radius);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    enableZoneNames
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_enableZoneNames
        (JNIEnv *, jclass, jboolean enabled) {
    EnableZoneNames(enabled);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    usePlayerPedAnims
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_usePlayerPedAnims
        (JNIEnv *, jclass) {
    UsePlayerPedAnims();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    disableInteriorEnterExits
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_disableInteriorEnterExits
        (JNIEnv *, jclass) {
    DisableInteriorEnterExits();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setNameTagDrawDistance
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setNameTagDrawDistance
        (JNIEnv *, jclass, jfloat distance) {
    SetNameTagDrawDistance(distance);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    disableNameTagLOS
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_disableNameTagLOS
        (JNIEnv *, jclass) {
    DisableNameTagLOS();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    limitGlobalChatRadius
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_limitGlobalChatRadius
        (JNIEnv *, jclass, jfloat chat_radius) {
    LimitGlobalChatRadius(chat_radius);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    limitPlayerMarkerRadius
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_limitPlayerMarkerRadius
        (JNIEnv *, jclass, jfloat chat_radius) {
    LimitPlayerMarkerRadius(chat_radius);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    connectNPC
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_connectNPC
        (JNIEnv *env, jclass, jstring name, jstring script) {
	assertNotNull(name, "name", __FUNCTION__); assertNotNull(script, "script", __FUNCTION__);

	auto str_name = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), name, 24);
	auto str_script = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), script, 64);

    ConnectNPC(str_name, str_script);

    delete[] str_name;
    delete[] str_script;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerNPC
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerNPC
        (JNIEnv *, jclass, jint playerid) {
    return (jboolean) IsPlayerNPC(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isPlayerAdmin
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isPlayerAdmin
        (JNIEnv *, jclass, jint playerid) {
    return (jboolean) IsPlayerAdmin(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    kick
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_kick
        (JNIEnv *, jclass, jint playerid) {
    Kick(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    ban
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_ban
        (JNIEnv *, jclass, jint playerid) {
    Ban(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    banEx
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_banEx
        (JNIEnv *env, jclass, jint playerid, jstring reason) {
	assertNotNull(reason, "reason", __FUNCTION__);

	auto str = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), reason, 1024);
    BanEx(playerid, str);
    delete[] str;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    sendRconCommand
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_sendRconCommand
        (JNIEnv *env, jclass, jstring cmd) {
	assertNotNull(cmd, "cmd", __FUNCTION__);

	auto str = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), cmd, 1024);
    SendRconCommand(str);
    delete[] str;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getServerVarAsString
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getServerVarAsString
        (JNIEnv *env, jclass, jstring varname) {
	assertNotNullReturn(varname, "varname", __FUNCTION__, NULL);

	auto str_varname = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), varname, 64);

    char var[2048];
    GetServerVarAsString(str_varname, var, sizeof(var));

    return mbs2wcs(env, Shoebill::GetInstance().getServerCodepage(), var, 2048);
}

JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setServerVarFloat
        (JNIEnv *env, jclass, jstring varname, jfloat value) {
	assertNotNull(varname, "varname", __FUNCTION__);

	auto str_varname = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), varname, 64);
    SetSVarFloat(str_varname, value);
    delete[] str_varname;
}

JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setServerVarInt
        (JNIEnv *env, jclass, jstring varname, jint value) {
	assertNotNull(varname, "varname", __FUNCTION__);

	auto str_varname = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), varname, 64);
    SetSVarInt(str_varname, value);
    delete[] str_varname;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getServerVarAsInt
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getServerVarAsInt
        (JNIEnv *env, jclass, jstring varname) {
	assertNotNullReturn(varname, "varname", __FUNCTION__, 0);

	auto str_varname = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), varname, 64);
    auto ret = GetServerVarAsInt(str_varname);
    delete[] str_varname;
    return ret;
}

JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setServerVarString
        (JNIEnv *env, jclass, jstring varname, jstring value) {
	assertNotNull(varname, "varname", __FUNCTION__); assertNotNull(value, "value", __FUNCTION__);

	auto str_varname = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), varname, 64);
	auto str_value = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), value, 2048);

    SetSVarString(str_varname, str_value);

    delete[] str_varname;
    delete[] str_value;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getServerVarAsBool
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getServerVarAsFloat
        (JNIEnv *env, jclass, jstring varname) {
	assertNotNullReturn(varname, "varname", __FUNCTION__, 0);

	auto str_varname = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), varname, 64);
    auto ret = GetSVarFloat(str_varname);
    delete[] str_varname;
    return ret;
}

JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_deleteServerVar
        (JNIEnv *env, jclass, jstring varname) {
	assertNotNullReturn(varname, "varname", __FUNCTION__, false);

    auto str_varname = env->GetStringUTFChars(varname, nullptr);

    auto result = DeleteSVar(str_varname);

    env->ReleaseStringUTFChars(varname, str_varname);
    return (jboolean) result;
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getServerVarsUpperIndex
        (JNIEnv *env, jclass) {
    return GetSVarsUpperIndex();
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getServerVarType
        (JNIEnv *env, jclass, jstring varname) {
	assertNotNullReturn(varname, "varname", __FUNCTION__, 0);

    auto str_varname = env->GetStringUTFChars(varname, nullptr);
    auto result = GetSVarType(str_varname);
    env->ReleaseStringUTFChars(varname, str_varname);
    return result;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerNetworkStats
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerNetworkStats
        (JNIEnv *env, jclass, jint playerid) {
    char retstr[2048];
    GetPlayerNetworkStats(playerid, retstr, sizeof(retstr));
    return mbs2wcs(env, Shoebill::GetInstance().getServerCodepage(), retstr, 2048);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getNetworkStats
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getNetworkStats
        (JNIEnv *env, jclass) {
    char retstr[2048];
    GetNetworkStats(retstr, sizeof(retstr));
    return mbs2wcs(env, Shoebill::GetInstance().getServerCodepage(), retstr, 2048);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerVersion
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerVersion
        (JNIEnv *env, jclass, jint playerid) {
    char str[128];
    GetPlayerVersion(playerid, str, sizeof(str));

    return env->NewStringUTF(str);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    getServerTickRate
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getServerTickRate
        (JNIEnv *, jclass) {
    return GetServerTickRate();
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    netStats_GetConnectedTime
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_netStats_1GetConnectedTime
        (JNIEnv *, jclass, jint playerid) {
    return NetStats_GetConnectedTime(playerid);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    netStats_MessagesReceived
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_netStats_1MessagesReceived
        (JNIEnv *, jclass, jint playerid) {
    return NetStats_MessagesReceived(playerid);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    netStats_BytesReceived
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_netStats_1BytesReceived
        (JNIEnv *, jclass, jint playerid) {
    return NetStats_BytesReceived(playerid);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    netStats_MessagesSent
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_netStats_1MessagesSent
        (JNIEnv *, jclass, jint playerid) {
    return NetStats_MessagesSent(playerid);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    netStats_BytesSent
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_netStats_1BytesSent
        (JNIEnv *, jclass, jint playerid) {
    return NetStats_BytesSent(playerid);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    netStats_MessagesRecvPerSecond
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_netStats_1MessagesRecvPerSecond
        (JNIEnv *, jclass, jint playerid) {
    return NetStats_MessagesRecvPerSecond(playerid);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    netStats_PacketLossPercent
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_netStats_1PacketLossPercent
        (JNIEnv *, jclass, jint playerid) {
    return NetStats_PacketLossPercent(playerid);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    netStats_ConnectionStatus
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_netStats_1ConnectionStatus
        (JNIEnv *, jclass, jint playerid) {
    return NetStats_ConnectionStatus(playerid);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    netStats_GetIpPort
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_netStats_1GetIpPort
        (JNIEnv *env, jclass, jint playerid) {
    char ipPort[24];
    NetStats_GetIpPort(playerid, ipPort, sizeof(ipPort));
    return env->NewStringUTF(ipPort);
}
/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    blockIpAddress
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_blockIpAddress
        (JNIEnv *env, jclass, jstring ip, jint timeMs) {
	assertNotNull(ip, "ip", __FUNCTION__);

    auto ip_str = env->GetStringUTFChars(ip, nullptr);
    BlockIpAddress(ip_str, timeMs);
    env->ReleaseStringUTFChars(ip, ip_str);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    unBlockIpAddress
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_unBlockIpAddress
        (JNIEnv *env, jclass, jstring ip) {
	assertNotNull(ip, "ip", __FUNCTION__);

    auto ip_str = env->GetStringUTFChars(ip, nullptr);
    UnBlockIpAddress(ip_str);
    env->ReleaseStringUTFChars(ip, ip_str);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createMenu
 * Signature: (Ljava/lang/String;IFFFF)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createMenu
        (JNIEnv *env, jclass, jstring title, jint columns, jfloat x, jfloat y, jfloat col1width, jfloat col2width) {
	assertNotNullReturn(title, "title", __FUNCTION__, 0);

	auto menuTitle = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), title, 32);
	auto ret = CreateMenu(menuTitle, columns, x, y, col1width, col2width);
    delete[] menuTitle;
    return ret;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    destroyMenu
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_destroyMenu
        (JNIEnv *, jclass, jint menuid) {
    DestroyMenu(menuid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    addMenuItem
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_addMenuItem
        (JNIEnv *env, jclass, jint menuid, jint column, jstring menutext) {
	assertNotNull(menutext, "menutext", __FUNCTION__);

	auto menuText = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), menutext, 32);
    AddMenuItem(menuid, column, menuText);
    delete[] menuText;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setMenuColumnHeader
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setMenuColumnHeader
        (JNIEnv *env, jclass, jint menuid, jint column, jstring columnheader) {
	assertNotNull(columnheader, "columnheader", __FUNCTION__);

	auto header = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), columnheader, 32);
    AddMenuItem(menuid, column, header);
    delete[] header;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    showMenuForPlayer
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_showMenuForPlayer
        (JNIEnv *, jclass, jint menuid, jint playerid) {
    ShowMenuForPlayer(menuid, playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    hideMenuForPlayer
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_hideMenuForPlayer
        (JNIEnv *, jclass, jint menuid, jint playerid) {
    HideMenuForPlayer(menuid, playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isValidMenu
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isValidMenu
        (JNIEnv *, jclass, jint menuid) {
    return (jboolean) IsValidMenu(menuid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    disableMenu
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_disableMenu
        (JNIEnv *, jclass, jint menuid) {
    DisableMenu(menuid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    disableMenuRow
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_disableMenuRow
        (JNIEnv *, jclass, jint menuid, jint row) {
    DisableMenuRow(menuid, row);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getPlayerMenu
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerMenu
        (JNIEnv *, jclass, jint playerid) {
    return GetPlayerMenu(playerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawCreate
 * Signature: (FFLjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawCreate
        (JNIEnv *env, jclass, jfloat x, jfloat y, jstring text) {
	assertNotNullReturn(text, "text", __FUNCTION__, 0);

	auto str = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), text, 1024);
    auto ret = TextDrawCreate(x, y, str);

    delete[] str;

    return ret;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawDestroy
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawDestroy
        (JNIEnv *, jclass, jint textid) {
    TextDrawDestroy(textid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawLetterSize
 * Signature: (IFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawLetterSize
        (JNIEnv *, jclass, jint textid, jfloat x, jfloat y) {
    TextDrawLetterSize(textid, x, y);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawTextSize
 * Signature: (IFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawTextSize
        (JNIEnv *, jclass, jint textid, jfloat x, jfloat y) {
    TextDrawTextSize(textid, x, y);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawAlignment
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawAlignment
        (JNIEnv *, jclass, jint textid, jint alignment) {
    TextDrawAlignment(textid, alignment);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawColor
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawColor
        (JNIEnv *, jclass, jint textid, jint color) {
    TextDrawColor(textid, color);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawUseBox
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawUseBox
        (JNIEnv *, jclass, jint textid, jboolean use) {
    TextDrawUseBox(textid, use);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawBoxColor
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawBoxColor
        (JNIEnv *, jclass, jint textid, jint color) {
    TextDrawBoxColor(textid, color);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawSetShadow
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetShadow
        (JNIEnv *, jclass, jint textid, jint size) {
    TextDrawSetShadow(textid, size);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawSetOutline
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetOutline
        (JNIEnv *, jclass, jint textid, jint size) {
    TextDrawSetOutline(textid, size);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawBackgroundColor
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawBackgroundColor
        (JNIEnv *, jclass, jint textid, jint color) {
    TextDrawBackgroundColor(textid, color);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawFont
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawFont
        (JNIEnv *, jclass, jint textid, jint font) {
    TextDrawFont(textid, font);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawSetProportional
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetProportional
        (JNIEnv *, jclass, jint textid, jint set) {
    TextDrawSetProportional(textid, (bool) set);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawSetSelectable
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetSelectable
        (JNIEnv *, jclass, jint textid, jint set) {
    TextDrawSetSelectable(textid, (bool) set);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawShowForPlayer
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawShowForPlayer
        (JNIEnv *, jclass, jint playerid, jint textid) {
    TextDrawShowForPlayer(playerid, textid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawHideForPlayer
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawHideForPlayer
        (JNIEnv *, jclass, jint playerid, jint textid) {
    TextDrawHideForPlayer(playerid, textid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawShowForAll
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawShowForAll
        (JNIEnv *, jclass, jint textid) {
    TextDrawShowForAll(textid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawHideForAll
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawHideForAll
        (JNIEnv *, jclass, jint textid) {
    TextDrawHideForAll(textid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    textDrawSetString
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetString
        (JNIEnv *env, jclass, jint textid, jstring string) {
	assertNotNull(string, "string", __FUNCTION__);

	auto str = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), string, 1024);
    TextDrawSetString(textid, str);

    delete[] str;
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    textDrawSetPreviewModel
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetPreviewModel
        (JNIEnv *, jclass, jint textid, jint modelindex) {
    TextDrawSetPreviewModel(textid, modelindex);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    textDrawSetPreviewRot
 * Signature: (IFFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetPreviewRot
        (JNIEnv *, jclass, jint textid, jfloat fRotX, jfloat fRotY, jfloat fRotZ, jfloat fZoom) {
    TextDrawSetPreviewRot(textid, fRotX, fRotY, fRotZ, fZoom);
}

/*
 * Class:     net_gtaun_shoebill_SampNativeFunction
 * Method:    textDrawSetPreviewVehCol
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_textDrawSetPreviewVehCol
        (JNIEnv *, jclass, jint textid, jint color1, jint color2) {
    TextDrawSetPreviewVehCol(textid, color1, color2);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneCreate
 * Signature: (FFFF)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneCreate
        (JNIEnv *, jclass, jfloat minx, jfloat miny, jfloat maxx, jfloat maxy) {
    return GangZoneCreate(minx, miny, maxx, maxy);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneDestroy
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneDestroy
        (JNIEnv *, jclass, jint zoneid) {
    GangZoneDestroy(zoneid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneShowForPlayer
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneShowForPlayer
        (JNIEnv *, jclass, jint playerid, jint zoneid, jint color) {
    GangZoneShowForPlayer(playerid, zoneid, color);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneShowForAll
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneShowForAll
        (JNIEnv *, jclass, jint zoneid, jint color) {
    GangZoneShowForAll(zoneid, color);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneHideForPlayer
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneHideForPlayer
        (JNIEnv *, jclass, jint playerid, jint zoneid) {
    GangZoneHideForPlayer(playerid, zoneid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneHideForAll
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneHideForAll
        (JNIEnv *, jclass, jint zoneid) {
    GangZoneHideForAll(zoneid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneFlashForPlayer
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneFlashForPlayer
        (JNIEnv *, jclass, jint playerid, jint zoneid, jint flashcolor) {
    GangZoneFlashForPlayer(playerid, zoneid, flashcolor);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneFlashForAll
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneFlashForAll
        (JNIEnv *, jclass, jint zoneid, jint flashcolor) {
    GangZoneFlashForAll(zoneid, flashcolor);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneStopFlashForPlayer
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneStopFlashForPlayer
        (JNIEnv *, jclass, jint playerid, jint zoneid) {
    GangZoneStopFlashForPlayer(playerid, zoneid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    gangZoneStopFlashForAll
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_gangZoneStopFlashForAll
        (JNIEnv *, jclass, jint zoneid) {
    GangZoneStopFlashForAll(zoneid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    create3DTextLabel
 * Signature: (Ljava/lang/String;IFFFFIZ)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_create3DTextLabel
        (JNIEnv *env, jclass, jstring text, jint color, jfloat x, jfloat y, jfloat z,
         jfloat drawDistance, jint worldid, jboolean testLOS) {
	assertNotNullReturn(text, "text", __FUNCTION__, 0);
	auto str = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), text, 1024);
    auto ret = Create3DTextLabel(str, color, x, y, z, drawDistance, worldid, testLOS);

    delete[] str;

    return ret;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    delete3DTextLabel
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_delete3DTextLabel
        (JNIEnv *, jclass, jint id) {
    Delete3DTextLabel(id);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attach3DTextLabelToPlayer
 * Signature: (IIFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attach3DTextLabelToPlayer
        (JNIEnv *, jclass, jint id, jint playerid, jfloat offsetX, jfloat offsetY, jfloat offsetZ) {
    Attach3DTextLabelToPlayer(id, playerid, offsetX, offsetY, offsetZ);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attach3DTextLabelToVehicle
 * Signature: (IIFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attach3DTextLabelToVehicle
        (JNIEnv *, jclass, jint id, jint vehicleid, jfloat offsetX, jfloat offsetY, jfloat offsetZ) {
    Attach3DTextLabelToVehicle(id, vehicleid, offsetX, offsetY, offsetZ);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    update3DTextLabelText
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_update3DTextLabelText
        (JNIEnv *env, jclass, jint id, jint color, jstring text) {
	assertNotNull(text, "text", __FUNCTION__);

	auto str = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), text, 1024);
    Update3DTextLabelText(id, color, str);

    delete[] str;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createPlayer3DTextLabel
 * Signature: (ILjava/lang/String;IFFFFIIZ)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createPlayer3DTextLabel
        (JNIEnv *env, jclass, jint playerid, jstring text, jint color, jfloat x, jfloat y, jfloat z,
         jfloat drawDistance, jint attachedplayerid, jint attachedvehicleid, jboolean testLOS) {
	assertNotNullReturn(text, "text", __FUNCTION__, 0);

    auto str = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(playerid), text, 1024);
    auto ret = CreatePlayer3DTextLabel(playerid, str, color, x, y, z, drawDistance, attachedplayerid, attachedvehicleid,
                                      testLOS);

    delete[] str;

    return ret;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    deletePlayer3DTextLabel
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_deletePlayer3DTextLabel
        (JNIEnv *, jclass, jint playerid, jint id) {
    DeletePlayer3DTextLabel(playerid, id);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    updatePlayer3DTextLabelText
 * Signature: (IIILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_updatePlayer3DTextLabelText
        (JNIEnv *env, jclass, jint playerid, jint id, jint color, jstring text) {
	assertNotNull(text, "text", __FUNCTION__);

	auto str = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(playerid), text, 1024);
    UpdatePlayer3DTextLabelText(playerid, id, color, str);

    delete[] str;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    showPlayerDialog
 * Signature: (IIILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_showPlayerDialog
        (JNIEnv *env, jclass, jint playerid, jint dialogid, jint style,
         jstring caption, jstring info, jstring button1, jstring button2) {
	assertNotNullReturn(caption, "caption", __FUNCTION__, 0); assertNotNullReturn(info, "info", __FUNCTION__, 0);
    assertNotNullReturn(button1, "button1", __FUNCTION__, 0); assertNotNullReturn(button2, "button2", __FUNCTION__, 0);

	auto codepage = Shoebill::GetInstance().getPlayerCodepage(playerid);
	auto str_caption = wcs2mbs(env, codepage, caption, 64), str_info = wcs2mbs(env, codepage, info, 1024),
         str_button1 = wcs2mbs(env, codepage, button1, 32), str_button2 = wcs2mbs(env, codepage, button2, 32);

    int ret = ShowPlayerDialog(playerid, dialogid, style, str_caption, str_info, str_button1, str_button2);

    delete[] str_caption;
    delete[] str_info;
    delete[] str_button1;
    delete[] str_button2;

    return ret;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    createVehicle
 * Signature: (IFFFFIII)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createVehicle
        (JNIEnv *, jclass, jint model, jfloat x, jfloat y, jfloat z, jfloat rotation, jint color1, jint color2,
         jint respawnDelay, jboolean addsiren) {
    return CreateVehicle(model, x, y, z, rotation, color1, color2, respawnDelay, addsiren);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    destroyVehicle
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_destroyVehicle
        (JNIEnv *, jclass, jint vehicleid) {
    DestroyVehicle(vehicleid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isVehicleStreamedIn
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isVehicleStreamedIn
        (JNIEnv *, jclass, jint vehicleid, jint forplayerid) {
    return (jboolean) IsVehicleStreamedIn(vehicleid, forplayerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehiclePos
 * Signature: (ILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehiclePos
        (JNIEnv *env, jclass, jint vehicleid, jobject vector3d) {
	assertNotNull(vector3d, "vector3d", __FUNCTION__);

    static auto cls = env->GetObjectClass(vector3d);
    static auto fidX = env->GetFieldID(cls, "x", "F");
    static auto fidY = env->GetFieldID(cls, "y", "F");
    static auto fidZ = env->GetFieldID(cls, "z", "F");

    float x, y, z;
    GetVehiclePos(vehicleid, &x, &y, &z);

    env->SetFloatField(vector3d, fidX, x);
    env->SetFloatField(vector3d, fidY, y);
    env->SetFloatField(vector3d, fidZ, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehiclePos
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehiclePos
        (JNIEnv *, jclass, jint vehicleid, jfloat x, jfloat y, jfloat z) {
    SetVehiclePos(vehicleid, x, y, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleZAngle
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleZAngle
        (JNIEnv *, jclass, jint vehicleid) {
    float angle;
    GetVehicleZAngle(vehicleid, &angle);

    return angle;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleRotationQuat
 * Signature: (ILnet/gtaun/shoebill/data/Quaternion;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleRotationQuat
        (JNIEnv *env, jclass, jint vehicleid, jobject quat) {
	assertNotNull(quat, "quat", __FUNCTION__);

    static auto cls = env->GetObjectClass(quat);
    static auto fidW = env->GetFieldID(cls, "w", "F");
    static auto fidX = env->GetFieldID(cls, "x", "F");
    static auto fidY = env->GetFieldID(cls, "y", "F");
    static auto fidZ = env->GetFieldID(cls, "z", "F");

    float w, x, y, z;
    GetVehicleRotationQuat(vehicleid, &w, &x, &y, &z);

    env->SetFloatField(quat, fidW, w);
    env->SetFloatField(quat, fidX, x);
    env->SetFloatField(quat, fidY, y);
    env->SetFloatField(quat, fidZ, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleZAngle
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleZAngle
        (JNIEnv *, jclass, jint vehicleid, jfloat angle) {
    SetVehicleZAngle(vehicleid, angle);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleParamsForPlayer
 * Signature: (IIZZ)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleParamsForPlayer
        (JNIEnv *, jclass, jint vehicleid, jint playerid, jboolean objective, jboolean doorslocked) {
    SetVehicleParamsForPlayer(vehicleid, playerid, objective, doorslocked);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    manualVehicleEngineAndLights
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_manualVehicleEngineAndLights
        (JNIEnv *, jclass) {
    ManualVehicleEngineAndLights();
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleParamsEx
 * Signature: (IZZZZZZZ)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleParamsEx
        (JNIEnv *, jclass, jint vehicleid, jint engine, jint lights,
         jint alarm, jint doors, jint bonnet, jint boot, jint objective) {
    //-1 is == true eventhough it should be false. Workaround:
    SetVehicleParamsEx(vehicleid, engine == 1, lights == 1, alarm == 1,
                       doors == 1, bonnet == 1, boot == 1, objective == 1);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleParamsEx
 * Signature: (ILnet/gtaun/shoebill/object/impl/VehicleParamImpl;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleParamsEx
        (JNIEnv *env, jclass, jint vehicleid, jobject state) {
	assertNotNull(state, "state", __FUNCTION__);

    static auto cls = env->GetObjectClass(state);
    static auto fidEngine = env->GetFieldID(cls, "engine", "I");
    static auto fidLights = env->GetFieldID(cls, "lights", "I");
    static auto fidAlarm = env->GetFieldID(cls, "alarm", "I");
    static auto fidDoors = env->GetFieldID(cls, "doors", "I");
    static auto fidBonnet = env->GetFieldID(cls, "bonnet", "I");
    static auto fidBoot = env->GetFieldID(cls, "boot", "I");
    static auto fidObjective = env->GetFieldID(cls, "objective", "I");

    int engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, &engine, &lights, &alarm, &doors, &bonnet, &boot, &objective);

    env->SetIntField(state, fidEngine, engine);
    env->SetIntField(state, fidLights, lights);
    env->SetIntField(state, fidAlarm, alarm);
    env->SetIntField(state, fidDoors, doors);
    env->SetIntField(state, fidBonnet, bonnet);
    env->SetIntField(state, fidBoot, boot);
    env->SetIntField(state, fidObjective, objective);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleToRespawn
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleToRespawn
        (JNIEnv *, jclass, jint vehicleid) {
    SetVehicleToRespawn(vehicleid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    linkVehicleToInterior
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_linkVehicleToInterior
        (JNIEnv *, jclass, jint vehicleid, jint interiorid) {
    LinkVehicleToInterior(vehicleid, interiorid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    addVehicleComponent
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_addVehicleComponent
        (JNIEnv *, jclass, jint vehicleid, jint componentid) {
    AddVehicleComponent(vehicleid, componentid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    removeVehicleComponent
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_removeVehicleComponent
        (JNIEnv *, jclass, jint vehicleid, jint componentid) {
    RemoveVehicleComponent(vehicleid, componentid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    changeVehicleColor
 * Signature: (III)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_changeVehicleColor
        (JNIEnv *, jclass, jint vehicleid, jint color1, jint color2) {
    ChangeVehicleColor(vehicleid, color1, color2);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    changeVehiclePaintjob
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_changeVehiclePaintjob
        (JNIEnv *, jclass, jint vehicleid, jint paintjobid) {
    ChangeVehiclePaintjob(vehicleid, paintjobid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleHealth
 * Signature: (IF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleHealth
        (JNIEnv *, jclass, jint vehicleid, jfloat health) {
    SetVehicleHealth(vehicleid, health);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleHealth
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleHealth
        (JNIEnv *, jclass, jint vehicleid) {
    float health;
    GetVehicleHealth(vehicleid, &health);

    return health;
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    attachTrailerToVehicle
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_attachTrailerToVehicle
        (JNIEnv *, jclass, jint trailerid, jint vehicleid) {
    AttachTrailerToVehicle(trailerid, vehicleid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    detachTrailerFromVehicle
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_detachTrailerFromVehicle
        (JNIEnv *, jclass, jint trailerid) {
    DetachTrailerFromVehicle(trailerid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    isTrailerAttachedToVehicle
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isTrailerAttachedToVehicle
        (JNIEnv *, jclass, jint vehicleid) {
    return (jboolean) IsTrailerAttachedToVehicle(vehicleid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleTrailer
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleTrailer
        (JNIEnv *, jclass, jint vehicleid) {
    return GetVehicleTrailer(vehicleid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleNumberPlate
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleNumberPlate
        (JNIEnv *env, jclass, jint vehicleid, jstring numberplate) {
	assertNotNull(numberplate, "numberplate", __FUNCTION__);

    auto str_numberplate = env->GetStringUTFChars(numberplate, nullptr);
    SetVehicleNumberPlate(vehicleid, str_numberplate);

    env->ReleaseStringUTFChars(numberplate, str_numberplate);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleModel
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleModel
        (JNIEnv *, jclass, jint vehicleid) {
    return GetVehicleModel(vehicleid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleComponentInSlot
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleComponentInSlot
        (JNIEnv *, jclass, jint vehicleid, jint slot) {
    return GetVehicleComponentInSlot(vehicleid, slot);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleComponentType
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleComponentType
        (JNIEnv *, jclass, jint component) {
    return GetVehicleComponentType(component);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    repairVehicle
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_repairVehicle
        (JNIEnv *, jclass, jint vehicleid) {
    RepairVehicle(vehicleid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleVelocity
 * Signature: (ILnet/gtaun/shoebill/data/Velocity;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleVelocity
        (JNIEnv *env, jclass, jint vehicleid, jobject velocity) {
	assertNotNull(velocity, "velocity", __FUNCTION__);

    static auto cls = env->GetObjectClass(velocity);
    static auto fidX = env->GetFieldID(cls, "x", "F");
    static auto fidY = env->GetFieldID(cls, "y", "F");
    static auto fidZ = env->GetFieldID(cls, "z", "F");

    float x, y, z;
    GetVehicleVelocity(vehicleid, &x, &y, &z);

    env->SetFloatField(velocity, fidX, x);
    env->SetFloatField(velocity, fidY, y);
    env->SetFloatField(velocity, fidZ, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleVelocity
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleVelocity
        (JNIEnv *, jclass, jint vehicleid, jfloat x, jfloat y, jfloat z) {
    SetVehicleVelocity(vehicleid, x, y, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleAngularVelocity
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleAngularVelocity
        (JNIEnv *, jclass, jint vehicleid, jfloat x, jfloat y, jfloat z) {
    SetVehicleAngularVelocity(vehicleid, x, y, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleDamageStatus
 * Signature: (ILnet/gtaun/shoebill/object/impl/VehicleDamageImpl;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleDamageStatus
        (JNIEnv *env, jclass, jint vehicleid, jobject damage) {
	assertNotNull(damage, "damage", __FUNCTION__);

    static auto cls = env->GetObjectClass(damage);
    static auto fidPanels = env->GetFieldID(cls, "panels", "I");
    static auto fidDoors = env->GetFieldID(cls, "doors", "I");
    static auto fidLights = env->GetFieldID(cls, "lights", "I");
    static auto fidTires = env->GetFieldID(cls, "tires", "I");

    int panels, doors, lights, tires;
    GetVehicleDamageStatus(vehicleid, &panels, &doors, &lights, &tires);

    env->SetIntField(damage, fidPanels, panels);
    env->SetIntField(damage, fidDoors, doors);
    env->SetIntField(damage, fidLights, lights);
    env->SetIntField(damage, fidTires, tires);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    updateVehicleDamageStatus
 * Signature: (IIIII)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_updateVehicleDamageStatus
        (JNIEnv *, jclass, jint vehicleid, jint panels, jint doors, jint lights, jint tires) {
    UpdateVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleModelInfo
 * Signature: (IILnet/gtaun/shoebill/data/Vector3D;)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleModelInfo
        (JNIEnv *env, jclass, jint vehiclemodel, jint infotype, jobject vector3d) {
	assertNotNull(vector3d, "vector3d", __FUNCTION__);

    static auto cls = env->GetObjectClass(vector3d);
    static auto fidX = env->GetFieldID(cls, "x", "F");
    static auto fidY = env->GetFieldID(cls, "y", "F");
    static auto fidZ = env->GetFieldID(cls, "z", "F");

    float x, y, z;
    GetVehicleModelInfo(vehiclemodel, infotype, &x, &y, &z);

    env->SetFloatField(vector3d, fidX, x);
    env->SetFloatField(vector3d, fidY, y);
    env->SetFloatField(vector3d, fidZ, z);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    setVehicleVirtualWorld
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleVirtualWorld
        (JNIEnv *, jclass, jint vehicleid, jint worldid) {
    SetVehicleVirtualWorld(vehicleid, worldid);
}

/*
 * Class:     net_gtaun_shoebill_samp_SampNativeFunction
 * Method:    getVehicleVirtualWorld
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleVirtualWorld
        (JNIEnv *, jclass, jint vehicleid) {
    return GetVehicleVirtualWorld(vehicleid);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPublic
        (JNIEnv *env, jclass, jint pAmx, jstring name) {
	assertNotNullReturn(name, "name", __FUNCTION__, 0);

    auto amx = reinterpret_cast<AMX *>(pAmx);
    int index;
    auto functionName = env->GetStringUTFChars(name, nullptr);
    amx_FindPublic(amx, functionName, &index);
    env->ReleaseStringUTFChars(name, functionName);
    return index;
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getNative
        (JNIEnv *env, jclass, jstring name) {
	assertNotNullReturn(name, "name", __FUNCTION__, 0);

    auto functionName = env->GetStringUTFChars(name, nullptr);
    auto native = sampgdk_FindNative(functionName);
    env->ReleaseStringUTFChars(name, functionName);
    return reinterpret_cast<int>(native);
}

JNIEXPORT jobject JNICALL Java_net_gtaun_shoebill_SampNativeFunction_callFunction(JNIEnv *env, jclass, jint pAmx,
                                                                                  jint index, jint returnType,
                                                                                  jobjectArray args,
                                                                                  jobjectArray types) {
	assertNotNullReturn(args, "args", __FUNCTION__, NULL); assertNotNullReturn(types, "txdname", __FUNCTION__, NULL);

	auto amx = (AMX *) pAmx;
    cell retval;
    int arrayLength = env->GetArrayLength(args);
    auto native = reinterpret_cast<AMX_NATIVE>(index);
    cell params[33];
    std::vector<cell> stringCells;
    std::map<std::pair<jobject, std::string>, std::pair<cell *, cell>> references;
    params[0] = (cell) (arrayLength * sizeof(cell));
    for (auto i = 0; i < arrayLength; i++) {
        auto object = env->GetObjectArrayElement(args, i);
        auto objectClass = env->GetObjectClass(object);
        auto mid = env->GetMethodID(objectClass, "toString", "()Ljava/lang/String;");
        if (!mid) {
            sampgdk_logprintf("[Shoebill] Error while getting toString() method.");
            return nullptr;
        }
        auto classNameString = static_cast<jstring>(env->CallObjectMethod(objectClass, mid));
        auto cstr = env->GetStringUTFChars(classNameString, nullptr);
        auto className = std::string(cstr);
        if (className == "class java.lang.String") {
            auto javaString = static_cast<jstring>(object);
            auto wmsg = env->GetStringChars(javaString, nullptr);
            auto len = env->GetStringLength(javaString);

            char str[1024];
            wcs2mbs(Shoebill::GetInstance().getServerCodepage(), wmsg, len, str, sizeof(str));
            env->ReleaseStringChars(javaString, wmsg);

            cell strCel;
            amx_PushString(amx, &strCel, nullptr, str, 0, 0);
            params[i + 1] = strCel;
            stringCells.push_back(strCel);
        } else if (className == "class java.lang.Integer") {
            auto ajf = env->GetMethodID(objectClass, "intValue", "()I");
            int integer = env->CallIntMethod(object, ajf);
            params[i + 1] = integer;
        } else if (className == "class java.lang.Float") {
            auto bjf = env->GetMethodID(objectClass, "floatValue", "()F");
            auto ffloat = env->CallFloatMethod(object, bjf);
            params[i + 1] = amx_ftoc(ffloat);
        } else if (className == "class net.gtaun.Shoebill::GetInstance().amx.types.ReferenceInt" ||
                   className == "class net.gtaun.Shoebill::GetInstance().amx.types.ReferenceFloat") {
            cell amx_addr, *phys_addr;

            amx_Allot(amx, 1, &amx_addr, &phys_addr);
            params[i + 1] = amx_addr;
            references[std::pair<jobject, std::string>(object, className)] = std::pair<cell *, cell>(
                    phys_addr, amx_addr);
        } else if (className == "class net.gtaun.Shoebill::GetInstance().amx.types.ReferenceString") {
            auto lengthMethodId = env->GetMethodID(objectClass, "getLength", "()I");
            auto length = env->CallIntMethod(object, lengthMethodId);
            cell amx_str, *amx_str_phys;
            amx_Allot(amx, length, &amx_str, &amx_str_phys);
            params[i + 1] = amx_str;
            references[std::pair<jobject, std::string>(object, className)] = std::pair<cell *, cell>(
                    amx_str_phys, amx_str);
        }
        env->ReleaseStringUTFChars(classNameString, cstr);
    }
    retval = native(amx, params);
    auto iterator = references.begin();
    while (iterator != references.end()) {
        auto className = iterator->first.second;
        auto object = iterator->first.first;
        auto type = env->GetObjectClass(object);
        if (className == "class net.gtaun.Shoebill::GetInstance().amx.types.ReferenceInt") {
            auto methodId = env->GetMethodID(type, "setValue", "(I)V");
            env->CallVoidMethod(object, methodId, *iterator->second.first);
        } else if (className == "class net.gtaun.Shoebill::GetInstance().amx.types.ReferenceFloat") {
            auto methodId = env->GetMethodID(type, "setValue", "(F)V");
	        auto result = amx_ctof(*iterator->second.first);
            env->CallVoidMethod(object, methodId, result);
        } else if (className == "class net.gtaun.Shoebill::GetInstance().amx.types.ReferenceString") {
            auto methodId = env->GetMethodID(type, "setValue", "(Ljava/lang/String;)V");
            char *text = nullptr;
            amx_StrParam(amx, iterator->second.second, text);

            jchar wstr[1024];
            auto len = mbs2wcs(Shoebill::GetInstance().getServerCodepage(), text, -1, wstr,
                               sizeof(wstr) / sizeof(wstr[0]));

            auto newText = env->NewString(wstr, len);
            env->CallVoidMethod(object, methodId, newText);
        }
        amx_Release(amx, iterator->second.second);
        ++iterator;
    }
    for (auto str : stringCells) amx_Release(amx, str);
    stringCells.clear();
    references.clear();
    return makeObjectFromReturnType(env, returnType, amx, retval);
}

JNIEXPORT jobject JNICALL Java_net_gtaun_shoebill_SampNativeFunction_callPublic
        (JNIEnv *env, jclass, jint pAmx, jint idx, jint returnType, jobjectArray args, jobjectArray types) {
	assertNotNullReturn(args, "args", __FUNCTION__, NULL); assertNotNullReturn(types, "types", __FUNCTION__, NULL);

    auto amx = reinterpret_cast<AMX *>(pAmx);
    auto shoebill = Shoebill::GetInstance();

    cell retval;
    std::vector<std::pair<cell, char *>> stringCells;
    std::vector<std::pair<cell *, int>> cellArrays;
    std::map<std::pair<int, char *>, std::pair<cell *, cell>> references;
    int arrayLength = env->GetArrayLength(args);
    amx->paramcount = 0;

    for (auto i = arrayLength - 1; i >= 0; i--) {
	    auto str_type = wcs2mbs(env, shoebill.getServerCodepage(), (jstring) env->GetObjectArrayElement(types, i), 8);
	    auto type = std::string(str_type);
	    auto object = env->GetObjectArrayElement(args, i);
        if (type == "s") {
            cell strCell;
	        auto content = wcs2mbs(env, shoebill.getServerCodepage(), (jstring) object, 1024);
            amx_PushString(amx, &strCell, nullptr, content, 0, 0);
            stringCells.push_back(std::pair<cell, char *>(strCell, content));
        } else if (type == "i") {
	        auto value = getIntegerFromObject(env, object);
            amx_Push(amx, value);
        } else if (type == "f") {
	        auto value = getFloatFromObject(env, object);
            amx_Push(amx, amx_ftoc(value));
        } else if (type == "ri" || type == "rf") {
            cell amx_addr, *phys_addr;
            amx_Allot(amx, 1, &amx_addr, &phys_addr);
            amx_Push(amx, amx_addr);
            references[std::pair<int, char *>(i, str_type)] = std::pair<cell *, cell>(phys_addr, amx_addr);
        } else if (type == "rs") {
            auto objectClass = env->GetObjectClass(object);
            auto lengthMethodId = env->GetMethodID(objectClass, "getLength", "()I");
            auto length = env->CallIntMethod(object, lengthMethodId);
            cell amx_str, *amx_str_phys;
            amx_Allot(amx, length, &amx_str, &amx_str_phys);
            amx_Push(amx, amx_str);
            references[std::pair<int, char *>(i, str_type)] = std::pair<cell *, cell>(amx_str_phys, amx_str);
        } else if (type.find("[]") != std::string::npos) {
	        auto javaArray = (jobjectArray) object;
            int javaArrayLength = env->GetArrayLength(javaArray);
	        auto array = new cell[javaArrayLength];
            if (type == "i[]") {
                for (auto a = 0; a < javaArrayLength; a++) {
                    cell amx_addr, *phys_addr;
                    amx_Allot(amx, 1, &amx_addr, &phys_addr);
                    array[a] = amx_addr;
                    *phys_addr = getIntegerFromObject(env, env->GetObjectArrayElement(javaArray, a));
                }
            } else if (type == "f[]") {
                for (auto a = 0; a < javaArrayLength; a++) {
                    auto value = getFloatFromObject(env, env->GetObjectArrayElement(javaArray, a));
                    array[a] = amx_ftoc(value);
                }
            } else if (type == "s[]") {
                for (auto a = 0; a < javaArrayLength; a++) {
	                auto text = wcs2mbs(env, shoebill.getServerCodepage(),
                                         (jstring) env->GetObjectArrayElement(javaArray, a), 1024);
                    auto amxString = amx_NewString(amx, text);
                    array[a] = amxString;
                    stringCells.push_back(std::pair<cell, char *>(amxString, text));
                }
            }
            references[std::pair<int, char *>(i, str_type)] = std::pair<cell *, cell>(array, javaArrayLength);
            cellArrays.push_back(std::pair<cell *, int>(array, javaArrayLength));
            cell tmpAddress;
            amx_Push(amx, javaArrayLength);
            amx_PushArray(amx, &tmpAddress, nullptr, array, javaArrayLength);
        }
    }
    amx_Exec(amx, &retval, idx);
    for (auto iterator : references) {
        auto str_type = iterator.first.second;
        auto type = std::string(str_type);
        auto object = env->GetObjectArrayElement(args, iterator.first.first);
        auto objectClass = env->GetObjectClass(object);
        if (type == "ri") {
            auto methodId = env->GetMethodID(objectClass, "setValue", "(I)V");
            env->CallVoidMethod(object, methodId, *iterator.second.first);
        } else if (type == "rf") {
            auto methodId = env->GetMethodID(objectClass, "setValue", "(F)V");
	        auto result = amx_ctof(*iterator.second.first);
            env->CallVoidMethod(object, methodId, result);
        } else if (type == "rs") {
            auto methodId = env->GetMethodID(objectClass, "setValue", "(Ljava/lang/String;)V");
            char text[1024];
            amx_GetString(amx, iterator.second.second, text, 1024);
            env->CallVoidMethod(object, methodId, mbs2wcs(env, shoebill.getServerCodepage(), text, 1024));
        } else if (type == "i[]") {
            auto javaArrayLength = iterator.second.second; //iterator.second.second will return the array length for array refs.
	        auto javaIntegerArray = (jobjectArray) env->GetObjectArrayElement(args, iterator.first.first);
            for (auto i = 0; i < javaArrayLength; i++) {
                env->SetObjectArrayElement(javaIntegerArray, i, makeJavaInteger(env, *iterator.second.first));
            }
        } else if (type == "f[]") {
            auto javaArrayLength = iterator.second.second;
	        auto javaFloatArray = (jobjectArray) env->GetObjectArrayElement(args, iterator.first.first);
            for (auto i = 0; i < javaArrayLength; i++) {
	            auto value = amx_ctof(*(iterator.second.first + i));
                sampgdk_logprintf("Val: %f", value);
                env->SetObjectArrayElement(javaFloatArray, i, makeJavaFloat(env, value));
            }
        }
        delete[] str_type;
        amx_Release(amx, iterator.second.second);
    }
    for (auto it : stringCells) {
        amx_Release(amx, it.first);
        delete[] it.second;
    }
    for (auto it : cellArrays) delete[] it.first;
    return makeObjectFromReturnType(env, returnType, amx, retval);
}

JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_restartShoebill
        (JNIEnv *, jclass) {
    Shoebill::GetInstance().Restart();
}

/*
* Class:     net_gtaun_shoebill_SampNativeFunction
* Method:    HookCallback
* Signature: (Ljava/lang/String;Ljava/lang/String)
*/
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_hookCallback
        (JNIEnv *env, jclass, jstring name, jobjectArray types) {
	assertNotNullReturn(name, "name", __FUNCTION__, false); assertNotNullReturn(types, "types", __FUNCTION__, false);

    static auto validClasses = {"s", "i", "f", "f[]", "i[]"};

	auto callbackName = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), name, 64);
    std::vector<std::string> classTypes;
    int arrayLength = env->GetArrayLength(types);
    for (auto i = 0; i < arrayLength; i++) {
        auto string = (jstring) env->GetObjectArrayElement(types, i);
        auto type = env->GetStringUTFChars(string, nullptr);
        if (std::find(validClasses.begin(), validClasses.end(), type) == validClasses.end()) {
            sampgdk_logprintf("[SHOEBILL] %s is not a valid type for hooking a callback (%s).", type, callbackName);
            env->ReleaseStringUTFChars(string, type);
            return (jboolean) false;
        }
        classTypes.push_back(std::string(type));
        env->ReleaseStringUTFChars(string, type);
    }
    auto success = HookCallback(std::string(callbackName), classTypes);
    env->ReleaseStringUTFChars(name, callbackName);
    return (jboolean) success;
}

/*
* Class:     net_gtaun_shoebill_SampNativeFunction
* Method:    UnhookCallback
* Signature: (Ljava/lang/String;)Z
*/
JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_unhookCallback
        (JNIEnv *env, jclass, jstring name) {
	assertNotNullReturn(name, "name", __FUNCTION__, false);

    auto callbackName = env->GetStringUTFChars(name, nullptr);
    auto success = UnhookCallback(std::string(callbackName));
    env->ReleaseStringUTFChars(name, callbackName);
    return (jboolean) success;
}

JNIEXPORT jint JNICALL
Java_net_gtaun_shoebill_SampNativeFunction_applyActorAnimation(JNIEnv *env, jclass, jint actor, jstring animLib,
                                                               jstring animName,
                                                               jfloat fDelta, jint loop, jint lockX, jint lockY,
                                                               jint freeze, jint time) {

	assertNotNullReturn(animLib, "animLib", __FUNCTION__, 0); assertNotNullReturn(animName, "animName", __FUNCTION__, 0);

    auto animLibStr = env->GetStringUTFChars(animLib, nullptr);
    auto animNameStr = env->GetStringUTFChars(animName, nullptr);
    auto ret = ApplyActorAnimation(actor, animLibStr, animNameStr, fDelta, (bool) loop, (bool) lockX, (bool) lockY,
                                   (bool) freeze, time);
    env->ReleaseStringUTFChars(animLib, animLibStr);
    env->ReleaseStringUTFChars(animName, animNameStr);
    return ret;
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_clearActorAnimations(JNIEnv *, jclass, jint actor) {
    return ClearActorAnimations(actor);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_destroyActor(JNIEnv *, jclass, jint actor) {
    return DestroyActor(actor);
}

JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_disableRemoteVehicleCollisions(JNIEnv *, jclass,
                                                                                                 jint playerid,
                                                                                                 jint disable) {
    DisableRemoteVehicleCollisions(playerid, (bool) disable);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_enablePlayerCameraTarget(JNIEnv *, jclass,
                                                                                           jint playerid, jint enable) {
    return EnablePlayerCameraTarget(playerid, (bool) enable);
}

JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getActorFacingAngle(JNIEnv *, jclass, jint actor) {
    float angle;
    GetActorFacingAngle(actor, &angle);
    return angle;
}

JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getActorHealth(JNIEnv *, jclass, jint actor) {
    float health;
    GetActorHealth(actor, &health);
    return health;
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getActorPoolSize(JNIEnv *, jclass) {
    return GetActorPoolSize();
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getActorPos(JNIEnv *env, jclass, jint actor,
                                                                              jobject vector3d) {
	assertNotNullReturn(vector3d, "vector3d", __FUNCTION__, 0);

    static auto cls = env->GetObjectClass(vector3d);
    static auto fidX = env->GetFieldID(cls, "x", "F");
    static auto fidY = env->GetFieldID(cls, "y", "F");
    static auto fidZ = env->GetFieldID(cls, "z", "F");

    float x, y, z;
    GetActorPos(actor, &x, &y, &z);

    env->SetFloatField(vector3d, fidX, x);
    env->SetFloatField(vector3d, fidY, y);
    env->SetFloatField(vector3d, fidZ, z);
    return 1;
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getActorVirtualWorld(JNIEnv *, jclass, jint actor) {
    return GetActorVirtualWorld(actor);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getObjectModel(JNIEnv *, jclass, jint objectId) {
    return GetObjectModel(objectId);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerCameraTargetActor(JNIEnv *, jclass,
                                                                                             jint player) {
    return GetPlayerCameraTargetActor(player);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerCameraTargetObject(JNIEnv *, jclass,
                                                                                              jint player) {
    return GetPlayerCameraTargetObject(player);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerCameraTargetPlayer(JNIEnv *, jclass,
                                                                                              jint player) {
    return GetPlayerCameraTargetPlayer(player);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerCameraTargetVehicle(JNIEnv *, jclass,
                                                                                               jint player) {
    return GetPlayerCameraTargetVehicle(player);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerObjectModel(JNIEnv *, jclass, jint player,
                                                                                       jint objectid) {
    return GetPlayerObjectModel(player, objectid);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerTargetActor(JNIEnv *, jclass, jint player) {
    return GetPlayerTargetActor(player);
}

JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleParamsCarDoors(JNIEnv *env, jclass,
                                                                                           jint vehicle,
                                                                                           jobject vehicleParams) {
	assertNotNull(vehicleParams, "vehicleParams", __FUNCTION__);

    static auto cls = env->GetObjectClass(vehicleParams);
    static auto fidDriver = env->GetFieldID(cls, "driver", "I");
    static auto fidPassenger = env->GetFieldID(cls, "passenger", "I");
    static auto fidBackLeft = env->GetFieldID(cls, "backLeft", "I");
    static auto fidBackRight = env->GetFieldID(cls, "backRight", "I");

    int driver, passenger, backleft, backright;
    GetVehicleParamsCarDoors(vehicle, &driver, &passenger, &backleft, &backright);

    env->SetIntField(vehicleParams, fidDriver, driver);
    env->SetIntField(vehicleParams, fidPassenger, passenger);
    env->SetIntField(vehicleParams, fidBackLeft, backleft);
    env->SetIntField(vehicleParams, fidBackRight, backright);
}

JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleParamsCarWindows(JNIEnv *env, jclass,
                                                                                             jint vehicle,
                                                                                             jobject vehicleParams) {
	assertNotNull(vehicleParams, "vehicleParams", __FUNCTION__);

    static auto cls = env->GetObjectClass(vehicleParams);
    static auto fidDriver = env->GetFieldID(cls, "driver", "I");
    static auto fidPassenger = env->GetFieldID(cls, "passenger", "I");
    static auto fidBackLeft = env->GetFieldID(cls, "backLeft", "I");
    static auto fidBackRight = env->GetFieldID(cls, "backRight", "I");

    int driver, passenger, backleft, backright;
    GetVehicleParamsCarWindows(vehicle, &driver, &passenger, &backleft, &backright);

    env->SetIntField(vehicleParams, fidDriver, driver);
    env->SetIntField(vehicleParams, fidPassenger, passenger);
    env->SetIntField(vehicleParams, fidBackLeft, backleft);
    env->SetIntField(vehicleParams, fidBackRight, backright);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehicleParamsSirenState(JNIEnv *, jclass,
                                                                                             jint vehicle) {
    return GetVehicleParamsSirenState(vehicle);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getVehiclePoolSize(JNIEnv *, jclass) {
    return GetVehiclePoolSize();
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isActorInvulnerable(JNIEnv *, jclass, jint actor) {
    return IsActorInvulnerable(actor);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isActorStreamedIn(JNIEnv *, jclass, jint actor,
                                                                                    jint player) {
    return IsActorStreamedIn(actor, player);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_isValidActor(JNIEnv *, jclass, jint actor) {
    return IsValidActor(actor);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setActorFacingAngle(JNIEnv *, jclass, jint actor,
                                                                                      jfloat angle) {
    return SetActorFacingAngle(actor, angle);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setActorHealth(JNIEnv *, jclass, jint actor,
                                                                                 jfloat health) {
    return SetActorHealth(actor, health);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setActorInvulnerable(JNIEnv *, jclass, jint actor,
                                                                                       jboolean invulnerable) {
    return SetActorInvulnerable(actor, invulnerable);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setActorPos(JNIEnv *, jclass, jint actor, jfloat x,
                                                                              jfloat y, jfloat z) {
    return SetActorPos(actor, x, y, z);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setActorVirtualWorld(JNIEnv *, jclass, jint actor,
                                                                                       jint virtualworld) {
    return SetActorVirtualWorld(actor, virtualworld);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setObjectNoCameraCol(JNIEnv *, jclass, jint object) {
    return SetObjectNoCameraCol(object);
}

JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setObjectsDefaultCameraCol(JNIEnv *, jclass,
                                                                                             jint disable) {
    SetObjectsDefaultCameraCol((bool) disable);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPlayerObjectNoCameraCol(JNIEnv *, jclass,
                                                                                             jint playerid,
                                                                                             jint object) {
    return SetPlayerObjectNoCameraCol(playerid, object);
}

JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleParamsCarDoors(JNIEnv *, jclass,
                                                                                           jint vehicle, jint driver,
                                                                                           jint passenger,
                                                                                           jint backleft,
                                                                                           jint backright) {
    SetVehicleParamsCarDoors(vehicle, (bool) driver, (bool) passenger, (bool) backleft, (bool) backright);
}

JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setVehicleParamsCarWindows(JNIEnv *, jclass,
                                                                                             jint vehicle, jint driver,
                                                                                             jint passenger,
                                                                                             jint backleft,
                                                                                             jint backright) {
    SetVehicleParamsCarWindows(vehicle, (bool) driver, (bool) passenger, (bool) backleft, (bool) backright);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_createActor(JNIEnv *, jclass, jint modelid, jfloat x,
                                                                              jfloat y, jfloat z, jfloat rotation) {
    return CreateActor(modelid, x, y, z, rotation);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPlayerPoolSize(JNIEnv *, jclass) {
    return GetPlayerPoolSize();
}

JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_unregisterFunction(JNIEnv *env, jclass,
                                                                                         jint amxHandle, jstring name) {
	assertNotNullReturn(name, "name", __FUNCTION__, false);

	auto amx = (AMX *) amxHandle;
    auto functionName = env->GetStringUTFChars(name, nullptr);
    auto functionString = std::string(functionName);
    env->ReleaseStringUTFChars(name, functionName);
    return (jboolean) AmxInstanceManager::GetInstance().UnregisterFunction(amx, functionString);
}

JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_registerFunction(JNIEnv *env, jclass,
                                                                                       jint amxHandle, jstring name,
                                                                                       jobjectArray classes) {
	assertNotNullReturn(name, "name", __FUNCTION__, false); assertNotNullReturn(classes, "classes", __FUNCTION__, false);

    static auto validClasses = {"s", "i", "f", "f[]", "i[]"};

	auto amx = (AMX *) amxHandle;
	auto functionName = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), name, 64);
    auto functionString = std::string(functionName);
    delete[] functionName;
    if (AmxInstanceManager::GetInstance().RegisteredFunctionExists(amx, functionString))
        return (jboolean) false;
    const int arrayLength = env->GetArrayLength(classes);
    std::vector<std::string> classNames;
    for (auto i = 0; i < arrayLength; i++) {
        auto string = (jstring) env->GetObjectArrayElement(classes, i);
	    auto str_className = env->GetStringUTFChars((jstring) string, nullptr);
        auto className = std::string(str_className);
        if (std::find(validClasses.begin(), validClasses.end(), className) == validClasses.end()) {
            sampgdk_logprintf("[SHOEBILL] %s is not a valid class type for registering a function (%s).", str_className,
                              functionString.c_str());
            env->ReleaseStringUTFChars(string, str_className);
            return (jboolean) false;
        }
        classNames.push_back(className);
        env->ReleaseStringUTFChars(string, str_className);
    }
    AmxInstanceManager::GetInstance().RegisterFunction(amx, functionString, classNames);
    return (jboolean) true;
}

JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPVarInt(JNIEnv *env, jclass, jint playerid,
                                                                             jstring name, jint value) {
	assertNotNull(name, "name", __FUNCTION__);

	auto str_varname = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(playerid), name, 64);
    SetPVarInt(playerid, str_varname, value);
    delete[] str_varname;
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPVarInt(JNIEnv *env, jclass, jint playerid,
                                                                             jstring name) {
	assertNotNullReturn(name, "name", __FUNCTION__, 0);

	auto str_varname = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(playerid), name, 64);
    auto result = GetPVarInt(playerid, str_varname);
    delete[] str_varname;
    return result;
}

JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPVarString(JNIEnv *env, jclass,
                                                                                jint playerid, jstring name,
                                                                                jstring value) {
	assertNotNull(name, "name", __FUNCTION__); assertNotNull(value, "value", __FUNCTION__);

	auto codepage = Shoebill::GetInstance().getPlayerCodepage(playerid);
	auto str_varname = wcs2mbs(env, codepage, name, 64);
	auto str_value = wcs2mbs(env, codepage, value, 1024);
    SetPVarString(playerid, str_varname, str_value);
    delete[] str_varname;
    delete[] str_value;
}

JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPVarString(JNIEnv *env, jclass,
                                                                                   jint playerid,
                                                                                   jstring name) {
	assertNotNullReturn(name, "name", __FUNCTION__, NULL);

	auto codepage = Shoebill::GetInstance().getPlayerCodepage(playerid);
	auto str_varname = wcs2mbs(env, codepage, name, 64);
    char value[1024];
    GetPVarString(playerid, str_varname, value, 1024);
    delete[](str_varname);
    return mbs2wcs(env, codepage, value, 1024);
}

JNIEXPORT void JNICALL Java_net_gtaun_shoebill_SampNativeFunction_setPVarFloat(JNIEnv *env, jclass,
                                                                               jint playerid, jstring name,
                                                                               jfloat value) {
	assertNotNull(name, "name", __FUNCTION__);

	auto str_varname = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(playerid), name, 64);
    SetPVarFloat(playerid, str_varname, value);
    delete[] str_varname;
}

JNIEXPORT jfloat JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPVarFloat(JNIEnv *env, jclass,
                                                                                 jint playerid,
                                                                                 jstring name) {
	assertNotNullReturn(name, "name", __FUNCTION__, 0);

	auto str_varname = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(playerid), name, 64);
    auto result = GetPVarFloat(playerid, str_varname);
    delete[] str_varname;
    return result;
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_deletePVar(JNIEnv *env, jclass, jint playerid,
                                                                             jstring name) {
	assertNotNullReturn(name, "name", __FUNCTION__, 0);

	auto str_varname = wcs2mbs(env, Shoebill::GetInstance().getPlayerCodepage(playerid), name, 64);
    auto result = DeletePVar(playerid, str_varname);
    delete[] str_varname;
    return result;
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPVarsUpperIndex(JNIEnv *, jclass,
                                                                                     jint playerid) {
    return GetPVarsUpperIndex(playerid);
}

JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPVarNameAtIndex(JNIEnv *env, jclass,
                                                                                        jint playerid,
                                                                                        jint index) {
    char varName[1024];
    GetPVarNameAtIndex(playerid, index, varName, 1024);
    return mbs2wcs(env, Shoebill::GetInstance().getPlayerCodepage(playerid), varName, 1024);
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getPVarType(JNIEnv *env, jclass,
                                                                              jint playerid, jstring name) {
	assertNotNullReturn(name, "name", __FUNCTION__, 0);

	auto str_varname = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), name, 64);
    auto result = GetPVarType(playerid, str_varname);
    delete[] str_varname;
    return result;
}

JNIEXPORT jint JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getConsoleVarAsInt(JNIEnv *env, jclass,
                                                                                     jstring name) {
	assertNotNullReturn(name, "name", __FUNCTION__, 0);

	auto str_varname = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), name, 64);
    auto result = GetServerVarAsInt(str_varname);
    delete[] str_varname;
    return result;
}

JNIEXPORT jboolean JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getConsoleVarAsBool(JNIEnv *env, jclass,
                                                                                          jstring name) {
	assertNotNullReturn(name, "name", __FUNCTION__, false);

	auto str_varname = wcs2mbs(env, Shoebill::GetInstance().getServerCodepage(), name, 64);
    auto result = GetServerVarAsBool(str_varname);
    delete[] str_varname;
    return (jboolean) result;
}

JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_getConsoleVarAsString(JNIEnv *env, jclass,
                                                                                           jstring name) {
	assertNotNullReturn(name, "name", __FUNCTION__, NULL);

	auto codepage = Shoebill::GetInstance().getServerCodepage();
	auto str_name = wcs2mbs(env, codepage, name, 64);
    char result[1024];
    GetServerVarAsString(str_name, result, 1024);
    delete[] str_name;
    return mbs2wcs(env, codepage, result, 1024);
}

JNIEXPORT jstring JNICALL Java_net_gtaun_shoebill_SampNativeFunction_sha256Hash(JNIEnv *env, jclass, jstring jPassword,
                                                                                jstring jSalt) {
	assertNotNullReturn(jPassword, "jPassword", __FUNCTION__, NULL); assertNotNullReturn(jSalt, "jSalt", __FUNCTION__, NULL);

	auto codepage = Shoebill::GetInstance().getServerCodepage();
	auto str_password = wcs2mbs(env, codepage, jPassword, 1024);
	auto str_salt = wcs2mbs(env, codepage, jSalt, 1024);

    char hash[1024];
	auto hash_len = 0;
    SHA256_PassHash(str_password, str_salt, hash, hash_len);

    delete[] str_password;
    delete[] str_salt;

    return mbs2wcs(env, codepage, hash, 1024);
}