#include "WhiteChessPiece.h"
#include "SpriteComponent.h"
#include "BasicMovementComponent.h"
#include "Game.h"

WhiteChessPiece::WhiteChessPiece(Game* game, Vector2 position)
	:Actor(game)
{
	SetRotation(Math::Pi*1.5);
	SetPosition(position);
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/WhiteChessPiece.png"));
	mc = new BasicMovementComponent(this);
	mc->SetForwardSpeed(100.0f);
}

void WhiteChessPiece::UpdateActor(const float deltaTime)
{
	if (GetPosition().y >= mFinalPosition.y)
	{
		SetPosition(mFinalPosition);
		mc->SetForwardSpeed(0.0f);
		GetGame()->SetAIOver();
		GetGame()->RemoveActor(this);
	}
}
