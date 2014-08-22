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

#ifndef _PHYSICS_MASKS_HPP_
#define _PHYSICS_MASKS_HPP_


enum ECollisionTypes {
  COL_NOTHING     = 0,    //!< Collide with nothing
  COL_STATIC      = 1,    //!< Collide with static objects
  COL_WALL        = 2,    //!< Collides with walls (for detection of borders)
	COL_CAMERA      = 4,   //!< Camera collision object
	COL_INTERACTIVE = 8,   //!< Collide with interactive


  COL_CHARACTER_P = 16,   //!< Friendly character (Positive)
  COL_CHARACTER_N = 32,   //!< Enemy character (Negative)
	COL_SHIELD_P    = 64,   //!< Collide with shield (only ray cast!)
	COL_SHIELD_N    = 128,  //!<
	COL_DAMAGE_P    = 256,  //!< Damage
	COL_DAMAGE_N    = 512,  //!<
};

const unsigned int MASK_NONE = COL_NOTHING;

const unsigned int MASK_PLAYER_P_COLLIDES_WITH = COL_STATIC | COL_WALL | COL_INTERACTIVE | COL_CHARACTER_P | COL_CHARACTER_N | COL_DAMAGE_N;
const unsigned int MASK_PLAYER_N_COLLIDES_WITH = COL_STATIC | COL_WALL | COL_INTERACTIVE | COL_CHARACTER_P | COL_CHARACTER_N | COL_DAMAGE_P;

const unsigned int MASK_PICKABLE_OBJECT_COLLIDES_WITH = COL_STATIC | COL_WALL;

const unsigned int MASK_BLOCKADE_COLLIDES_WITH = COL_CHARACTER_P;

const unsigned int MASK_STATIC_COLLIDES_WITH = COL_CHARACTER_N | COL_CHARACTER_P | COL_CAMERA | COL_DAMAGE_P | COL_DAMAGE_N;
const unsigned int MASK_INTERACIVE_OBJECT_COLLIDES_WITH = COL_CHARACTER_N | COL_CHARACTER_P | COL_DAMAGE_P | COL_DAMAGE_N;

const unsigned int MASK_SHIELD_P_COLLIDES_WITH = COL_DAMAGE_N;
const unsigned int MASK_SHIELD_N_COLLIDES_WITH = COL_DAMAGE_P;

const unsigned int MASK_CAMERA_COLLIDES_WITH = COL_STATIC | COL_WALL;
const unsigned int MASK_DAMAGE_P_COLLIDES_WITH = COL_CHARACTER_N | COL_SHIELD_N | COL_STATIC | COL_WALL | COL_INTERACTIVE;
const unsigned int MASK_DAMAGE_N_COLLIDES_WITH = COL_CHARACTER_P | COL_SHIELD_P | COL_STATIC | COL_WALL | COL_INTERACTIVE;
//int powerupCollidesWith = COL_SHIP | COL_WALL;

#endif // _PHYSICS_MASKS_HPP_
