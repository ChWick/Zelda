#ifndef _CHARACTER_CREATOR_HPP_
#define _CHARACTER_CREATOR_HPP_

class CWorldEntity;
class CMap;

namespace tinyxml2 {
  class XMLElement;
};

class CCharacterCreator {
public:
  //! Creates a character based on a xml element.
  /**
    * @return the newly created world entity or a nullptr if the character type is unknown or an error occurred
    */
  static CWorldEntity* createCharacter(const tinyxml2::XMLElement *pElem, CWorldEntity *pParent, CMap *pMap, CWorldEntity *pPlayer);
};

#endif // _CHARACTER_CREATOR_HPP_
