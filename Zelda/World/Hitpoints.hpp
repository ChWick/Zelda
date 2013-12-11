/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

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
