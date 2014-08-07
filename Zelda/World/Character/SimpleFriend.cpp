#include "StdAfx.h"
#include "SimpleFriend.h"
#include "PlayerTool.h"

SimpleFriend::SimpleFriend(ESimpleFriendTypes eType, CMap *pMap)
: CPerson(pMap, FOE_FRIENDLY), m_eType(eType){
    //ctor
}

SimpleFriend::~SimpleFriend()
{
    //dtor
}
void SimpleFriend::setupInternal() {
	switch (m_eType) {
	case SF_LINKS_FATHER:
		createTool(CPlayerTool::TOOL_SWORD, true);
		createShield(CShield::ST_SIMPLE_SHIELD, true);
		m_bSwordsDrawn = true;
		break;
	}
}
