/* Copyright: Copyright 2014 Beijing Noitom Technology Ltd. All Rights reserved.
* Pending Patents: PCT/CN2014/085659 PCT/CN2014/071006
* 
* Licensed under the Perception Neuron SDK License Beta Version (the ¡°License");
* You may only use the Perception Neuron SDK when in compliance with the License,
* which is provided at the time of installation or download, or which
* otherwise accompanies this software in the form of either an electronic or a hard copy.
* 
* Unless required by applicable law or agreed to in writing, the Perception Neuron SDK
* distributed under the License is provided on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing conditions and
* limitations under the License.
*/
#pragma once


/**********************************************************
 *               Symbol import definition                 *
 *   If you are using this sdk in Mac OS X, Linux or unix *
 * like OS, you should define '__OS_XUN__' symbol in your *
 * 'Preprocessing' config segment.                        *
 **********************************************************/

#ifdef NEURONDATAREADER_EXPORTS
#ifdef __OS_XUN__
#define BDR_API __attribute__((visibility("default")))
#else
#define BDR_API __declspec(dllexport)
#endif
#else
#ifdef __OS_XUN__
#define BDR_API // EMPTY
#else
#define BDR_API __declspec(dllimport)
#endif
#endif

/**********************************************************
 *               Customed data types definition           *
 **********************************************************/
#ifdef __OS_XUN__
// Mac OS X, Linux or unix like OS data type definetion
typedef unsigned short       UINT16;
typedef unsigned int         UINT32;
typedef unsigned long long   UINT64;
typedef unsigned short       USHORT;
typedef unsigned char        UCHAR;
typedef unsigned char        BYTE;
typedef wchar_t              WCHAR;
#ifdef NEURONDATAREADER_EXPORTS
typedef unsigned int         BOOL;
#endif
#define TRUE                 1
#define FALSE                0
#define CALLBACK             // Empty
#else
#define CALLBACK             __stdcall

#endif


// Include command communication data types
#include "SocketCommand.h"

// Reference to a tcp/ip socket. It is only used as an ID to identify what connector is used.
#define SOCKET_REF void*

// Socket status
typedef enum _SocketStatus
{
    CS_Running,              // Socket is working correctly
    CS_Starting,             // Is trying to start service
    CS_OffWork,              // Not working
}SocketStatus;

#pragma pack(push, 1)

// Data version
typedef union DataVersion
{
    UINT32 _VersionMask;
    struct
    {
        BYTE BuildNumb;      // Build number
        BYTE Revision;       // Revision number
        BYTE Minor;          // Subversion number
        BYTE Major;          // Major version number
    };
}DATA_VER;

// Header format of BVH data
typedef struct _BvhDataHeader
{
	UINT16   Token1;           // Package start token: 0xDDFF
	DATA_VER DataVersion;      // Version of community data format. e.g.: 1.0.0.2
	UINT32   DataCount;        // Values count, 180 for without disp data
	BOOL     WithDisp;         // With/out dispement
	BOOL     WithReference;    // With/out reference bone data at first
	UINT32   AvatarIndex;      // Avatar index
	UCHAR    AvatarName[32];   // Avatar name
	UINT32   Reserved1;        // Reserved, only enable this package has 64bytes length
	UINT32   Reserved2;        // Reserved, only enable this package has 64bytes length
	UINT16   Token2;           // Package end token: 0xEEFF
}BvhDataHeader;

// Header format of compressed BVH data
typedef struct _BvhDataHeaderEx
{
	UINT16   Token1;           // Package start token: 0xDDFF
	DATA_VER DataVersion;      // Version of community data format. e.g.: 1.0.0.2
	UINT32   DataCount;        // Values count
	BOOL     WithDisp;         // With/out dispement
	BOOL     WithReference;    // With/out reference bone data at first
	UINT32   AvatarIndex;      // Avatar index
	UCHAR    AvatarName[32];   // Avatar name
	UINT32   IsCompressed;     // Wether BVH data is compressed
	UINT32   Reserved1;        // Reserved, only enable this package has 64bytes length
	UINT16   Token2;           // Package end token: 0xEEFF
}BvhDataHeaderEx;

#pragma pack(pop)


/**********************************************************
 *         Data output callback types definition          *
 **********************************************************/
/* FrameDataReceived CALLBACK
*Parameters
*	customedObj
*		User defined object.
*	sender
*      Connector reference of TCP/IP client or UDP service as identity.
*	header
*      A BvhDataHeader type pointer, to output the BVH data format information.
*	data
*		Float type array pointer, to output binary data.
*Remarks
*      The related information of the data stream can be obtained from BvhDataHeader.
*/
typedef void(CALLBACK *FrameDataReceived)(void* customedObj, SOCKET_REF sender, BvhDataHeaderEx* header, float* data);

/* Callback for command communication data with TCP/IP server
*Parameters
*	customedObj
*		User defined object.
*	sender
*      Connector reference of TCP/IP client as identity.
*	pack
*      A command package send to or received from server.
*	data
*		Data pointer of command, related to the specific command.
*Remarks
*      The related information of the command data can be obtained from command identity. The data content is identified by its command id.
*/
typedef void(CALLBACK *CommandDataReceived)(void* customedObj, SOCKET_REF sender, CommandPack* pack, void* data);

/* SocketStatusChanged CALLBACK
*Parameters
*	customedObj
*		User defined object.
*	sender
*      Connector reference of TCP/IP client as identity.
*	status
*		Socket status.
*	message
*		Socket status description.
*Remarks
*      As convenient,use BRGetConnectionStatus() to get status manually other than register this callback
*/
typedef void(CALLBACK *SocketStatusChanged)(void* customedObj, SOCKET_REF sender, SocketStatus status, char* message);


/**********************************************************
 *          Export pure carbon intface/functions          *
 *  So that C# or Unity can use it directly               *
 **********************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// Register receiving and parsed frame bvh data callback
BDR_API void BRRegisterFrameDataCallback(void* customedObj, FrameDataReceived handle);

// Register socket status callback
BDR_API void BRRegisterSocketStatusCallback(void* customedObj, SocketStatusChanged handle);

// Create a TCP/IP client to connect to server, return a handle of connector, Null is returned if connect failed.
BDR_API SOCKET_REF BRConnectTo(char* serverIP, int nPort);

// Create a UDP service to receive data at local 'nPort'
BDR_API SOCKET_REF BRStartUDPServiceAt(int nPort);

// Close a socket
BDR_API void BRCloseSocket(SOCKET_REF sockRef);

// Check socket status
BDR_API SocketStatus BRGetSocketStatus(SOCKET_REF sockRef);

// If any error, you can call 'BRGetLastErrorMessage' to get error information
BDR_API char* BRGetLastErrorMessage();

/**********************************************************
*                   Commd communication                   *
**********************************************************/
// Register command data callback
BDR_API void BRRegisterCommandDataCallback(void* customedObj, CommandDataReceived handle);

// Send command to get data from server with refered command Id. command data will be received by callback.
// Return FALSE if any error occured. UDP service is not supported now.
BDR_API BOOL BRCommandFetchDataFromServer(SOCKET_REF sockRef, CmdId cmdId);

// Get avatar data from server with refered command Id. UDP service is not supported now.
BDR_API BOOL BRCommandFetchAvatarDataFromServer(SOCKET_REF sockRef, int avatarIndex, CmdId cmdId);

// Register paramemt(s) to server for automatically notifying status changed.
BDR_API BOOL BRRegisterAutoSyncParmeter(SOCKET_REF sockRef, CmdId cmdId);

// Unregister paramemt(s) to server for automatically notifying status changed.
BDR_API BOOL BRUnregisterAutoSyncParmeter(SOCKET_REF sockRef, CmdId cmdId);

#ifdef __cplusplus
};
#endif