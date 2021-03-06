//----------------------------------------------------------------------------//
// AnimationExportSheet.cpp                                                   //
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
#include "AnimationExportSheet.h"

//----------------------------------------------------------------------------//
// Message mapping                                                            //
//----------------------------------------------------------------------------//

BEGIN_MESSAGE_MAP(CAnimationExportSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CAnimationExportSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

CAnimationExportSheet::CAnimationExportSheet(CBaseInterface* iface, UINT nIDCaption, CWnd *pParentWnd, UINT iSelectPage)
: CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
, m_skeletonHierarchyPage(iface)
{
	FillPages();
}

CAnimationExportSheet::CAnimationExportSheet(CBaseInterface* iface, LPCTSTR pszCaption, CWnd *pParentWnd, UINT iSelectPage)
: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
, m_skeletonHierarchyPage(iface)
{
	FillPages();
}

//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//

CAnimationExportSheet::~CAnimationExportSheet()
{
}

//----------------------------------------------------------------------------//
// Fill in all propoerty pages                                                //
//----------------------------------------------------------------------------//

void CAnimationExportSheet::FillPages()
{
	// add all property pages to this sheet
	m_skeletonFilePage.SetStep(1, 3);
	m_skeletonFilePage.SetDescription(IDS_ANIMATION_EXPORT_DESCRIPTION);
	AddPage(&m_skeletonFilePage);

	m_skeletonHierarchyPage.SetStep(2, 3);
	m_skeletonHierarchyPage.SetDescription(IDS_ANIMATION_EXPORT_DESCRIPTION_2);
	AddPage(&m_skeletonHierarchyPage);

	m_animationTimePage.SetStep(3, 3);
	m_animationTimePage.SetDescription(IDS_ANIMATION_EXPORT_DESCRIPTION_3);
	AddPage(&m_animationTimePage);
}

//----------------------------------------------------------------------------//
// Get the displacement in frames                                             //
//----------------------------------------------------------------------------//

int CAnimationExportSheet::GetDisplacement()
{
	return m_animationTimePage.GetDisplacement();
}

//----------------------------------------------------------------------------//
// Get the end frame                                                          //
//----------------------------------------------------------------------------//

int CAnimationExportSheet::GetEndFrame()
{
	return m_animationTimePage.GetEndFrame();
}

//----------------------------------------------------------------------------//
// Get the frames per second (Fps)                                            //
//----------------------------------------------------------------------------//

int CAnimationExportSheet::GetFps()
{
	return m_animationTimePage.GetFps();
}

//----------------------------------------------------------------------------//
// Get the start frame                                                        //
//----------------------------------------------------------------------------//

int CAnimationExportSheet::GetStartFrame()
{
	return m_animationTimePage.GetStartFrame();
}

//----------------------------------------------------------------------------//
// Set the animation time values                                              //
//----------------------------------------------------------------------------//

void CAnimationExportSheet::SetAnimationTime(int startFrame, int endFrame, int displacementFrame, int fps)
{
	m_animationTimePage.SetAnimationTime(startFrame, endFrame, displacementFrame, fps);
}

//----------------------------------------------------------------------------//
// Set the node hierarchy                                                     //
//----------------------------------------------------------------------------//

void CAnimationExportSheet::SetSkeletonCandidate(CSkeletonCandidate *pSkeletonCandidate)
{
	m_skeletonFilePage.SetSkeletonCandidate(pSkeletonCandidate);
	m_skeletonHierarchyPage.SetSkeletonCandidate(pSkeletonCandidate);
}

//----------------------------------------------------------------------------//
