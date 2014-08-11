#ifndef _MAP_PACK_HPP_
#define _MAP_PACK_HPP_

#include <string>
#include <memory>

class CMapPackParserListener;

class CMapPack {
private:
  const std::string m_sPath;
  const std::string m_sName;
  const std::string m_sResourceGroup;

  bool m_bInitialized;


  std::string m_sSceneFile;
  CMapPackParserListener *m_pListener;
public:
  CMapPack(const std::string &path, const std::string &name);
  ~CMapPack();

  void init(CMapPackParserListener *pListener);
  void exit();

  const std::string &getPath() const {return m_sPath;}
  const std::string &getName() const {return m_sName;}
  const std::string &getResourceGroup() const {return m_sResourceGroup;}

  const std::string &getSceneFile() const {return m_sSceneFile;}

private:
  void parseXMLFile();
};

typedef std::shared_ptr<CMapPack> CMapPackPtr;
typedef std::shared_ptr<const CMapPack> CMapPackPtrConst;

#endif // _MAP_PACK_HPP_
