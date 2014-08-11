#ifndef _REGION_HPP_
#define _REGION_HPP_

#include "../WorldEntity.hpp"

struct SRegionInfo;

class CRegion : public CWorldEntity {
public:
  CRegion(CWorldEntity *pParent, const SRegionInfo &info);
};

#endif // _REGION_HPP_
