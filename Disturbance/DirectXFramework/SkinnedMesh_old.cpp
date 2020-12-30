#include "stdafx.h"
#include "SkinnedMesh.h"

void SkinnedMesh::Render(D3DXFRAME* node, D3DXMATRIX& parentWorldMat)
{
	D3DXMATRIX combinedMat = node->TransformationMatrix * parentWorldMat;
	gD3Device->SetTransform(D3DTS_WORLD, &combinedMat);
	for (UINT i = 0; i < node->pMeshContainer->NumMaterials; ++i)
	{
		D3DXMESHCONTAINER* mc = node->pMeshContainer;
		D3DMATERIAL9& mtrl = mc->pMaterials[i].MatD3D;
		gD3Device->SetMaterial(&mtrl);
		node->pMeshContainer->MeshData.pMesh->DrawSubset(i);
	}
	if(node->pFrameFirstChild)
	{
		Render(node->pFrameSibling, parentWorldMat);
	}
	if(node->pFrameFirstChild)
	{
		Render(node->pFrameFirstChild, combinedMat);
	}
}

void SkinnedMesh::Render()
{
	D3DXMATRIXA16 identityMat;
	D3DXMatrixIdentity(&identityMat);
	Render(rootFrame, identityMat);
}

HRESULT AllocateHierarchyEx::AllocateName(LPCSTR Name, LPSTR* pNewName)
{
	UINT cbLength;

	if (Name != NULL)
	{
		cbLength = (UINT)strlen(Name) + 1;
		*pNewName = new CHAR[cbLength];
		if (*pNewName == NULL)
			return E_OUTOFMEMORY;
		memcpy(*pNewName, Name, cbLength * sizeof(CHAR));
	}
	else
	{
		*pNewName = NULL;
	}

	return S_OK;
}

HRESULT AllocateHierarchyEx::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	HRESULT hr = S_OK;
	FrameEx* pFrame;

	*ppNewFrame = NULL;

	pFrame = new FrameEx;
	if (pFrame == NULL)
	{
		hr = E_OUTOFMEMORY;
		return hr;
	}

	hr = AllocateName(Name, &pFrame->Name);
	if (FAILED(hr))
	{
		return hr;
	}

	// initialize other data members of the frame
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransform);

	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;

	*ppNewFrame = pFrame;
	pFrame = NULL;

	delete pFrame;
	return hr;
}

HRESULT AllocateHierarchyEx::CreateMeshContainer(LPCSTR Name, const D3DXMESHDATA* pMeshData,
	const D3DXMATERIAL* pMaterials, const D3DXEFFECTINSTANCE* pEffectInstances, DWORD NumMaterials,
	const DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER* ppNewMeshContainer)
{
    HRESULT hr;
    MeshContainerEx* pMeshContainer = NULL;
    UINT NumFaces;
    UINT iMaterial;
    UINT iBone, cBones;
    LPDIRECT3DDEVICE9 pd3dDevice = NULL;

    LPD3DXMESH pMesh = NULL;

    *ppNewMeshContainer = NULL;

    // this sample does not handle patch meshes, so fail when one is found
    if (pMeshData->Type != D3DXMESHTYPE_MESH)
    {
        hr = E_FAIL;
        goto e_Exit;
    }

    // get the pMesh interface pointer out of the mesh data structure
    pMesh = pMeshData->pMesh;

    // this sample does not FVF compatible meshes, so fail when one is found
    if (pMesh->GetFVF() == 0)
    {
        hr = E_FAIL;
        goto e_Exit;
    }

    // allocate the overloaded structure to return as a D3DXMESHCONTAINER
    pMeshContainer = new MeshContainerEx;
    if (pMeshContainer == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    memset(pMeshContainer, 0, sizeof(MeshContainerEx));

    // make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
    hr = AllocateName(Name, &pMeshContainer->Name);
    if (FAILED(hr))
        goto e_Exit;

    pMesh->GetDevice(&pd3dDevice);
    NumFaces = pMesh->GetNumFaces();

    // if no normals are in the mesh, add them
    if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
    {
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

        // clone the mesh to make room for the normals
        hr = pMesh->CloneMeshFVF(pMesh->GetOptions(),
            pMesh->GetFVF() | D3DFVF_NORMAL,
            pd3dDevice, &pMeshContainer->MeshData.pMesh);
        if (FAILED(hr))
            goto e_Exit;

        // get the new pMesh pointer back out of the mesh container to use
        // NOTE: we do not release pMesh because we do not have a reference to it yet
        pMesh = pMeshContainer->MeshData.pMesh;

        // now generate the normals for the pmesh
        D3DXComputeNormals(pMesh, NULL);
    }
    else  // if no normals, just add a reference to the mesh for the mesh container
    {
        pMeshContainer->MeshData.pMesh = pMesh;
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

        pMesh->AddRef();
    }

    // allocate memory to contain the material information.  This sample uses
    //   the D3D9 materials and texture names instead of the EffectInstance style materials
    pMeshContainer->NumMaterials = max(1, NumMaterials);
    pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
    pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
    pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
    if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces * 3);
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

    // if materials provided, copy them
    if (NumMaterials > 0)
    {
        memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

        for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
        {
            if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
            {
                WCHAR strTexturePath[MAX_PATH];
                WCHAR wszBuf[MAX_PATH];
                MultiByteToWideChar(CP_ACP, 0, pMeshContainer->pMaterials[iMaterial].pTextureFilename, -1, wszBuf, MAX_PATH);
                wszBuf[MAX_PATH - 1] = L'\0';
                wsprintfW(strTexturePath, L"%s/%s", folderPath, wszBuf);
                if (FAILED(D3DXCreateTextureFromFile(pd3dDevice, strTexturePath,
                    &pMeshContainer->ppTextures[iMaterial])))
                    pMeshContainer->ppTextures[iMaterial] = NULL;

                // don't remember a pointer into the dynamic memory, just forget the name after loading
                pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
            }
        }
    }
    else // if no materials provided, use a default one
    {
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }

    // if there is skinning information, save off the required data and then setup for HW skinning
    if (pSkinInfo != NULL)
    {
        // first save off the SkinInfo and original mesh data
        pMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();

        pMeshContainer->pOrigMesh = pMesh;
        pMesh->AddRef();

        // Will need an array of offset matrices to move the vertices from the figure space to the bone's space
        cBones = pSkinInfo->GetNumBones();
        pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
        if (pMeshContainer->pBoneOffsetMatrices == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        // get each of the bone offset matrices so that we don't need to get them later
        for (iBone = 0; iBone < cBones; iBone++)
        {
            pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
        }

        // GenerateSkinnedMesh will take the general skinning information and transform it to a HW friendly version
        hr = GenerateSkinnedMesh(pd3dDevice, pMeshContainer);
        if (FAILED(hr))
            goto e_Exit;
    }

    *ppNewMeshContainer = pMeshContainer;
    pMeshContainer = NULL;

e_Exit:
    SAFE_RELEASE(pd3dDevice);

    // call Destroy function to properly clean up the memory allocated 
    if (pMeshContainer != NULL)
    {
        DestroyMeshContainer(pMeshContainer);
    }

    return hr;
}

HRESULT AllocateHierarchyEx::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
}

HRESULT AllocateHierarchyEx::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
}

HRESULT GenerateSkinnedMesh(IDirect3DDevice9* pd3dDevice, MeshContainerEx* pMeshContainer)
{
    HRESULT hr = S_OK;
    D3DCAPS9 d3dCaps;
    pd3dDevice->GetDeviceCaps(&d3dCaps);

    if (pMeshContainer->pSkinInfo == NULL)
        return hr;


    SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
    SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
    // Get palette size
    // First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
    // (96 - 9) /3 i.e. Maximum constant count - used constants 
    UINT MaxMatrices = 26;
    pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

    DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
    if (d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
    {
        pMeshContainer->UseSoftwareVP = false;
        Flags |= D3DXMESH_MANAGED;
    }
    else
    {
        pMeshContainer->UseSoftwareVP = true;
        Flags |= D3DXMESH_SYSTEMMEM;
    }

    SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

    hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
    (
        pMeshContainer->pOrigMesh,
        Flags,
        pMeshContainer->NumPaletteEntries,
        pMeshContainer->pAdjacency,
        NULL, NULL, NULL,
        &pMeshContainer->NumInfl,
        &pMeshContainer->NumAttributeGroups,
        &pMeshContainer->pBoneCombinationBuf,
        &pMeshContainer->MeshData.pMesh);
    if (FAILED(hr))
        goto e_Exit;


    // FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
    DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL |
        D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
    if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
    {
        LPD3DXMESH pMesh;
        hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF,
            pd3dDevice, &pMesh);
        if (!FAILED(hr))
        {
            pMeshContainer->MeshData.pMesh->Release();
            pMeshContainer->MeshData.pMesh = pMesh;
            pMesh = NULL;
        }
    }

    D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
    LPD3DVERTEXELEMENT9 pDeclCur;
    hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
    if (FAILED(hr))
        goto e_Exit;

    // the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
    //   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
    //          this is more of a "cast" operation
    pDeclCur = pDecl;
    while (pDeclCur->Stream != 0xff)
    {
        if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
            pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
        pDeclCur++;
    }

    hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
    if (FAILED(hr))
        goto e_Exit;

    // allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
    if (g_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
    {
        g_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

        // Allocate space for blend matrices
        delete[] g_pBoneMatrices;
        g_pBoneMatrices = new D3DXMATRIXA16[g_NumBoneMatricesMax];
        if (g_pBoneMatrices == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }
    }

e_Exit:
    return hr;
}