#ifndef _ATLAS_HPP_
#define _ATLAS_HPP_

#include "../../Common/GameLogic/Entity.hpp"

class CMap;

class CAtlas : public CEntity {
private:
  CMap *m_pCurrentMap;
  CMap *m_pNextMap;

public:
  CAtlas(CEntity *pParent);
  ~CAtlas();
};

#endif // _ATLAS_HPP_
