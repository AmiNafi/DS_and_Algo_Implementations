#include <bits/stdc++.h>
using namespace std;
const long long oo = 1e18 + 10;
int n, m;
void printmat(vector < vector < long long > >& mat) {
	cout << "Shortest distance matrix" << endl;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (mat[i][j] == oo) cout << "INF ";
			else cout << mat[i][j] << " ";
		}
		cout << endl;
	}
}
void floyd(vector < vector < long long > >& dis) {
	for (int k = 1; k <= n; k++) {
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				if (dis[i][k] != oo && dis[k][j] != oo) {
					dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
				}
			}
		}
	}
}
vector < vector < long long > > multiply(const vector < vector < long long > >& mat1, const vector < vector < long long > >& mat2 ) {
	vector < vector < long long > > nmat(n + 2, vector < long long > (n + 2, oo)); 
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			for (int k = 1; k <= n; k++) {
				if (mat1[i][k] != oo && mat2[k][j] != oo) {
					nmat[i][j] = min(mat1[i][k] + mat2[k][j], nmat[i][j]);
				}
			}
		}
	}
	return nmat;
}
vector < vector < long long > > power(vector < vector < long long > > mat, int p) {
	vector < vector < long long > > ret(n + 2, vector < long long > (n + 2, oo));
	if (p == 0) {	
		for (int i = 1; i <= n; i++) ret[i][i] = 0;
		return ret;
	}
	ret = power(mat, p / 2);
	ret = multiply(ret, ret);
	if (p % 2 == 1) ret = multiply(ret, mat);
	return ret;
}

int main () {
	cin >> n >> m;
	vector < vector < long long > > dis(n + 2, vector < long long > (n + 2, oo));
	for (int i = 1; i <= n; i++) dis[i][i] = 0;
	long long w;
	for (int i = 1, u, v; i <= m; i++) {
		cin >> u >> v >> w;
		dis[u][v] = min(dis[u][v], w);
	}
	// dis = power(dis, n - 1);
	floyd(dis);
	printmat(dis);
	return 0;
}