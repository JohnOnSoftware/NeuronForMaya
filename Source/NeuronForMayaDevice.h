#pragma once

#include <windows.h>
#include <vector>
#include "maya/MPxThreadedDeviceNode.h"
#include "maya/MDagPath.h"
#include "maya/MFnCamera.h"
#include <NeuronDataReader.h>
#include <maya/MSpinLock.h>
//#include <queue>

using namespace std;

struct FrameData{
    int nFrame;
    float data[60][6];
};



class NeuronForMayaDevice : public MPxThreadedDeviceNode
{

public:
    NeuronForMayaDevice();
    virtual 			~NeuronForMayaDevice();

    virtual void		postConstructor();
    virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
    virtual void		threadHandler();
    virtual void		threadShutdownHandler();

    static void*		creator();
    static MStatus		initialize();


    // 3 callbacks to fetch data from Neuron
    static void myFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeaderEx* header, float* data); 
    static void myCommandDataReceived(void* customedObj, SOCKET_REF sender, CommandPack* pack, void* data); 
    static void mySocketStatusChanged(void* customedObj, SOCKET_REF sender, SocketStatus status, char* message); 


public:

    static MObject      inputIp;
    static MObject      inputPort;
    static MObject      inputRecord;

    static MObject      outputTranslate;
    static MObject      outputTranslations;
    static MObject      outputRotations;


    static MTypeId      id;
    static MSpinLock    spinLock;

private:
    //static queue<FrameData>    frameBuffur;
    static FrameData           curFrameData;
    static bool                bRecord;
    static bool                bLive;
};
