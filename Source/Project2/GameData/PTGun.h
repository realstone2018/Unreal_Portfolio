#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#define INF 100000000

using namespace std;

const int N = 6;
vector<pair<int, int>> adj[N+1];	// 간선
int d[N+1];	// 최소비용

void dijkstra(int start) {
	// 자동으로 힙구조로 관리 (가장 큰 값이 root)
	priority_queue<pair<int, int>> pq;
	pq.push({start, 0});
	d[start] = 0;

	// 가까운 순서대로 처리하므로 큐를 사용
	while (!pq.empty()) {
		int current = pq.top().first;
		int distance = -pq.top().second; // 짧은 거리가 루트가 되도록 음수화되어 있음
		pq.pop();

		if (d[current] < distance) continue; // 최단거리가 아니면 스킵 

		// 방문한 최단거리의 노드를 거쳐 인접노드로 가는 비용 비교
		for (auto edge : adj[current]) {
			int next = edge.first;

			// 방문한 노드로 가는 비용 + 방문한 노드에서 인접노드로 가는 비용 
			int nextDistance = distance + edge.second; 

			if (nextDistance < d[next]) {
				pq.push({next, -nextDistance}); // 작은 비용 roo로 오도록 음수화
				d[next] = nextDistance;
			}
		} 
	}
}

int main(void) {
	// 노드 간에 연결되지 않은 경우 비용은 무한
	fill(d, d + N + 1, INF);

    adj[1].emplace_back(2, 2);
    adj[1].emplace_back(3, 5);
    adj[1].emplace_back(4, 1);

    adj[2].emplace_back(1, 2);
    adj[2].emplace_back(3, 3);
    adj[2].emplace_back(4, 2);

    adj[3].emplace_back(1, 5);
    adj[3].emplace_back(2, 3);
    adj[3].emplace_back(4, 3);
    adj[3].emplace_back(5, 1);
    adj[3].emplace_back(6, 5);

    adj[4].emplace_back(1, 1);
    adj[4].emplace_back(2, 2);
    adj[4].emplace_back(3, 3);
    adj[4].emplace_back(5, 1);

    adj[5].emplace_back(3, 1);
    adj[5].emplace_back(4, 1);
    adj[5].emplace_back(6, 2);

    adj[6].emplace_back(3, 5);
    adj[6].emplace_back(5, 2);

	dijstra(1);

	for (int i = 01; i <= N; i++) {
		cout << d[i] << " ";
	}
}