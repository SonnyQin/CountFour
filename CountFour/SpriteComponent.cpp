#include "SpriteComponent.h"
#include "Game.h"
#include "Actor.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner),
	mTexture(nullptr),
	mTexHeight(0),
	mTexWidth(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		SDL_Rect r;
		r.x = static_cast<int>(mOwner->GetPosition().x);
		r.y = static_cast<int>(mOwner->GetPosition().y);
		r.w = static_cast<int>(mOwner->GetScale() * mTexWidth);
		r.h = static_cast<int>(mOwner->GetScale() * mTexHeight);

		SDL_RenderCopyEx(renderer, mTexture, nullptr, &r, -Math::ToDegrees(mOwner->GetRotation()), nullptr,SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;
	SDL_QueryTexture(texture, NULL, NULL, &mTexWidth, &mTexHeight);
}
