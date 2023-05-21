#include <bits/stdc++.h>
// #include <chrono>
using namespace std;
// using namespace std::chrono;

#define pii pair < int, int >
const long long oo = 1e9 + 7;
const int M = 101, M2 = 10200;
const double gr = (1.0 + sqrt(5.0)) / 2.0;

struct node {
	int key, clr, sz;
	node *left, *right, *parent;
	node () {
		key = 0;
		left = NULL;
		right = NULL;
		parent = NULL;
		clr = 0;
		sz = 1;
	}
	node *getUncle() {
		if (parent == NULL || parent->parent == NULL) return NULL;
		else if (this == parent->left) return parent->parent->right;
		return parent->parent->left;
	}
	node *getSibling () {
		if (parent == NULL) return NULL;
		if (this == parent->left) return parent->right;
		return parent->left;
	}
};
struct rbTree {
	node *rt = NULL;
	void leftRotate (node *x) {
		// right child will be parent
		node *y = x->right;
		node *pr = x->parent;
		int xs = x->sz, ys = y->sz;
		int lys = 0;
		if (y->left != NULL) lys = y->left->sz;
		x->sz = xs - ys + lys;
		y->sz = ys - lys + x->sz;
		if (x == rt) rt = y;
		x->right = y->left;
		if (y->left != NULL) y->left->parent = x;
		y->left = x;
		x->parent = y;
		y->parent = pr;
		if (pr != NULL) {
			if (pr->left == x) pr->left = y;
			else pr->right = y;
		}

	}
	void rightRotate (node *x) {
		node *y = x->left;
		node *pr = x->parent;
		if (x == rt) rt = y;
		int xs = x->sz, ys = y->sz;
		int rys = 0;
		if (y->right != NULL) rys = y->right->sz;
		x->sz = xs - ys + rys;
		y->sz = ys - rys + x->sz;
		x->left = y->right;
		if (y->right != NULL) y->right->parent = x;
		y->right = x;
		x->parent = y;
		y->parent = pr;
		if (pr != NULL) {
			if (pr->left == x) pr->left = y;
			else pr->right = y;
		}
	}
	void redFix (node *x) {
		if (x == rt) {
			x->clr = 1;
			return;
		}
		node *pr = x->parent, *gp = pr->parent, *uc = x->getUncle();
		if (pr->clr == 0) {
			if (uc != NULL && uc->clr == 0) {
				pr->clr = 1;
				uc->clr = 1;
				gp->clr = 0;
				redFix(gp);
			}
			else {
				if (pr == pr->parent->left) {
					if (x == x->parent->left) {
						swap(pr->clr, gp->clr);
					}
					else {
						leftRotate(pr);
						swap(x->clr, gp->clr);
					}
					rightRotate(gp);
				}
				else {
					if (x == x->parent->left) {
						rightRotate(pr);
						swap(x->clr, gp->clr);
					}
					else {
						swap(pr->clr, gp->clr);
					}
					leftRotate(gp);
				}
			}
		}
	}
	node *find (int kval) {
		node *tmp = rt;
		while (tmp != NULL) {
			if (kval < tmp->key) {
				if (tmp->left == NULL) break;
				else tmp = tmp->left;
			}
			else if (kval == tmp->key) {
				break;
			}
			else {
				if (tmp->right == NULL) break;
				tmp = tmp->right;
			}
		}
		return tmp;
	}
	node *noLeftChild(node *x) { // ckd
		node *tmp = x;
		while (tmp->left != NULL) {
			tmp = tmp->left;
		}
		return tmp;
	}
	node *delReplace(node *x) { // ckd
		if (x->left != NULL && x->right != NULL) {
			return noLeftChild(x->right);
		}
		else if (x->left == NULL && x->right == NULL) {
			return NULL;
		}
		else if (x->left != NULL) {
			return x->left;
		}
		return x->right;
	}
	void blackFix(node *x) { // ckd
		if (x == rt) {
			return;
		}
		node *sb = x->getSibling(), *pr = x->parent;
		if (sb == NULL) blackFix(pr);
		else {
			if (sb->clr == 0) {
				pr->clr = 0;
				sb->clr = 1;
				if (sb == sb->parent->left) rightRotate(pr);
				else leftRotate(pr);
				blackFix(x);
			}
			else {
				if (sb->left != NULL && sb->left->clr == 0) {
					if (sb == sb->parent->left) {
						sb->left->clr = sb->clr;
						sb->clr = pr->clr;
						rightRotate(pr);
					}
					else {
						sb->left->clr = pr->clr;
						rightRotate(sb);
						leftRotate(pr);
					}
					pr->clr = 1;
				}
				else if (sb->right != NULL && sb->right->clr == 0) {
					if (sb == sb->parent->left) {
						sb->right->clr = pr->clr;
						leftRotate(sb);
						rightRotate(pr);
					}
					else {
						sb->right->clr = sb->clr;
						sb->clr = pr->clr;
						leftRotate(pr);
					}
					pr->clr = 1;
				}
				else {
					sb->clr = 0;
					if (pr->clr) {
						blackFix(pr);
					}
					else pr->clr = 1;
				}
			}		
		}
	}

	bool insert (int kval) {
		node *nd = new node();
		nd->key = kval;
		if (rt == NULL) {
			nd->clr = 1;
			rt = nd;
		}
		else {
			node *tmp = find(kval);
			if (tmp != NULL && tmp->key == kval) {
				return 0;
			}
			nd->parent = tmp;
			if (kval < tmp->key) {
				tmp->left = nd;
			}
			else tmp->right = nd;
			while (tmp != rt) {
				tmp->sz++;
				tmp = tmp->parent;
			}
			rt->sz++;
			redFix(nd);
		}
		return 1;
	}
	void deleteNode (node *x) {
		node *y = delReplace(x);
		// if (x->key == 1) if (y == NULL) cout << " del replace value " << endl;
		node *pr = x->parent;
		int bothblack = 0;
		if ((x->clr) && (y == NULL || y->clr)) {
			bothblack = 1;
		}
		if (y == NULL) {
			if (x == rt) {
				rt = NULL;
			}
			else {
				if (bothblack) {
					blackFix(x);
				}
				else {
					if (x->getSibling() != NULL) {
						x->getSibling()->clr = 0;
					}
				}
				if (x == x->parent->left) {
					pr->left = NULL;
				}
				else pr->right = NULL;
				node *tmp = x;
				while (tmp != rt) {
					tmp->sz--;
					tmp = tmp->parent;
				}
				rt->sz--;
			}
			delete x;
			return;
		}
		if (x->left == NULL || x->right == NULL) {
			if (x == rt) {
				x->key = y->key;
				x->left = x->right = NULL;
				x->sz--;
				delete y;
			}
			else {
				if (x == x->parent->left) {
					pr->left = y;
				}
				else pr->right = y;
				node *tmp = x;
				while (tmp != rt) {
					tmp->sz--;
					tmp = tmp->parent;
				}
				rt->sz--;
				delete x;
				y->parent = pr;
				if (bothblack) {
					blackFix(y);
				}
				else {
					y->clr = 1;
				}
			}
			return;
		} 
		swap(y->key, x->key);
		deleteNode(y);
	}

	bool erase (int kval) {
		if (rt == NULL) {
			return 0;
		}
		node *x = find(kval);
		if (x->key != kval) {
			return 0;
		}
		deleteNode(x);
		return 1;
	}
	int getcnt (int kval) {
		node *tmp = rt;
		int ret = 0;
		while (tmp != NULL) {
			if (kval < tmp->key) tmp = tmp->left;
			else {
				if (tmp->left != NULL) ret += tmp->left->sz;
				if (kval > tmp->key) ret++;
				tmp = tmp->right;
			}
		}
		return ret;
	}
};
int main () {
	// freopen("in.txt", "r", stdin);
 	//freopen("out.txt", "w", stdout);
	int q;
	cin >> q;
	cout << q << endl;
	rbTree rb;
	// auto start = high_resolution_clock::now();
	while (q--) {
		int a, b;
		cin >> a >> b;
		int res;
		if (a == 0) {
			res = rb.erase(b);
		}
		else if (a == 1) {
			res = rb.insert(b);
		}
		else if (a == 2) {
			node *nd = rb.find(b);
			if (nd == NULL || nd->key != b) res = 0;
			else res = 1;
		}
		else res = rb.getcnt(b);
		cout << a << " " << b << " " << res << endl;
	}
	// auto stop = high_resolution_clock::now();
	// auto duration = duration_cast<microseconds>(stop - start);
	// cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
}