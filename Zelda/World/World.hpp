#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "../Common/GameLogic/GameState.hpp"
#include <OgrePrerequisites.h>

class CPlayer;
class CAtlas;
class CAerialCameraPerspective;

class CWorld : public CGameState {
private:
  CPlayer *m_pPlayer;
  CAtlas *m_pAtlas;
  Ogre::Camera *m_pWorldCamera;
  CAerialCameraPerspective *m_pCameraPerspective;
public:
  CWorld();
  ~CWorld();

  void update(Ogre::Real tpf);
  bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
};

#endif // _WORLD_HPP_
