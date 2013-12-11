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
