#include "StdAfx.h"
#include "Util.h"

void destroySceneNode(Ogre::SceneNode* i_pSceneNode, bool destroyItself)
{
   if ( !i_pSceneNode )
   {
      return;
   }

   // Destroy all the attached objects
   Ogre::SceneNode::ObjectIterator itObject = i_pSceneNode->getAttachedObjectIterator();

   while (itObject.hasMoreElements())
   {
      Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(itObject.getNext());
      i_pSceneNode->getCreator()->destroyMovableObject( pObject );
   }

   // Recurse to child SceneNodes
   Ogre::SceneNode::ChildNodeIterator itChild = i_pSceneNode->getChildIterator();

   while ( itChild.hasMoreElements() )
   {
      Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
      destroySceneNode(pChildNode, false);
   }

   i_pSceneNode->removeAndDestroyAllChildren();
   if (destroyItself) {
	   i_pSceneNode->getCreator()->destroySceneNode(i_pSceneNode);
   }
}
Ogre::Vector3 getMaxBB(const Ogre::Vector3 &v1, const Ogre::Vector3 &v2) {
    using namespace std;
	return Ogre::Vector3(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z));
}
Ogre::Vector3 getMinBB(const Ogre::Vector3 &v1, const Ogre::Vector3 &v2) {
    using namespace std;
	return Ogre::Vector3(min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z));
}
