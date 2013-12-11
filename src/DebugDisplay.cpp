#include "StdAfx.h"
#include "DebugDisplay.h"

template<> CDebugDisplay* Ogre::Singleton<CDebugDisplay>::msSingleton = 0;

CDebugDisplay* CDebugDisplay::getSingletonPtr(void)
{
    return msSingleton;
}
CDebugDisplay& CDebugDisplay::getSingleton(void)
{  
    assert( msSingleton );  return ( *msSingleton );
}
