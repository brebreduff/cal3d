//****************************************************************************//
// coreskeleton.h                                                             //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#pragma once

#include <list>
#include <map>
#include <string>
#include "cal3d/global.h"
#include "cal3d/vector.h"

class CalCoreBone;
class CalCoreModel;

class CAL3D_API CalCoreSkeleton : public Cal::Object
{
public:
  CalCoreSkeleton();
  ~CalCoreSkeleton();

  int addCoreBone(CalCoreBone *pCoreBone);
  void calculateState();
  CalCoreBone* getCoreBone(int coreBoneId);
  CalCoreBone* getCoreBone(const std::string& strName);
  int getCoreBoneId(const std::string& strName);
  bool mapCoreBoneName(int coreBoneId, const std::string& strName);
  const std::vector<int>& getListRootCoreBoneId();
  std::vector<CalCoreBone *>& getVectorCoreBone();
  void scale(float factor);
  unsigned int getNumCoreBones() { return ( unsigned int ) m_vectorCoreBone.size(); }
  void setSceneAmbientColor( CalVector const & color );
  void getSceneAmbientColor( CalVector & color );

private:
  std::vector<CalCoreBone*> m_vectorCoreBone;
  std::map<std::string, int> m_mapCoreBoneNames;
  std::vector<int> m_listRootCoreBoneId;
  CalVector m_sceneAmbientColor;
};
