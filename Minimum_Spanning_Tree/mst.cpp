#include <bits/stdc++.h>
using namespace std;
#define info pair < double, pair <int, int> >
vector < pair <int, int> > primedges, kruskaledges;
vector < pair < double, int> > adj[1002];
vector < info > edges;
int vis[1002];
double ans = 0;
int n, m;
void prim () {
	for (int i = 0; i < n; i++) vis[i] = 0;
	ans = 0;
	priority_queue < info, vector < info >, greater < info > > pq;
	pq.push(make_pair(0.0, make_pair(0, 0)));
	while (pq.size()) {
		info ui = pq.top();
		pq.pop();
		double uw = ui.first;
		int u = ui.second.second;
		if (!vis[u]) {
			vis[u] = 1;
			if (u != ui.second.first) primedges.push_back(ui.second);
			ans += uw;
			for (pair < double, int > vv : adj[u]) {
				int v = vv.second, vw = vv.first;
				info vi = make_pair(vw, make_pair(u, v));
				if (!vis[v]) pq.push(vi);
			}
		}
	}
}
int parent[1002], sz[1002];
int find (int u) {
	if (u == parent[u]) return u;
	return parent[u] = find(parent[u]);
}
int joint (pair < int, int > ed) {
	int pu = find(ed.first);
	int pv = find(ed.second);
	if (pu == pv) return 0;
	if (sz[pu] < sz[pv]) swap(pu, pv);
	sz[pu] += sz[pv];
	sz[pv] = 0;
	parent[pv] = pu;

	return 1;
}
void kruskal () {
	ans = 0;
	for (int i = 0; i < n; i++) parent[i] = i;
	sort(edges.begin(), edges.end());
	for (info cur : edges) {
		if(joint(cur.second)) {
			ans += cur.first;
			kruskaledges.push_back(cur.second);
		}
	}
}
int main () {
	cin >> n >> m;
	double w;
	for (int i = 0, u, v; i < m; i++) {
		cin >> u >> v >> w;
		adj[u].push_back(make_pair(w, v));
		adj[v].push_back(make_pair(w, u));
		edges.push_back(make_pair(w, make_pair(u, v)));
	}
	prim();
	kruskal();
	cout << fixed << setprecision(1) << "Cost of the minimum spanning tree : " << ans << endl;
	cout << "List of edges selected by Prim's: ";
	cout << "{";
	for (int i = 0; i < (int)primedges.size(); i++) {
		if (i > 0) cout << ",";
		cout << "(" << primedges[i].first << "," << primedges[i].second << ")";
	}
	cout << "}" << endl;
	cout << "List of edges selected by Kruskal's: ";
	cout << "{";
	for (int i = 0; i < (int)kruskaledges.size(); i++) {
		if (i > 0) cout << ",";
		cout << "(" << kruskaledges[i].first << "," << kruskaledges[i].second << ")";
	}
	cout << "}" << endl;
	return 0;
}
