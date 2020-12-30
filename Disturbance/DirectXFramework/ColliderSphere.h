#pragma once
#include "ColliderPolygon.h"

class ColliderSphere : public ColliderPolygon
{
public:
	ColliderSphere();
	virtual ~ColliderSphere();
	void Setup() override;
	void Update() override;
	void Render() override;
	
	void SetRadius(float radius);
	float& GetRadius() { return mRadius; }
	
private:
	float mRadius;
};

