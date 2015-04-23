#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

typedef long long int64;

#define MAXN 100000

int depth[MAXN], parent[MAXN], treesz[MAXN];
int chain[MAXN], chainpos[MAXN], chainleader[MAXN];

int citytax[MAXN], roadtax[MAXN];
int64 ctsegt[1<<18], rdsegt[1<<18], change[1<<18];

int N, cur_chain, pos;

vector< pair<int,int> > gr[MAXN];

void explore(int u) {
    int v;
    treesz[u] = 1;
    for (size_t i = 0, sz = gr[u].size(); i < sz; ++i) {
        v = gr[u][i].first;
        if (parent[v] == -1) {
            parent[v] = u;
            depth[v] = depth[u] + 1;
            explore(v);
            treesz[u] += treesz[v];
        }
    }
}

void decompose(int u, bool light = true) {
    if (light) {
        ++cur_chain;
        chainleader[cur_chain] = u;
    }
    chain[u] = cur_chain;
    chainpos[u] = pos++;
    
    int v, ind, mx = -1;
    for (size_t i = 0, sz = gr[u].size(); i < sz; ++i) {
        v = gr[u][i].first;
        if (parent[v] == u && ((mx == -1) || (treesz[mx] < treesz[v])))
            mx = v, ind = i;
    }
    
    if (mx != -1) {
        roadtax[pos-1] = gr[u][ind].second;
        decompose(mx, false);
    }
    
    for (size_t i = 0, sz = gr[u].size(); i < sz; ++i) {
        v = gr[u][i].first;
        if ((parent[v] == u) && (v != mx)) {
            roadtax[pos-1] = gr[u][i].second;
            decompose(v);
        }
    }
}

int lca(int u, int v) {
    while (chain[u] != chain[v]) {
		if (depth[chainleader[chain[u]]] < depth[chainleader[chain[v]]]) {
			v = parent[chainleader[chain[v]]];
		} else {
			u = parent[chainleader[chain[u]]];
        } 
    }
    if (depth[u] < depth[v]) return u;
    return v;
}

int64 *tree;
int *seq;

void build(int node, int lo, int hi) {
    change[node] = 0;
    if (lo == hi) {
        tree[node] = int64(seq[lo]);
    } else {
        int mid = (lo+hi)>>1;
        int ls = (node<<1) + 1;
        int rs = ls + 1;
        build(ls,lo,mid);
        build(rs,mid+1,hi);
        tree[node] = tree[ls] + tree[rs];
    }
}

inline void push(int node, int len) {
    if (len > 1) {
        int ls = (node<<1) + 1;
        int rs = ls + 1;
        change[ls] += change[node];
        change[rs] += change[node];
    }
    tree[node] += int64(len) * change[node];
    change[node] = 0;
}

bool laziness;
int from, to, X;
int64 result;

void update(int node, int lo, int hi) {    
    if (laziness && change[node]) push(node, hi-lo+1);
    
    if (from <= lo && hi <= to) {
        if (laziness) {
            change[node] += X;
            push(node,hi-lo+1);
        } else {
            citytax[lo] += X;
            tree[node] += X;
        }
    } else {
        int mid = (lo+hi)>>1;
        int ls = (node<<1) + 1;
        int rs = ls + 1;
        if (from <= mid) update(ls,lo,mid);
        if ((mid+1) <= to) update(rs,mid+1,hi);
        if (laziness && change[ls]) push(ls,mid-lo+1);
        if (laziness && change[rs]) push(rs,hi-mid);
        tree[node] = tree[ls] + tree[rs];
    }
}

void get(int node, int lo, int hi) {
    if (laziness && change[node]) push(node, hi-lo+1);
    if (from <= lo && hi <= to) {
        result += tree[node];
    } else {
        int mid = (lo+hi)>>1;
        int ls = (node<<1) + 1;
        int rs = ls + 1;
        if (from <= mid) get(ls,lo,mid);
        if ((mid+1) <= to) get(rs,mid+1,hi);
    }
}

void update(int u, int v) {
    while (u != v) {
        if (chain[u] != chain[v]) {
            if (chainleader[chain[u]] == u) {
                roadtax[chainpos[u]-1] += X;
                u = parent[u];
            } else {
                from = chainpos[chainleader[chain[u]]], to = chainpos[u]-1;
                if (from <= to) update(0,0,N-2);
                u = chainleader[chain[u]];
            }
        } else {
            from = chainpos[v], to = chainpos[u]-1;
            if (from <= to) update(0,0,N-2);    
            u = v;
        }
    }
}

void get(int u, int v) {
    while (u != v) {
        if (chain[u] != chain[v]) {
            if (chainleader[chain[u]] == u) {
                result += int64(roadtax[chainpos[u]-1]);
                result += int64(citytax[chainpos[u]]);
                u = parent[u];
            } else {
                from = chainpos[chainleader[chain[u]]], to = chainpos[u]-1;
                if (from <= to) {
                    tree = rdsegt; laziness = true;
                    get(0,0,N-2);
                    from++, to++;
                    tree = ctsegt; laziness = false;
                    get(0,0,N-1);
                }
                
                u = chainleader[chain[u]];
            }
        } else {
            from = chainpos[v], to = chainpos[u]-1;
            if (from <= to) {
                tree = rdsegt; laziness = true;
                get(0,0,N-2);
                from++, to++;
                tree = ctsegt; laziness = false;
                get(0,0,N-1);
            }
            u = v;
        }
    }
}


int main() {
    ios::sync_with_stdio(false);
    
    while (cin >> N && N) {

        for (int i = 0; i < N; ++i) {
            cin >> ctsegt[i];
            parent[i] = -1;
            gr[i].clear();
        }
        
        for (int i = 0; i < N-1; ++i) {
            int u, v, w;
            cin >> u >> v >> w;            
            gr[u].push_back(make_pair(v,w));
            gr[v].push_back(make_pair(u,w));            
        }
        
        parent[0] = 0;
        explore(0);
        parent[0] = -1;
        cur_chain = -1, pos = 0;
        decompose(0);
        
        for (int i = 0; i < N; ++i) {
            citytax[chainpos[i]] = ctsegt[i];
        }
        
        tree = ctsegt;
        seq = citytax;
        build(0,0,N-1);
        
        tree = rdsegt;
        seq = roadtax;
        build(0,0,N-2);
        
        int Q, cmd, A, B, ancestor;
        cin >> Q;
             
        while (Q--) {
            cin >> cmd;
            switch (cmd) {
                case 0:
                    cin >> A >> B >> X;                    
                    laziness = true;
                    tree = rdsegt;
                    ancestor = lca(A,B);
                    update(A, ancestor);
                    update(B, ancestor);
                break;
                case 1:
                    cin >> A >> X;                                       
                    from = chainpos[A]; to = from;
                    laziness = false;
                    tree = ctsegt;
                    update(0,0,N-1);
                break;
                case 2:
                    cin >> A >> B;          
                    ancestor = lca(A,B);
                    result = 0;
                    get(A, ancestor);
                    get(B, ancestor);
                    result += int64(citytax[chainpos[ancestor]]);
                    cout << result << "\n";
                break;
            }
        }
    }
    
    return 0;
}