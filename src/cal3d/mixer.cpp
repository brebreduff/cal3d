//****************************************************************************//
// mixer.cpp                                                                  //
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
#include "cal3d/mixer.h"
#include "cal3d/corebone.h"
#include "cal3d/coreanimation.h"
#include "cal3d/coretrack.h"
#include "cal3d/corekeyframe.h"
#include "cal3d/skeleton.h"
#include "cal3d/bone.h"
#include "cal3d/animation.h"


void CalMixer::addManualAnimation(const CalAnimationPtr& animation) {
    activeAnimations.push_front(animation);
}

void CalMixer::removeManualAnimation(const CalAnimationPtr& animation) {
    activeAnimations.remove(animation);
}

void CalMixer::setManualAnimationAttributes(
    const CalAnimationPtr& animation,
    const AnimationAttributes& p
) {
    animation->time = p.time_;
    animation->weight = p.weight_;
    animation->scale = p.scale_;
    animation->rampValue = p.rampValue_;

    // now update composition function

    CalAnimation::CompositionFunction oldValue = animation->compositionFunction;

    // If the value isn't changing, then exit here.  Otherwise I would remove it and reinsert
    // it at the front, which wouldn't preserve the property that the most recently inserted
    // animation is highest priority.
    if (oldValue == p.compositionFunction_) {
        return;
    }
    animation->compositionFunction = p.compositionFunction_;

    activeAnimations.remove(animation);

    // Now insert it back in in the appropriate position.  Replace animations go in at the front.
    // Average animations go in after the replace animations.
    switch (p.compositionFunction_) {
        case CalAnimation::CompositionFunctionReplace: {
            // Replace animations go on the front of the list.
            activeAnimations.push_front(animation);
            break;
        }
        case CalAnimation::CompositionFunctionCrossFade: {
            // Crossfade animations go after replace, but before Average.
            std::list<CalAnimationPtr>::iterator aait2 = activeAnimations.begin();
            for (; aait2 != activeAnimations.end(); aait2++) {
                CalAnimation::CompositionFunction cf = (*aait2)->compositionFunction;
                if (cf != CalAnimation::CompositionFunctionReplace) {
                    break;
                }
            }
            activeAnimations.insert(aait2, animation);
            break;
        }
        case CalAnimation::CompositionFunctionAverage: {
            // Average animations go before the first Average animation.
            std::list<CalAnimationPtr>::iterator aait2 = activeAnimations.begin();
            for (; aait2 != activeAnimations.end(); aait2++) {
                CalAnimation::CompositionFunction cf = (*aait2)->compositionFunction;
                if (cf == CalAnimation::CompositionFunctionAverage) {  // Skip over replace and crossFade animations
                    break;
                }
            }
            activeAnimations.insert(aait2, animation);
            break;
        }
        default: {
            assert(!"Unexpected");
            break;
        }
    }
}


void CalMixer::updateSkeleton(
    CalSkeleton* skeleton,
    const std::vector<BoneTransformAdjustment>& boneTransformAdjustments,
    const std::vector<BoneScaleAdjustment>& boneScaleAdjustments
) {
    skeleton->clearState();

    std::vector<CalBone>& bones = skeleton->bones;

    // The bone adjustments are "replace" so they have to go first, giving them
    // highest priority and full influence.  Subsequent animations affecting the same bones,
    // including subsequent replace animations, will have their incluence attenuated appropriately.
    applyBoneAdjustments(skeleton, boneTransformAdjustments, boneScaleAdjustments);

    // loop through all animation actions
    for (
        std::list<CalAnimationPtr>::const_iterator itaa = activeAnimations.begin();
        itaa != activeAnimations.end();
        ++itaa
    ) {
        const CalAnimation* animation = itaa->get();

        const CalCoreAnimation::TrackList& tracks = animation->coreAnimation->tracks;

        for (
            CalCoreAnimation::TrackList::const_iterator itct = tracks.begin();
            itct != tracks.end();
            ++itct
        ) {
            if (itct->coreBoneId >= int(bones.size())) {
                continue;
            }

            // Replace and CrossFade both blend with the replace function.
            bool replace = animation->compositionFunction != CalAnimation::CompositionFunctionAverage;
            bones[itct->coreBoneId].blendState(
                animation->weight,
                itct->getState(animation->time),
                animation->scale,
                replace,
                animation->rampValue);
        }
    }

    // === What does lockState() mean?  Why do we need it at all?  It seems only to allow us
    // to blend all the animation actions together into a temporary sum, and then
    // blend all the animation cycles together into a different sum, and then blend
    // the two sums together according to their relative weight sums.  I believe this is mathematically
    // equivalent of blending all the animation actions and cycles together into a single sum,
    // according to their relative weights.
    skeleton->lockState();

    // let the skeleton calculate its final state
    skeleton->calculateState();
}

void CalMixer::applyBoneAdjustments(
    CalSkeleton* skeleton,
    const std::vector<BoneTransformAdjustment>& boneTransformAdjustments,
    const std::vector<BoneScaleAdjustment>& boneScaleAdjustments
) {
    std::vector<CalBone>& bones = skeleton->bones;

    for (size_t i = 0; i < boneTransformAdjustments.size(); ++i) {
        const BoneTransformAdjustment& ba = boneTransformAdjustments[i];
        CalBone& bo = bones[ba.boneId];
        bo.blendState(
            1.0f, /* unrampedWeight */
            cal3d::Transform(
                ba.localOri,
                bo.getOriginalTranslation() /* adjustedLocalPos */),
            1.0f, /* scale */
            true, /* replace */
            ba.rampValue /* rampValue */);
    }

    for (size_t i = 0; i < boneScaleAdjustments.size(); i++) {
        const BoneScaleAdjustment& ba = boneScaleAdjustments[i];
        bones[ba.boneId].setMeshScaleAbsolute(ba.meshScaleAbsolute);
    }
}
