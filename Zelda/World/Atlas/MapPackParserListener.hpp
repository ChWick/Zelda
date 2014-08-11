#ifndef _MAP_PACK_PARSER_LISTENER_HPP_
#define _MAP_PACK_PARSER_LISTENER_HPP_

struct SRegionInfo;

class CMapPackParserListener {
public:
  virtual void parseRegion(const SRegionInfo &) {}
};

#endif // _MAP_PACK_PARSER_LISTENER_HPP_
