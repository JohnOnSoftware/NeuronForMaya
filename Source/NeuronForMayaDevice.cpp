//-
// ==========================================================================
// Copyright 2011 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

// Description:
// This example demonstrates how to use a secondary thread to generate
// translate data which controls an object.
//



#include <stdlib.h>

#include <maya/MGlobal.h>

#include <api_macros.h>
#include <maya/MIOStream.h>

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MPxThreadedDeviceNode.h>
#include "NeuronForMayaDevice.h"


#define USERAMDOM


MTypeId NeuronForMayaDevice::id( 0x00081051 );
MObject NeuronForMayaDevice::outputTranslate;
MObject NeuronForMayaDevice::outputTranslateX;
MObject NeuronForMayaDevice::outputTranslateY;
MObject NeuronForMayaDevice::outputTranslateZ;

MObject NeuronForMayaDevice::outputLeftCameraPosition;
MObject NeuronForMayaDevice::outputLeftCameraPositionX;
MObject NeuronForMayaDevice::outputLeftCameraPositionY;
MObject NeuronForMayaDevice::outputLeftCameraPositionZ;


MObject NeuronForMayaDevice::outputLeftCameraRotation;
MObject NeuronForMayaDevice::outputLeftCameraRotationX;
MObject NeuronForMayaDevice::outputLeftCameraRotationY;
MObject NeuronForMayaDevice::outputLeftCameraRotationZ;

MObject NeuronForMayaDevice::outputRightCameraPosition;
MObject NeuronForMayaDevice::outputRightCameraPositionX;
MObject NeuronForMayaDevice::outputRightCameraPositionY;
MObject NeuronForMayaDevice::outputRightCameraPositionZ;

MObject NeuronForMayaDevice::outputRightCameraRotation;
MObject NeuronForMayaDevice::outputRightCameraRotationX;
MObject NeuronForMayaDevice::outputRightCameraRotationY;
MObject NeuronForMayaDevice::outputRightCameraRotationZ;

NeuronForMayaDevice::NeuronForMayaDevice() 
{}

NeuronForMayaDevice::~NeuronForMayaDevice()
{
	destroyMemoryPools();
}

void NeuronForMayaDevice::postConstructor()
{
	MObjectArray attrArray;
	attrArray.append( NeuronForMayaDevice::outputTranslate );
	setRefreshOutputAttributes( attrArray );

	// we'll be reading one set of translate x,y, z's at a time
	createMemoryPools( 24, 15, sizeof(double));
}

static double getRandomX()
{
	// rand() is not thread safe for getting
	// the same results in different threads.
	// But this does not matter for this simple
	// example.
	const double kScale = 10.0;
	double i = (double) rand();
	return ( i / RAND_MAX ) * kScale;
}

void NeuronForMayaDevice::threadHandler()
{
#ifdef DEBUG
	// Info message from a thread
	MGlobal::executeCommandOnIdle( MString("warning \"NeuronForMayaDevice::threadHandler start.\";") );
#endif
	//
	MStatus status;
	setDone( false );
	while ( ! isDone() )
	{
		// Skip processing if we
		// are not live
		if ( ! isLive() )
			continue;

		MCharBuffer buffer;
		status = acquireDataStorage(buffer);
		if ( ! status )
			continue;

		beginThreadLoop();
		{
			double* doubleData = reinterpret_cast<double*>(buffer.ptr());

            MPoint position[2];
            MVector viewDir[2];
            MVector upDir[2];
            MVector rotation[2];
            //adapter.getCameraInformation(position, viewDir, upDir, rotation );


#ifdef USERAMDOM
            int i = 0;
            doubleData[i++] = getRandomX(); doubleData[i++] = getRandomX(); doubleData[i++] = getRandomX();
            doubleData[i++] = getRandomX(); doubleData[i++] = getRandomX(); doubleData[i++] = getRandomX();

            doubleData[i++] = getRandomX(); doubleData[i++] = getRandomX(); doubleData[i++] = getRandomX();
            doubleData[i++] = getRandomX(); doubleData[i++] = getRandomX(); doubleData[i++] = getRandomX();

            doubleData[i++] = getRandomX(); doubleData[i++] = getRandomX(); doubleData[i++] = getRandomX();

#else
            int i = 0;
            doubleData[i++] = position[0].x; doubleData[i++] = position[0].y; doubleData[i++] = position[0].z;
            doubleData[i++] = rotation[0].x; doubleData[i++] = rotation[0].y; doubleData[i++] = rotation[0].z;

            doubleData[i++] = position[1].x; doubleData[i++] = position[1].y; doubleData[i++] = position[1].z;
            doubleData[i++] = rotation[1].x; doubleData[i++] = rotation[1].y; doubleData[i++] = rotation[1].z;

            doubleData[i++] = getRandomX(); doubleData[i++] = getRandomX(); doubleData[i++] = getRandomX();

#endif
            pushThreadData( buffer );
        }
        endThreadLoop();
    }
    setDone( true );
    //
#ifdef DEBUG
	// Info message from a thread
	MGlobal::executeCommandOnIdle( MString("warning \"NeuronForMayaDevice::threadHandler end.\";") );
#endif
}

void NeuronForMayaDevice::threadShutdownHandler()
{
	// Stops the loop in the thread handler
	setDone( true );
}

void* NeuronForMayaDevice::creator()
{
	return new NeuronForMayaDevice;
}


MStatus NeuronForMayaDevice::createOculusAttribute( MObject& obj, MObject& objX, MObject& objY, MObject& objZ, const MString& longName, const MString& shortName, bool hidden )
{
    MStatus status;
    MFnNumericAttribute numAttr;

    objX = numAttr.create( longName+"X", shortName+"X", MFnNumericData::kDouble, 0.0, &status);
    MCHECKERROR(status, "create " + longName+"X");
    objY = numAttr.create( longName+"Y", shortName+"Y", MFnNumericData::kDouble, 0.0, &status);
    MCHECKERROR(status, "create " + longName+"Y");
    objZ = numAttr.create( longName+"Z", shortName+"Z", MFnNumericData::kDouble, 0.0, &status);
    MCHECKERROR(status, "create " + longName+"Z");

    obj = numAttr.create(longName, shortName, objX, objY, objZ, &status);
    MCHECKERROR(status, "create "+ longName);
    numAttr.setHidden(hidden);

    ADD_ATTRIBUTE(obj);

    return MS::kSuccess;
}


MStatus NeuronForMayaDevice::initialize()
{
	MStatus status;
	//MFnNumericAttribute numAttr;

	//outputTranslateX = numAttr.create("outputTranslateX", "otx", MFnNumericData::kDouble, 0.0, &status);
	//MCHECKERROR(status, "create outputTranslateX");
	//outputTranslateY = numAttr.create("outputTranslateY", "oty", MFnNumericData::kDouble, 0.0, &status);
	//MCHECKERROR(status, "create outputTranslateY");
	//outputTranslateZ = numAttr.create("outputTranslateZ", "otz", MFnNumericData::kDouble, 0.0, &status);
	//MCHECKERROR(status, "create outputTranslateZ");

 //   outputTranslate = numAttr.create("outputTranslate", "ot", outputTranslateX, outputTranslateY, 
	//								 outputTranslateZ, &status);
	//MCHECKERROR(status, "create outputTranslate");
	//
	//ADD_ATTRIBUTE(outputTranslate);


    status = createOculusAttribute(outputTranslate, outputTranslateX, outputTranslateY, outputTranslateZ, "outputTranslate", "ot", true );

    ATTRIBUTE_AFFECTS( live, outputTranslate);
    ATTRIBUTE_AFFECTS( frameRate, outputTranslate);


    status = createOculusAttribute(outputLeftCameraPosition, outputLeftCameraPositionX, outputLeftCameraPositionY, outputLeftCameraPositionZ, "outputLeftCameraPosition", "olcp" );

    ATTRIBUTE_AFFECTS( live, outputLeftCameraPosition);
    ATTRIBUTE_AFFECTS( frameRate, outputLeftCameraPosition);

    status = createOculusAttribute(outputLeftCameraRotation, outputLeftCameraRotationX, outputLeftCameraRotationY, outputLeftCameraRotationZ, "outputLeftCameraRotation", "olcr" );

    ATTRIBUTE_AFFECTS( live, outputLeftCameraRotation);
    ATTRIBUTE_AFFECTS( frameRate, outputLeftCameraRotation);


    status = createOculusAttribute(outputRightCameraPosition, outputRightCameraPositionX, outputRightCameraPositionY, outputRightCameraPositionZ, "outputRightCameraPosition", "orcp" );

    ATTRIBUTE_AFFECTS( live, outputRightCameraPosition);
    ATTRIBUTE_AFFECTS( frameRate, outputRightCameraPosition);

    status = createOculusAttribute(outputRightCameraRotation, outputRightCameraRotationX, outputRightCameraRotationY, outputRightCameraRotationZ, "outputRightCameraRotation", "orcr" );

    ATTRIBUTE_AFFECTS( live, outputRightCameraRotation);
    ATTRIBUTE_AFFECTS( frameRate, outputRightCameraRotation);


    ATTRIBUTE_AFFECTS( outputTranslate, outputLeftCameraPosition);
    ATTRIBUTE_AFFECTS( outputTranslate, outputLeftCameraRotation);
    ATTRIBUTE_AFFECTS( outputTranslate, outputRightCameraPosition);
    ATTRIBUTE_AFFECTS( outputTranslate, outputRightCameraRotation);

    return MS::kSuccess;
}


MStatus NeuronForMayaDevice::compute( const MPlug& plug, MDataBlock& block )
{
    MStatus status;
    if( plug == outputTranslate || plug == outputTranslateX || plug == outputTranslateY || plug == outputTranslateZ 
        || plug == outputLeftCameraPosition || plug == outputLeftCameraPositionX || plug == outputLeftCameraPositionY || plug == outputLeftCameraPositionZ
        || plug == outputLeftCameraRotation || plug == outputLeftCameraRotationX || plug == outputLeftCameraRotationY || plug == outputLeftCameraRotationZ
        || plug == outputRightCameraPosition || plug == outputRightCameraPositionX || plug == outputRightCameraPositionY || plug == outputRightCameraPositionZ
        || plug == outputRightCameraRotation || plug == outputRightCameraRotationX || plug == outputRightCameraRotationY || plug == outputRightCameraRotationZ
        )
    {
        MCharBuffer buffer;
        if ( popThreadData(buffer) )
        {
            double* doubleData = reinterpret_cast<double*>(buffer.ptr());

            int i = 0;
            MDataHandle outputLeftCameraPositionHandle = block.outputValue( outputLeftCameraPosition, &status );
            MCHECKERROR(status, "Error in block.outputValue for outputLeftCameraPosition");

            double3& outputLeftCameraPosition = outputLeftCameraPositionHandle.asDouble3();
            outputLeftCameraPosition[0] = doubleData[i++];
            outputLeftCameraPosition[1] = doubleData[i++];
            outputLeftCameraPosition[2] = doubleData[i++];


            MDataHandle outputLeftCameraRotationHandle = block.outputValue( outputLeftCameraRotation, &status );
            MCHECKERROR(status, "Error in block.outputValue for outputLeftCameraRotation");

            double3& outputLeftCameraRotation = outputLeftCameraRotationHandle.asDouble3();
            outputLeftCameraRotation[0] = doubleData[i++];
            outputLeftCameraRotation[1] = doubleData[i++];
            outputLeftCameraRotation[2] = doubleData[i++];


            MDataHandle outputRightCameraPositionHandle = block.outputValue( outputRightCameraPosition, &status );
            MCHECKERROR(status, "Error in block.outputValue for outputRightCameraPosition");

            double3& outputRightCameraPosition = outputRightCameraPositionHandle.asDouble3();
            outputRightCameraPosition[0] = doubleData[i++];
            outputRightCameraPosition[1] = doubleData[i++];
            outputRightCameraPosition[2] = doubleData[i++];

            MDataHandle outputRightCameraRotationHandle = block.outputValue( outputRightCameraRotation, &status );
            MCHECKERROR(status, "Error in block.outputValue for outputRightCameraRotation");

            double3& outputRightCameraRotation = outputRightCameraRotationHandle.asDouble3();
            outputRightCameraRotation[0] = doubleData[i++];
            outputRightCameraRotation[1] = doubleData[i++];
            outputRightCameraRotation[2] = doubleData[i++];


            MDataHandle outputTranslateHandle = block.outputValue( outputTranslate, &status );
            MCHECKERROR(status, "Error in block.outputValue for outputTranslate");

            double3& outputTranslate = outputTranslateHandle.asDouble3();
            outputTranslate[0] = doubleData[i++];
            outputTranslate[1] = doubleData[i++];
            outputTranslate[2] = doubleData[i++];

            block.setClean( plug );

            releaseDataStorage(buffer);
            return ( MS::kSuccess );
        }
        else
        {
            return MS::kFailure;
        }
    }

    return ( MS::kUnknownParameter );
}



