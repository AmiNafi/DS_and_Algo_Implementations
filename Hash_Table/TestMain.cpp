#include <bits/stdc++.h>
#include "hash_table.cpp"
using namespace std;
using namespace std::chrono;
#define int long long 
#define ll long long
#define pii pair < int, int >

vector < string > strgenerator (int n) {
	srand(time(0));
	map < string, int > got;
	vector < string > ret;
	while ((int)(ret.size()) != n) {
		int ln = 7;
		string s = "";
		while (ln--) {
			s += (char)('a' + rand() % 26);
		}
		if (!got.count(s)) {
			ret.push_back(s);
			got[s] = 1;
		}
	}
	return ret;
}
int checkh1 (int n, const vector<string>&vs){
	set < int > st;
	for (int i = 0; i < (int)(vs.size()); i++) {
		st.insert(h1(n, vs[i]));
	}
	return (int)(st.size());
}
int checkh2 (int n, const vector<string>&vs){
	set < int > st;
	for (int i = 0; i < (int)(vs.size()); i++) {
		st.insert(h2(n, vs[i]));
	}
	return (int)(st.size());
}
void precomp () {
	pw[0] = 1;
	for (int i = 1; i < 10; i++) pw[i] = (pw[i - 1] * 31);
}
void solve () {
	seperate_chaining_hash sep;
    linear_probing linear;
    quadratic_probing quad;
    double_hashing dh;
    int n, lfcnt = 6, wc, wc10, wc5;
    cin >> n;
    double tdiv = 1e6;
    vector < double > load_factor={.4, .5, .6, .7, .8, .9}; // Load Factor = (n / word_count)
    vector < double > lf;
    lf.push_back(.4);
    lf.push_back(.5);
    lf.push_back(.6);
    lf.push_back(.7);
    lf.push_back(.8);
    lf.push_back(.9);
    vector < int > sid, delid, halfid;
    vector < double > sepBefore(lfcnt), sepAfter(lfcnt), linearBefore(lfcnt), linearAfter(lfcnt), linearBeforeProb(lfcnt), linearAfterProb(lfcnt), 
    quadBefore(lfcnt), quadAfter(lfcnt), quadBeforeProb(lfcnt), quadAfterProb(lfcnt), doubleBefore(lfcnt), doubleAfter(lfcnt), doubleBeforeProb(lfcnt), doubleAfterProb(lfcnt);
    int ck1 = checkh1(n, strgenerator(100)), ck2 = checkh2(n, strgenerator(100));
    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    for (int i = 0; i < lfcnt; i++) {
    	sep.makeTable(n);
    	linear.makeTable(n);
    	quad.makeTable(n);
    	dh.makeTable(n);
    	wc = lf[i] * n;
    	wc5 = lf[i] * n * 0.05;
    	wc10 = lf[i] * n * 0.1;
    	vector < string > vs = strgenerator(wc);
    	for (int j = 0; j < wc; j++) {
    		sep.insert(vs[j], j + 1);
    		linear.insert(vs[j], j + 1);
    		quad.insert(vs[j], j + 1);
    		dh.insert(vs[j], j + 1);
    	}
    	
    	srand(time(0));
    	map < int, int > mp;
    	for (int cnt = 0, x; cnt < wc10; ) {
    		x = rand() % wc;
    		if (!mp.count(x)) {
    			sid.push_back(x);
    			cnt++;
    			mp[x] = 0;
    		}
    	}
    	start = high_resolution_clock::now();
    	for (int j = 0; j < (int)(sid.size()); j++) {
    		sep.findString(vs[sid[j]]);
    	}
    	stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        sepBefore[i] = duration.count() / (wc10 * tdiv);
        double tprob = 0;
        start = high_resolution_clock::now();
        for (int j = 0; j < wc10; j++) {
        	pii p = linear.findString(vs[sid[j]]);
        	tprob += p.second;
        }
        
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        linearBefore[i] = duration.count() / (wc10 * tdiv);
        linearBeforeProb[i] = tprob / wc10;

        tprob = 0;
        start = high_resolution_clock::now();
        for (int j = 0; j < wc10; j++) {
        	pii p = quad.findString(vs[sid[j]]);
        	tprob += p.second;
        }
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        quadBefore[i] = duration.count() / (wc10 * tdiv);
        quadBeforeProb[i] = tprob / wc10;

        tprob = 0;
        start = high_resolution_clock::now();
        for (int j = 0; j < wc10; j++) {
        	pii p = dh.findString(vs[sid[j]]);
        	tprob += p.second;
        }
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        doubleBefore[i] = duration.count() / (wc10 * tdiv);
        doubleBeforeProb[i] = tprob / wc10;
        srand(time(0));
        mp.clear();
        
        for (int cnt = 0; cnt < wc10; ) {
        	int x = rand() % wc;
        	if (mp.count(x) == 0) {
        		delid.push_back(x);
        		mp[x] = 0;
        		cnt++;
        	}
        }
        for (int j = 0; j < (int)(delid.size()); j++) {
        	sep.erase(vs[delid[j]]);
        	linear.erase(vs[delid[j]]);
        	quad.erase(vs[delid[j]]);
        	dh.erase(vs[delid[j]]);
        }
        srand(time(0));
        mp.clear();
        for (int cnt = 0; cnt < wc5; ) {
        	int x = delid[rand() % wc10];
        	if (mp.count(x) == 0) {
        		cnt++;
        		mp[x] = 0;
        		halfid.push_back(x);
        	}
        }
        for (int j = 0; j < wc10; j++) {
        	mp[delid[j]] = 0;
        }
        
        for (int cnt = 0, x; cnt < wc5; ) {
        	x = rand() % wc;
        	if (mp.count(x) == 0) {
        		cnt++;
        		halfid.push_back(x);
        		mp[x] = 0;
        	}
        }
        //if (i == 3) cout << " got 3 3333 185" << endl;
        mp.clear();
        start = high_resolution_clock::now();
    	for (int j = 0; j < (int)(halfid.size()); j++) {
    		sep.findString(vs[halfid[j]]);
    	}
    	//if (i == 3) cout << " got 3 3333 3223" << endl;
    	stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        sepAfter[i] = duration.count() / (wc10 * tdiv);
        tprob = 0;
        start = high_resolution_clock::now();
        for (int j = 0; j < wc10; j++) {
        	pii p = linear.findString(vs[delid[j]]);
        	tprob += p.second;
        }
        
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        linearAfter[i] = duration.count() / (wc10 * tdiv);
        linearAfterProb[i] = tprob / wc10;

        tprob = 0;
        start = high_resolution_clock::now();
        for (int j = 0; j < wc10; j++) {
        	pii p = quad.findString(vs[delid[j]]);
        	tprob += p.second;
        }
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        quadAfter[i] = duration.count() / (wc10 * tdiv);
        //cout << " tprob " << tprob << endl;
        quadAfterProb[i] = tprob / wc10; 
        
        tprob = 0;
        start = high_resolution_clock::now();
        for (int j = 0; j < wc10; j++) {
        	pii p = dh.findString(vs[delid[j]]);
        	tprob += p.second;
        }
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        doubleAfter[i] = duration.count() / (wc10 * tdiv);
        doubleAfterProb[i] = tprob / wc10;
        sep.clear();
        linear.clear();
        quad.clear();
        dh.clear();
        sid.clear();
        delid.clear();
        halfid.clear();
    }
    cout << endl;
    cout << "Table 1: Performance of separate chaining in various load factors"<<endl;
    cout << "------------------------------------------------------------------"<<endl;
    cout << "|                   |   Before Deletion   |     After Deletion    |"<<endl;
    cout << "|-------------------|---------------------|-----------------------|"<<endl;
    cout << "|    Load Factor    |   Avg Search Time   |    Avg Search Time    |"<<endl;
    cout << "|-------------------|---------------------|-----------------------|"<<endl;
    for(int i = 0; i < lfcnt; i++) {
    cout << "|      "<<lf[i]<<"     |      "<<sepBefore[i]<<"ms     |      "<<sepAfter[i]<<"ms       |"<<endl;
    cout << "|-------------------|---------------------|-----------------------|"<<endl;
    }

    cout << endl;
    cout << "Table 2: Performance of linear probing in various load factors"<<endl;
    cout << " ------------------------------------------------------------------------------------------------------------"<<endl;
    cout << "|                          |             Before Deletion            |              After Deletion            |"<<endl;
    cout << "|--------------------------|----------------------------------------|----------------------------------------|"<<endl;
    cout << "|        Load Factor       |  Avg Search Time  |  Avg Num. Of Probe |  Avg Search Time  |  Avg Num. Of Probe |"<<endl;
    cout << "|--------------------------|-------------------|--------------------|-------------------|--------------------|"<<endl;
    for (int i = 0; i < lfcnt; i++) {
    cout << "|          "<<lf[i]<<"        |     "<<linearBefore[i]<<"ms    |     "<<linearBeforeProb[i]<<"       |";
    cout << "     "<<linearAfter[i]<<"ms    |      "<<linearAfterProb[i]<<"      |"<<endl;
    cout << "|--------------------------|----------------------------------------|----------------------------------------|"<<endl;
    }

    cout << endl;
    cout << "Table 3: Performance of quadratic probing in various load factors"<<endl;
    cout << " ------------------------------------------------------------------------------------------------------------" << endl;
    cout << "|                          |             Before Deletion            |              After Deletion            |" << endl;
    cout << "|--------------------------|----------------------------------------|----------------------------------------|" << endl;
    cout << "|        Load Factor       |  Avg Search Time  |  Avg Num. Of Probe |  Avg Search Time  |  Avg Num. Of Probe |" << endl;
    cout << "|--------------------------|----------------------------------------|----------------------------------------|" << endl;
    for(int i = 0; i < lfcnt; i++) {
    cout << "|          "<<lf[i]<<"        |     " << quadBefore[i]<<"ms    |     " << quadBeforeProb[i] <<"       |";
    cout << "     "<< quadAfter[i]<<"ms    |      "<< quadAfterProb[i]<<"      |" <<endl;
    cout << "|--------------------------|----------------------------------------|----------------------------------------"<<endl;
    }

    cout<< endl;
    cout << "Table 4: Performance of double hashing in various load factors"<<endl;
    cout << " ------------------------------------------------------------------------------------------------------------"<<endl;
    cout << "|                          |             Before Deletion            |              After Deletion            |"<<endl;
    cout << "|--------------------------|----------------------------------------|----------------------------------------|"<<endl;
    cout << "|        Load Factor       |  Avg Search Time  |  Avg Num. Of Probe |  Avg Search Time  |  Avg Num. Of Probe |"<<endl;
    cout << "|--------------------------|----------------------------------------|----------------------------------------|"<<endl;
    for(int i = 0; i < lfcnt; i++) {
    cout << "|          "<<lf[i]<<"        |     " << doubleBefore[i]<<"ms    |     "<<doubleBeforeProb[i]<<"       |";
    cout << "     " << doubleAfter[i]<<"ms    |      " << doubleAfterProb[i]<<"      |"<<endl;
    cout << "|--------------------------|----------------------------------------|----------------------------------------|"<<endl;
    }
    for(int i = 0; i < lfcnt; i++) {
	    cout<< endl;
	    cout<<"Table " << i + 5 << ": Performance of various collision resolution methods in load factor " << lf[i] <<endl;
	    cout<<" ------------------------------------------------------------------------------------------------------------"<<endl;
	    cout<<"|                          |             Before Deletion            |              After Deletion            |"<<endl;
	    cout<<"|--------------------------|----------------------------------------|----------------------------------------|"<<endl;
	    cout<<"|           Method         |  Avg Search Time  |  Avg Num. Of Probe |  Avg Search Time  |  Avg Num. Of Probe |"<<endl;
	    cout<<"|--------------------------|----------------------------------------|----------------------------------------|"<<endl;
	    cout << "|     Seperate Chaining    |      "<<sepBefore[i]<<"     |         N/A        |      " << sepAfter[i]<<"     |         N/A        |"<<endl;
	    cout<<"|--------------------------|----------------------------------------|----------------------------------------|"<<endl;
	    cout <<"|      Linear Probing      |     "<<linearBefore[i]<<"ms    |     " << linearBeforeProb[i]<<"       |";
	    cout <<"     "<<linearAfter[i]<<"ms    |      "<<linearAfterProb[i]<<"      |"<<endl;
	    cout<<"|--------------------------|----------------------------------------|----------------------------------------|"<<endl;
	    cout<<"|     Quadratic Probing    |     "<<quadBefore[i]<<"ms    |     "<<quadBeforeProb[i]<<"       |";
	    cout<<"     "<<quadAfter[i]<<"ms    |      "<<quadAfterProb[i]<<"      |"<<endl;
	    cout<<"|--------------------------|----------------------------------------|----------------------------------------|"<<endl;
	    cout<<"|      Double Hashing      |     "<<doubleBefore[i]<<"ms    |     "<<doubleBeforeProb[i]<<"       |";
	    cout<<"     "<<doubleAfter[i]<<"ms    |      "<<doubleAfterProb[i]<<"      |" <<endl;
	    cout<<"|--------------------------|----------------------------------------|----------------------------------------|"<<endl;
	}
}
int32_t main () {
	
    // freopen("E:/newts/in.txt", "r", stdin);
    freopen("E:/newts/out.txt", "w", stdout);
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    	precomp();
    	cout << fixed << setprecision(6);
        solve ();
   return 0;
}
