#pragma once
enum class eKeyName;
class Player;

enum class eCharacterStateName
{
	IDLE_STATE = 0, WALK_STATE, INTERACTION_STATE
};

class CharacterState
{
protected:
	eCharacterStateName mStateName;
public:
	CharacterState();
	virtual ~CharacterState() = default;

	virtual void Enter(Player& player) {}
	virtual CharacterState* HandleInput(Player& player, eEventName eventName, eKeyName &key)
	{
		return nullptr;
	}
	virtual CharacterState* Update(Player& player) { return nullptr; }
	eCharacterStateName GetStateName() const { return mStateName; }
};

