#include <bits/stdc++.h>
using namespace std;
#define  info pair < long long, pair < long long, long long > >
const long long oo = 1e18;
vector < info > all;
long long n, m;
long long dis[1002];
vector < pair < long long, long long > > adj[1002];
long long pr[1002];
void init () {
	for (long long i = 0; i <= n; i++) dis[i] = oo;
	for (long long i = 0; i <= n; i++) pr[i] = -1;
}
void dijkstra (long long s) {
	init();
	dis[s] = 0;
	priority_queue < pair < long long, long long > > pq;
	pq.push(make_pair(0, s));
	while (pq.size()) {
		pair < long long, long long > pu = pq.top();
		long long u = pu.second, du = pu.first;
		pq.pop();
		if (du != dis[u]) continue;
		for (auto pv : adj[u]) {
			long long v = pv.second;
			long long dv = pv.first;
			if (du + dv < dis[v]) {
				dis[v] = du + dv;
				pq.push(make_pair(dis[v], v));
				pr[v] = u;
			}
		}
	}
}
bool belman (long long s) {
	init();
	dis[s] = 0;
	long long ok = 1;
	for (long long i = 0; i <= n - 1; i++) {
		for (auto e : all) {
			if (dis[e.second.first] == oo) continue;
			long long u = e.second.first, v = e.second.second, w = e.first;
			if (dis[v] > dis[u] + w) {
				pr[v] = u;
				dis[v] = dis[u] + w;
				if (i == n - 1) ok = 0;
			}
		}
	}
	return ok;
}
void printpath(int t) {
	stack < long long > stk;
	long long cur = t;
	while (cur != -1) {
		stk.push(cur);
		cur = pr[cur];
	}
	while (stk.size()) {
		cout << stk.top();
		stk.pop();
		if (stk.size()) cout << " -> ";
		else cout << endl;
	}
}
int main () {
	cin >> n >> m;
	for (long long i = 0, u, v, w; i < m; i++) {
		cin >> u >> v >> w;
		all.push_back(make_pair(w, make_pair(u, v)));
		adj[u].push_back(make_pair(w, v));
	}
	long long s, t;
	cin >> s >> t;
	/*
	dijkstra(s);
	if (dis[t] == oo) cout << "No path possible" << endl;
	else {
		cout << "Shortest path cost: " << dis[t] << endl;
		printpath(t);
	}
	*/
	if (!belman(s)) cout << "The graph contains a negative cycle" << endl;
	else if (dis[t] == oo) cout << "No path possible" << endl;
	else {
		cout << "The graph does not contain a negative cycle" << endl << "Shortest path cost: " << dis[t] << endl;
		printpath(t);
	}
}
