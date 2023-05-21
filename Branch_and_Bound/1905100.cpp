#include <bits/stdc++.h>
using namespace std;
#define pii pair < int, int >
char mat[101][101], nmat[101][101][101], frow, fcol;
int n, ans = 0;
int swapcol(int id, int c1, int c2) {
    for (int i = 0; i < n; i++) {
        swap(nmat[id][i][c1], nmat[id][i][c2]);
    }
}
int swaprow(int id, int r1, int r2) {
    for (int i = 0; i < n; i++) {
        swap(nmat[id][r1][i], nmat[id][r2][i]);
    }
}
struct mymat
{
    int fr = 0, fc = 0;
    vector < vector < int > > tmat;
    mymat(int n) {
        tmat = vector < vector < int > > (n, vector < int > (n));
    }
    int b, l, o;
    bool operator< (const mymat m) const {
        return make_pair(-1 * b, make_pair(l, o)) < make_pair( -1 * m.b, make_pair(m.l, m.o));
    }
};
priority_queue < mymat > pq;
int calcbound(int id, int fr, int fc) {
    int fbound = 0, ubound = 0;
    for (int r = 0; r < n; r++) {
        if (fr > r) {
            // fixed bound row
            int pre = 1, cnt = 0;
            for (int c = 0; c <= r; c++) {
                if (nmat[id][r][c] == 'X') pre = max(pre, 1 + r - c);
            }
            int pst = 1;
            for (int c = r; c < n; c++) {
                if (nmat[id][r][c] == 'X') {
                    if (c >= fc) {
                        cnt++;
                        pst = fc - r + cnt;
                    }
                    else pst = 1 + c - r;
                }
            }
            fbound = max(fbound, max(pre, pst));
        }
        else {
            int cnt = 0;
            for (int c = fc; c < n; c++) {
                if (nmat[id][r][c] == 'X') cnt++;
            }
            ubound = max(ubound, (cnt + 1) / 2);
        }
    }
    for (int c = 0; c < n; c++) {
        if (fc > c) {
            int pre = 1, cnt = 0;
            for (int r = 0; r <= c; r++) {
                if (nmat[id][r][c] == 'X') pre = max(pre, 1 + c - r);
            }
            int pst = 1;
            for (int r = c; r < n; r++) {
                if (nmat[id][r][c] == 'X') {
                    if (r >= fr) {
                        cnt++;
                        pst = fr - c + cnt;
                    }
                    else pst = 1 + r - c;
                }
            }
            fbound = max(fbound, max(pre, pst));
        }
        else {
            int cnt = 0;
            for (int r = fr; r < n; r++) if (nmat[id][r][c] == 'X') cnt++;
            ubound = max(ubound, (cnt + 1) / 2);
        }
    }
    return max(fbound, ubound);
}
void columncalc (int lv) {
    for (int id = 0; id < n - fcol; id++) 
        for (int i = 0; i < n; i++) 
            for (int j = 0; j < n; j++) nmat[id][i][j] = mat[i][j];
    for (int c = fcol; c < n; c++) {
        for (int i = c; i > fcol; i--) swapcol(c - fcol, i, i - 1);
        int b = calcbound(c - fcol, frow, fcol + 1);
        mymat mm(n);
        mm.b = b;
        mm.o = c - fcol;
        mm.l = lv;
        mm.fr = frow;
        mm.fc = fcol + 1;
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) mm.tmat[i][j] = nmat[c - fcol][i][j];
        pq.push(mm);
    }
}
void rowcalc (int lv) {
    for (int id = 0; id < n - frow; id++) 
        for (int i = 0; i < n; i++) 
            for (int j = 0; j < n; j++) nmat[id][i][j] = mat[i][j];
    for (int r = frow; r < n; r++) {
        for (int i = r; i > frow; i--) swaprow(r - frow, i, i - 1);
        int b = calcbound(r - frow, frow + 1, fcol);
        mymat mm(n);
        mm.b = b;
        mm.o = r - frow;
        mm.l = lv;
        mm.fr = frow + 1;
        mm.fc = fcol;
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) mm.tmat[i][j] = nmat[r - frow][i][j];
        pq.push(mm);
    }
}
int main () {
    cin >> n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) cin >> mat[i][j];
    }
    ans = 1;
    int lv = 1;
    while (1) {
        if (frow == n - 1 && fcol == n - 1) break;
        if (frow == fcol) {
            columncalc(lv);
        }
        else {
            rowcalc(lv);
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                mat[i][j] = (pq.top()).tmat[i][j];
            }
        }
        lv = (pq.top().l + 1);
        ans = (pq.top()).b;
        frow = (pq.top()).fr;
        fcol = (pq.top()).fc;
        pq.pop();
    }
    cout << ans << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) cout << mat[i][j] << " ";
        cout << endl;
    }
}