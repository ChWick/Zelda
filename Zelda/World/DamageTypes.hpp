#ifndef _DAMAGE_TYPES_HPP_
#define _DAMAGE_TYPES_HPP_


enum EDamageType {
  DMG_NONE	= 0,
  DMG_WORLD	= 1,
  DMG_SWORD   = 2,
  DMG_ARROW   = 4,
  DMG_HAMMER  = 8,
  DMG_BOMB    = 16,
  DMG_FIRE    = 32,

  DMG_ALL     = 511,                  //!< Flag for take/block all damage types
};

#endif // _DAMAGE_TYPES_HPP_
