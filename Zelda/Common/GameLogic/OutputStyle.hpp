/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Mencus.
 *
 * Mencus is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Mencus is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Mencus. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef _OUTPUT_STYLE_HPP_
#define _OUTPUT_STYLE_HPP_

//! Enumeration for defining the style of a output to file
/**
 * \brief When saveing e.g. the map, this can be due to saving as snapshot or if the user wants to
 *        store the file. A snapshot needs all information while a map only needs the minimal information
 *        for reloading in the initial state
 */
enum EOutputStyle {
  OS_FULL,         //!< Full output to file, as needed for snapshots
  OS_MINIMAL,      //!< Minimal output to file as needed for saving maps
};

#endif
