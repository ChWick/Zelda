#ifndef _ATLAS_HPP_
#define _ATLAS_HPP_

#include "../WorldEntity.hpp"

class CMap;

class CAtlas : public CWorldEntity {
private:
  CMap *m_pCurrentMap;
  CMap *m_pNextMap;
  CWorldEntity *m_pPlayer;

public:
  CAtlas(CEntity *pParent, Ogre::SceneNode *pRootSceneNode, CWorldEntity *pPlayer);
  ~CAtlas();

  CMap *getCurrentMap() const {return m_pCurrentMap;}

protected:
  void handleMessage(const CMessage &message);
};

#endif // _ATLAS_HPP_
