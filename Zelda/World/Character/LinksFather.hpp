#ifndef _LINKS_FATHER_HPP_
#define _LINKS_FATHER_HPP_

#include "Person.hpp"

class CLinksFather : public CPerson {
public:
  enum ELinksFatherAnimations {
    LF_ANIM_SIT,
    LF_ANIM_WALK,

    LF_ANIM_COUNT,
  };

private:
public:
	CLinksFather(const tinyxml2::XMLElement *pElem, CEntity *pParent, CMap *pMap);

private:
  void setupAnimations();
};
#endif // _LINKS_FATHER_HPP_
