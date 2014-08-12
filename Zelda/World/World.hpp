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

  enum ECameraEdgeRays {
    RAY_TOP_LEFT = 0,
    RAY_TOP_RIGHT,
    RAY_BOTTOM_LEFT,
    RAY_BOTTOM_RIGHT,
    RAY_COUNT
  };

  Ogre::Vector3 m_vEdgeRayDirections[RAY_COUNT];

public:
  CWorld();
  ~CWorld();

  void update(Ogre::Real tpf);
  bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
  void setupEdgeRays();
  Ogre::Vector3 getRayPlaneHitPosition(int iRayIndex, Ogre::Real fPlaneY = 0) const;
};

#endif // _WORLD_HPP_
