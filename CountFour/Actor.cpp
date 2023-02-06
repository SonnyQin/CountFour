#include "Actor.h"
#include "Component.h"
#include "Game.h"

Actor::Actor(Game* game)
	:mPosition(Vector2(0,0)),
	mRotation(0.0f),
	mScale(1.0f),
	mGame(game),
	mState(State::EActive)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);
}

void Actor::Update(const float deltaTime)
{
	if (mState == EActive)
	{
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(const float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(const float deltaTime)
{
}

void Actor::ProcessInput(const uint8_t* keyState)
{
	if (mState == EActive)
	{
		for (auto comp : mComponents)
		{
			comp->ProcessInput(keyState);
		}
		Actor::ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t* keyState)
{
}

void Actor::AddComponent(Component* component)
{
	int myUpdateOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); iter++)
	{
		if ((*iter)->GetUpdateOrder() < myUpdateOrder)
		{
			break;
		}
	}
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
