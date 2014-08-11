#ifndef _MAP_PACK_PARSER_LISTENER_HPP_
#define _MAP_PACK_PARSER_LISTENER_HPP_

struct SRegionInfo;

namespace tinyxml2 {
  class XMLElement;
};

class CMapPackParserListener {
public:
  virtual void parsePlayer(const tinyxml2::XMLElement *) {}
  virtual void parseRegion(const SRegionInfo &) {}
};

#endif // _MAP_PACK_PARSER_LISTENER_HPP_
