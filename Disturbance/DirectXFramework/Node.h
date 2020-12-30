#pragma once

class Node
{
public:
	Node();
	~Node();

	void Setup();
	
	static void ConnectNodePoint(Node* node1, Node* node2);

	D3DXVECTOR3 GetNodePoint() { return mPosition; }
	void SetNodePoint(D3DXVECTOR3 position) { mPosition = position; }
	
	float GetF() { return mF; }
	float GetG() { return mG; }
	float GetH() { return mH; }

	void SetF(float F) { mF = F; }
	void SetG(float G) { mG = G; }
	void SetH(float H) { mH = H; }

	bool GetIsClosed() { return mIsClosed; }
	void SetIsClosed(bool isClosed) { mIsClosed = isClosed; }

	vector<Node*> GetPath() { return mPath; }
	
	void SetParent(Node* parent) { mParent = parent; }
	Node* GetParent() { return mParent; }
	
private:
	vector<Node*> mPath;
	D3DXVECTOR3 mPosition;
	Node*		mParent;
	
	float		mF;
	float		mG;
	float		mH;
	
	bool		mIsClosed;
};

