#include "StdAfx.h"
#include "Fader.h"

 using namespace Ogre;

 Fader::Fader(const char *OverlayName, const char *MaterialName, FaderCallback *instance)
 {
     _fadeop = FADE_NONE;
     _alpha = 0.0;
     _inst = instance;

     // Get the material by name
     Ogre::ResourcePtr resptr = Ogre::MaterialManager::getSingleton().getByName(MaterialName);
     Ogre::Material * mat = dynamic_cast<Ogre::Material*>(resptr.getPointer());

     Ogre::Technique *tech = mat->getTechnique(0);    // Get the technique
     Ogre::Pass *pass = tech->getPass(0);            // Get the pass
     _tex_unit = pass->getTextureUnitState(0);        // Get the texture_unit state

     // Get the _overlay
     _overlay = Ogre::OverlayManager::getSingleton().getByName(OverlayName);
     _overlay->hide();

 }

 Fader::~Fader(void)
 {
 }

 void Fader::startFadeIn(double duration )
 {
     if( duration < 0 )
         duration = -duration;
     if( duration < 0.000001 )
         duration = 1.0;

     _alpha = 1.0;
     _total_dur = duration;
     _current_dur = duration;
     _fadeop = FADE_IN;
     _overlay->show();
 }

 void Fader::startFadeOut(double duration )
 {
     if( duration < 0 )
         duration = -duration;
     if( duration < 0.000001 )
         duration = 1.0;

     _alpha = 0.0;
     _total_dur = duration;
     _current_dur = 0.0;
     _fadeop = FADE_OUT;
     _overlay->show();
 }

 void Fader::fade(double timeSinceLastFrame)
 {
     if( _fadeop != FADE_NONE && _tex_unit )
     {
         // Set the _alpha value of the _overlay
         _tex_unit->setAlphaOperation(LBX_MODULATE, LBS_MANUAL, LBS_TEXTURE, _alpha);    // Change the _alpha operation

         // If fading in, decrease the _alpha until it reaches 0.0
         if( _fadeop == FADE_IN )
         {
             _current_dur -= timeSinceLastFrame;
             _alpha = _current_dur / _total_dur;
             if( _alpha < 0.0 )
             {
                 _overlay->hide();
                 _fadeop = FADE_NONE;
                 if( _inst )
                     _inst->fadeInCallback();
             }
         }

         // If fading out, increase the _alpha until it reaches 1.0
         else if( _fadeop == FADE_OUT )
         {
             _current_dur += timeSinceLastFrame;
             _alpha = _current_dur / _total_dur;
             if( _alpha > 1.0 )
             {
                 _fadeop = FADE_NONE;
                 if( _inst )
                     _inst->fadeOutCallback();
             }
         }
     }
 }
