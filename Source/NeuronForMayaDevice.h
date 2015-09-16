#pragma once

#include "maya/MPxThreadedDeviceNode.h"
//#include "neuronForMayaAdapter.h"
#include "maya/MDagPath.h"
#include "maya/MFnCamera.h"


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

public:

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

};