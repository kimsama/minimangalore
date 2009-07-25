//------------------------------------------------------------------------------
//  properties/inputproperty.cc
//  (C) je.a.le@wanadoo.fr
//------------------------------------------------------------------------------
#include "mtutorial05b/property/opelphysicsproperty.h"

#include "properties/physicsproperty.h"
#include "game/entity.h"
#include "foundation/factory.h" 

#include "mtutorial05b/message/opelcontrolvalues.h"
#include "mtutorial05b/mTestapp.h"

// handler some math functions nmath.h doesn't....
#include "mtutorial05b/mathext.h"

#include "properties/physicsproperty.h"
#include "physics/entity.h"
#include "physics/composite.h"
#include "physics/joint.h"
#include "physics/hinge2joint.h"
#include "physics/jointaxis.h"
#include "ode/ode.h"

//------------------------------------------------------------------------------
namespace Attr
{
	DefineFloat(RequestSteeringVelocity);	// request force on the direction
	DefineFloat(RequestEngineVelocity);	// request force on the wheels
	
	DefineFloat(SteeringAngle);	// actual steering angle, in RAD ; READ ONLY
	DefineFloat(SteeringVelocity);	// actual velocity on the steering
	DefineFloat(EngineVelocity);	// actual velocity on the wheel
};

namespace Properties
{
ImplementRtti(Properties::OpelPhysicsProperty, Properties::PhysicsProperty);
ImplementFactory(Properties::OpelPhysicsProperty);

using namespace Message;
using namespace Game;
using namespace Managers;
using namespace Math;

//------------------------------------------------------------------------------
OpelPhysicsProperty::OpelPhysicsProperty()
{
    // empty
}

//------------------------------------------------------------------------------
OpelPhysicsProperty::~OpelPhysicsProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
/// setup default entity attributes
*/
void OpelPhysicsProperty::SetupDefaultAttributes()
{
	// don't forget to call subclass !!
	PhysicsProperty::SetupDefaultAttributes();
	
	// default steering angle : forward
	GetEntity()->SetFloat( Attr::RequestSteeringVelocity, 0.0f );
	GetEntity()->SetFloat( Attr::RequestEngineVelocity, 0.0f );
	GetEntity()->SetFloat( Attr::SteeringAngle, 0.0f );
	GetEntity()->SetFloat( Attr::SteeringVelocity, 0.0f );
	GetEntity()->SetFloat( Attr::EngineVelocity, 0.0f );
}

//------------------------------------------------------------------------------
/// called before movement happens : transfer physics forces from mangalore to 
/// ode hinge. Simulation happen every frame, so yes, we have to comput and 
/// set values every frame....

// this "try" to fix some issues from tutorial05. 
// apply different velocity on left and right wheels
// apply progressive velocity
// NOTA : this is not a "real" simualtion, it use fake solution, close enough to create
// realistic effects.
void OpelPhysicsProperty::OnMoveBefore()
{
	// transfer to local value....
	float	steering = GetEntity()->GetFloat( Attr::RequestSteeringVelocity );
	float	engine =  GetEntity()->GetFloat( Attr::EngineVelocity );
	
	// steering is done by applying forces directly on the motor of the hinge2joint.
	// (both front hinge)
	// you might think all those mangalore/physics functions are enough ; no. 
	// their only help for controling the entire entity. That not what we
	// want here. It would be like pushing the all opelblitz (with a bulldozer !)
	// So we have to work directly at ode level... but be carrefull, else, you
	// may screw up the simulation ! (subclass...)

	// now get hinge from entity...
	Physics::Hinge2Joint *joint_g = null;
	Physics::Hinge2Joint *joint_d = null;
	Physics::Composite *comp = GetPhysicsEntity()->GetComposite();
	
	// there're no other solution (yet) beside iteration...
	for( int i=0 ; i < comp->GetNumJoints() ; i++ ) {
		if( comp->GetJointAt( i )->GetLinkName() == "hinge_av_g" ) {
			joint_g = (Physics::Hinge2Joint *)(comp->GetJointAt( i ));
		}
		if( comp->GetJointAt( i )->GetLinkName() == "hinge_av_d" ) {
			joint_d = (Physics::Hinge2Joint *)(comp->GetJointAt( i ));
		}				
	}

	//  not suppose to happen, anyway....
	n_assert( ( joint_d != null ) && ( joint_g != null ) );

	// several values are already set inside the xml file
	// NOTA : that's more mecanic than mangalore :-) : to allow steering,
	// you must apply a velocity, ok, but allow a force too !
	// by default fmax = 0
	dJointSetHinge2Param(joint_g->GetJointId(), dParamVel, steering);
	dJointSetHinge2Param(joint_d->GetJointId(), dParamVel, steering);
	
	// set the steering force. releasing it (0.0f) does a good effect : environement
	// interact with the steering.  But since fronts wheels aren't physically linked
	// each frame you have to correct vel/fmax in order to keep the wheels // ; have fun...
	dJointSetHinge2Param(joint_g->GetJointId(), dParamFMax, STEERING_FMAX);
	dJointSetHinge2Param(joint_d->GetJointId(), dParamFMax, STEERING_FMAX);
	
	// now take care about force on wheels
	// it's almost the same thing, exept this time velocity apply 
	// to axis 2 (ode) ; Note on the xml file params for rear axis...	
	
	// comput the velocity. each frame increase a little bit the velocity until
	// requested is reached. This is not a real simulation but the result is acceptable
	float rengine = GetEntity()->GetFloat( Attr::RequestEngineVelocity );
	if( rengine < 0.0f ) {
		engine = n_clamp( engine - ENGINE_VELOCITY_INC, rengine, 0.0f );
	} else if ( rengine  > 0.0f ) {
		engine = n_clamp( engine + ENGINE_VELOCITY_INC, 0.0f, rengine );	
	} else {
		// perhaps slow down, let's do it
		if( engine > 0.0f ) {
			engine = n_clamp( engine - ENGINE_VELOCITY_DEC, 0.0f, engine );		
		} else {
			engine = n_clamp( engine + ENGINE_VELOCITY_DEC, engine, 0.0f );
		}
	}
	
	// there still a big problem : when turning right, the left wheel
	// does more rotation than the right, etc...
	// NOTA : there're lots of params : wheel size, l/r length, f/b wheels length,
	// steering angle... ; here, let's use a constant... I use the angle (rad). 
	float scalling = n_clamp( GetEntity()->GetFloat( Attr::SteeringAngle ), -1.0f, 1.0f );
	
	dJointSetHinge2Param(joint_g->GetJointId(), dParamVel2, engine * (1.0f + scalling));	
	dJointSetHinge2Param(joint_d->GetJointId(), dParamVel2, engine * (1.0f - scalling));

	// Why setting fmax to 0 when not moving ??? 1) read the ode doc :-) !
	// ok, when vel = O and fmax > 0 you set a sort of brake force...
	// in the current implementation it would like brake at all...
	float enginef = ( engine != 0.0f ? ENGINE_VELOCITY_MAX : 0.0f );

	dJointSetHinge2Param(joint_g->GetJointId(), dParamFMax2, enginef);
	dJointSetHinge2Param(joint_d->GetJointId(), dParamFMax2, enginef);

	// time to update EngineVelocity 
	GetEntity()->SetFloat( Attr::EngineVelocity, engine );

	// that's a common mistake, i do too lot of time ! : update variables
	// but forget to tell mangalore this entity will be part of simulation...
	// simulation is costy, so when velocity (entity) == 0, mangalore
	// disable the entity (BTW we're not doing an optimal code, this is just
	// the begining :-) ...)
	GetPhysicsEntity()->SetEnabled( true );

	PhysicsProperty::OnMoveBefore();
}

//------------------------------------------------------------------------------
/**
    Called after the physics subsystem has been triggered.
    the simulation proably changed the steering angle (terrain, collision, etc...)
    it's important to update values
*/
void OpelPhysicsProperty::OnMoveAfter()
{
	Physics::Hinge2Joint *joint_g = null;
	Physics::Composite *comp = GetPhysicsEntity()->GetComposite();
	
	// there're no other solution (yet) beside iteration...
	for( int i=0 ; i < comp->GetNumJoints() ; i++ ) {
		if( comp->GetJointAt( i )->GetLinkName() == "hinge_av_g" ) {
			joint_g = (Physics::Hinge2Joint *)(comp->GetJointAt( i ));
			break;
		}
	}

	// update attr from simuation result
	GetEntity()->SetFloat( Attr::SteeringAngle, 
		(float)dJointGetHinge2Angle1( joint_g->GetJointId() ) );

	// default physics has buisness too...
	PhysicsProperty::OnMoveAfter();
}

//------------------------------------------------------------------------------
/**
    This method is inherited from the Port class. If your property acts as
    a message handler you must implement the Accepts() method to return
    true for each message that is accepted. By default no messages are accepted.
*/
bool OpelPhysicsProperty::Accepts(Message::Msg* msg)
{
    n_assert(msg);
	
	if( msg->CheckId(Message::OpelControlValues::Id) ) {
		return true;
	}

	// Physics handle some message too
    return( PhysicsProperty::Accepts(msg) );
}

//------------------------------------------------------------------------------
/**
    This method is inherited from the Port class. If your property acts as
    a message handler you must implement the HandleMessage() method to
    process the incoming messages. Please note that HandleMessage() will
    not be called "automagically" because the Property doesn't know at which
    point in the frame you want to handle pending messages.

    Thus, you must call the HandlePendingMessages() yourself from either OnBeginFrame(),
    OnMoveBefore(), OnMoveAfter() or OnRender().

    The simple rule is: if you override the Accepts() method, you must also call
    HandlePendingMessages() either in OnBeginFrame(), OnMoveBefore(), OnMoveAfter()
    or OnRender()
*/
// NOTA : HandlePendingMessages is already call from Property::OnFrame
void OpelPhysicsProperty::HandleMessage(Message::Msg* msg)
{
    n_assert(msg);
   
	// a request for some change on the physics value (velocity, etc....)
	if (msg->CheckId(Message::OpelControlValues::Id))
    {
		OpelControlValues *	val = static_cast<OpelControlValues *>(msg);

		if( val->GetMask() && OpelControlValues::M_STEERING ) {
			GetEntity()->SetFloat( Attr::RequestSteeringVelocity, 
				MathExt::clampVal( -300.0f, 300.0f, val->GetSteeringVelocity() ) );
		}
		
		if( val->GetMask() && OpelControlValues::M_SPEED ) {
			GetEntity()->SetFloat( Attr::RequestEngineVelocity, 
				MathExt::clampVal( 1.0f - ENGINE_VELOCITY_MAX, ENGINE_VELOCITY_MAX, val->GetSpeedVelocity() ) );
		}
			
		return;
    }

	// pass to PhysicsProperty
	PhysicsProperty::HandleMessage(msg);
}

}; // namespace Properties