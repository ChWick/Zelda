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

#ifndef _FADER_H_
#define _FADER_H_

#include "OgrePrerequisites.h"

 namespace Ogre {
     class TextureUnitState;
     class Overlay;
 }

 class FaderCallback
 {
 public:
     virtual void fadeInCallback(void) {}
     virtual void fadeOutCallback(void) {}
 };

 class Fader
 {
 public:
	 enum FadeOperation {
         FADE_NONE,
         FADE_IN,
         FADE_OUT,
     };
	 FadeOperation _fadeop;
 public:
     Fader(const char *OverlayName, const char *MaterialName, FaderCallback *instance = 0);
     ~Fader(void);

     void startFadeIn(double duration = 1.0f);
     void startFadeOut(double duration = 1.0f);
     void fade(double timeSinceLastFrame);

	 FadeOperation getFadeOp() const {return _fadeop;}

 protected:
     double _alpha;
     double _current_dur;
     double _total_dur;
     FaderCallback *_inst;
     Ogre::TextureUnitState *_tex_unit;
     Ogre::Overlay *_overlay;
 };

 #endif // _FADER_H_
