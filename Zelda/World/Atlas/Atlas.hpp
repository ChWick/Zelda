#ifndef _ATLAS_HPP_
#define _ATLAS_HPP_

#include "../WorldEntity.hpp"

class CMap;
class CAerialCameraPerspective;

class CAtlas : public CWorldEntity {
private:
  CMap *m_pCurrentMap;
  CMap *m_pNextMap;
  CWorldEntity *m_pPlayer;
  CAerialCameraPerspective* &m_pCameraPerspective;

  bool m_bSwitchingMaps;          //!< Is the map currently switch from current to next map
  bool m_bPlayerTargetReached;

public:
  CAtlas(CEntity *pParent, Ogre::SceneNode *pRootSceneNode, CWorldEntity *pPlayer, CAerialCameraPerspective* &pCameraPerspective);
  ~CAtlas();

  CMap *getCurrentMap() const {return m_pCurrentMap;}

  void update(Ogre::Real tpf);
  bool frameRenderingQueued(const Ogre::FrameEvent& evt);
  bool frameStarted(const Ogre::FrameEvent& evt);
  bool frameEnded(const Ogre::FrameEvent& evt);

protected:
  void handleMessage(const CMessage &message);
};

#endif // _ATLAS_HPP_
