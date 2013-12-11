#ifndef CHATABLEINTERFACE_H
#define CHATABLEINTERFACE_H

#include "InteractionInterface.h"

//! Data for chats with persons or signs
class ChatableInterface : public InteractionInterface {
private:
    Ogre::String m_sChatText[ID_COUNT];                       //!< possible text if the player interacts with this person
    lua_State *m_pLuaState;
    bool m_bDeleteLuaState;
public:
    ChatableInterface();
    virtual ~ChatableInterface();


	void setChatText(const Ogre::String &sChat, EInteractDirection id = ID_ALL);
	void setChatScript(const Ogre::String &sLuaScript, const Ogre::String &sResourceGroup);
	void setChatLuaState(lua_State *pLuaState);

	virtual bool interactOnKeypress(EInteractType eInteractType, const Ogre::Vector3 &vInteractDir = Ogre::Vector3::ZERO);

    virtual const Ogre::Vector3 &getPosition() const = 0;
	virtual const Ogre::Quaternion &getOrientation() const = 0;
protected:
private:
    Ogre::String getChatText(EInteractDirection id);
    Ogre::String getLuaChatText(EInteractDirection id);
};

#endif // CHATABLEINTERFACE_H
