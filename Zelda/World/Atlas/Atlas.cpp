#include "Atlas.hpp"
#include "Map.hpp"

CAtlas::CAtlas(CEntity *pParent)
  : CEntity("atlas", pParent),
    m_pCurrentMap(nullptr),
    m_pNextMap(nullptr) {

  m_pCurrentMap = new CMap(this, CMapPackPtr(new CMapPack("../maps/Atlases/TestMap/Map/", "Map")));
}
CAtlas::~CAtlas() {
}
