#ifndef INNEROBJECTCONTAINERINTERFACE_H
#define INNEROBJECTCONTAINERINTERFACE_H

#include "InnerObjectGenerator.h"

class InnerObjectContainerInterface {
private:
	const inner_object_list_type *m_plInnerObject;    //!< the inner object of this object, may never be NULL set to IOLT_EMPTY instead!)
public:
    InnerObjectContainerInterface();
    virtual ~InnerObjectContainerInterface();

	void setInnerObject(const inner_object_list_type *plInnerObject) {assert(plInnerObject); m_plInnerObject = plInnerObject;}
	const inner_object_list_type &getInnerObject() const {return *m_plInnerObject;}
	CPickableObject *createInnerObject(CMap &map, const Ogre::Vector3 &vPosition);
protected:
private:
};

#endif // INNEROBJECTCONTAINERINTERFACE_H
