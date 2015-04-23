#include <iostream>

using namespace std;

struct seg_info {
	int first, last;
	bool inc, dec;
	seg_info() {}
	void merge(seg_info* left, int key, seg_info* right) {        
        bool linc = true, ldec = true;
        bool rinc = true, rdec = true;
        int llast = key, rfirst = key;
        
        if (left) {
            first = left->first;
            llast = left->last;
            linc = left->inc;
            ldec = left->dec;
        } else first = key;
        
        if (right) {
            last = right->last;
            rfirst = right->first;
            rinc = right->inc;
            rdec = right->dec;
        } else last = key;
        
        inc = linc && (llast <= key) && (key <= rfirst) && rinc;
        dec = ldec && (llast >= key) && (key >= rfirst) && rdec;
	}
};

struct node_t {
	int key, pr, sz;
	seg_info seg;
    
	node_t *l, *r;
	
    node_t(int k) : key(k), pr(rand()), sz(0), l(NULL), r(NULL) {}
	
    ~node_t() {
		if(l) delete l;
		if(r) delete r;
	}

/*
	void walk() {
		if(l) l->walk();
		cout << key << " ";
		if(r) r->walk();
	}
*/    
};

void rotate_right(node_t* &t) {
	node_t *n = t->l;
	t->l = n->r;
	n->r = t;
	t = n;
}

void rotate_left(node_t* &t) {
	node_t *n = t->r;
	t->r = n->l;
	n->l = t;
	t = n;
}

void fix(node_t* t) {
	if(!t) return;
	t->sz = 1 + ((t->l)?(t->l->sz):(0)) + ((t->r)?(t->r->sz):(0));
    seg_info *lseg, *rseg;
    lseg = (t->l)?(&(t->l->seg)):(NULL);
    rseg = (t->r)?(&(t->r->seg)):(NULL);
	t->seg.merge(lseg, t->key, rseg);
}

void insert(node_t* &t, int val, int pos) {
	if(!t) t = new node_t(val);
	else {
		int lsz = ((t->l)?(t->l->sz):(0));
		if (lsz >= pos) insert(t->l, val, pos);
		else insert(t->r, val, pos-lsz-1);
	}
	
	if (t->l && ((t->l->pr) > (t->pr))) rotate_right(t);
	else if (t->r && ((t->r->pr) > (t->pr))) rotate_left(t);
	
	fix(t->l); fix(t->r); fix(t);
}

inline int p(node_t* t) { return (t) ? (t->pr) : (-1); }

void erase(node_t* &t, int pos) {
	if(!t) return;
	
	int lsz = ((t->l)?(t->l->sz):(0));
	if (lsz+1 != pos) {
		if (lsz >= pos) erase(t->l, pos);
		else erase(t->r, pos-lsz-1);
	} else {
		if (!t->l && !t->r) {
			delete t; 
			t = NULL;
		} else {
			if (p(t->l) < p(t->r)) rotate_left(t);
            else rotate_right(t);
			erase(t, pos);
		}
	}
	
	if (t) { fix(t->l); fix(t->r); fix(t); }
}

void replace(node_t* t, int pos, int val) {
	if(!t) return;
	
	int lsz = ((t->l)?(t->l->sz):(0));
	if (lsz+1 != pos) {
		if (lsz >= pos) replace(t->l, pos, val);
		else replace(t->r, pos-lsz-1, val);
	} else t->key = val;
	
	fix(t);
}

int ith(node_t* t, int i) {
	int lsz = (t->l)?(t->l->sz):(0);
	if (lsz+1 == i) return t->key;
	if (lsz >= i) return ith(t->l, i);
	return ith(t->r, i-lsz-1 );
}

int x, y;
seg_info query(node_t* t, int lo, int hi) {
	if (x <= lo && hi <= y) {
        return t->seg;
	}
    
    int mid = lo + ((t->l)?(t->l->sz):(0)) - 1;
	
	seg_info q1, q2, ans;
	bool f1 = false, f2 = false;
    
    if (mid >= lo && mid >= x) { f1 = true; q1 = query(t->l, lo, mid); }
	if (mid+2 <= hi && mid+2 <= y) { f2 = true; q2 = query(t->r, mid+2, hi); }
	
	if (!f1 && !f2) {
		ans.inc = true;
        ans.dec = true;
        ans.first = t->key;
        ans.last = t->key;
        return ans;
	}
	
	if (f1 && f2) {
		ans.merge(&q1, t->key, &q2);
	} else if (f1) {
		if (x <= mid+1 && mid+1 <= y) ans.merge(&q1, t->key, NULL);
		else return q1;
	} else if (x <= mid+1 && mid+1 <= y) {
        ans.merge(NULL, t->key, &q2);
    } else return q2;
    
	return ans;
}

int main() {
	ios::sync_with_stdio(false);
    
    srand(time(NULL));
	
    int N, Q, ox, oy, cmd;
    while (cin >> N) {
	    node_t* treap = NULL;
	    for (int i = 0; i < N; ++i) {
            cin >> x;
            insert(treap, x, i);
        }
        
        cin >> Q;
        while (Q--) {
		    cin >> cmd;
            switch (cmd) {
                case 0:
                    cin >> x >> y;
                    ox = ith(treap, x);
                    oy = ith(treap, y);
                    replace(treap, x, oy);
                    replace(treap, y, ox);
                break;
                case 1:
                    cin >> x >> y;
                    replace(treap, x, y);
                break;
                case 2:
                    cin >> x >> y;
                    insert(treap, y, x-1);
                break;
                case 3:
                    cin >> x;
                    erase(treap, x);
                break;
                case 4:
                    cin >> x >> y;
                    seg_info res = query(treap, 1, treap->sz);
                    if (res.inc && res.dec) cout << "ALL EQUAL\n";
                    else if (res.inc) cout << "NON DECREASING\n";
                    else if (res.dec) cout << "NON INCREASING\n";
                    else cout << "NONE\n";
                break;
            }
        }   		
	}
	
	return 0;
}


