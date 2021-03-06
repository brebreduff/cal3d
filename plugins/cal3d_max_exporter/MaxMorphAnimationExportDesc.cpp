//----------------------------------------------------------------------------//
// MaxMorphAnimationExportDesc.cpp                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "StdAfx.h"
#include "MaxMorphAnimationExportDesc.h"
#include "MaxMorphAnimationExport.h"

#if MAX_VERSION_MAJOR < 14
//  these come from stdafx.h anyway
#include "maxscrpt/maxscrpt.h"
#include "maxscrpt\Strings.h"
#include "maxscrpt\arrays.h"
#include "maxscrpt\numbers.h"
#include "maxscrpt\maxobj.h"
#include "maxscrpt\definsfn.h"
#endif


//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CMaxMorphAnimationExportDesc::CMaxMorphAnimationExportDesc()
{
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CMaxMorphAnimationExportDesc::~CMaxMorphAnimationExportDesc()
{
}

//----------------------------------------------------------------------------//
// Set the DLL instance handle of this plugin                                 //
//----------------------------------------------------------------------------//

void CMaxMorphAnimationExportDesc::SetInstance(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
}

//----------------------------------------------------------------------------//
// Following methods have to be implemented to make it a valid plugin         //
//----------------------------------------------------------------------------//

const TCHAR *CMaxMorphAnimationExportDesc::Category()
{
	return _T("Game Utilities");
}

Class_ID CMaxMorphAnimationExportDesc::ClassID()
{
	return MAX_MORPHANIMATION_EXPORT_ID;
}

const TCHAR *CMaxMorphAnimationExportDesc::ClassName()
{
	return _T("Cal3D MorphAnimation Export");
}

void *CMaxMorphAnimationExportDesc::Create(BOOL loading)
{
	return new CMaxMorphAnimationExport();
}

HINSTANCE CMaxMorphAnimationExportDesc::HInstance()
{
	return m_hInstance;
}

const TCHAR *CMaxMorphAnimationExportDesc::InternalName()
{
	return _T("Cal3D_MorphAnimation_Export");
} 

int CMaxMorphAnimationExportDesc::IsPublic()
{
	return 1;
}

SClass_ID CMaxMorphAnimationExportDesc::SuperClassID()
{
	return SCENE_EXPORT_CLASS_ID;
}


char * CMaxMorphAnimationExportDesc::GetRsrcString(long n)
{
	return NULL;
}


//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//Create a C++ function to be called by Maxscript to start the MorphAnimation exporter automatically
def_visible_primitive( ExportCalAnimMorph,	"ExportCalAnimMorph" );
Value* ExportCalAnimMorph_cf(Value** arg_list, int count)
{	
	int i								;
	char const*	Filefullpathfilename	;
	TCHAR const* wFilefullpathfilename	;
	char const*	Skeletonfullpathfilename;
	TCHAR const* wSkeletonfullpathfilename;
	Array*	BonesArray					;
	int		StartFrame					;
	int		EndFrame					;
	int		FrameOffset					;
	int		FrameRate					;

	check_arg_count(ExportCalAnimMorph, 7, count);
	type_check(arg_list[0], String	, _T("[The first argument of ExportCalAnim should be a string that is a full path name of the file to export]"));
	type_check(arg_list[1], String	, _T("[The 2nd argument of ExportCalAnim should be a string that is the fullpath name of the skeleton file]"));
	type_check(arg_list[2], Array	, _T("[The 3rd argument of ExportCalAnim should be an array of nodes to get anim from]"));
	type_check(arg_list[3], Integer	, _T("[The 4th argument of ExportCalAnim should be an integer that is the start frame number]"));
	type_check(arg_list[4], Integer	, _T("[The 5th argument of ExportCalAnim should be an integer that is the end frame number]"));
	type_check(arg_list[5], Integer , _T("[The 6th argument of ExportCalAnim should be an integer that is the frame offset]"));
	type_check(arg_list[6], Integer , _T("[The 7th argument of ExportCalAnim should be an integer that is the framerate]"));
	
	try
	{
		wFilefullpathfilename		= arg_list[0]->to_string();
        std::string wffpfn(wFilefullpathfilename, wFilefullpathfilename + _tcslen(wFilefullpathfilename));
        Filefullpathfilename = wffpfn.c_str();
		wSkeletonfullpathfilename	= arg_list[1]->to_string();
        std::string wsfpfn(wSkeletonfullpathfilename, wSkeletonfullpathfilename + _tcslen(wSkeletonfullpathfilename));
		BonesArray					= static_cast<Array*>(arg_list[2]);
		StartFrame					= arg_list[3]->to_int();
		EndFrame					= arg_list[4]->to_int();
		FrameOffset					= arg_list[5]->to_int();
		FrameRate					= arg_list[6]->to_int();

		if (! strcmp(Filefullpathfilename,""))return new Integer(1);
		if (! strcmp(Skeletonfullpathfilename,"")) return new Integer(2);

		//Does skeleton file exist ?
		FILE* _stream;
		_stream = fopen(Skeletonfullpathfilename,"r");
		if (! _stream)return new Integer(3); //Error code number 3
		fclose(_stream);

		//Get the elements of the bones array
		int ArraySize;
		ArraySize = BonesArray->size;

		if (! ArraySize) return new Integer(4);
		
		if (StartFrame < 0)	return new Integer(5);

		if (EndFrame < 0)return new Integer(6);

		if (StartFrame > EndFrame ) return new Integer(7);

		if (FrameOffset < 0) return new Integer(8);

		if (FrameRate < 0) return new Integer(9);

		INodeTab	tabnode;
		for (i=0;i<ArraySize;i++)
		{
			if (BonesArray->data[i]->is_kind_of(class_tag(MAXNode)) )
			{
				INode* _node	= 	BonesArray->data[i]->to_node();
				if (! _node)return new Integer(10);
				tabnode.Append(1,&_node);
			}
		}		
		
		AnimExportParams param(Skeletonfullpathfilename, tabnode, StartFrame, EndFrame, FrameOffset, FrameRate);

		if (CMaxMorphAnimationExport::ExportMorphAnimationFromMaxscriptCall(wFilefullpathfilename, &param))
			return new Integer(0);
		
		return new Integer(-1);
	}
	catch(...)
	{	
		//MessageBox(NULL,"Exception catched in ExportCalAnim C++ function","Error",MB_OK);
		return new Integer(-2);
	}
}
