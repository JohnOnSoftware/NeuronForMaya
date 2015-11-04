#include "NeuronForMayaDevice.h"
#include "NeuronForMayaCmd.h"

#include <maya/MFnPlugin.h>

MStatus initializePlugin( MObject obj )
{ 
    MStatus   status;
    MFnPlugin plugin( obj, "Autodesk", "2016", "Any");

    status = plugin.registerNode( "NeuronForMayaDevice", 
        NeuronForMayaDevice::id,
        NeuronForMayaDevice::creator,
        NeuronForMayaDevice::initialize,
        MPxNode::kThreadedDeviceNode );
    if( !status ) {
        status.perror("failed to registerNode NeuronForMayaDevice");
    }

    status = plugin.registerCommand( "NeuronForMayaCmd", NeuronForMayaCmd::creator, NeuronForMayaCmd::newSyntax );
    if (!status) {
        status.perror("registerCommand");
        return status;
    }

    return status;
}

MStatus uninitializePlugin( MObject obj)
{
    MStatus   status;
    MFnPlugin plugin( obj );

    status = plugin.deregisterCommand("NeuronForMayaCmd");
    if(!status)
    {
        status.perror("deregisterCommand");
        return status;
    }

    status = plugin.deregisterNode( NeuronForMayaDevice::id );
    if( !status ) {
        status.perror("failed to deregisterNode NeuronForMayaDevice");
    }
    return status;
}
