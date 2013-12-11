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
