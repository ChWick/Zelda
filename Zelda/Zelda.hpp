#ifndef _ZELDA_HPP_
#define _ZELDA_HPP_

#include "Common/Game.hpp"

class CZelda : public CGame {
protected:
  void chooseGameState();

  virtual bool keyPressed( const OIS::KeyEvent &arg );

};

#endif // _ZELDA_HPP_
