#include "BlackChessPiece.h"
#include "SpriteComponent.h"
#include "BasicMovementComponent.h"
#include "Game.h"

BlackChessPiece::BlackChessPiece(class Game* game, Vector2 position)
	:Actor(game),
	mFinalPosition(Vector2(0.0f,0.f))
{
	SetRotation(Math::Pi*1.5);
	SetPosition(position);
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/BlackChessPiece.png"));
	mc = new BasicMovementComponent(this);
	mc->SetForwardSpeed(100.0f);
}

void BlackChessPiece::UpdateActor(float deltaTime)
{
	if (GetPosition().y >= mFinalPosition.y)
	{
		SetPosition(mFinalPosition);
		mc->SetForwardSpeed(0.0f);
		GetGame()->SetPlayerOver();
		GetGame()->RemoveActor(this);
	}
}
