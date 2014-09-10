#ifndef PAUSE_MANAGER_HPP
#define PAUSE_MANAGER_HPP

#include <OgreSingleton.h>
#include <list>
#include <vector>

class CPauseCaller;
class CPauseListener;

class CPauseManager : public Ogre::Singleton<CPauseManager> {
private:
  std::list<CPauseCaller *> m_lPauseCallers;
  std::list<CPauseListener *> m_lPauseListeners;

  unsigned int m_uiOldPauseFlags;
  bool m_bNeedsUpdate;
public:
  static CPauseManager& getSingleton(void);
  static CPauseManager* getSingletonPtr(void);

  CPauseManager();

  void update();

  void addCaller(CPauseCaller *pCaller) {m_lPauseCallers.push_back(pCaller);}
  void removeCaller(CPauseCaller *pCaller) {m_lPauseCallers.remove(pCaller);}

  void addListener(CPauseListener *pListener);
  void removeListener(CPauseListener *pListener) {m_lPauseListeners.remove(pListener);}

  bool isPause(unsigned int uiFlags) {return (m_uiOldPauseFlags & uiFlags) == uiFlags;}

  void needUpdate() {m_bNeedsUpdate = true;}
};

#endif // PAUSE_MANAGER_HPP
