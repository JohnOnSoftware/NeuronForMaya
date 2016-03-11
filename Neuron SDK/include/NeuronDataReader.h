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
 *   If you are using this SDK in Mac OS X, Linux or Unix *
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

#include "DataType.h"

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
*      A BvhDataHeader type pointer, contains the BVH data information.
*	data
*		Float type array pointer, to output binary data.
*Remarks
*      The related information of the data stream can be obtained from BvhDataHeader.
*/
typedef void(__stdcall *FrameDataReceived)(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data);


/* Calculation frame data received callback
*Parameters
*	customedObj
*		User defined object.
*	sender
*      Connector reference of TCP/IP client or UDP service as identity.
*	header
*      A CalcDataHeader type pointer, contains the calculation data information.
*	data
*		Float type array pointer, to output binary data.
*Remarks
*      The related information of the data stream can be obtained from CalcDataHeader.
*/
typedef void(__stdcall *CalculationDataReceived)(void* customedObj, SOCKET_REF sender, CalcDataHeader* header, float* data);


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
typedef void(__stdcall *SocketStatusChanged)(void* customedObj, SOCKET_REF sender, SocketStatus status, char* message);


/**********************************************************
 *          Export pure carbon interface/functions          *
 *  So that C# or Unity can use it directly               *
 **********************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// Register receiving and parsed frame bvh data callback
BDR_API void BRRegisterFrameDataCallback(void* customedObj, FrameDataReceived handle);

// Register receiving and parsed frame calculation data callback
BDR_API void BRRegisterCalculationDataCallback(void* customedObj, CalculationDataReceived handle);

// Register socket status callback
BDR_API void BRRegisterSocketStatusCallback(void* customedObj, SocketStatusChanged handle);

// Create a TCP/IP client to connect to server, return a handle of connector, NULL is returned if connect failed.
BDR_API SOCKET_REF BRConnectTo(char* serverIP, int nPort);

// Create a UDP service to receive data at local 'nPort'
BDR_API SOCKET_REF BRStartUDPServiceAt(int nPort);

// Close a socket
BDR_API void BRCloseSocket(SOCKET_REF sockRef);

// Check socket status
BDR_API SocketStatus BRGetSocketStatus(SOCKET_REF sockRef);

// If any error, you can call 'BRGetLastErrorMessage' to get error information
BDR_API char* BRGetLastErrorMessage();

#ifdef __cplusplus
};
#endif