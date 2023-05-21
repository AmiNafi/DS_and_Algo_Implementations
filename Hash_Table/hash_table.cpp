#include <bits/stdc++.h>
using namespace std;
#define pii pair < int, int >
#define int long long
const int mod = 1e9 + 7;
int pw[11];
//Polynomial Rolling Hash Function
int h1(int n, string s){
    int ret = 0;
    for (int i = 0; i < (int)(s.length()); i++) {
        ret += s[i] * pw[i];
        // cout << i << " now " << pw[i] << endl;
    }
    return ret % n;
}

//djb2 Hash Function  
int h2(int n, string s){
	int ret = 5381;
    for (int i = 0; i < (int)(s.length()); i++) {
    	ret = ((ret << 5) + ret) + s[i];
    }
    ret = (ret % (n - 1)) + 1;
    return ret;
} 

struct node{
	string key = "";
	int val = 0;
	node *next = NULL;
};

//For Open Addressing
struct openNode {
    string key="";
    bool empty = true, deleted = false;
    int val = 0;
};
//Seperate Chaining
struct seperate_chaining_hash {
    node* hTable;
    int tableSize = 0;
    public:

    seperate_chaining_hash(){ 
    }

    void makeTable (int n){
    	tableSize = n;
        hTable = new node[tableSize];  
    }

    int findString(string s) {
        int cur = h1(tableSize, s);
        for(node* tmp = (hTable + cur)->next; tmp != NULL; tmp = tmp->next) {
            if(tmp->key == s) {
                return tmp->val;
            }
        }
        return 0;
    }    

    void insert(string s, int val) {
    	int cur = h1(tableSize, s) % tableSize;
        node* nd = new node;
        nd->val = val;
        nd->key = s;
        nd->next=NULL;
        node *tmp = (hTable + cur);
        while (tmp->next != NULL) {
        	if (val >= tmp->val && val <= tmp->next->val) {
        		nd->next = tmp->next->next;
        		tmp->next = nd;
        		break;
        	}
        	else tmp = tmp->next;
        }
        if (tmp->next == NULL) tmp->next = nd;
    }

    

    int erase (string s){
        int cur = h1(tableSize, s);
        node* tmp = (hTable + cur)->next;
        node *pre = (hTable + cur);
        while(tmp != NULL){
            if(tmp->key==s){
                pre->next = tmp->next;
                int curval = tmp->val;
                delete tmp;
              	return curval;
            }
            pre = tmp;
            tmp = tmp->next;
        }
        return 0;
    }

    void clear () {
        node *curpos,*tmp;
        for (int i = 0; i < tableSize; i++) {
        	curpos = (hTable + i)->next;
        	while (curpos != NULL) {
        		tmp = curpos;
        		curpos = curpos->next;
        		delete tmp;
        	}
        }
        delete[] hTable;
        tableSize = 0;
    }

    void print() {
        node* tmp;
        for(int i = 0;i<tableSize;i++){
            tmp=(hTable+i);
            tmp=tmp->next;

            cout<<i<<"---->";
            while(tmp!=NULL){
                cout<<"("<<tmp->key<<" "<<tmp->val<<") ";
                tmp=tmp->next;
            }
            cout<<endl;
        }
    }
};



struct linear_probing {
    openNode* hTable;
    int tableSize = 0;
    linear_probing () { 
    }
    void makeTable (int n) {
        tableSize = n;
        hTable = new openNode[tableSize];  
    }
    pii findString (string s) {
    	//cout << tableSize << " Here tableSize " << endl;
    	//cout << s << " string here " << endl;
    	int cur = h1(tableSize, s);
    	int prob = 0;
		for (int i = cur, id; i < tableSize + cur; i++) {
			prob++; 
			id = i % tableSize;
			//cout << i << " " << cur << " " << prob << " " << id << " kl " << (hTable + id)->key << " " << (hTable + id)->deleted << endl;
			
			if ((hTable + id)->empty) {
                if ((hTable + id)->deleted == 0) return pii(0, prob);
            }
           	else if ((hTable + id)->key == s) {
           		//cout << " got here " << endl;
           		return pii((hTable + id)->val, prob);
            }
        }
        return pii(0, prob); 
    }
    pii insert(string s, int val){
        int cur = h1(tableSize, s);
        int prob = 0;
        for(int i = cur, id; i < tableSize + cur; i++){
        	prob++;
        	id = i % tableSize;
            if((hTable + id)->empty) {
            	(hTable + id)->val = val;
                (hTable + id)->key = s;
                (hTable + id)->empty = 0;
                (hTable + id)->deleted = 0;
                return pii(val, prob);
            }
        }   
        return pii(0, prob);
    }
    //p.first --> value ,p.second --> number of probs
   pii erase (string s) {
   		int cur = h1(tableSize, s);
   		int prob = 0;
   		for(int i = cur, id; i < tableSize + cur; i++) {
   			prob++;
        	id = i % tableSize;
            if((hTable + id)->empty == 0 && (hTable + id)->key == s) {
            	(hTable + id)->deleted = 1;
            	(hTable + id)->empty = 1;
            	return pii((hTable + id)->val, prob);
            }
        }
        return pii(0, prob);
    }

    void clear(){
    	tableSize = 0;
        delete[] hTable;
    }

    void print(){
        for(int i=0;i<tableSize;i++){
            cout<<i<<"--> ";

            if((hTable+i)->empty==false){
                cout<<"("<<(hTable+i)->key<<" "<<(hTable+i)->val<<")";
            }
            cout<<endl;
        }
    }
};
struct quadratic_probing {
    openNode* hTable;
    int tableSize = 0, c1 = 29, c2 = 37;
    quadratic_probing () { 
    }
    void makeTable (int n) {
        tableSize = n;
        hTable = new openNode[tableSize];  
    }
    pii findString(string s){
    	int cur = h1(tableSize, s);
    	int prob = 0;
		for (int i = 0, id; i < tableSize; i++) {
			prob++;
			id = (cur + c1 * i + c2 * i * i) % tableSize;
			if ((hTable + id)->empty) {
                if ((hTable + id)->deleted == 0) return pii(0, prob);
            }
           	else if ((hTable + id)->key == s) {
           		return pii((hTable + id)->val, prob);
            }
        }
        return pii(0, prob); 
    }
    pii insert(string s, int val){
        int cur = h1(tableSize, s);
        int prob = 0;
        for(int i = 0,  id; i < tableSize; i++){
        	prob++;
        	id = (cur + c1 * i + c2 * i * i) % tableSize;
            if((hTable + id)->empty) {
            	(hTable + id)->val = val;
                (hTable + id)->key = s;
                (hTable + id)->empty = 0;
                (hTable + id)->deleted = 0;
                return pii(val, prob);
            }
        }   
        return pii(0, prob);
    }
    //p.first --> value ,p.second --> number of probs
   pii erase (string s) {
   		int cur = h1(tableSize, s);
   		int prob = 0;
   		for(int i = 0, id; i < tableSize; i++) {
   			prob++;
        	id = (cur + c1 * i + c2 * i * i) % tableSize;
            if((hTable + id)->empty == 0 && (hTable + id)->key == s) {
            	(hTable + id)->deleted = 1;
            	(hTable + id)->empty = 1;
            	return pii((hTable + id)->val, prob);
            }
        }
        return pii(0, prob);
    }

    void clear(){
    	tableSize = 0;
        delete[] hTable;
    }

    void print(){
        for(int i=0;i<tableSize;i++){
            cout<<i<<"--> ";

            if((hTable+i)->empty==false){
                cout<<"("<<(hTable+i)->key<<" "<<(hTable+i)->val<<")";
            }
            cout<<endl;
        }
    }
};

struct double_hashing {
    openNode* hTable;
    int tableSize = 0;
    double_hashing () { 
    }
    void makeTable (int n) {
        tableSize = n;
        hTable = new openNode[tableSize];  
    }
    pii findString(string s){
    	int cur1 = h1(tableSize, s), cur2 = h2(tableSize, s);
    	int prob = 0;
		for (int i = 0, id; i < tableSize; i++) {
			prob++;
			id = (cur1 + i * cur2) % tableSize;
			if ((hTable + id)->empty) {
                if ((hTable + id)->deleted == 0) return pii(0, prob);
            }
           	else if ((hTable + id)->key == s) {
           		return pii((hTable + id)->val, prob);
            }
        }
        return pii(0, prob); 
    }
    pii insert(string s, int val){
        int cur1 = h1(tableSize, s), cur2 = h2(tableSize, s);
        int prob = 0;
        for(int i = 0, id; i < tableSize; i++){
        	prob++;
        	id = (cur1 + i * cur2) % tableSize;
            if((hTable + id)->empty) {
            	(hTable + id)->val = val;
                (hTable + id)->key = s;
                (hTable + id)->empty = 0;
                (hTable + id)->deleted = 0;
                return pii(val, prob);
            }
        }   
        return pii(0, prob);
    }
    //p.first --> value ,p.second --> number of probs
   pii erase (string s) {
   		int cur1 = h1(tableSize, s), cur2 = h2(tableSize, s);
   		int prob = 0;
   		for(int i = 0, id; i < tableSize; i++) {
   			prob++;
        	id = (cur1 + i * cur2) % tableSize;
            if((hTable + id)->empty == 0 && (hTable + id)->key == s) {
            	(hTable + id)->deleted = 1;
            	(hTable + id)->empty = 1;
            	return pii((hTable + id)->val, prob);
            }
        }
        return pii(0, prob);
    }

    void clear(){
    	tableSize = 0;
        delete[] hTable;
    }

    void print(){
        for(int i=0;i<tableSize;i++){
            cout<<i<<"--> ";

            if((hTable+i)->empty==false){
                cout<<"("<<(hTable+i)->key<<" "<<(hTable+i)->val<<")";
            }
            cout<<endl;
        }
    }
};