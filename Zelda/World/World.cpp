#include "World.hpp"
#include "Atlas/Atlas.hpp"

CWorld::CWorld()
  : CGameState(GST_WORLD) {
  new CAtlas(this);
}
