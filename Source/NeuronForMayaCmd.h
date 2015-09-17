#pragma once

#include <windows.h>
#include "maya/MPxCommand.h"
#include <NeuronForMayaDevice.h>

class NeuronForMayaCmd : public MPxCommand
{
public:
					NeuronForMayaCmd() { };
	virtual			~NeuronForMayaCmd(); 

	MStatus     	doIt( const MArgList& args );
	static void*	creator();

private:
    static SOCKET_REF socketInfo;

public:
    static CRITICAL_SECTION  critical_sec; 

};
