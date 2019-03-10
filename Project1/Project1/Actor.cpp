
#include "Actor.h"
#include "Game.h"
#include "Vector2.h"


Actor::Actor(class Game* game)
{
	mGame = game;
	mState = State::EActive;
	position = Vector3(-512,384,0);
	mRotation = Quaternion(0,0,0,0);
	mScale = 1.0f;
	recomputeWorldTransform = true; //atleast one transform will happen
	mGame->AddActor(this);
}


Actor::~Actor()
{
	mGame->RemoveActor(this);
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

Actor::State Actor::GetState()
{
	return this->mState;
}

void Actor::Update(float deltaTime)
{
	if (mState == State::EActive)
	{
		CreateWorldTransform();
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
		CreateWorldTransform();
	}
}

void Actor::UpdateActor(float deltaTime)
{

}

void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void Actor::AddComponent(Component* component)
{
	// Find the insertion point in the sorted vector
	// (The first element with a order higher than me)
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
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

void Actor::ProcessInput(const Uint8* state)
{
	if (mState == State::EActive)
	{
		for (auto comp : mComponents)
		{
			comp->ProcessInput(state);
		}
		ActorInput(state);
	}
}

void Actor::ActorInput(const Uint8 * state)
{
}

void Actor::CreateWorldTransform()
{
	if (recomputeWorldTransform)
	{
		//recomputeWorldTransform = false;
		//Tranform order is translation, rotation, scale
		//They have to be multiplied in reverse order because 
		//Transform is computed right to left
		worldTransform = Matrix4::CreateScale(mScale);
		worldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		worldTransform *= Matrix4::CreateTranslation(position);

		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}
