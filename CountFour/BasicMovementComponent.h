#pragma once
#include "Component.h"

class BasicMovementComponent:public Component
{
public:
	BasicMovementComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;

	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetAngularSpeed(const float angularspeed) { mAngularSpeed = angularspeed; }
	void SetForwardSpeed(const float forwardspeed) { mForwardSpeed = forwardspeed; }
protected:
	float mAngularSpeed;
	float mForwardSpeed;
};

