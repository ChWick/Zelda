#ifndef _DELETE_SCENE_NODE_HPP_
#define _DELETE_SCENE_NODE_HPP_

namespace Ogre {
  class SceneNode;
};

void destroySceneNode(Ogre::SceneNode* i_pSceneNode, bool destroyItself);
#endif // _DELETE_SCENE_NODE_HPP_
