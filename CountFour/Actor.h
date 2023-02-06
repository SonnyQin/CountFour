#pragma once
#include "Math.h"
#include <cstdint>
#include <vector>

class Actor
{
public:
	Actor(class Game* game);
	virtual ~Actor();

	enum State
	{
		EActive,
		EPaused,
		EDead
	};
	void Update(const float deltaTime);
	void UpdateComponents(const float deltaTime);
	virtual void UpdateActor(const float deltaTime);

	void ProcessInput(const uint8_t* keyState);//all input functions from components and actor at last
	virtual void ActorInput(const uint8_t* keyState);

	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() const { return mGame; }

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);
protected:
	State mState;
	Vector2 mPosition;
	float mScale;
	float mRotation;
	class Game* mGame;
	std::vector<class Component*> mComponents;
};

