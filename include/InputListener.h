#ifndef _INPUT_LISTENER_H_
#define _INPUT_LISTENER_H_

#include "StdAfx.h"

class CInputListener {
private:
	bool m_bInputListenerEnabled;
public:
	CInputListener() {m_bInputListenerEnabled = true;}
	virtual ~CInputListener();
	 // OIS::KeyListener
	virtual bool keyPressed( const OIS::KeyEvent &arg ) {return true;}
    virtual bool keyReleased( const OIS::KeyEvent &arg ) {return true;}
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg ) {return true;}
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {return true;}
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {return true;}

	bool isInputListenerEnabled() const {return m_bInputListenerEnabled;}
	void setInputListenerEnabled(bool bEnable) {m_bInputListenerEnabled = bEnable;}
};

class CInputListenerManager : public Ogre::Singleton<CInputListenerManager>, public OIS::KeyListener, public OIS::MouseListener {
private:
	Ogre::list<CInputListener*>::type m_ListenerList;
public:
	void addInputListener(CInputListener *pListener) {
		m_ListenerList.push_back(pListener);
	}
	void removeInputListener(CInputListener *pListener) {
		m_ListenerList.remove(pListener);
	}
	bool hasInputListener(CInputListener *pListener) {
		for (auto l : m_ListenerList) {
			if (l == pListener) {
				return true;
			}
		}
		return false;
	}

	static CInputListenerManager& getSingleton(void);
    static CInputListenerManager* getSingletonPtr(void);
	 // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg ) {
        for (auto pListener : m_ListenerList) {
			if (pListener->isInputListenerEnabled()) {
				if (!pListener->keyPressed(arg)) {
					break;
				}
			}
		}
		return true;
	}
    virtual bool keyReleased( const OIS::KeyEvent &arg ) {
        for (auto pListener : m_ListenerList) {
			if (pListener->isInputListenerEnabled()) {
				if (!pListener->keyReleased(arg)) {
					break;
				}
			}
		}
		return true;
	}
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg ) {
        for (auto pListener : m_ListenerList) {
			if (pListener->isInputListenerEnabled()) {
				if (!pListener->mouseMoved(arg)) {
					break;
				}
			}
        }
		return true;
	}
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
        for (auto pListener : m_ListenerList) {
			if (pListener->isInputListenerEnabled()) {
				if (!pListener->mousePressed(arg, id)) {
					break;
				}
			}
        }
		return true;
	}
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
        for (auto pListener : m_ListenerList) {
			if (pListener->isInputListenerEnabled()) {
				if (!pListener->mouseReleased(arg, id)) {
					break;
				}
			}
        }
		return true;
	}
};


#endif //_INPUT_LISTENER_H_
