#include "BasicMovementComponent.h"
#include "Actor.h"

BasicMovementComponent::BasicMovementComponent(Actor* owner, int updateOrder)
	:Component(owner,updateOrder),
	mAngularSpeed(0.0f),
	mForwardSpeed(0.0f)
{
}

void BasicMovementComponent::Update(float deltaTime)
{
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}
	
	if (!Math::NearZero(mForwardSpeed))
	{
		Vector2 pos = mOwner->GetPosition();
		pos.x += mOwner->GetForward().x * mForwardSpeed * deltaTime;
		pos.y += mOwner->GetForward().y * mForwardSpeed * deltaTime;
		mOwner->SetPosition(pos);
	}
}
