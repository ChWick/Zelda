#ifndef _MAP_HPP_
#define _MAP_HPP_

#include "../../Common/GameLogic/Entity.hpp"
#include "../../Common/Physics/PhysicsManager.h"

class CMap : public CEntity {
private:
  CPhysicsManager m_PhysicsManager;
public:
  CMap(CEntity *pAtlas);

  void update(float tpf);
};
#endif // _MAP_HPP_
