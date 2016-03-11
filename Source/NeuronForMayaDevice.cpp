#include <stdlib.h>

#include <maya/MGlobal.h>

#include <api_macros.h>
#include <maya/MIOStream.h>

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MPxThreadedDeviceNode.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MArrayDataBuilder.h>

#include "NeuronForMayaDevice.h"
#include "NeuronForMayaCmd.h"


//queue<FrameData>    NeuronForMayaDevice::frameBuffur;
FrameData           NeuronForMayaDevice::curFrameData;
MSpinLock           NeuronForMayaDevice::spinLock;

bool    NeuronForMayaDevice::bLive = false;
bool    NeuronForMayaDevice::bRecord = false;

MTypeId NeuronForMayaDevice::id( 0x00081051 );

MObject NeuronForMayaDevice::inputIp;
MObject NeuronForMayaDevice::inputPort;
MObject NeuronForMayaDevice::inputRecord;

MObject NeuronForMayaDevice::outputTranslate;
MObject NeuronForMayaDevice::outputTranslations;
MObject NeuronForMayaDevice::outputRotations;



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

    createMemoryPools( 24, 1, sizeof(FrameData));
}


void NeuronForMayaDevice::threadHandler()
{
    MStatus status;
    setDone( false );
    while ( ! isDone() )
    {
        // Skip processing if we are not live
        if ( ! isLive() )
            continue;

        MCharBuffer buffer;
        status = acquireDataStorage(buffer);
        if ( ! status )
            continue;

        beginThreadLoop();
        {
            FrameData* frameData = reinterpret_cast<FrameData*>(buffer.ptr());

            // add mutex here
            spinLock.lock();
            frameData->nFrame = curFrameData.nFrame;
            for(UINT32 i = 0; i < 60; ++i )
                for( UINT32 j = 0; j< 6; j++ )
                    frameData->data[i][j] = curFrameData.data[i][j];

            spinLock.unlock();
            pushThreadData( buffer );
        }
        endThreadLoop();
    }
    setDone( true );
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


MStatus NeuronForMayaDevice::initialize()
{
    MStatus status;
    MFnNumericAttribute numAttr;
    MFnTypedAttribute tAttr;

    MFnStringData fnStringIp;
    MObject stringIp = fnStringIp.create("127.0.0.1");
    inputIp = tAttr.create("inputIp", "ii", MFnData::kString, stringIp, &status);
    MCHECKERROR(status, "create input Ip");
    tAttr.setWritable(true);
    ADD_ATTRIBUTE(inputIp)

        inputPort = numAttr.create("inputPort", "ip", MFnNumericData::kInt, 7001, &status );
    MCHECKERROR(status, "create input Port");
    numAttr.setWritable(true);
    numAttr.setReadable(false);
    //numAttr.setConnectable(false);
    ADD_ATTRIBUTE(inputPort)


        inputRecord = numAttr.create("record", "ird", MFnNumericData::kBoolean, false, &status );
    MCHECKERROR(status, "create input Record");
    numAttr.setWritable(true);
    numAttr.setConnectable(false);
    ADD_ATTRIBUTE(inputRecord)


        outputTranslate = numAttr.create("outputTranslate", "ot", MFnNumericData::k3Float, 0.0, &status);
    MCHECKERROR(status, "create outputTranslate");
    numAttr.setHidden(true);
    ADD_ATTRIBUTE(outputTranslate);


    outputTranslations = numAttr.create("outputTranslations", "ots", MFnNumericData::k3Float, 0.0, &status);
    MCHECKERROR(status, "create outputTranslations");
    numAttr.setArray(true);
    numAttr.setUsesArrayDataBuilder(true); 
    ADD_ATTRIBUTE(outputTranslations);


    outputRotations = numAttr.create("outputRotations", "ors", MFnNumericData::k3Float, 0.0, &status);
    MCHECKERROR(status, "create outputRotations");
    numAttr.setArray(true);
    numAttr.setUsesArrayDataBuilder(true); 
    ADD_ATTRIBUTE(outputRotations);


    ATTRIBUTE_AFFECTS( live, outputTranslate);
    ATTRIBUTE_AFFECTS( frameRate, outputTranslate);
    ATTRIBUTE_AFFECTS( inputIp, outputTranslate);
    ATTRIBUTE_AFFECTS( inputPort, outputTranslate);

    ATTRIBUTE_AFFECTS( live, outputTranslations);
    ATTRIBUTE_AFFECTS( frameRate, outputTranslations);
    ATTRIBUTE_AFFECTS( inputIp, outputTranslations);
    ATTRIBUTE_AFFECTS( inputPort, outputTranslations);

    ATTRIBUTE_AFFECTS( live, outputRotations);
    ATTRIBUTE_AFFECTS( frameRate, outputRotations);
    ATTRIBUTE_AFFECTS( inputIp, outputRotations);
    ATTRIBUTE_AFFECTS( inputPort, outputRotations);

    ATTRIBUTE_AFFECTS( outputTranslate, outputTranslations);
    ATTRIBUTE_AFFECTS( outputTranslate, outputRotations);

    return MS::kSuccess;
}


MStatus NeuronForMayaDevice::compute( const MPlug& plug, MDataBlock& block )
{
    MStatus status;
    if( plug == outputTranslate || plug.parent() == outputTranslate ||
        plug == outputTranslations ||  plug.parent() == outputTranslations ||
        plug == outputRotations || plug.parent() == outputRotations )
    {
        bLive = isLive();

        MCharBuffer buffer;
        if ( popThreadData(buffer) )
        {
            FrameData* curData = reinterpret_cast<FrameData*>(buffer.ptr());
            printf( "current frame is %d \n ",  curData->nFrame);
            MArrayDataHandle translationsHandle = block.outputArrayValue( outputTranslations, &status );
            MCHECKERROR(status, "Error in block.outputArrayValue for outputTranslations");

            MArrayDataBuilder translationsBuilder = translationsHandle.builder( &status );
            MCHECKERROR(status, "Error in translationsBuilder = translationsHandle.builder.\n");

            MArrayDataHandle rotationsHandle = block.outputArrayValue( outputRotations, &status );
            MCHECKERROR(status, "Error in block.outputArrayValue for outputRotations");

            MArrayDataBuilder rotationsBuilder = rotationsHandle.builder( &status );
            MCHECKERROR(status, "Error in rotationsBuilder = rotationsHandle.builder.\n");


            for(UINT32 i=0; i< 60; ++i )
            {
                float3& translate = translationsBuilder.addElement(i, &status).asFloat3();
                MCHECKERROR(status, "ERROR in translate = translationsBuilder.addElement.\n");
                translate[0] = curData->data[i][0];
                translate[1] = curData->data[i][1];
                translate[2] = curData->data[i][2];

                float3& rotate = rotationsBuilder.addElement(i, &status).asFloat3();
                MCHECKERROR(status, "ERROR in translate = translationsBuilder.addElement.\n");
                rotate[0] = curData->data[i][3];
                rotate[1] = curData->data[i][4];
                rotate[2] = curData->data[i][5];
            }
            status = translationsHandle.set(translationsBuilder);
            MCHECKERROR(status, "set translationsBuilder failed\n");

            status = rotationsHandle.set(rotationsBuilder);
            MCHECKERROR(status, "set rotationsBuilder failed\n");

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


//void 
//    NeuronForMayaDevice::myCommandDataReceived(void* customedObj, SOCKET_REF sender, CommandPack* pack, void* data)
//{
//    MGlobal::displayInfo("myCommandDataReceived");
//}


void 
    NeuronForMayaDevice::myFrameDataReceived(void* customedObj, SOCKET_REF sender, BvhDataHeader* header, float* data)
{
    if( !bLive )
        return;

    BOOL withDisp = header->WithDisp;
    BOOL withReference = header->WithReference;
    UINT32 count = header->DataCount;

    static int nFrame = 0;
    // add mutex 
    spinLock.lock();
    // push the data for each frame into a queue
    curFrameData.nFrame = nFrame++;
    for(UINT32 i = 0; i < 60; ++i )
        for( UINT32 j = 0; j< 6; j++ )
            curFrameData.data[i][j] = data[i*6+j]; 

    spinLock.unlock();
}

void
    NeuronForMayaDevice::mySocketStatusChanged(void* customedObj, SOCKET_REF sender, SocketStatus status, char* message)
{
    MGlobal::displayInfo("mySocketStatusChanged");

}
