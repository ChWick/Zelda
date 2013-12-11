#include "StdAfx.h"
#include "ChatableInterface.h"
#include "HUD.h"
#include "MapManager.h"
#include "CameraController.h"
#include "TextLoader.h"

ChatableInterface::ChatableInterface()
{
    m_bDeleteLuaState = false;
    m_pLuaState = NULL;
}

ChatableInterface::~ChatableInterface()
{
    if (m_bDeleteLuaState) {
        lua_close(m_pLuaState);
        m_pLuaState = NULL;
        m_bDeleteLuaState = false;
    }
}

bool ChatableInterface::interactOnKeypress(EInteractType eInteractType, const Ogre::Vector3 &vInteractDir) {
    if (eInteractType == IT_USE) {
        Ogre::Vector3 vPos(getPosition());
        Ogre::Radian radYaw(getOrientation().getYaw());

        Ogre::String s;
        s = getChatText(getInteractDirection(vInteractDir, radYaw.valueRadians()));
        if (s.length() > 0) {
            CHUD::getSingleton().showText(s);
            CMapManager::getSingleton().getCameraController()->moveCameraToTarget(vPos - vInteractDir * 3 + Ogre::Vector3::UNIT_Y * 1, vPos);
        }


        return true;
    }

	// default for liftable object, lift it, throw it, etc...
	return InteractionInterface::interactOnKeypress(eInteractType, vInteractDir);
}
void ChatableInterface::setChatText(const Ogre::String &sChat, EInteractDirection id) {
    if (id == ID_ALL) {
        for (int i = 0; i < ID_COUNT; i++) {
            m_sChatText[i] = sChat;
        }
    }
    else {
        m_sChatText[id] = sChat;
    }
}
void ChatableInterface::setChatLuaState(lua_State *pLuaState) {
    m_pLuaState = pLuaState;
    m_bDeleteLuaState = false;
}
void ChatableInterface::setChatScript(const Ogre::String &sLuaScript, const Ogre::String &sResourceGroup) {
    Ogre::LogManager::getSingleton().logMessage("Parsing lua script: " + sLuaScript);

    assert(!m_pLuaState);
    m_pLuaState = luaL_newstate();

    // load Lua libraries

    static const luaL_Reg lualibs[] =
    {
        {"base", luaopen_base},
        {"io", luaopen_io},
        {NULL, NULL}
    };
    const luaL_Reg *lib = lualibs;
    for(; lib->func != NULL; lib++)
    {
        luaL_requiref(m_pLuaState, lib->name, lib->func, 1);
        lua_settop(m_pLuaState, 0);
    }

    // Strip the path
    Ogre::String basename, path;
    Ogre::StringUtil::splitFilename(sLuaScript, basename, path);

    Ogre::DataStreamPtr pStream = Ogre::ResourceGroupManager::getSingleton().openResource(basename, sResourceGroup, false);
    Ogre::String data = pStream->getAsString();

    int status = luaL_loadstring(m_pLuaState, data.c_str());
    int result = 0;
    if(status == LUA_OK) {
        result = lua_pcall(m_pLuaState, 0, LUA_MULTRET, 0);
    }
    else {
        throw Ogre::Exception(__LINE__, "Could not load the lua script", __FILE__);
    }

    m_bDeleteLuaState = true;
}
Ogre::String ChatableInterface::getChatText(EInteractDirection id) {
    Ogre::String s;
    if (m_pLuaState) {
        s = getLuaChatText(id);
    }
    else {
        s = m_sChatText[id];
    }

    CTextLoader::getSingleton().replaceVariablesInMapText(s);

    return s;
}
Ogre::String ChatableInterface::getLuaChatText(EInteractDirection id) {
    assert(m_pLuaState);
    lua_getglobal(m_pLuaState, "getNext");
    assert(lua_isfunction(m_pLuaState, -1));
    lua_pushstring(m_pLuaState, InteractionInterface::getInteractionDirectionFromId(id).c_str());
    lua_pcall(m_pLuaState, 1, 1, 0);
    Ogre::String text = lua_tostring(m_pLuaState, -1);
    lua_pop(m_pLuaState, 1);

    return text;
}
