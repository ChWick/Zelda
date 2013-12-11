#include "StdAfx.h"
#include "InnerObjectContainerInterface.h"

InnerObjectContainerInterface::InnerObjectContainerInterface()
: m_plInnerObject(&InnerObjectGenerator::IOLT_EMPTY) {
    //ctor
}

InnerObjectContainerInterface::~InnerObjectContainerInterface()
{
    //dtor
}
CPickableObject *InnerObjectContainerInterface::createInnerObject(CMap &map, const Ogre::Vector3 &vPosition) {
    return InnerObjectGenerator::createInnerObject(map, vPosition, *m_plInnerObject);
}
