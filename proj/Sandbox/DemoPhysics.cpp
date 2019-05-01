/* * * * * * * * * * * * * * * * * * * * */

#include "DemoPhysics.hpp"

#include <MemeCore/Debug.hpp>
#include <MemeCore/Time.hpp>
#include <MemeEngine/Engine.hpp>
#include <MemeEngine/Resources.hpp>
#include <MemePhysics/Physics.hpp>
#include <MemePhysics/Collider.hpp>
#include <MemePhysics/Particle.hpp>
#include <MemePhysics/Rigidbody.hpp>

/* * * * * * * * * * * * * * * * * * * * */

namespace DEMO
{
	// Init
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void demo_physics::init()
	{
		// While the window is alive and open
		while (ML_Engine.isRunning())
		{
			ML_Physics.updateAll(demo_physics::update);
		}
	}


	// Update
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void demo_physics::update(const int32_t i, ml::PhysicsState & state)
	{
		static float deltaT, totalT;
		totalT = ML_Time.elapsed().delta();
		deltaT = ML_Engine.elapsed().delta<ml::Milliseconds, ml::Micro>();

		// Get the RB
		ml::Rigidbody	* rb = ML_Physics.getLinkedRigidbody(i);
		ml::Collider	* c = rb->collider();
		ml::Particle	* p = rb->particle();
		ml::Transform	* t = rb->transform();

		// Get copy state's data
		ml::vec3 pos;
		ml::quat rot;
		ml::mat4 mat;
		ml::mat4 inv;
		if (state.get<state.T_Pos>(i, pos) &&
			state.get<state.T_Rot>(i, rot) &&
			state.get<state.T_Mat>(i, mat) &&
			state.get<state.T_Inv>(i, inv) &&
			rb->enabled)
		{
			// Modify copy state's data
			switch (i)
			{
			// Borg
			/* * * * * * * * * * * * * * * * * * * * */
			case RB_BORG:
			{
				(*p).applyForce(ml::vec3::Zero);
				pos = { pos[0], +ML_Time.cos(), pos[2] };
				rot = ml::quat::angleAxis(totalT, ml::vec3::One);
			}
			break;

			// Cube
			/* * * * * * * * * * * * * * * * * * * * */
			case RB_CUBE: 
			{
				(*p).applyForce(ml::vec3::Zero);
				pos = { pos[0], -ML_Time.sin(), pos[2] };
				rot = ml::quat::angleAxis(totalT, ml::vec3::One);
			}
			break;

			// Navball
			/* * * * * * * * * * * * * * * * * * * * */
			case RB_NAVBALL:
			{
				(*p).applyForce(ml::vec3::Zero);
				pos = { pos[0], -ML_Time.cos(), pos[2] };
				rot = ml::quat::angleAxis(totalT, ml::vec3::Forward);
			}
			break;

			// Moon
			/* * * * * * * * * * * * * * * * * * * * */
			case RB_MOON:
			{
				(*p).applyForce(ml::vec3::Zero);
				pos = { pos[0], +ML_Time.sin(), pos[2] };
				rot = ml::quat::angleAxis(totalT, ml::vec3::Up);
			}
			break;

			// Earth
			/* * * * * * * * * * * * * * * * * * * * */
			case RB_EARTH:
			{
				(*p).applyForce(ml::Force::gravity(ml::vec3::Up, p->mass))
					.integrateEulerExplicit(deltaT)
					.convertForce()
					.updateCenterMass()
					.updateInertiaTensor()
					;
				//rot = ml::quat::angleAxis(totalT, ml::vec3::Up);
				rot = p->rotation;
				pos = p->pos;
			}
			break;

			// Ground
			/* * * * * * * * * * * * * * * * * * * * */
			case RB_GROUND:
			{

			}
			break;
			}

			// Apply changes to copy state
			if (!state.set<state.T_Pos>(i, pos) ||
				!state.set<state.T_Rot>(i, rot) ||
				!state.set<state.T_Mat>(i, mat) ||
				!state.set<state.T_Inv>(i, inv))
			{
				ml::Debug::logError("Physics | Failed setting copy state: {0}", i);
			}
		}
		else
		{
			ml::Debug::logError("Physics | Failed getting copy state: {0}", i);
		}
	}


	// Sync
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void demo_physics::sync(const ml::PhysicsState & state)
	{
		for (auto & pair : ML_Res.entities)
		{
			if (ml::Rigidbody * rb = pair.second->get<ml::Rigidbody>())
			{
				ml::vec3 scl = rb->transform()->getScl();
				ml::vec3 pos;
				ml::quat rot;
				ml::mat4 mat;
				ml::mat4 inv;

				if (state.get<state.T_Pos>(rb->index(), pos) &&
					state.get<state.T_Rot>(rb->index(), rot) &&
					state.get<state.T_Mat>(rb->index(), mat) &&
					state.get<state.T_Inv>(rb->index(), inv))
				{
					(*rb->transform())
						.update		(ml::mat4::Identity())
						.translate	(pos)
						.rotate		(rot)
						.scale		(scl)
						;
				}
			}
		}
	}

}