//****************************************************************************//
// mesh.h                                                                     //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#pragma once

#include <boost/shared_ptr.hpp>
#include "cal3d/global.h"

class CalModel;
class CalCoreMesh;
class CalSubmesh;

class CAL3D_API CalMesh : public Cal::Object
{
private:
  CalModel *m_pModel;
  boost::shared_ptr<CalCoreMesh> m_pCoreMesh;
  std::vector<CalSubmesh *> m_vectorSubmesh;

public:
  CalMesh(CalModel* pModel, const boost::shared_ptr<CalCoreMesh>& pCoreMesh);
  ~CalMesh();

  const boost::shared_ptr<CalCoreMesh>& getCoreMesh() const {
      return m_pCoreMesh;
  }
  CalSubmesh *getSubmesh(int id);
  int getSubmeshCount();
  std::vector<CalSubmesh *>& getVectorSubmesh();
  void setLodLevel(float lodLevel);
  void setMaterialSet(int setId);
};
