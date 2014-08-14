#ifndef _REGION_HPP_
#define _REGION_HPP_

#include "../WorldEntity.hpp"
#include "RegionInfo.hpp"

class CRegion : public CWorldEntity {
  const SRegionInfo m_Info;
public:
  CRegion(CWorldEntity *pParent, const SRegionInfo &info);
  ~CRegion();

  void exit();

  void start();
};

#endif // _REGION_HPP_
