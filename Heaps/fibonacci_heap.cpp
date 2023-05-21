
 
#include <bits/stdc++.h>
using namespace std;
#define int long long
typedef long long ll;

#define pb push_back
#define pii pair <int, int>
#define pll pair <long long, long long >
#define PI 2 * acos(0.0)
#define  info pair < int, node* >
const long long M = 1e5 + 10, oo = 1e18 + 7;
const double gr = (1.0 + sqrt(5.0)) / 2.0;
struct node {
  pii key;
  int deg;
  bool mark;
  node *parent;
  node *child;
  node *left;
  node *right;
};
node *createNode(pii k) {
  node *ret = new node;
  ret->key = k;
  return ret;
}
class FibonacciHeap {
public:
  int n;
  node *H;
  node *makeFibHeap();
  void addtorootlist(node* &);
  void fibLink (node* &, node* &);
  void insert(node* &);
  void consolidate();
  node *extractMin();
  void cut(node* &, node* &);
  void cascadingCut(node* &);
  void decreaseKey(node* &, pii);
  FibonacciHeap() {
    H = nullptr;
  }
};
void FibonacciHeap::addtorootlist(node* &x) {
 node* it = H;
  while (1) {
    it = it->right;
    if (it == H) break;
  }
  (H->left)->right = x;
  x->right = H;
  x->left = H->left;
  H->left = x;
  if (H->key > x->key) {
    H = x;
  }
}
void FibonacciHeap::insert(node* &x) {
  x->deg = 0;
  x->parent = nullptr;
  x->child = nullptr;
  x->mark = 0;
  if (H == nullptr) {
    n = 0;
    //create a root list just containing just x
    x->left = x;
    x->right = x;
    H = x;
  }
  else {
    addtorootlist(x);
  }
  n++;
  node* it = H;
}
void FibonacciHeap::fibLink (node* &y, node* &x) {
  // only one node in list
  if (x->right == x) H = x;
  // remove y from the root list
  (y->left)->right = y->right;
  (y->right)->left = y->left;
  y->left = y;
  y->right = y;
  // make y a child of x
  y->parent = x;
  if (x->child == nullptr) x->child = y;
  else {
    ((x->child)->left)->right = y;
    y->right = (x->child);
    y->left = (x->child)->left;
    ((x->child)->left) = y;
    if (y->key < (x->child)->key) {
      x->child = y;
    }
  }
  // increment degree of x
  x->deg++;
  y->mark = false;
  // cout << " end of link " << endl;
}
void FibonacciHeap::consolidate() {
  int ln = (log2(n) / log2(gr)) + 1;
  node *ara[ln];
  for (int i = 0; i < ln; i++) {
    ara[i] = nullptr;
  }
  node *x = H;
  node *y;
  node *tmp;
  int rootcount = 0;
  node *it = H;
  while (1) {
    rootcount++;
    it = it->right;
    if (it == H) break;
  }
  while(rootcount--) {
    x = it;
    it = it->right;
    int d = x->deg;
    while (ara[d] != nullptr) {
      y = ara[d];
      if (x->key > y->key) {
        tmp = x;
        x = y;
        y = tmp;
      }
      fibLink(y, x);
      ara[d] = nullptr;
      d++;
    }
    ara[d] = x;
  }
  H = nullptr;
  for (int i = 0; i < ln; i++) {
    if (ara[i] != nullptr) {
      if (H == nullptr) {
        ara[i]->left = ara[i];
        ara[i]->right = ara[i];
        H = ara[i];
      }
      else {
        addtorootlist(ara[i]);
      }
    }
  }
}
node *FibonacciHeap::extractMin() {

  node *z = H;
  node *x;
  node *tmp;
  if (z != nullptr) {
    if (z->child != nullptr) {
      x = z->child;
      while (1) {
        tmp = x->right;
        addtorootlist(x);
        x->parent = nullptr;
        x = tmp;
        if (tmp == z->child) break;
      }
    }
    (z->left)->right = z->right;
    (z->right)->left = z->left;
    if (z == z->right && z->child == nullptr) {
      H = nullptr;
    }
    else {
      H = z->right;
      if (z == z->right) {
        H = z->child;
      }
      consolidate();
    }
    z->right = z->left = z;
    n--;
  }
  return z;
}
void FibonacciHeap::cut(node* &x, node* &y) {
  if (x == x->right) {
    y->child = nullptr;
  }
  (x->left)->right = x->right;
  (x->right)->left = x->left;
  if (x == y->child) {
    y->child = x->right;
  }
  y->deg = y->deg - 1;
  x->right = x;
  x->left = x;
  addtorootlist(x);
  x->parent = nullptr;
  x->mark = false;
}
void FibonacciHeap::cascadingCut(node* &y) {
  if (y->parent != nullptr) {
    if (y->mark == false) {
      y->mark = true;
    }
    else {
      node* z = y->parent;
      cut(y, z);
      cascadingCut(z);
    }
  }
}
void FibonacciHeap::decreaseKey(node* &x, pii k) {
  if (k > x->key) {
    return;
  }
  if (H == nullptr) {
    return;
  }
  x->key = k;
  node *y = x->parent;
  if (y != nullptr && x->key < y->key) {
    cut(x, y);
    cascadingCut(y);
  }
  if (x->key < H->key) {
    H = x;
  }
}