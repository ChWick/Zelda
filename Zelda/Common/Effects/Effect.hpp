/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef _EFFECT_HPP_
#define _EFFECT_HPP_

#include <OgreNameGenerator.h>
#include <ParticleUniverseCommon.h>
#include <ParticleUniverseSystemListener.h>
#include "../World/AbstractWorldEntity.hpp"

class CEffectConstructionInfo;

class CEffect
    : public CAbstractWorldEntity,
      public ParticleUniverse::ParticleSystemListener {
 private:
  static Ogre::NameGenerator mNameGenerator;

  bool mDeleteOnFinished;
 public:
  CEffect(CAbstractWorldEntity *parent, const CEffectConstructionInfo &info);

  virtual void exit() override;
  virtual void start() override;
  virtual void stop() override;

 private:
  //! This function checks if all effects have finished and deletes
  //! itself. Only used if mDeleteOnFinished is set
  void checkIfFinished();
  
  virtual void handleParticleSystemEvent(
      ParticleUniverse::ParticleSystem* particleSystem,
      ParticleUniverse::ParticleUniverseEvent& particleUniverseEvent) override;
  
};

#endif /* _EFFECT_HPP_ */
