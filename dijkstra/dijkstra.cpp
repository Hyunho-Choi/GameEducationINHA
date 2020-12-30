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

	printf("시작지를 입력하세요 ( 1 ~ %d 사이의 점 ) : ", NUM_NODE);
	cin >> start;
	printf("목적지를 입력하세요 ( 1 ~ %d 사이의 점 ) : ", NUM_NODE);
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
		// 현재 노드에서 연결된 노드 검색
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

		//next만큼 반복
		for (int i = 0; i < next.size(); i++)
		{
			if (cal[next[i]] == I) 
			{
				// 첫번째 접근일 경우 그냥 대입
				cal[next[i]] = cal[closeNode[closeNode.size() - 1]] + aEdgeCost[closeNode[closeNode.size() - 1]][next[i]];
			}
			else if (cal[next[i]] > (cal[closeNode[closeNode.size() - 1]] + aEdgeCost[closeNode[closeNode.size() - 1]][next[i]]))
			{
				// 이미 값이 있을 경우 비교후 낮은쪽 대입
				cal[next[i]] = cal[closeNode[closeNode.size() - 1]] + aEdgeCost[closeNode[closeNode.size() - 1]][next[i]];
				minRoad[next[i]] = closeNode[closeNode.size() - 1];
			}
			if (minRoad[next[i]] == I)
				//자신으로 오기위해 지나온 노드
				minRoad[next[i]] = closeNode[closeNode.size() - 1];
		}

		// 현재 노드중 가장 거리가 작은 노드로 이동
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

		// 모든 노드를 체크했으면 종료
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
	cout << "최단거리 : " << cal[end] << endl;

    return 0;
}





