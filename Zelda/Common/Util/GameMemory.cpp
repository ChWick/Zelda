#include "GameMemory.hpp"


template<> CGameMemory *Ogre::Singleton<CGameMemory>::msSingleton = 0;

CGameMemory &CGameMemory::getSingleton() {
  assert(msSingleton);
  return *msSingleton;
}
CGameMemory *CGameMemory::getSingletonPtr() {
  return msSingleton;
}
