//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

////////////////////////////////////////////////////////////////////////
//
// NeuronForMayaCmd.cc
//		- NeuronForMayaCmd objects by name
//
//     EGs:  doPick curveShape1
//           doPick "curveShape*"
//
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>
#include "NeuronForMayaCmd.h"
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>


SOCKET_REF NeuronForMayaCmd::socketInfo = NULL;

NeuronForMayaCmd::~NeuronForMayaCmd() {}

void* NeuronForMayaCmd::creator()
{
	return new NeuronForMayaCmd();
}
	
MStatus NeuronForMayaCmd::doIt( const MArgList& args )
{
    MStatus status;

    bool start = false;
    // Parse the arguments.
    for ( int i = 0; i < args.length(); i++ )
    {
        if ( MString( "-s" ) == args.asString( i, &status )
            && MS::kSuccess == status )
        {
            bool tmp = args.asBool( ++i, &status );
            if ( MS::kSuccess == status )
                start = tmp;
        }
        else
        {
            MString msg = "Invalid flag: ";
            msg += args.asString( i );
            displayError( msg );
            return MS::kFailure;
        }
    }

    if( start ){
        // to register the 3 callback to fetch data from Neuron
        BRRegisterFrameDataCallback(NULL, NeuronForMayaDevice::myFrameDataReceived );
        BRRegisterCommandDataCallback(NULL, NeuronForMayaDevice::myCommandDataReceived );
        BRRegisterSocketStatusCallback (NULL, NeuronForMayaDevice::mySocketStatusChanged );

        socketInfo = BRConnectTo("127.0.0.1", 7001);
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
