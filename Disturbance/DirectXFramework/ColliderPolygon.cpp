#include "stdafx.h"
#include "ColliderPolygon.h"

ColliderPolygon::ColliderPolygon()
	: mMesh(nullptr)
	, mPosition(nullptr)
{
}

ColliderPolygon::~ColliderPolygon()
{
	SAFE_DELETE(mMesh);
}
