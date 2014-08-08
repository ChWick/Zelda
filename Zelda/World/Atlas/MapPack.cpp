#include "MapPack.hpp"
#include <OgreResourceGroupManager.h>
#include <tinyxml2.h>
#include "../../Common/Util/XMLHelper.hpp"
#include <OgreLogManager.h>

using namespace tinyxml2;
using namespace XMLHelper;

CMapPack::CMapPack(const std::string &path, const std::string &name)
  : m_sPath(path),
    m_sName(name),
    m_sResourceGroup(name),
    m_bInitialized(false) {
}

CMapPack::~CMapPack() {
  exit();
}

void CMapPack::init() {
  if (m_bInitialized) {return;}
  m_bInitialized = true;

  Ogre::ResourceGroupManager::getSingleton().createResourceGroup(m_sResourceGroup);
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(m_sPath + m_sName + ".zip", "Zip", m_sResourceGroup);
  Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(m_sResourceGroup);
  Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(m_sResourceGroup);

  parseXMLFile();
}

void CMapPack::exit() {
  if (!m_bInitialized) {return;}
  m_bInitialized = false;

  Ogre::ResourceGroupManager::getSingleton().unloadResourceGroup(m_sResourceGroup);
  Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(m_sResourceGroup);
}

void CMapPack::parseXMLFile() {
  Ogre::DataStreamPtr dataStream
    = Ogre::ResourceGroupManager::getSingleton().openResource(m_sName + ".xml", m_sResourceGroup);

  if (dataStream.isNull()) {
    throw Ogre::Exception(0, "File " + m_sName + ".zip not found in resource group " + m_sResourceGroup + "!", __FILE__);
  }

  Ogre::LogManager::getSingleton().logMessage("Reading map xml file.");

  XMLDocument doc;
  doc.Parse(dataStream->getAsString().c_str());

  XMLElement *pElem = doc.FirstChildElement();
  m_sSceneFile = Attribute(pElem, "scene");
}
