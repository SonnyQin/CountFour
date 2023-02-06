#pragma once
#include "Actor.h"

class BlackChessPiece:Actor
{
public:
	BlackChessPiece(class Game* game, Vector2 position);
	void UpdateActor(const float deltaTime) override;

	Vector2& GetFinalPosition() { return mFinalPosition; }
	void SetFinalPosition(Vector2& finalPosition) { mFinalPosition = finalPosition; }
private:
	class BasicMovementComponent* mc;
	Vector2 mFinalPosition;
};

