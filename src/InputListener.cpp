#include "StdAfx.h"
#include "InputListener.h"

CInputListener::~CInputListener() {
	if (CInputListenerManager::getSingletonPtr()) {
		CInputListenerManager::getSingleton().removeInputListener(this);
	}
}


template<> CInputListenerManager* Ogre::Singleton<CInputListenerManager>::msSingleton = 0;

CInputListenerManager* CInputListenerManager::getSingletonPtr(void)
{
    return msSingleton;
}
CInputListenerManager& CInputListenerManager::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
