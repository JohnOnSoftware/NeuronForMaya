#pragma once

#pragma pack(push,1)
/**********************************************************
*                   command data structure                *
**********************************************************/
// Command identitys
typedef enum _CommandIdentity
{
	Cmd_BoneSize,                      // Id can be used to request bone size from server or register avatar name command.
	Cmd_AvatarName,                    // Id can be used to request avatar name from server or register avatar name command.
	Cmd_FaceDirection,                 // Id used to request face direction from server
	Cmd_DataFrequency,                 // Id can be used to request data frequency from server or register data frequency command.
	Cmd_BvhInheritanceTxt,        	   // Id can be used to request bvh header txt from server or register bvh header txt command.
	Cmd_AvatarCount,				   // Id can be used to request avatar count from server or register avatar count command.
	Cmd_CombinationMode,               // Id can be used to request combination mode from server or register combination mode command.
	Cmd_RegisterEvent,                 // Id can be used to register event.
	Cmd_UnRegisterEvent,               // Id can be used to unregister event.
}CmdId;

// Sensor binding combination mode
typedef enum _SensorCombinationModes
{
	SC_ArmOnly,                        // Left arm or right arm only
	SC_UpperBody,                      // Upper body, include one arm or both arm, must have chest node
	SC_FullBody,                       // Full body mode
}SensorCombinationModes;

// Header format of Command returned from server
typedef struct _CommandPack
{
	UINT16 Token1;                     // Command start token: 0xAAFF
	UINT32 DataVersion;                // Version of community data format. e.g.: 1.0.0.2
	UINT32 DataLength;                 // Package length of command data, by byte.
	UINT32 DataCount;                  // Count in data array, related to the specific command.
	CmdId  CommandId;                  // Identity of command.
	UCHAR  CmdParaments[40];           // Parameters depend on identity.
	UINT32 Reserved1;                  // Reserved, only enable this package has 32bytes length. Maybe used in the future.
	UINT16 Token2;                     // Package end token: 0xBBFF
}CommandPack;


// Fetched bone size from server
typedef struct _CmdResponseBoneSize
{
	UCHAR BoneName[60];               // Bone name
	float BoneLength;                 // Bone length
}CmdResponseBoneSize;

#pragma pack(pop)

