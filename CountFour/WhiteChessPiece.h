#pragma once
#include "Actor.h"

class WhiteChessPiece:public Actor
{
public:
	WhiteChessPiece(class Game* game, Vector2 Position);
	void UpdateActor(const float deltaTime) override;

	Vector2& GetFinalPosition() { return mFinalPosition; }
	void SetFinalPosition(Vector2& finalPosition) { mFinalPosition = finalPosition; }
private:
	class BasicMovementComponent* mc;
	Vector2 mFinalPosition;
};

