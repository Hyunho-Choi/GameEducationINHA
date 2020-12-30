#pragma once
#include "Static3DObject.h"
#include "functional"
class Interactable3DObject :
	public Static3DObject
{
protected:
	bool mbTryInteractionCalled;
	vector<function<bool()>> mInteractionCondition;
	vector<function<void()>> mInteractionBehavior;

	bool CheckCondition();
	void DoBehavior();
public :
	Interactable3DObject();
	Interactable3DObject(eObjTag type);
	~Interactable3DObject();

	void Update() override;

	bool GetTryInteractionCalled() const { return mbTryInteractionCalled; }
	
	void AddInteractionCondition(function<bool()> conditionFunc)
	{
		mInteractionCondition.push_back(conditionFunc);
	}

	void AddInteractionBehavior(function<void()> behaviorFunc)
	{
		mInteractionBehavior.push_back(behaviorFunc);
	}
	
	bool TryInteraction();
	void ChangeToStaticObject();
	void ChangeToInteractObject();
};

