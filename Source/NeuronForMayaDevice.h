#pragma once

#include <windows.h>
#include <vector>
#include "maya/MPxThreadedDeviceNode.h"
#include "maya/MDagPath.h"
#include "maya/MFnCamera.h"
#include <NeuronDataReader.h>
#include <queue>
#include <map>

using namespace std;

struct FrameData{
    int nFrame;
    float data[60][6];
};



//map<string, UINT32> SkeletonMap;

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

    static MObject		outputTranslate;
    static MObject 		outputTranslateX;
    static MObject		outputTranslateY;
    static MObject 		outputTranslateZ;

    static MObject      outputLeftCameraPosition;
    static MObject      outputLeftCameraPositionX;
    static MObject      outputLeftCameraPositionY;
    static MObject      outputLeftCameraPositionZ;


    static MObject      outputLeftCameraRotation;
    static MObject      outputLeftCameraRotationX;
    static MObject      outputLeftCameraRotationY;
    static MObject      outputLeftCameraRotationZ;


    static MObject      outputRightCameraPosition;
    static MObject      outputRightCameraPositionX;
    static MObject      outputRightCameraPositionY;
    static MObject      outputRightCameraPositionZ;


    static MObject      outputRightCameraRotation;
    static MObject      outputRightCameraRotationX;
    static MObject      outputRightCameraRotationY;
    static MObject      outputRightCameraRotationZ;

    static MTypeId		id;



private:
    static MStatus createOculusAttribute(  MObject& obj, MObject& objX, MObject& objY, MObject& objZ, const MString& longName, const MString& shortName, bool hidden = false );

    static queue<FrameData>    frameBuffur;
    static bool                bRecord;
    static bool                bLive;
};