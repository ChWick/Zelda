#ifndef CHARACTERCONTROLLER_PHYSICS_H
#define CHARACTERCONTROLLER_PHYSICS_H

#include <list>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btMultiSphereShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>
#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

class CWorldEntity;

///btKinematicCharacterController is an object that supports a sliding motion in a world.
///It uses a ghost object and convex sweep test to test for upcoming collisions. This is combined with discrete collision detection to recover from penetrations.
///Interaction between btKinematicCharacterController and dynamic rigid bodies needs to be explicity implemented by the user.
class CharacterControllerPhysics : public btCharacterControllerInterface
{
protected:

	btScalar m_halfHeight;

	btPairCachingGhostObject* m_ghostObject;
	btConvexShape*	m_convexShape;//is also in m_ghostObject, but it needs to be convex, so we store it here to avoid upcast

	btScalar m_verticalVelocity;
	btScalar m_verticalOffset;
	btScalar m_fallSpeed;
	btScalar m_jumpSpeed;
	btScalar m_maxJumpHeight;
	btScalar m_maxSlopeRadians; // Slope angle that is set (used for returning the exact value)
	btScalar m_maxSlopeCosine;  // Cosine equivalent of m_maxSlopeRadians (calculated once when set, for optimization)
	btScalar m_gravity;

	btScalar m_turnAngle;

	btScalar m_stepHeight;

	btScalar	m_addedMargin;//@todo: remove this and fix the code

	///this is the desired walk direction, set by the user
	btVector3	m_walkDirection;
	btVector3	m_normalizedDirection;

	//some internal variables
	btVector3 m_currentPosition;
	btScalar  m_currentStepOffset;
	btVector3 m_targetPosition;

	///keep track of the contact manifolds
	btManifoldArray	m_manifoldArray;

	bool m_touchingContact;
	btVector3 m_touchingNormal;

	bool  m_wasOnGround;
	bool  m_wasJumping;
	bool	m_useGhostObjectSweepTest;
	bool	m_useWalkDirection;
	btScalar	m_velocityTimeInterval;
	int m_upAxis;

	static btVector3* getUpAxisDirections();
	bool  m_interpolateUp;
	bool  full_drop;
	bool  bounce_fix;

	std::list<CWorldEntity *> m_lCollidingWorldEntities;

	btVector3 computeReflectionDirection (const btVector3& direction, const btVector3& normal);
	btVector3 parallelComponent (const btVector3& direction, const btVector3& normal);
	btVector3 perpindicularComponent (const btVector3& direction, const btVector3& normal);

	bool recoverFromPenetration ( btCollisionWorld* collisionWorld);
	void stepUp (btCollisionWorld* collisionWorld);
	void updateTargetPositionBasedOnCollision (const btVector3& hit_normal, btScalar tangentMag = btScalar(0.0), btScalar normalMag = btScalar(1.0));
	void stepForwardAndStrafe (btCollisionWorld* collisionWorld, const btVector3& walkMove);
	void stepDown (btCollisionWorld* collisionWorld, btScalar dt);
public:

	BT_DECLARE_ALIGNED_ALLOCATOR();

	CharacterControllerPhysics (btPairCachingGhostObject* ghostObject,btConvexShape* convexShape,btScalar stepHeight, int upAxis = 1);
	~CharacterControllerPhysics ();


	///btActionInterface interface
	virtual void updateAction( btCollisionWorld* collisionWorld,btScalar deltaTime)
	{
		preStep ( collisionWorld);
		playerStep (collisionWorld, deltaTime);
	}

	///btActionInterface interface
	void	debugDraw(btIDebugDraw* debugDrawer);

	void setUpAxis (int axis)
	{
		if (axis < 0)
			axis = 0;
		if (axis > 2)
			axis = 2;
		m_upAxis = axis;
	}

	/// This should probably be called setPositionIncrementPerSimulatorStep.
	/// This is neither a direction nor a velocity, but the amount to
	///	increment the position each simulation iteration, regardless
	///	of dt.
	/// This call will reset any velocity set by setVelocityForTimeInterval().
	virtual void	setWalkDirection(const btVector3& walkDirection);

	/// Caller provides a velocity with which the character should move for
	///	the given time period.  After the time period, velocity is reset
	///	to zero.
	/// This call will reset any walk direction set by setWalkDirection().
	/// Negative time intervals will result in no motion.
	virtual void setVelocityForTimeInterval(const btVector3& velocity,
				btScalar timeInterval);

	void reset ( btCollisionWorld* collisionWorld );
	void reset () {}
	void warp (const btVector3& origin);

	void preStep (  btCollisionWorld* collisionWorld);
	void playerStep ( btCollisionWorld* collisionWorld, btScalar dt);

	void setFallSpeed (btScalar fallSpeed);
	void setJumpSpeed (btScalar jumpSpeed);
	void setMaxJumpHeight (btScalar maxJumpHeight);
	bool canJump () const;

	void jump ();

	void setGravity(btScalar gravity);
	btScalar getGravity() const;

	/// The max slope determines the maximum angle that the controller can walk up.
	/// The slope angle is measured in radians.
	void setMaxSlope(btScalar slopeRadians);
	btScalar getMaxSlope() const;

	btPairCachingGhostObject* getGhostObject();
	void	setUseGhostSweepTest(bool useGhostObjectSweepTest)
	{
		m_useGhostObjectSweepTest = useGhostObjectSweepTest;
	}

	bool onGround () const;
	void setUpInterpolate (bool value);


	const std::list<CWorldEntity *> &getCollidingWorldEntities() const {return m_lCollidingWorldEntities;}
};


#endif // CHARACTERCONTROLLER_PHYSICS_H
