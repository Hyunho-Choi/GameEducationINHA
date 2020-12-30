#include "stdafx.h"
#include "Astar.h"
#include "Node.h"

Astar::Astar()
{
}

Astar::~Astar()
{
	for (size_t i = 0; i < mNode.size(); ++i)
	{
		SAFE_DELETE(mNode[i]);
	}
	mNode.clear();
}

void Astar::Setup()
{
	mSetOpenList.clear();
	mSetCloseList.clear();
	mPath.clear();
}

void Astar::LoadNode()
{
	mNode.clear();

	gJSON->LoadJSON("Resource/json/Node.json");
	Value& documentValue = gJSON->mDocument["Node"];

	for (SizeType i = 0; i < documentValue.Size(); ++i)
	{
		D3DXVECTOR3 vec;
		vec.x = documentValue[i]["Position"]["PositionX"].GetDouble();
		vec.y = 0.0f;
		vec.z = documentValue[i]["Position"]["PositionZ"].GetDouble();

		Node* node = new Node;
		node->SetNodePoint(vec);
		mNode.push_back(node);
	}

	for (SizeType i = 0; i < documentValue.Size(); ++i)
	{
		Value& pathValue = documentValue[i]["Path"];

		for (SizeType j = 0; j < pathValue.Size(); ++j)
		{
			D3DXVECTOR3 vec;
			vec.x = pathValue[j]["PathX"].GetDouble();
			vec.y = 0.0f;
			vec.z = pathValue[j]["PathZ"].GetDouble();

			for (size_t k = 0; k < mNode.size(); ++k)
			{
				if (fabs(mNode[k]->GetNodePoint().x - vec.x) < 0.49f && fabs(mNode[k]->GetNodePoint().z - vec.z) < 0.49f)
				{
					Node::ConnectNodePoint(mNode[k], mNode[i]);
				}
			}
		}
	}
}

vector<D3DXVECTOR3> Astar::FindPath(Node* startNode, Node* destNode)
{
	Setup();

	for (auto e : mNode)
	{
		e->Setup();
	}
	
	startNode->SetG(0.0);
	startNode->SetH(CalcHeuristic(startNode, destNode));
	startNode->SetF(startNode->GetG() + startNode->GetH());

	mSetOpenList.insert(startNode);
	
	while (true)
	{
		Node* minFNode = GetMinFNodeAtOpenList();

		// not found Path
		if (minFNode == NULL)
		{
			return mPath;
		}

		mSetOpenList.erase(minFNode);
		mSetCloseList.insert(minFNode);

		minFNode->SetIsClosed(true);

		Extend(minFNode, destNode);

		if (minFNode == destNode)
		{
			//  return Path
			Node* pTemp = destNode;

			while (pTemp)
			{
				mPath.push_back(pTemp->GetNodePoint());
				pTemp = pTemp->GetParent();
			}
			return mPath;
		}
		
	}
	
}

FLOAT Astar::CalcHeuristic(Node* pNode1, Node* pNode2)
{
	return D3DXVec3Length(&(pNode1->GetNodePoint() - pNode2->GetNodePoint()));
}

Node * Astar::GetMinFNodeAtOpenList()
{
	Node* minFNode = NULL;

	for each(auto p  in mSetOpenList)
	{
		if (minFNode == NULL)
		{
			minFNode = p;
			continue;
		}

		if (p->GetF() < minFNode->GetF())
			minFNode = p;
	}
	return minFNode;
}

void Astar::Extend(Node * currNode, Node * destNode)
{
	for(int i = 0; i < currNode->GetPath().size(); ++i)
	{
		if (currNode->GetPath()[i]->GetIsClosed())
			continue;

		if (IsInList(currNode->GetPath()[i], mSetOpenList))
		{
			if (currNode->GetPath()[i]->GetG() > currNode->GetG() + CalcHeuristic(currNode, currNode->GetPath()[i]))
			{
				currNode->GetPath()[i]->SetG(currNode->GetG() + CalcHeuristic(currNode, currNode->GetPath()[i]));
				currNode->GetPath()[i]->SetF(currNode->GetPath()[i]->GetG() + currNode->GetPath()[i]->GetH());
				currNode->GetPath()[i]->SetParent(currNode);
			}
		}
		else
		{
			currNode->GetPath()[i]->SetG(currNode->GetG() + CalcHeuristic(currNode, currNode->GetPath()[i]));
			currNode->GetPath()[i]->SetH(CalcHeuristic(currNode->GetPath()[i], destNode));
			currNode->GetPath()[i]->SetF(currNode->GetPath()[i]->GetG() + currNode->GetPath()[i]->GetH());
			currNode->GetPath()[i]->SetParent(currNode);
			mSetOpenList.insert(currNode->GetPath()[i]);
		}
	}
}

bool Astar::IsInList(Node * pNode, std::set<Node*>& setNodeList)
{
	return setNodeList.find(pNode) != setNodeList.end();
}