#pragma once

#include <windows.h>
#include "maya/MPxCommand.h"
#include <NeuronForMayaDevice.h>

class NeuronForMayaCmd : public MPxCommand
{
public:
    NeuronForMayaCmd() {  mDeviceName=""; mStart=false;};
    virtual         ~NeuronForMayaCmd(); 

    MStatus         doIt( const MArgList& args );
    static void*    creator();

    static MSyntax  newSyntax();

private:
	MStatus			parseArgs( const MArgList& args );
    static SOCKET_REF socketInfo;

    MString         mDeviceName;
    bool            mStart;

public:
    static CRITICAL_SECTION  critical_sec; 

};
