#pragma once
#include "Component.h"
#include <SDL.h>

class SpriteComponent:Component
{
public :
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	virtual ~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void SetTexture(SDL_Texture* texture);
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }

	int GetDrawOrder() const { return mDrawOrder; }
protected:
	SDL_Texture* mTexture;
	int mTexHeight;
	int mTexWidth;
	int mDrawOrder;
};

