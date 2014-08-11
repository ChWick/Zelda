#ifndef _MAP_PACK_HPP_
#define _MAP_PACK_HPP_

#include <string>
#include <memory>
#include <OgreVector3.h>
#include <OgreVector2.h>

class CMapPackParserListener;

class CMapPack {
private:
  const std::string m_sPath;
  const std::string m_sName;
  const std::string m_sResourceGroup;

  bool m_bInitialized;
  CMapPackParserListener *m_pListener;


  std::string m_sSceneFile;

  Ogre::Vector3 m_vGlobalPosition;
  Ogre::Vector2 m_vGlobalSize;
public:
  CMapPack(const std::string &path, const std::string &name);
  ~CMapPack();

  void init(CMapPackParserListener *pListener);
  void exit();

  const std::string &getPath() const {return m_sPath;}
  const std::string &getName() const {return m_sName;}
  const std::string &getResourceGroup() const {return m_sResourceGroup;}

  const Ogre::Vector3 &getGlobalPosition() const {return m_vGlobalPosition;}
  const Ogre::Vector2 &getGlobalSize() const {return m_vGlobalSize;}

  const std::string &getSceneFile() const {return m_sSceneFile;}

private:
  void parseXMLFile();
};

typedef std::shared_ptr<CMapPack> CMapPackPtr;
typedef std::shared_ptr<const CMapPack> CMapPackPtrConst;

#endif // _MAP_PACK_HPP_
