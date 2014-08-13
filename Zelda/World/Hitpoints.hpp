#ifndef _HITPOINTS_HPP_
#define _HITPOINTS_HPP_

#include <limits>

typedef int Hitpoints;
enum EHitpointsConstants {
	HP_ONE_HEART	= 100,
	HP_HALF_HEART	= 50,
	HP_QUARTER_HEART = 25,
	HP_TWO_HEARTS	= HP_ONE_HEART * 2,
	HP_FULL_HEART	= 100 * 20,
	HP_INFINITY		= std::numeric_limits<Hitpoints>::max(),
	//HP_INFINITY = INT_MAX,
};

static const float DEFAULT_INVULNERABLE_TIME = 0.7f;						//!< default time for being invulnerable

#endif
