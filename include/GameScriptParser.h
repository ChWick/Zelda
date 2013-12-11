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

#ifndef GAMESCRIPTPARSER_H
#define GAMESCRIPTPARSER_H

#include "StdAfx.h"
#include "HUDListener.h"
#include "CharacterControllerListener.h"

class CMap;
class InteractionInterface;

class GameScriptParser : public Ogre::Singleton<GameScriptParser>, public HUDListener, public CharacterControllerListener {
public:
    struct PauseData {
        enum EWaitFor {
            WAIT_DISABLED,
            WAIT_END_DISPLAY_TEXT,
            WAIT_TARGET_REACHED,
        };

        EWaitFor eWaitfor;
        Ogre::String sFunction;
        unsigned int uiPointer;
        void *pPointer;
    };
    struct UserPointerData {
        enum Types {
            PHYSICS_USER_POINTER,
            RIGID_BODY,
            LUA_STATE,
        };

        Types m_eType;
        void *pPointer;
    };
private:
    lua_State *lua_state;
    CMap *m_pCurrentMap;
    PauseData m_PauseData;
    Ogre::String m_sFileName;
    Ogre::String m_sResourceGroupName;
public:

    static GameScriptParser& getSingleton(void);
    static GameScriptParser* getSingletonPtr(void);

    GameScriptParser();
    virtual ~GameScriptParser();

    void parseScript(const Ogre::String &sFileName, const Ogre::String &sResourceGroupName);
    void restart();

    void callInitFunction();
    void callStartFunction();
    void callStopFunction();

    void setCurrentMap(CMap *pMap) {m_pCurrentMap = pMap;}
    CMap *getMap() {return m_pCurrentMap;}

    const Ogre::String &getScriptName() const {return m_sFileName;}
    const Ogre::String &getResourceGroupName() const {return m_sResourceGroupName;}

    void clearIdMap();
    unsigned int addPointerToMap(void *pPointer, UserPointerData::Types type);
    void *getPointerFromMap(unsigned int id) const;
    template <class T>
    T *getPointerFromMap(unsigned int id) const;
    bool containsPointer(void *pPointer) const;
    unsigned int getIDOfPointer(void *pPointer) const;
    unsigned int getIDOfPointerOrAdd(void *pPointer, UserPointerData::Types type);
    void deletePointer(unsigned int id);

    void setWaitFor(PauseData::EWaitFor eWaitFor, const Ogre::String &sFunction, unsigned int uiPointerId = 0);
    // HUD listener
    void endDisplayText();

    // character controller listener
    void targetReached(CCharacter *pChar);
protected:
private:
    void callLuaScriptFunction(const Ogre::String &func);
    void registerCFunctionsToLua();
    void registerSingleCFunctionsToLua(lua_CFunction fn, const char *label);

    unsigned int m_uiNextFreeId;
    std::map<unsigned int, UserPointerData> m_mapPointerId;
};

#endif // GAMESCRIPTPARSER_H
