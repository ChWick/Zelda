#include "StdAfx.h"
#include "GameScriptParser.h"
#include "Map.h"
#include "SimpleEnemy.h"
#include "SimpleFriend.h"
#include "HUD.h"
#include "InnerObjectContainerInterface.h"
#include "CharacterController.h"
#include "PhysicsManager.h"
#include "TextLoader.h"

const Ogre::String INIT_SCRIPT_FUNCTION = "init";
const Ogre::String START_SCRIPT_FUNCTION = "start";
const Ogre::String STOP_SCRIPT_FUNCTION = "stop";

template<> GameScriptParser* Ogre::Singleton<GameScriptParser>::msSingleton = 0;

GameScriptParser* GameScriptParser::getSingletonPtr(void)
{
    return msSingleton;
}
GameScriptParser& GameScriptParser::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}

GameScriptParser::GameScriptParser()
: m_pCurrentMap(NULL)
{
    CHUD::getSingleton().addListener(this);
    clearIdMap();

    lua_state = luaL_newstate();

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
        luaL_requiref(lua_state, lib->name, lib->func, 1);
        lua_settop(lua_state, 0);
    }

    registerCFunctionsToLua();
}

GameScriptParser::~GameScriptParser()
{
    CHUD::getSingleton().removeListener(this);
    lua_close(lua_state);
    lua_state = 0;
}
void GameScriptParser::clearIdMap() {
    m_uiNextFreeId = 0;
    m_mapPointerId.clear();
}
unsigned int GameScriptParser::addPointerToMap(void *pPointer, UserPointerData::Types type) {
    m_mapPointerId[m_uiNextFreeId].m_eType = type;
    m_mapPointerId[m_uiNextFreeId].pPointer = pPointer;
    return m_uiNextFreeId++;
}
void *GameScriptParser::getPointerFromMap(unsigned int id) const {
    return m_mapPointerId.at(id).pPointer;
}
template <class T>
T *GameScriptParser::getPointerFromMap(unsigned int id) const {
    return static_cast<T*>(getPointerFromMap(id));
}
void GameScriptParser::parseScript(const Ogre::String &sFileName, const Ogre::String &sResourceGroupName) {
    m_sFileName = sFileName;
    m_sResourceGroupName = sResourceGroupName;
    clearIdMap();
    setWaitFor(PauseData::WAIT_DISABLED, Ogre::StringUtil::BLANK);

    // Strip the path
    Ogre::String basename, path;
    Ogre::StringUtil::splitFilename(sFileName, basename, path);

    Ogre::DataStreamPtr pStream = Ogre::ResourceGroupManager::getSingleton().openResource(basename, sResourceGroupName, false);
    Ogre::String data = pStream->getAsString();

    int status = luaL_loadstring(lua_state, data.c_str());
    int result = 0;
    if(status == LUA_OK) {
        result = lua_pcall(lua_state, 0, LUA_MULTRET, 0);
    }
    else {
        throw Ogre::Exception(__LINE__, "Could not load the lua script", __FILE__);
    }
}
void GameScriptParser::restart() {
    callStopFunction();
    parseScript(m_sFileName, m_sResourceGroupName);
	callStartFunction();
}
void GameScriptParser::callInitFunction() {
    callLuaScriptFunction(INIT_SCRIPT_FUNCTION);
}
void GameScriptParser::callStartFunction() {
    callLuaScriptFunction(START_SCRIPT_FUNCTION);
}
void GameScriptParser::callStopFunction() {
    callLuaScriptFunction(STOP_SCRIPT_FUNCTION);
}
void GameScriptParser::callLuaScriptFunction(const Ogre::String &func) {
    lua_getglobal(lua_state, func.c_str()); // function to be called, without parameters and without return value!
    lua_call(lua_state,0, 0);
}
void GameScriptParser::endDisplayText() {
    if (m_PauseData.eWaitfor == PauseData::WAIT_END_DISPLAY_TEXT) {
        m_PauseData.eWaitfor = PauseData::WAIT_DISABLED;
        callLuaScriptFunction(m_PauseData.sFunction);
    }
}
void GameScriptParser::targetReached(CCharacter *pChar) {
    if (m_PauseData.eWaitfor == PauseData::WAIT_TARGET_REACHED && pChar == m_PauseData.pPointer) {
        m_PauseData.eWaitfor = PauseData::WAIT_DISABLED;
        callLuaScriptFunction(m_PauseData.sFunction);
    }
}
void GameScriptParser::setWaitFor(PauseData::EWaitFor eWaitFor, const Ogre::String &sFunction, unsigned int uiPointerId) {
    m_PauseData.eWaitfor = eWaitFor;
    m_PauseData.sFunction = sFunction;
    m_PauseData.uiPointer = uiPointerId;
    if (m_mapPointerId.find(uiPointerId) == m_mapPointerId.end()){
        m_PauseData.pPointer = NULL;
    }
    else {
        m_PauseData.pPointer = getPointerFromMap(uiPointerId);
    }
}
bool GameScriptParser::containsPointer(void *pPointer) const {
    for (auto &pair : m_mapPointerId) {
        if (pair.second.pPointer == pPointer) {
            return true;
        }
    }
    return false;
}
unsigned int GameScriptParser::getIDOfPointer(void *pPointer) const {
    for (auto &pair : m_mapPointerId) {
        if (pair.second.pPointer == pPointer) {
            return pair.first;
        }
    }
    throw Ogre::Exception(__LINE__, "Pointer is not in map, no id can be found!", __FILE__);
}
unsigned int GameScriptParser::getIDOfPointerOrAdd(void *pPointer, UserPointerData::Types type) {
    if (containsPointer(pPointer)) {
        return getIDOfPointer(pPointer);
    }
    else {
        return addPointerToMap(pPointer, type);
    }
}
void GameScriptParser::deletePointer(unsigned int id) {
    if (m_mapPointerId.find(id) == m_mapPointerId.end()) {return;}

    if (m_mapPointerId[id].m_eType == UserPointerData::PHYSICS_USER_POINTER) {
        CPhysicsUserPointer *pPointer = static_cast<CPhysicsUserPointer*>(m_mapPointerId[id].pPointer);
        if (dynamic_cast<CCharacter*>(pPointer)) {
            m_pCurrentMap->destroyLater(dynamic_cast<CCharacter*>(pPointer));
        }
    }
    else if (m_mapPointerId[id].m_eType == UserPointerData::RIGID_BODY) {
        btRigidBody *pRB = static_cast<btRigidBody*>(m_mapPointerId[id].pPointer);
        delete pRB->getMotionState();
        delete pRB->getCollisionShape();
        m_pCurrentMap->getPhysicsManager()->getWorld()->removeRigidBody(pRB);
        delete pRB;
    }
    else if (m_mapPointerId[id].m_eType == UserPointerData::LUA_STATE) {
        lua_close(static_cast<lua_State*>(m_mapPointerId[id].pPointer));
    }

    m_mapPointerId.erase(id);
}
// ===================================================================================
// ! C-Functions, for lua
// ===================================================================================
int logToOgre(lua_State *l) {
    // simple function to write to ogrle log
    assert(lua_gettop(l) == 1); // number of arguments
    Ogre::LogManager::getSingleton().logMessage(Ogre::String("Lua script: ") + lua_tostring(l, lua_gettop(l)));

    return 0;
}
//! Function to create a character
/**
 * it will return an integer id of the newly created character
 */
int createCharacter(lua_State *l) {
    CMap *pMap(GameScriptParser::getSingleton().getMap());
    assert(lua_gettop(l) >= 1); // number of arguments
    Ogre::Quaternion rot(Ogre::Quaternion::IDENTITY);
    Ogre::Vector3 pos(Ogre::Vector3::ZERO);
    Ogre::String sBuffer;
    double dBuffer;
    if (lua_gettop(l) == 3) {
        dBuffer = lua_tonumber(l, lua_gettop(l));
        lua_pop(l,1);
        rot.FromAngleAxis(Ogre::Degree(dBuffer), Ogre::Vector3::UNIT_Y);
    }
    if (lua_gettop(l) == 2) {
        sBuffer = lua_tostring(l, lua_gettop(l));
        lua_pop(l,1);
        pos = Ogre::StringConverter::parseVector3(sBuffer);
    }

    Ogre::String sCharType(lua_tostring(l, lua_gettop(l)));
    Ogre::StringUtil::toLowerCase(sCharType);
    lua_pop(l,1);

    CCharacter *pChar(NULL);

    if (sCharType == "greensword") {
        pChar = new CSimpleEnemy(CSimpleEnemy::ET_GREEN_SWORD, pMap);
    }
    else if (sCharType == "blocker") {
        pChar = new CSimpleEnemy(CSimpleEnemy::ET_BLOCKER, pMap);
    }
    else if (sCharType == "fatheroflink") {
        pChar = new SimpleFriend(SimpleFriend::SF_LINKS_FATHER, pMap);
    }
    else {
        throw Ogre::Exception(__LINE__, "Unknown character type \"" + sCharType + "\" by parsing lua script", __FILE__);
    }

    assert(pChar);
    pMap->addAndInitialiseNewChar(pChar);
    pChar->setPosition(pos);
    pChar->setOrientation(rot);
    pChar->getCharacterController()->addListener(GameScriptParser::getSingletonPtr());
    lua_pushinteger(l, GameScriptParser::getSingleton().addPointerToMap(dynamic_cast<CPhysicsUserPointer*>(pChar), GameScriptParser::UserPointerData::PHYSICS_USER_POINTER));
    return 1; // number of return values: id of the char in void * table
}
//! Function to destroy a pointer object, multible ids, allowed
int deletePointer(lua_State *l) {
    while (lua_gettop(l) > 0) {
        unsigned int id = lua_tointeger(l, lua_gettop(l));
        GameScriptParser::getSingleton().deletePointer(id);
        lua_pop(l,1);
    }
    return 0;
}
//! Function to display text in the HUD-Text-Field
int displayText(lua_State *l) {
    assert(lua_gettop(l) == 1); // number of arguments
    Ogre::String sText(lua_tostring(l, lua_gettop(l)));
    lua_pop(l,1);

    CTextLoader::getSingleton().replaceVariablesInMapText(sText);
    CHUD::getSingleton().showText(sText);

    return 0;
}
//! Function to move a character to a given position
int moveToTarget(lua_State *l) {
    Ogre::Degree degDevLookDir(5);
    double dRadius = 0.1;
    Ogre::Vector3 vPos;
    int charId(-1);
    if (lua_gettop(l) == 4) {
        degDevLookDir = Ogre::Degree(lua_tonumber(l, lua_gettop(l)));
        lua_pop(l, 1);
    }
    if (lua_gettop(l) == 3) {
        dRadius = lua_tonumber(l, lua_gettop(l));
        lua_pop(l, 1);
    }
    vPos = Ogre::StringConverter::parseVector3(lua_tostring(l, lua_gettop(l)));
    lua_pop(l, 1);
    charId = lua_tointeger(l, lua_gettop(l));

    CCharacter *pChar = dynamic_cast<CCharacter*>(GameScriptParser::getSingleton().getPointerFromMap<CPhysicsUserPointer>(charId));
    assert(pChar);

    pChar->getCharacterController()->moveToTarget(vPos, dRadius, degDevLookDir);

	return 0;
}
int waitFor(lua_State *l) {
    GameScriptParser::PauseData::EWaitFor waitFor = GameScriptParser::PauseData::WAIT_DISABLED;
    Ogre::String sFunction;
    unsigned int id = 0;
    if (lua_gettop(l) == 3) {
        id = lua_tounsigned(l, lua_gettop(l));
        lua_pop(l, 1);
    }
    sFunction = lua_tostring(l, 2);
    Ogre::String buffer = lua_tostring(l, 1);

    if (buffer == "disabled") {
        waitFor = GameScriptParser::PauseData::WAIT_DISABLED;
    }
    else if (buffer == "endDisplayText") {
        waitFor = GameScriptParser::PauseData::WAIT_END_DISPLAY_TEXT;
    }
    else if (buffer == "waitTargetReached") {
        waitFor = GameScriptParser::PauseData::WAIT_TARGET_REACHED;
    }
    else {
        throw Ogre::Exception(__LINE__, "Unknown pause type \"" + buffer + "\" by parsing lua script", __FILE__);
    }

    GameScriptParser::getSingleton().setWaitFor(waitFor, sFunction, id);

    return 0;
}
int setChat(lua_State*l) {
    unsigned int id = lua_tounsigned(l, 1);
    Ogre::String sText;
    Ogre::String sInteractionDirection("all"); // default are all directions
    unsigned int luaStateID;
    if (lua_gettop(l) == 3) {
        // specify direction
        sInteractionDirection = lua_tostring(l, 3);
    }
    if (lua_isnumber(l, 2)) {
        sInteractionDirection = "lua_state";
        luaStateID = lua_tounsigned(l, 2);
    }
    else {
        sText = lua_tostring(l, 2);
    }

    ChatableInterface *pChatInterface = dynamic_cast<ChatableInterface*>(GameScriptParser::getSingleton().getPointerFromMap<CPhysicsUserPointer>(id));

    if (sInteractionDirection == "lua") {
        pChatInterface->setChatScript(sText, GameScriptParser::getSingleton().getResourceGroupName());
    }
    else if (sInteractionDirection == "lua_state") {
        pChatInterface->setChatLuaState(GameScriptParser::getSingleton().getPointerFromMap<lua_State>(luaStateID));
    }
    else {
        boost::regex chatReplaceRegEx("\\$\\{([\\d\\w]+)\\}");
        boost::cmatch matches;
        if (boost::regex_match(sText.c_str(), matches, chatReplaceRegEx)){
            std::string m2 = matches[1];
            sText = CTextLoader::getSingleton().getChatText(GameScriptParser::getSingleton().getMap()->getName(), m2);
        }


        pChatInterface->setChatText(sText, InteractionInterface::getInteractionDirectionFromString(sInteractionDirection));
    }
    return 0;
}
int getObjectById(lua_State *l) {
    Ogre::String sID(lua_tostring(l, 1));

    CPhysicsUserPointer *pObject = GameScriptParser::getSingleton().getMap()->getObjectManager().getObjectBySafeStateId(sID);
    unsigned int id = GameScriptParser::getSingleton().getIDOfPointerOrAdd(pObject, GameScriptParser::UserPointerData::PHYSICS_USER_POINTER);

    lua_pushunsigned(l,id);
    return 1;
}
int setInnerObject(lua_State *l) {
    unsigned int id = lua_tounsigned(l, 1);
    Ogre::String sInnerObject = lua_tostring(l, 2);

    InnerObjectContainerInterface *pObject = dynamic_cast<InnerObjectContainerInterface*>(GameScriptParser::getSingleton().getPointerFromMap<CPhysicsUserPointer>(id));
    pObject->setInnerObject(&InnerObjectGenerator::getInnerObjectByName(sInnerObject));

    return 0;
}
int addInvisibleBlockade(lua_State *l) {
    Ogre::Vector3 vPos(Ogre::StringConverter::parseVector3(lua_tostring(l, 1)));
    Ogre::Vector3 vSize(Ogre::StringConverter::parseVector3(lua_tostring(l, 2)));

    btRigidBody::btRigidBodyConstructionInfo rbConstruction(0, new btDefaultMotionState(), new btBoxShape(BtOgre::Convert::toBullet(vSize)));
    btRigidBody *pRB = new btRigidBody(rbConstruction);
    pRB->setWorldTransform(btTransform(btQuaternion::getIdentity(), BtOgre::Convert::toBullet(vPos)));

    GameScriptParser::getSingleton().getMap()->getPhysicsManager()->getWorld()->addRigidBody(pRB, COL_WALL, MASK_BLOCKADE_COLLIDES_WITH);


    lua_pushinteger(l, GameScriptParser::getSingleton().addPointerToMap(pRB, GameScriptParser::UserPointerData::RIGID_BODY));
    return 1;
}
int createLuaScript(lua_State *l) {
    Ogre::String sLuaScript = lua_tostring(l, 1);
    Ogre::String sResourceGroup = GameScriptParser::getSingleton().getResourceGroupName();
    Ogre::LogManager::getSingleton().logMessage("Parsing lua script: " + sLuaScript + " in " + sResourceGroup);

    lua_State *pNewLuaState = luaL_newstate();

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
        luaL_requiref(pNewLuaState, lib->name, lib->func, 1);
        lua_settop(pNewLuaState, 0);
    }
    // Strip the path
    Ogre::String basename, path;
    Ogre::StringUtil::splitFilename(sLuaScript, basename, path);

    Ogre::DataStreamPtr pStream = Ogre::ResourceGroupManager::getSingleton().openResource(basename, sResourceGroup, false);
    Ogre::String data = pStream->getAsString();

    int status = luaL_loadstring(pNewLuaState, data.c_str());
    int result = 0;
    if(status == LUA_OK) {
        result = lua_pcall(pNewLuaState, 0, LUA_MULTRET, 0);
    }
    else {
        throw Ogre::Exception(__LINE__, "Could not load the lua script", __FILE__);
    }

    lua_pushinteger(l, GameScriptParser::getSingleton().addPointerToMap(pNewLuaState, GameScriptParser::UserPointerData::LUA_STATE));
    return 1;
}
// ===================================================================================
// ! Declare c functions for lua!
// ===================================================================================
void GameScriptParser::registerCFunctionsToLua() {
    registerSingleCFunctionsToLua(logToOgre, "log");
    registerSingleCFunctionsToLua(createCharacter, "createCharacter");
    registerSingleCFunctionsToLua(::deletePointer, "delete");
    registerSingleCFunctionsToLua(displayText, "displayText");
    registerSingleCFunctionsToLua(moveToTarget, "moveToTarget");
    registerSingleCFunctionsToLua(waitFor, "wait");
    registerSingleCFunctionsToLua(setChat, "setChat");
    registerSingleCFunctionsToLua(getObjectById, "getObject");
    registerSingleCFunctionsToLua(setInnerObject, "setInnerObject");
    registerSingleCFunctionsToLua(addInvisibleBlockade, "addInvisibleBlockade");
    registerSingleCFunctionsToLua(createLuaScript, "createLuaScript");
}
void GameScriptParser::registerSingleCFunctionsToLua(lua_CFunction fn, const char *label) {
    lua_pushcfunction(lua_state, fn);
    lua_setglobal(lua_state, label);
}
