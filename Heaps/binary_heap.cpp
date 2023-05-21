#include <vector>
#include <cassert>
using namespace std;
class Heap {
	int* ara;
	int mxsize, lastpos;
public:
	Heap(int sz = 1) {
		mxsize = sz + 1;
		ara = new int[mxsize];
		lastpos = 0;	
	}
	Heap(vector <int> &numbers) {
        mxsize = numbers.size()+1;
        ara = new int[mxsize];
        lastpos = 0;
        for(int i = 0;i < numbers.size(); i++) {
            insert(numbers[i]);
        }
    }
	void insert (int x) {
		if (lastpos == mxsize) {
			return;
		}
		ara[++lastpos] = x;
		int cur = lastpos;
		while (cur > 1 && ara[cur] > ara[cur / 2]) {
			swap(ara[cur], ara[cur / 2]);
			cur /= 2;
		}
	}
	int getMax() {
		if (mxsize == 1) assert(false);
		return ara[1];
	}
	void deleteKey() {
		if (mxsize == 1) return;
		ara[1] = ara[lastpos];
		lastpos--;
		int cur = 1;
		while (cur < lastpos) {
			int lpos = cur * 2;
			int rpos = cur * 2 + 1;
			int pos = lpos;
			if (lpos > lastpos) break;
			if (rpos <= lastpos && ara[rpos] > ara[lpos]) {
				pos = rpos;
			}
			if (ara[cur] > ara[pos]) break;
			//swap(ara[cur], ara[pos]);
			swap(ara[cur], ara[pos]);
			cur = pos;
		}
	}
	int size() {
        return lastpos;
    }

};
void heapsort(vector<int>& v) {
    int ln = v.size();
    Heap h(ln);
    for (int i = 0; i < ln; i++) h.insert(v[i]);
    for (int i = 0; i < ln; i++) {
        v[i] = h.getMax();
        h.deleteKey();
    }
}
