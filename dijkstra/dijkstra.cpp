// dijkstra.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define NUM_NODE 8
#define I 10000

using namespace std;

int main()
{
	int aEdgeCost[NUM_NODE][NUM_NODE] =
	{
		{ 0,2,I,I,I,3,I,I },
		{ 2,0,4,1,I,I,I,I },
		{ I,4,0,I,3,I,I,I },
		{ I,1,I,0,3,I,2,I },
		{ I,I,3,3,0,I,I,4 },
		{ 3,I,I,I,I,0,6,I },
		{ I,I,I,2,I,6,0,4 },
		{ I,I,I,I,4,I,4,0 },
	};

	int start, end;

	printf("�������� �Է��ϼ��� ( 1 ~ %d ������ �� ) : ", NUM_NODE);
	cin >> start;
	printf("�������� �Է��ϼ��� ( 1 ~ %d ������ �� ) : ", NUM_NODE);
	cin >> end;
	start--;
	end--;

	int cal[NUM_NODE] = { I,I,I,I,I,I,I,I };
	int minRoad[NUM_NODE] = { I,I,I,I,I,I,I,I };
	vector<int> closeNode;
	closeNode.push_back(start);
	cal[start] = 0;

	while (1)
	{
		// ���� ��忡�� ����� ��� �˻�
		vector<int> next;
		for (int j = 0; j < NUM_NODE; j++)
		{
			if (aEdgeCost[closeNode[closeNode.size() - 1]][j] != I)
			{
				vector<int>::iterator it;
				it = find(closeNode.begin(), closeNode.end(), j);
				if (it == closeNode.end())
					next.push_back(j);
			}
		}

		//next��ŭ �ݺ�
		for (int i = 0; i < next.size(); i++)
		{
			if (cal[next[i]] == I) 
			{
				// ù��° ������ ��� �׳� ����
				cal[next[i]] = cal[closeNode[closeNode.size() - 1]] + aEdgeCost[closeNode[closeNode.size() - 1]][next[i]];
			}
			else if (cal[next[i]] > (cal[closeNode[closeNode.size() - 1]] + aEdgeCost[closeNode[closeNode.size() - 1]][next[i]]))
			{
				// �̹� ���� ���� ��� ���� ������ ����
				cal[next[i]] = cal[closeNode[closeNode.size() - 1]] + aEdgeCost[closeNode[closeNode.size() - 1]][next[i]];
				minRoad[next[i]] = closeNode[closeNode.size() - 1];
			}
			if (minRoad[next[i]] == I)
				//�ڽ����� �������� ������ ���
				minRoad[next[i]] = closeNode[closeNode.size() - 1];
		}

		// ���� ����� ���� �Ÿ��� ���� ���� �̵�
		int tempJ;
		int tempMin = I;
		for (int j = 0; j < NUM_NODE; j++)
		{
			vector<int>::iterator it;
			it = find(closeNode.begin(), closeNode.end(), j);
			if (it == closeNode.end())
			{
				if (tempMin > cal[j])
				{
					tempMin = cal[j];
					tempJ = j;
				}
			}
		}
		closeNode.push_back(tempJ);

		// ��� ��带 üũ������ ����
		if (closeNode.size() == NUM_NODE)
			break;
	}

	closeNode.clear();
	
	int g = end;

	while (g != start)
	{
		closeNode.push_back(g);
		g = minRoad[g];
	}
	closeNode.push_back(start);
	int i;
	for (i = closeNode.size()-1; i > 0 ; i--)
		cout << closeNode[i]+1 << " -> ";
	cout << closeNode[i]+1 << endl;
	cout << "�ִܰŸ� : " << cal[end] << endl;

    return 0;
}





