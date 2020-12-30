#include "stdafx.h"
#include "FBXLoader.h"

FBXLoader::FBXLoader() :
	mFbxManager(nullptr),
	mFbxScene(nullptr),
	mFbxNode(nullptr),
	mFbxMesh(nullptr)
{
	mFbxManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(mFbxManager, IOSROOT);
	mFbxManager->SetIOSettings(ios);
	mFbxScene = FbxScene::Create(mFbxManager, "Scene");
	mFbxNode = FbxNode::Create(mFbxScene, "");
	mFbxMesh = FbxMesh::Create(mFbxScene,"");
}

FBXLoader::~FBXLoader()
{
	mFbxScene->Destroy();
	mFbxNode->Destroy();
	mFbxMesh->Destroy();
	mFbxManager->Destroy();
}

void FBXLoader::GetControlPoints(FbxNode* fbxRootNode)
{
	FbxMesh* currMesh = (FbxMesh*)fbxRootNode->GetNodeAttribute();

	unsigned int ctrlPointCount = currMesh->GetControlPointsCount();
	for (unsigned int i = 0; i < ctrlPointCount; ++i)
	{
		CtrlPoint* currCtrlPoint = new CtrlPoint();

		D3DXVECTOR3 currPosition;
		currPosition.x = static_cast<float>(currMesh->GetControlPointAt(i).mData[0]);
		currPosition.y = static_cast<float>(currMesh->GetControlPointAt(i).mData[1]);
		currPosition.z = static_cast<float>(currMesh->GetControlPointAt(i).mData[2]);

		currCtrlPoint->Position = currPosition;
		mControlPoints[i] = currCtrlPoint;
	}
}

void FBXLoader::GetVerticesAndIndice(FbxMesh* pMesh, std::vector<CharacterVertex>& outVertexVector,
	std::vector<uint32_t>& outIndexVector, SkinnedData* outSkinnedData)
{
	std::unordered_map<std::string, std::vector<uint32_t>> IndexVector;
	unordered_map<Vertex, uint32_t> IndexMapping;
	uint32_t VertexIndex = 0;
	uint32_t tCount = pMesh->GetPolygonCount(); // Triangle

	for (uint32_t i = 0; i < tCount; ++i)
	{
		// For indexing by bone
		std::string CurrBoneName = mControlPoints[pMesh->GetPolygonVertex(i, 1)]->BoneName;

		// Vertex and Index info
		for (int j = 0; j < 3; ++j)
		{
			int controlPointIndex = pMesh->GetPolygonVertex(i, j);
			CtrlPoint* CurrCtrlPoint = mControlPoints[controlPointIndex];

			// Normal
			FbxVector4 pNormal;
			pMesh->GetPolygonVertexNormal(i, j, pNormal);

			// UV
			float* lUVs = NULL;
			FbxStringList lUVNames;
			pMesh->GetUVSetNames(lUVNames);
			const char* lUVName = NULL;
			if (lUVNames.GetCount())
			{
				lUVName = lUVNames[0];
			}

			FbxVector2 pUVs;
			bool bUnMappedUV;
			if (!pMesh->GetPolygonVertexUV(i, j, lUVName, pUVs, bUnMappedUV))
			{
				MessageBox(0, L"UV not found", 0, 0);
			}

			Vertex Temp;
			// Position
			Temp.Pos.x = CurrCtrlPoint->Position.x;
			Temp.Pos.y = CurrCtrlPoint->Position.y;
			Temp.Pos.z = CurrCtrlPoint->Position.z;

			// Normal
			Temp.Normal.x = static_cast<float>(pNormal.mData[0]);
			Temp.Normal.y = static_cast<float>(pNormal.mData[1]);
			Temp.Normal.z = static_cast<float>(pNormal.mData[2]);

			// UV
			Temp.TexUV.x = static_cast<float>(pUVs.mData[0]);
			Temp.TexUV.y = static_cast<float>(1.0f - pUVs.mData[1]);

			// push vertex and index
			auto lookup = IndexMapping.find(Temp);

			if (lookup != IndexMapping.end())
			{
				IndexVector[CurrBoneName].push_back(lookup->second);
			}
			else
			{
				// Index
				uint32_t Index = VertexIndex++;
				IndexMapping[Temp] = Index;
				IndexVector[CurrBoneName].push_back(Index);

				// Vertex
				CharacterVertex SkinnedVertexInfo;
				SkinnedVertexInfo.Pos = Temp.Pos;
				SkinnedVertexInfo.Normal = Temp.Normal;
				SkinnedVertexInfo.TexUV = Temp.TexUV;

				CurrCtrlPoint->SortBlendingInfoByWeight();

				// Set the Bone information
				for (int l = 0; l < CurrCtrlPoint->BoneInfo.size(); ++l)
				{
					if (l >= 4)
						break;

					SkinnedVertexInfo.BoneIndices[l] = CurrCtrlPoint->BoneInfo[l].BoneIndices;

					switch (l)
					{
					case 0:
						SkinnedVertexInfo.BoneWeights.x = CurrCtrlPoint->BoneInfo[l].BoneWeight;
						break;
					case 1:
						SkinnedVertexInfo.BoneWeights.y = CurrCtrlPoint->BoneInfo[l].BoneWeight;
						break;
					case 2:
						SkinnedVertexInfo.BoneWeights.z = CurrCtrlPoint->BoneInfo[l].BoneWeight;
						break;
					}
				}

				outVertexVector.push_back(SkinnedVertexInfo);
			}
		}

	}

	/*for (int i = 0; i < mBoneName.size(); ++i)
	{
		auto CurrIndexVector = IndexVector[mBoneName[i]];
		int IndexCount = CurrIndexVector.size();

		(*outSkinnedData).SetSubmeshOffset(IndexCount);

		outIndexVector.insert(outIndexVector.end(), CurrIndexVector.begin(), CurrIndexVector.end());
	}*/

}

void FBXLoader::GetMaterials(FbxNode* node, std::vector<D3DXMATERIAL>& outMaterial)
{
	int MaterialCount = node->GetMaterialCount();

	for (int i = 0; i < MaterialCount; ++i)
	{
		D3DXMATERIAL tempMaterial;
		FbxSurfaceMaterial* SurfaceMaterial = node->GetMaterial(i);
		GetMaterialAttribute(SurfaceMaterial, tempMaterial);
		GetMaterialTexture(SurfaceMaterial, tempMaterial);

		if (tempMaterial.pTextureFilename != "")
		{
			outMaterial.push_back(tempMaterial);
		}
	}
}

void FBXLoader::GetMaterialAttribute(FbxSurfaceMaterial* material, D3DXMATERIAL& outMaterial)
{
	FbxDouble3 double3;
	FbxDouble double1;
	
	outMaterial.MatD3D.Ambient.a = 1.f;
	outMaterial.MatD3D.Diffuse.a = 1.f;
	outMaterial.MatD3D.Specular.a = 1.f;
	outMaterial.MatD3D.Emissive.a = 1.f;
	
	if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		// Amibent Color
		double3 = reinterpret_cast<FbxSurfacePhong*>(material)->Ambient;
		outMaterial.MatD3D.Ambient.r = static_cast<float>(double3.mData[0]);
		outMaterial.MatD3D.Ambient.g = static_cast<float>(double3.mData[1]);
		outMaterial.MatD3D.Ambient.b = static_cast<float>(double3.mData[2]);

		// Diffuse Color
		double3 = reinterpret_cast<FbxSurfacePhong*>(material)->Diffuse;
		outMaterial.MatD3D.Diffuse.r = static_cast<float>(double3.mData[0]);
		outMaterial.MatD3D.Diffuse.g = static_cast<float>(double3.mData[1]);
		outMaterial.MatD3D.Diffuse.b = static_cast<float>(double3.mData[2]);

		// Roughness 
		double1 = reinterpret_cast<FbxSurfacePhong*>(material)->Shininess;
		outMaterial.MatD3D.Power = 1 - double1;

		//// Reflection
		//double3 = reinterpret_cast<FbxSurfacePhong*>(pMaterial)->Reflection;
		//outMaterial.FresnelR0.x = static_cast<float>(double3.mData[0]);
		//outMaterial.FresnelR0.y = static_cast<float>(double3.mData[1]);
		//outMaterial.FresnelR0.z = static_cast<float>(double3.mData[2]);

		// Specular Color
		double3 = reinterpret_cast<FbxSurfacePhong*>(material)->Specular;
		outMaterial.MatD3D.Specular.r = static_cast<float>(double3.mData[0]);
		outMaterial.MatD3D.Specular.g = static_cast<float>(double3.mData[1]);
		outMaterial.MatD3D.Specular.b = static_cast<float>(double3.mData[2]);

		// Emissive Color
		double3 = reinterpret_cast<FbxSurfacePhong*>(material)->Emissive;
		outMaterial.MatD3D.Emissive.r = static_cast<float>(double3.mData[0]);
		outMaterial.MatD3D.Emissive.g = static_cast<float>(double3.mData[1]);
		outMaterial.MatD3D.Emissive.b = static_cast<float>(double3.mData[2]);

		/*
		// Transparency Factor
		double1 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->TransparencyFactor;
		currMaterial->mTransparencyFactor = double1;
		// Specular Factor
		double1 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->SpecularFactor;
		currMaterial->mSpecularPower = double1;
		// Reflection Factor
	double1 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->ReflectionFactor;
		currMaterial->mReflectionFactor = double1;	*/
	}
	else if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		// Amibent Color
		double3 = reinterpret_cast<FbxSurfaceLambert*>(material)->Ambient;
		outMaterial.MatD3D.Ambient.r = static_cast<float>(double3.mData[0]);
		outMaterial.MatD3D.Ambient.g = static_cast<float>(double3.mData[1]);
		outMaterial.MatD3D.Ambient.b = static_cast<float>(double3.mData[2]);

		// Diffuse Color
		double3 = reinterpret_cast<FbxSurfaceLambert*>(material)->Diffuse;
		outMaterial.MatD3D.Ambient.r = static_cast<float>(double3.mData[0]);
		outMaterial.MatD3D.Ambient.g = static_cast<float>(double3.mData[1]);
		outMaterial.MatD3D.Ambient.b = static_cast<float>(double3.mData[2]);

		// Emissive Color
		double3 = reinterpret_cast<FbxSurfaceLambert*>(material)->Emissive;
		outMaterial.MatD3D.Emissive.r = static_cast<float>(double3.mData[0]);
		outMaterial.MatD3D.Emissive.g = static_cast<float>(double3.mData[1]);
		outMaterial.MatD3D.Emissive.b = static_cast<float>(double3.mData[2]);
	}
}

void FBXLoader::GetMaterialTexture(FbxSurfaceMaterial* material, D3DXMATERIAL& mat)
{
	unsigned int textureIndex = 0;
	FbxProperty property;

	FBXSDK_FOR_EACH_TEXTURE(textureIndex)
	{
		property = material->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);
		if (property.IsValid())
		{
			unsigned int textureCount = property.GetSrcObjectCount<FbxTexture>();
			for (unsigned int i = 0; i < textureCount; ++i)
			{
				FbxLayeredTexture* layeredTexture = property.GetSrcObject<FbxLayeredTexture>(i);
				if (layeredTexture)
				{
					throw std::exception("Layered Texture is currently unsupported\n");
				}
				else
				{
					FbxTexture* texture = property.GetSrcObject<FbxTexture>(i);
					if (texture)
					{
						std::string textureType = property.GetNameAsCStr();
						FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);

						if (fileTexture)
						{
							cout << textureType << endl;
							cout << fileTexture->GetRelativeFileName() << endl;
							if (textureType == "DiffuseColor")
							{
								mat.pTextureFilename = new char[strlen(fileTexture->GetRelativeFileName()) + 1];
								lstrcpyA(mat.pTextureFilename, fileTexture->GetRelativeFileName());
							}
							/*else if (textureType == "SpecularColor")
							{
							Mat->mSpecularMapName = fileTexture->GetFileName();
							}
							else if (textureType == "Bump")
							{
							Mat->mNormalMapName = fileTexture->GetFileName();
							}*/
						}
					}
				}
			}
		}
	}
}

LPD3DXMESH FBXLoader::loadFbx(string fileName)
{
	LPD3DXMESH fbxMesh = nullptr;
	FbxImporter* importer = FbxImporter::Create(mFbxManager, "");
	if (!importer->Initialize(fileName.c_str(), -1, mFbxManager->GetIOSettings()))
	{
		assert("importer Initialize Error\n");
	}
	importer->Import(mFbxScene);
	mFbxNode = mFbxScene->GetRootNode();
	
	FbxAxisSystem sceneAxisSystem = mFbxScene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem::DirectX.ConvertScene(mFbxScene);

	FbxGeometryConverter geometryConverter(mFbxManager);
	geometryConverter.Triangulate(mFbxScene, true);

	FbxNode* fbxRootNode = mFbxScene->GetRootNode();

	vector<CharacterVertex> outVertexVector;
	vector<uint32_t> outIndexVector;
	vector<D3DXMATERIAL> outMaterial;
	
	if (fbxRootNode)
	{
		int childNodeCount = fbxRootNode->GetChildCount();
		// Skeleton Bone Hierarchy Index 
		//for (int i = 0; i < fbxRootNode->GetChildCount(); i++)
		//{
		//	FbxNode* pFbxChildNode = fbxRootNode->GetChild(i);
		//	FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();
		//	FbxNodeAttribute::EType AttributeType = pMesh->GetAttributeType();
		//	if (!pMesh || !AttributeType) { continue; }

		//	switch (AttributeType)
		//	{
		//	/*case FbxNodeAttribute::eSkeleton:
		//		GetSkeletonHierarchy(pFbxChildNode, outSkinnedData, 0, -1);
		//		break;*/
		//	}
		//}

		//mBoneName = outSkinnedData.GetBoneName();
		// Bone offset, Control point, Vertex, Index Data
		// And Animation Data
		for (int i = 0; i < childNodeCount; i++)
		{
			FbxNode* fbxChildNode = fbxRootNode->GetChild(i);
			FbxMesh* mesh = (FbxMesh*)fbxChildNode->GetMesh();
			FbxNodeAttribute::EType AttributeType = fbxChildNode->GetNodeAttribute()->GetAttributeType();
			if (!mesh || !AttributeType) { continue; }

			if (AttributeType == FbxNodeAttribute::eMesh)
			{
				GetControlPoints(fbxChildNode);

				// To access the bone index directly
				//mBoneOffsets.resize(mBoneHierarchy.size());

				// Get Animation Clip
				//GetAnimation(pFbxScene, pFbxChildNode, outSkinnedData, clipName, false);
				/*std::string outAnimationName;
				GetAnimation(pFbxScene, pFbxChildNode, outAnimationName, clipName);
				outSkinnedData.SetAnimationName(clipName);*/
				//outSkinnedData.SetAnimationName(outAnimationName);

				// Get Vertices and indices info
				GetVerticesAndIndice(mesh, outVertexVector, outIndexVector, nullptr);

				GetMaterials(fbxChildNode, outMaterial);

			}
		}

		//outSkinnedData.Set(mBoneHierarchy, mBoneOffsets, &mAnimations);
	}
	
	importer->Destroy();
	return fbxMesh;
}

//void FBXLoader::loadNode(FbxNode* node)
//{
//	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
//
//	GetFbxInfo(node);
//	
//	vector<D3DXVECTOR3> positions;
//	if(nodeAttribute)
//	{
//		if(nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
//		{
//			FbxMesh* mesh = node->GetMesh();
//			processControlPoints(mesh, positions);
//			UINT triangleCount = mesh->GetPolygonCount();
//			UINT vertexCount = 0;
//			for (int i = 0; i < triangleCount; ++i)
//			{
//				for (int j = 0; j < 3; ++j)
//				{
//					int controlPointIndex = mesh->GetPolygonVertex(i, j);
//
//					D3DXVECTOR3& position = positions[controlPointIndex];
//					D3DXVECTOR3 normal = readNormal(mesh, controlPointIndex, vertexCount);
//					D3DXVECTOR2 texUV = readTexUV(mesh, controlPointIndex, mesh->GetTextureUVIndex(i, j));
//					if (FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0))
//					{
//						D3DXVECTOR3 binormal = readBinormal(vertexBinormal, controlPointIndex, vertexCount);
//					}
//					if (FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0))
//					{
//						D3DXVECTOR3 tangent = readTangent(vertexTangent, controlPointIndex, vertexCount);
//					}
//					vertexCount++;
//				}
//			}
//		}
//	}
//	
//	const int childCount = node->GetChildCount();
//	for (int i = 0; i < childCount; ++i)
//	{
//		loadNode(node->GetChild(i));
//	}
//}
//
//void FBXLoader::GetFbxInfo(FbxNode* Node)
//{
//	auto mesh = Node->GetMesh();
//	if (mesh != NULL)
//	{
//		// get vertices, indices, uv, etc
//		// some more code...
//		// get vertices, indices, uv, etc
//
//		//================= Texture ========================================
//
//		int materialCount = Node->GetSrcObjectCount<FbxSurfaceMaterial>();
//
//		for (int index = 0; index < materialCount; index++)
//		{
//			FbxSurfaceMaterial* material = (FbxSurfaceMaterial*)Node->GetSrcObject<FbxSurfaceMaterial>(index);
//
//			if (material != NULL)
//			{
//				// This only gets the material of type sDiffuse, you probably need to traverse all Standard Material Property by its name to get all possible textures.
//				FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
//
//				// Check if it's layeredtextures
//				int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();
//
//				if (layeredTextureCount > 0)
//				{
//					for (int j = 0; j < layeredTextureCount; j++)
//					{
//						FbxLayeredTexture* layered_texture = FbxCast<FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));
//						int lcount = layered_texture->GetSrcObjectCount<FbxTexture>();
//
//						for (int k = 0; k < lcount; k++)
//						{
//							FbxTexture* texture = FbxCast<FbxTexture>(layered_texture->GetSrcObject<FbxTexture>(k));
//							// Then, you can get all the properties of the texture, include its name
//							const char* textureName = texture->GetName();
//						}
//					}
//				}
//				else
//				{
//					// Directly get textures
//					int textureCount = prop.GetSrcObjectCount<FbxTexture>();
//					for (int j = 0; j < textureCount; j++)
//					{
//						FbxTexture* texture = FbxCast<FbxTexture>(prop.GetSrcObject<FbxTexture>(j));
//						// Then, you can get all the properties of the texture, include its name
//						const char* textureName = texture->GetName();
//						FbxProperty p = texture->RootProperty.Find("Filename");
//
//
//						HRESULT hr;
//						/*D3DX11CreateTextureFromFile(Game::GetInstance()->GetRenderer()->GetDevice(), textureName, 0, 0, &m_texture, &hr);
//						if (FAILED(hr))
//						{
//							std::string message;
//							message.append("Load Texture: ");
//							message.append(texture->GetName());
//							message.append(" failed");
//							SHOWMESSAGEBOX(hr, message.c_str());
//						}*/
//					}
//				}
//			}
//		}
//	}
//}
//
//void FBXLoader::processControlPoints(FbxMesh* mesh, vector<D3DXVECTOR3>& positions)
//{
//	UINT pointCount = mesh->GetControlPointsCount();
//
//	for (int i = 0; i < pointCount; ++i)
//	{
//		D3DXVECTOR3 position;
//
//		position.x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]);
//		position.y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]);
//		position.z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]);
//
//		positions.push_back(position);
//	}
//}
//
//D3DXVECTOR3 FBXLoader::readNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter)
//{
//	if(mesh->GetElementNormalCount() < 1)
//	{
//		cout << "No normals" << endl; // TODO Logger·Î ´ëÃ¼
//	}
//
//	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
//	D3DXVECTOR3 normal;
//
//	switch (vertexNormal->GetMappingMode())
//	{
//		case FbxGeometryElement::eByControlPoint:
//		{
//			normal = readNormalByControlPoint(vertexNormal, controlPointIndex);
//		}
//		break;
//		case FbxGeometryElement::eByPolygonVertex :
//		{
//			normal = readNormalByPolygonVertex(vertexNormal, controlPointIndex);
//		}
//		break;
//	}
//	return normal;
//}
//
//D3DXVECTOR3 FBXLoader::readBinormal(const FbxGeometryElementBinormal* vertexBinormal, int controlPointIndex, int vertexCounter)
//{
//	D3DXVECTOR3 binormal;
//	switch (vertexBinormal->GetMappingMode())
//	{
//	case FbxGeometryElement::eByControlPoint:
//	{
//		binormal = readBinormalByControlPoint(vertexBinormal, controlPointIndex);
//	}
//	break;
//	case FbxGeometryElement::eByPolygonVertex:
//	{
//		binormal = readBinormalByPolygonVertex(vertexBinormal, vertexCounter);
//	}
//	break;
//	}
//	return binormal;
//}
//
//D3DXVECTOR3 FBXLoader::readTangent(const FbxGeometryElementTangent* vertexTangent, int controlPointIndex, int vertexCounter)
//{
//	D3DXVECTOR3 tangent;
//	switch (vertexTangent->GetMappingMode())
//	{
//	case FbxGeometryElement::eByControlPoint:
//	{
//		tangent = readTangentByControlPoint(vertexTangent, controlPointIndex);
//	}
//	break;
//	case FbxGeometryElement::eByPolygonVertex:
//	{
//		tangent = readTangentByPolygonVertex(vertexTangent, vertexCounter);
//	}
//	break;
//	}
//	return tangent;
//}
//
//D3DXVECTOR2 FBXLoader::readTexUV(const FbxMesh* mesh, int controlPointIndex, int uvIndex)
//{
//	const FbxGeometryElementUV* vertexUV = mesh->GetElementUV(0);
//	D3DXVECTOR2 uvVertex;
//	switch (vertexUV->GetMappingMode())
//	{
//	case FbxGeometryElement::eByControlPoint:
//	{
//		switch (vertexUV->GetReferenceMode())
//		{
//			case FbxGeometryElement::eDirect:
//			{
//				uvVertex.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(controlPointIndex).mData[0]);
//				uvVertex.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(controlPointIndex).mData[1]);
//			}
//		break;
//			case FbxGeometryElement::eIndexToDirect:
//			{
//				int index = vertexUV->GetIndexArray().GetAt(controlPointIndex);
//				uvVertex.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
//				uvVertex.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
//			}
//		break;
//		}
//	}
//	break;
//	case FbxGeometryElement::eByPolygonVertex:
//	{
//	}
//	break;
//	}
//	return uvVertex;
//}
//
//D3DXVECTOR3 FBXLoader::readNormalByControlPoint(const FbxGeometryElementNormal* vertexNormal, int controlPointIndex)
//{
//	D3DXVECTOR3 normal;
//	switch (vertexNormal->GetReferenceMode())
//	{
//		case FbxGeometryElement::eDirect:
//		{
//			normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
//			normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
//			normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
//		}
//		break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
//			normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
//			normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
//			normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
//		}
//		break;
//	}
//	return normal;
//}
//
//D3DXVECTOR3 FBXLoader::readNormalByPolygonVertex(const FbxGeometryElementNormal* vertexNormal, int vertexCounter)
//{
//	D3DXVECTOR3 normal;
//	switch (vertexNormal->GetReferenceMode())
//	{
//		case FbxGeometryElement::eDirect:
//		{
//			normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
//			normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
//			normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
//		}
//		break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
//			normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
//			normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
//			normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
//		}
//		break;
//	}
//	return normal;
//}
//
//D3DXVECTOR3 FBXLoader::readBinormalByControlPoint(const FbxGeometryElementBinormal* vertexBinormal, int controlPointIndex)
//{
//	D3DXVECTOR3 binormal;
//	switch (vertexBinormal->GetReferenceMode())
//	{
//		case FbxGeometryElement::eDirect:
//		{
//			binormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
//			binormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
//			binormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
//		}
//		break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int index = vertexBinormal->GetIndexArray().GetAt(controlPointIndex);
//			binormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
//			binormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
//			binormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
//		}
//		break;
//	}
//	return binormal;
//}
//
//D3DXVECTOR3 FBXLoader::readBinormalByPolygonVertex(const FbxGeometryElementBinormal* vertexBinormal, int vertexCounter)
//{
//	D3DXVECTOR3 binormal;
//	switch (vertexBinormal->GetReferenceMode())
//	{
//		case FbxGeometryElement::eDirect:
//		{
//			binormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
//			binormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
//			binormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
//		}
//		break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int index = vertexBinormal->GetIndexArray().GetAt(vertexCounter);
//			binormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
//			binormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
//			binormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
//		}
//		break;
//	}
//	return binormal;
//}
//
//D3DXVECTOR3 FBXLoader::readTangentByControlPoint(const FbxGeometryElementTangent* vertexTangent, int controlPointIndex)
//{
//	D3DXVECTOR3 tangent;
//	switch (vertexTangent->GetReferenceMode())
//	{
//	case FbxGeometryElement::eDirect:
//	{
//		tangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[0]);
//		tangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[1]);
//		tangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[2]);
//	}
//	break;
//	case FbxGeometryElement::eIndexToDirect:
//	{
//		int index = vertexTangent->GetIndexArray().GetAt(controlPointIndex);
//		tangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
//		tangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
//		tangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
//	}
//	break;
//	}
//	return tangent;
//}
//
//D3DXVECTOR3 FBXLoader::readTangentByPolygonVertex(const FbxGeometryElementTangent* vertexTangent, int vertexCounter)
//{
//	D3DXVECTOR3 tangent;
//	switch (vertexTangent->GetReferenceMode())
//	{
//		case FbxGeometryElement::eDirect:
//		{
//			tangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[0]);
//			tangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[1]);
//			tangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[2]);
//		}
//		break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int index = vertexTangent->GetIndexArray().GetAt(vertexCounter);
//			tangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
//			tangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
//			tangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
//		}
//		break;
//	}
//	return tangent;
//}
