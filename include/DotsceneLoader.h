#ifndef DOT_SCENELOADER_H
#define DOT_SCENELOADER_H

// Includes
#include "StdAfx.h"
#include "DotSceneLoaderCallback.h"

class UserData;
class CPhysicsManager;

// Forward declarations
namespace tinyxml2 {
    class XMLElement;
}


namespace Ogre
{
    // Forward declarations
    class SceneManager;
    class SceneNode;

    class nodeProperty
    {
    public:
        String nodeName;
        String propertyNm;
        String valueName;
        String typeName;

        nodeProperty(const String &node, const String &propertyName, const String &value, const String &type)
            : nodeName(node), propertyNm(propertyName), valueName(value), typeName(type) {}
    };


    class DotSceneLoader
    {
	private:
		std::list<CDotSceneLoaderCallback*> m_lCallbacks;
		std::map<Ogre::String, Ogre::Entity*> m_lEntityBufferMap;
		CPhysicsManager *m_pPhysicsManager;
	public:
        DotSceneLoader() : mSceneMgr(0) {}
        virtual ~DotSceneLoader() {cleanup();}

		void cleanup();

		void addCallback(CDotSceneLoaderCallback *pCallback) {m_lCallbacks.push_back(pCallback);}

        void parseDotScene(const String &SceneName, const String &groupName, SceneManager *yourSceneMgr, CPhysicsManager *pPhysicsManager, SceneNode *pAttachNode = NULL, const String &sPrependNode = "");
        String getProperty(const String &ndNm, const String &prop);

		const Ogre::String &getPrependNode() const {return m_sPrependNode;}

        std::vector<nodeProperty> nodeProperties;
        std::vector<String> staticObjects;
        std::vector<String> dynamicObjects;
		std::vector<Entity*> mStaticObjects;


    protected:
        void processScene(tinyxml2::XMLElement *XMLRoot);

        void processNodes(tinyxml2::XMLElement *XMLNode);
        void processExternals(tinyxml2::XMLElement *XMLNode);
        void processEnvironment(tinyxml2::XMLElement *XMLNode);
        void processTerrain(tinyxml2::XMLElement *XMLNode);
        void processUserDataReference(tinyxml2::XMLElement *XMLNode, SceneNode *pParent = 0);
        void processUserDataReference(tinyxml2::XMLElement *XMLNode, Entity *pEntity);
        void processOctree(tinyxml2::XMLElement *XMLNode);
        void processLight(tinyxml2::XMLElement *XMLNode, SceneNode *pParent = 0);
        void processCamera(tinyxml2::XMLElement *XMLNode, SceneNode *pParent = 0);

        void processNode(tinyxml2::XMLElement *XMLNode, SceneNode *pParent = 0);
        void processLookTarget(tinyxml2::XMLElement *XMLNode, SceneNode *pParent);
        void processTrackTarget(tinyxml2::XMLElement *XMLNode, SceneNode *pParent);
        void processEntity(tinyxml2::XMLElement *XMLNode, SceneNode *pParent, UserData &userData);
        void processParticleSystem(tinyxml2::XMLElement *XMLNode, SceneNode *pParent);
        void processBillboardSet(tinyxml2::XMLElement *XMLNode, SceneNode *pParent);
        void processPlane(tinyxml2::XMLElement *XMLNode, SceneNode *pParent);

        void processFog(tinyxml2::XMLElement *XMLNode);
        void processSkyBox(tinyxml2::XMLElement *XMLNode);
        void processSkyDome(tinyxml2::XMLElement *XMLNode);
        void processSkyPlane(tinyxml2::XMLElement *XMLNode);
        void processClipping(tinyxml2::XMLElement *XMLNode);

        void processLightRange(tinyxml2::XMLElement *XMLNode, Light *pLight);
        void processLightAttenuation(tinyxml2::XMLElement *XMLNode, Light *pLight);

        String getAttrib(tinyxml2::XMLElement *XMLNode, const String &parameter, const String &defaultValue = "");
        Real getAttribReal(tinyxml2::XMLElement *XMLNode, const String &parameter, Real defaultValue = 0);
        bool getAttribBool(tinyxml2::XMLElement *XMLNode, const String &parameter, bool defaultValue = false);

        Vector3 parseVector3(tinyxml2::XMLElement *XMLNode);
        Quaternion parseQuaternion(tinyxml2::XMLElement *XMLNode);
        ColourValue parseColour(tinyxml2::XMLElement *XMLNode);


        SceneManager *mSceneMgr;
        SceneNode *mAttachNode;
        String m_sGroupName;
        String m_sPrependNode;
		SceneNode *mStaticSceneNode;
    };
}

#endif // DOT_SCENELOADER_H
