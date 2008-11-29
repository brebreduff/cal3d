//****************************************************************************//
// matrix.h                                                                   //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#pragma once

#include "cal3d/global.h"
#include "cal3d/vector.h"

class CalQuaternion;

class CAL3D_API CalMatrix
{
public:
    float dxdx,dydx,dzdx;
    float dxdy,dydy,dzdy;
    float dxdz,dydz,dzdz;
	
    inline CalMatrix()
        : dxdx(0.0), dydx(0.0), dzdx(0.0)
	, dxdy(0.0), dydy(0.0), dzdy(0.0)
        , dxdz(0.0), dydz(0.0), dzdz(0.0)
    {
    }

    CalMatrix(const CalQuaternion &q);

    inline CalMatrix(float weight, const CalMatrix &m)
    {
        dxdx = m.dxdx*weight;
        dxdy = m.dxdy*weight;
        dxdz = m.dxdz*weight;
        dydx = m.dydx*weight;
        dydy = m.dydy*weight;
        dydz = m.dydz*weight;
        dzdx = m.dzdx*weight;
        dzdy = m.dzdy*weight;
        dzdz = m.dzdz*weight;
    }
	
    void operator=(const CalQuaternion& q);

    inline void operator=(const CalMatrix& m)
    {
        dxdx=m.dxdx; dxdy=m.dxdy; dxdz=m.dxdz;
        dydx=m.dydx; dydy=m.dydy; dydz=m.dydz;
        dzdx=m.dzdx; dzdy=m.dzdy; dzdz=m.dzdz;
    }
    inline void operator *= (const CalMatrix &m)
    {
        float ndxdx=m.dxdx*dxdx+m.dxdy*dydx+m.dxdz*dzdx;
        float ndydx=m.dydx*dxdx+m.dydy*dydx+m.dydz*dzdx;
        float ndzdx=m.dzdx*dxdx+m.dzdy*dydx+m.dzdz*dzdx;
    	
        float ndxdy=m.dxdx*dxdy+m.dxdy*dydy+m.dxdz*dzdy;
        float ndydy=m.dydx*dxdy+m.dydy*dydy+m.dydz*dzdy;
        float ndzdy=m.dzdx*dxdy+m.dzdy*dydy+m.dzdz*dzdy;
    	
        float ndxdz=m.dxdx*dxdz+m.dxdy*dydz+m.dxdz*dzdz;
        float ndydz=m.dydx*dxdz+m.dydy*dydz+m.dydz*dzdz;
        float ndzdz=m.dzdx*dxdz+m.dzdy*dydz+m.dzdz*dzdz;
    	
        dxdx=ndxdx;
        dydx=ndydx;
        dzdx=ndzdx;
        dxdy=ndxdy;
        dydy=ndydy;
        dzdy=ndzdy;
        dxdz=ndxdz;
        dydz=ndydz;
        dzdz=ndzdz;
    }

    inline void operator *= (float factor)
    {
        dxdx *= factor;
        dydx *= factor;
        dzdx *= factor;
        dxdy *= factor;
        dydy *= factor;
        dzdy *= factor;
        dxdz *= factor;
        dydz *= factor;
        dzdz *= factor;
    }
    inline void blend(float factor, const CalMatrix& m)
    {
        dxdx += m.dxdx*factor;
        dydx += m.dydx*factor;
        dzdx += m.dzdx*factor;
        dxdy += m.dxdy*factor;
        dydy += m.dydy*factor;
        dzdy += m.dzdy*factor;
        dxdz += m.dxdz*factor;
        dydz += m.dydz*factor;
        dzdz += m.dzdz*factor;
    }

    inline float det()
    {
        return dxdx * (dydy*dzdz-dydz*dzdy)
              -dxdy* ( dydx*dzdz-dzdx*dydz)
	      +dxdz* (dydx*dzdy-dzdx*dydy);
    }
};

__forceinline void transform(CalVector& v, const CalMatrix& m) {
  float ox = v.x;
  float oy = v.y;
  float oz = v.z;
  v.x = m.dxdx*ox + m.dxdy*oy + m.dxdz*oz;
  v.y = m.dydx*ox + m.dydy*oy + m.dydz*oz;
  v.z = m.dzdx*ox + m.dzdy*oy + m.dzdz*oz;
}
