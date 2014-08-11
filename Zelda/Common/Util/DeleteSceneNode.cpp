#include "DeleteSceneNode.hpp"
#include <OgreSceneNode.h>
#include <OgreMovableObject.h>
#include <OgreSceneManager.h>

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
