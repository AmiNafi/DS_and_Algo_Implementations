#include <bits/stdc++.h>
using namespace std;
const long long oo = 1e9 + 7;
const int M = 101, M2 = 10200;
string names[M];
int win[M], loss[M], lft[M], against[M][M], n, cap[5001][5001], vis[M2], parent[M2];
int bestflow;
vector < int > adj[M2], badset;
int src = 0, des;
int getid (int a, int b) {
	if (b == 0) return a;
	return a * (n + 1) + b;
}
void init () {
	bestflow = 0;
	int src = 0;
	for (int i = 0; i <= des; i++) {
		adj[i].clear();
	}
}
int bfs (int s) {
	int curflow = 0;
	for (int i = 0; i <= des; i++) vis[i] = 0;
	queue < pair < int, int > > q;
	q.push(make_pair(0, oo));
	vis[s] = 0;
	// int mnflow = oo;
	while (q.size()) {
		pair < int, int >  pu = q.front();
		int u = pu.first;
		if (u == des) return pu.second;
		q.pop();
		for (int v : adj[u]) {
			if (!vis[v] && cap[u][v] != 0) {
				parent[v] = u;
				vis[v] = 1;
				int minflow = min(pu.second, cap[u][v]);
				q.push(make_pair(v, minflow));
			}
		}
	}
	return oo;
}
long long edmonds(int s) {
	long long nflow = 0, tflow = 0;
	while (1) {
		int nflow = bfs(src);
		if (!vis[des]) break;
		tflow += nflow;
		int v = des;
		while (v != src) {
			int u = parent[v];
			cap[u][v] -= nflow;
			cap[v][u] += nflow;
			v = u;
		}
	}
	return tflow;
}
void makeadj(int node) {
	init();
	for (int i = 1; i <= n; i++) {
		for (int j = i + 1; j <= n; j++) {
			if (i == node || j == node) continue;
			int id = getid(i, j);
			adj[src].push_back(id);
			cap[src][id] = against[i][j];
			bestflow += against[i][j];
			adj[id].push_back(src);
			adj[id].push_back(i);
			cap[id][i] = oo;
			adj[i].push_back(id);
			adj[id].push_back(j);
			cap[id][j] = oo;
			adj[j].push_back(id);
		}
		if (i == node) continue;
		adj[i].push_back(des);
		cap[i][des] = win[node] + lft[node] - win[i];
		adj[des].push_back(i);
		if (cap[i][des] < 0) {
			badset.push_back(i);
		}
	}
}
void printans(int node) {
	int sz = (int)(badset.size());
	double totwin = 0, extra = 0;
	for (int i = 0; i < sz; i++) {
		int x = badset[i];
		totwin += win[x];
		for (int j = i + 1; j < sz; j++) {
			int y = badset[j];
			extra += against[x][y];
		}
	}
	double avg = (totwin + extra) / sz;
	cout << names[node] << " is eliminated." << endl;
	cout << "They can win at most " << win[node] << " + " << lft[node] << " = " << win[node] + lft[node] << " games." << endl; 
	for (int i = 0; i < sz; i++) {
		cout << names[badset[i]];
		if (i != sz - 1) cout << ", ";
	}
	cout << " have won a total of " << totwin << " games."<<endl;
    cout << "They play each other " << extra << " times."<<endl;
    cout<<"So on average, each of the teams in this group wins "<< totwin + extra << " / " << sz << " = " << avg << " games." <<endl;
    cout<<endl;
    badset.clear();
}
int main () {
	cin >> n;
	des = (n + 1) * n + n + 1;
	for (int i = 1; i <= n; i++) {
		cin >> names[i] >> win[i] >> loss[i] >> lft[i];
		for (int j = 1; j <= n; j++) {
			cin >> against[i][j];
		}
	}
	for (int node = 1; node <= n; node++) {
		makeadj(node);
		if ((int)(badset.size())) {
			printans(node);
			continue;
		}
		int maxflow = edmonds(node);
		// cout << node << " Max Flow Value " << maxflow << endl;
		if (maxflow != bestflow) {
			bfs(src);
			for (int i = 1; i <= n; i++) {
				if (i != node && vis[i]) {
					badset.push_back(i);
				}
			}
			printans(node);
		}
		// break;
	}
	return 0;
}
