//
// Copyright (C) Autodesk
// 
// File: pluginMain.cpp
//
// Author: 
//


#include "NeuronForMayaDevice.h"
#include "NeuronForMayaCmd.h"

#include <maya/MFnPlugin.h>

MStatus initializePlugin( MObject obj )
//
//	Description:
//		this method is called when the plug-in is loaded into Maya.  It 
//		registers all of the services that this plug-in provides with 
//		Maya.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "Autodesk", "2016", "Any");

	//status = plugin.registerNode( "MayaToOculus", MayaToOculus::id, MayaToOculus::creator,
	//							  MayaToOculus::initialize );
	//if (!status) {
	//	status.perror("registerNode");
	//	return status;
	//}

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
//
//	Description:
//		this method is called when the plug-in is unloaded from Maya. It 
//		deregisters all of the services that it was providing.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
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


    //status = plugin.deregisterNode( MayaToOculus::id );
    //if (!status) {
    //    status.perror("deregisterNode");
    //    return status;
    //}

	return status;
}
