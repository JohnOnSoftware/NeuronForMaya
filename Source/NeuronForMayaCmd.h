#pragma once

#include "maya/MPxCommand.h"

class NeuronForMayaCmd : public MPxCommand
{
public:
					NeuronForMayaCmd() {};
	virtual			~NeuronForMayaCmd(); 

	MStatus     	doIt( const MArgList& args );
	static void*	creator();
};
