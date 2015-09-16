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

#include <Windows.h>
#include <maya/MIOStream.h>
#include "NeuronForMayaCmd.h"
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <NeuronDataReader.h>


NeuronForMayaCmd::~NeuronForMayaCmd() {}

void* NeuronForMayaCmd::creator()
{
	return new NeuronForMayaCmd();
}
	
MStatus NeuronForMayaCmd::doIt( const MArgList& args )
{



	return MStatus::kSuccess;
}
