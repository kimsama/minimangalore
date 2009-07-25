#ifndef PROPERTIES_UEPLAYERPHYSICSPROPERTY_H
#define PROPERTIES_UEPLAYERPHYSICSPROPERTY_H
//------------------------------------------------------------------------------
/**
    @class Properties::PlayerPhysicsProperty

    PlayerInputProperty ���� broadcasting�� �޽����� ��ŷ,
	PlayerEntity�� �ʿ��� ������ �����Ѵ�.
*/
#include "properties/actorphysicsproperty.h"
#include "msg/ueplayerkey.h"

#include "game/entity.h"
#include "physics/ueplayerentity.h"
#include "util/npfeedbackloop.h"
#include "util/nangularpfeedbackloop.h"

#include "attributes/ueattributes.h"
#include "attr/attributes.h"



namespace Properties
{
class Message::UePlayerKey;

class UePlayerPhysicsProperty : public ActorPhysicsProperty
{
    DeclareRtti;
    DeclareFactory(UePlayerPhysicsProperty);

public:

	enum { Stand,Crounch,Prone };

    /// constructor
    UePlayerPhysicsProperty();
    /// destructor
    virtual ~UePlayerPhysicsProperty();

	virtual void SetupDefaultAttributes();

	virtual void UePlayerPhysicsProperty::EnablePhysics();
	virtual void UePlayerPhysicsProperty::DisablePhysics();
	
	virtual bool Accepts(Message::Msg* msg);	
	virtual void HandleMessage(Message::Msg* msg);
	
	virtual void OnBeginFrame();	
	virtual void OnMoveAfter();
protected:   

	bool acceleratePrev;
	bool dobreakePrev;
	bool leftPrev;
	bool rightPrev;
	bool poseUpPrev;
	bool poseDnPrev;

	int posePrev;

	
	float resistanceAir[3];
	float resistanceGround;

	float defVelocity[3];
	float addVelocity[3];
	float accVelocity[3];

	float tempAccVelocity;

	float maxVelocity[3];
	float breakVelocity[3];

	

	vector3 lastPos;
};

RegisterFactory(UePlayerPhysicsProperty);

} // namespace Properties
#endif