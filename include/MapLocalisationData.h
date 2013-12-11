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

#ifndef MAPLOCALISATIONDATA_H
#define MAPLOCALISATIONDATA_H

class MapLocalisationData {
public:
    Ogre::String m_sPath;
    Ogre::String m_sName;

    MapLocalisationData() {
    }
    MapLocalisationData(const Ogre::String &sPath, const Ogre::String &sName)
        : m_sPath(sPath), m_sName(sName) {
        m_sPath = sPath;
        validate();
    }
    MapLocalisationData(const MapLocalisationData &src)
        : m_sPath(src.m_sPath), m_sName(src.m_sName) {
        validate();
    }
    void operator=(const MapLocalisationData &src) {
        m_sPath = src.m_sPath;
        m_sName = src.m_sName;
    }
    bool operator==(const MapLocalisationData &comp) const {
        return m_sName == comp.m_sName && m_sPath == comp.m_sPath;
    }

    void validate() {
        if (boost::algorithm::ends_with(m_sPath, "/") == false) {
            m_sPath.append("/");
        }
    }
};


#endif
