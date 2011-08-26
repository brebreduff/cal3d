//****************************************************************************//
// coreskeleton.cpp                                                           //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cal3d/error.h"
#include "cal3d/coreskeleton.h"
#include "cal3d/corebone.h"

CalCoreSkeleton::CalCoreSkeleton()
    : coreBones(m_coreBones)
{}

size_t CalCoreSkeleton::addCoreBone(const CalCoreBonePtr& coreBone) {
    size_t boneId = coreBones.size();
    m_coreBones.push_back(coreBone);

    if (coreBone->parentId == -1) {
        rootBoneIds.push_back(boneId);
    }

    mapCoreBoneName(boneId, coreBone->name);
    return boneId;
}

CalCoreBone* CalCoreSkeleton::getCoreBone(const std::string& strName) {
    return coreBones[getCoreBoneId(strName)].get();
}

size_t CalCoreSkeleton::getCoreBoneId(const std::string& strName) {
    //Check to make sure the mapping exists
    if (m_mapCoreBoneNames.count(strName) <= 0) {
        CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
        return -1;
    }

    return m_mapCoreBoneNames[strName];
}

bool CalCoreSkeleton::mapCoreBoneName(size_t coreBoneId, const std::string& strName) {
    //Make sure the ID given is a valid corebone ID number
    if (coreBoneId >= coreBones.size()) {
        return false;
    }

    //Add the mapping or overwrite an existing mapping
    m_mapCoreBoneNames[strName] = coreBoneId;

    return true;
}

void CalCoreSkeleton::scale(float factor) {
    for (
        std::vector<CalCoreBonePtr>::const_iterator i = coreBones.begin();
        i != coreBones.end();
        ++i
    ) {
        (*i)->scale(factor);
    }
}
