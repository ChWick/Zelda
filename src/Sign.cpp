#include "StdAfx.h"
#include "Sign.h"
#include "HUD.h"
#include "MapManager.h"
#include "CameraController.h"

bool CSign::interactOnKeypress(EInteractType eInteractType, const Ogre::Vector3 &vInteractDir) {
	return ChatableInterface::interactOnKeypress(eInteractType, vInteractDir);
}
