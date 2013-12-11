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
