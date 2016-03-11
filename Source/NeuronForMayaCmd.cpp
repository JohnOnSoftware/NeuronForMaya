#include <maya/MIOStream.h>
#include "NeuronForMayaCmd.h"
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>


#define kStartFlag       "-s"
#define kStartFlagLong   "-start"

#define kDeviceNameFlag       "-dn"
#define kDeviceNameFlagLong   "-device name"

SOCKET_REF NeuronForMayaCmd::socketInfo = NULL;

NeuronForMayaCmd::~NeuronForMayaCmd() {}

void* NeuronForMayaCmd::creator()
{
	return new NeuronForMayaCmd();
}

MSyntax
NeuronForMayaCmd::newSyntax()
{
    MSyntax syntax;
    syntax.addFlag(kStartFlag, kStartFlagLong, MSyntax::kBoolean);
    syntax.addFlag(kDeviceNameFlag, kDeviceNameFlagLong, MSyntax::kString );
    return syntax;
}


MStatus
NeuronForMayaCmd::parseArgs( const MArgList& args )
{
    MStatus status = MStatus::kSuccess;
    MArgDatabase argData(syntax(), args);

    if (argData.isFlagSet(kStartFlag))
        status = argData.getFlagArgument(kStartFlag, 0, mStart);

    if( argData.isFlagSet(kDeviceNameFlag))
        status = argData.getFlagArgument(kDeviceNameFlag, 0, mDeviceName );

    return status;

}

MStatus NeuronForMayaCmd::doIt( const MArgList& args )
{
    MStatus status;

    status = parseArgs( args ); 
    if( status != MStatus::kSuccess)
    {
        MGlobal::displayError( "parameters are not correct." );
        return status;
    }

    MSelectionList sl;
    sl.add( mDeviceName );
    MObject deviceObj;
    status = sl.getDependNode(0, deviceObj );
    if(status != MStatus::kSuccess )
    {
        MGlobal::displayError("Please create your device first.");
        return status;
    }
    MFnDependencyNode fnDevice(deviceObj);
    MString ip = fnDevice.findPlug( "inputIp", &status ).asString();
    int     port = fnDevice.findPlug("inputPort", &status).asInt();


    if( mStart ){
        // to register the 3 callback to fetch data from Neuron
        BRRegisterFrameDataCallback(NULL, NeuronForMayaDevice::myFrameDataReceived );
        //BRRegisterCommandDataCallback(NULL, NeuronForMayaDevice::myCommandDataReceived );
        BRRegisterSocketStatusCallback (NULL, NeuronForMayaDevice::mySocketStatusChanged );

        socketInfo = BRConnectTo(const_cast<char*> (ip.asChar()), port);
        if(socketInfo == NULL )
            MGlobal::displayError("Failed to connect to device.");
    }
    else
    {
        // stop socket
        BRCloseSocket( socketInfo);
    }
    
    return MStatus::kSuccess;
}
