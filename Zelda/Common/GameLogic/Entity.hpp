/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

#include "../Config/TypeDefines.hpp"
#include SPATIAL_VECTOR_HEADER
#include "../Message/MessageInjector.hpp"
#include <string>
#include <list>
#include "../tinyxml2/tinyxml2.h"
#include "OutputStyle.hpp"
#include "EntityStates.hpp"
#include <OgreResourceGroupManager.h>
#include <mutex>

namespace events {
  class CEvent;
};

namespace Ogre {
  class SceneNode;
};

class btCollisionObject;

class CMessage;

//! Class for an arbitrary entity
class CEntity : public CMessageInjector {
protected:
  std::mutex mMutex;                          //!< mutex of the entity, when data is changed/accessed
  std::string m_sID;                          //!< id of the entity
  std::string m_sResourceGroup;               //!< in which resource group to search for entities resources
  unsigned int m_uiType;                      //!< one can set a type
  EEntityStateTypes m_eState;                 //!< state of the entity

  bool m_bPauseRender;		                    //!< if true the entity and its children will not be rendered anymore
  bool m_bPauseUpdate;                        //!< if true the entity and its children will not be updated anymore

  CEntity *m_pParent;
  std::list<CEntity*> m_lChildren;
  std::list<events::CEvent*> m_lEvents;
  std::list<events::CEvent*> m_lEventsToDelete;
public:
  CEntity(const std::string &sID, unsigned int uiType, CEntity *pParent, const std::string &sResourceGroup = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
  CEntity(const std::string &sID, CEntity *pParent, const std::string &sResourceGroup = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
  CEntity(
	  CEntity *pParent,
	  const tinyxml2::XMLElement *pElem, const std::string &sResourceGroup = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
  CEntity(const CEntity &src);
  virtual ~CEntity();

  // tree access
  void attachTo(CEntity *pParent);
  CEntity *getParent() {return m_pParent;}
  const CEntity *getParent() const {return m_pParent;}
  CEntity *getParent(const std::string &sID);
  const CEntity *getParent(const std::string &sID) const;
  std::list<CEntity *> &getChildren() {return m_lChildren;}
  const std::list<CEntity *> &getChildren() const {return m_lChildren;}
  CEntity *getRoot();
  const CEntity *getRoot() const;
  CEntity *getChild(const std::string &sID);
  CEntity *getChildRecursive(const std::string &sID);
  const CEntity *getChild(const std::string &sID) const ;
  const CEntity *getChildRecursive(const std::string &sID) const;
  void destroyChildren();
  void destroy();
  void deleteLater();
  void deleteNow();

  void sendCallToAll(void (CEntity::*pFunction)(), bool bCallThis = true);
  void sendCallToAllChildrenFirst(void (CEntity::*pFunction)(), bool bCallThis = true);

  // events access
  std::list<events::CEvent*> &getEvents() {return m_lEvents;}
  const std::list<events::CEvent*> &getEvents() const {return m_lEvents;}
  void addEvent(events::CEvent *pEvent) {m_lEvents.push_back(pEvent);}
  void destroyEvent(events::CEvent *pEvent);

  // messages access
  void sendMessageToAll(const CMessage &message);

  // methods
  virtual void init();
  virtual void exit();

  virtual void start() {}
  virtual void stop() {}


  virtual bool frameStarted(const Ogre::FrameEvent& evt);
  virtual void update(Ogre::Real tpf);
  virtual void preRender(Ogre::Real tpf);
  virtual void render(Ogre::Real tpf);
  virtual void renderDebug(Ogre::Real tpf);
  virtual bool frameEnded(const Ogre::FrameEvent& evt);

  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

  virtual void writeToXMLElement(tinyxml2::XMLElement *pElement, EOutputStyle eStyle) const;

  void clearEvents();
  void readEventsFromXMLElement(const tinyxml2::XMLElement *pElement, bool bClearEvents = true);

  // getter setter
  // =============

  // general
  std::mutex &getMutex() {return mMutex;}

  const std::string &getID() const {return m_sID;}
  std::string &getID() {return m_sID;}
  void setID(const std::string &sID) {m_sID = sID;}

  //! Resource group in which the assigned resources are searched for
  const std::string &getResourceGroup() const {return m_sResourceGroup;}

  virtual void changeState(EEntityStateTypes eState);
  EEntityStateTypes getState() const {return m_eState;}

  void setPauseRender(bool bPause) {m_bPauseRender = bPause;}
  bool isRenderPaused() const {return m_bPauseRender;}

  void setPauseUpdate(bool bPause) {m_bPauseUpdate = bPause;}
  bool isUpdatePaused() const {return m_bPauseUpdate;}

  virtual const SPATIAL_VECTOR &getPosition() const {return SPATIAL_VECTOR::ZERO;}
  virtual void setPosition(const SPATIAL_VECTOR &vPos) {}
  virtual void translate(const SPATIAL_VECTOR &vOffset) {setPosition(getPosition() + vOffset);}

  virtual const SPATIAL_VECTOR &getCenter() const {return getPosition();}
  virtual void setCenter(const SPATIAL_VECTOR &vCenter) {setPosition(vCenter);}

  virtual const SPATIAL_VECTOR &getSize() const {return SPATIAL_VECTOR::ZERO;}
  virtual void setSize(const SPATIAL_VECTOR &vSize) {}

  virtual const SPATIAL_VECTOR &getScale() const {return SPATIAL_VECTOR::UNIT_SCALE;}
  virtual void setScale(const SPATIAL_VECTOR &vScale) {}

	virtual const Ogre::Quaternion &getOrientation() const {return Ogre::Quaternion::IDENTITY;}
	virtual void setOrientation(const Ogre::Quaternion &quat) {}

  unsigned int getType() const {return m_uiType;}
  void setType(unsigned int uiType) {m_uiType = uiType;}

  virtual void moveToTarget(const SPATIAL_VECTOR &vPosition, const Ogre::Quaternion &qRotation = Ogre::Quaternion::IDENTITY, const Ogre::Real fMaxDistanceDeviation = 0, const Ogre::Radian fMaxAngleDeviation = Ogre::Radian(Ogre::Math::TWO_PI));


  virtual Ogre::SceneNode *getSceneNode() const {return nullptr;}
  virtual btCollisionObject *getCollisionObject() const {return nullptr;}
  virtual bool collidesWith(const std::string &sEntityID) const {return false;}

protected:

  virtual void handleMessage(const CMessage &message);
};


#endif
