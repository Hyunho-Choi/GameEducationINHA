#pragma once
class BaseObject
{
protected:
	string mObjectName;
public:
	BaseObject();
	virtual ~BaseObject();

	virtual void Update();
	virtual void Render();

	string GetObjectName()
	{
		return mObjectName;
	}

	void SetObjectName(string objName)
	{
		mObjectName = objName;
	}
};
