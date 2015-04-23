#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <queue> 
#include <cfloat>
#include <string> 
#include <climits> 
#include <cstring> 
#include <cassert> 
#include <complex>

using namespace std;

#define INF 0x3F3F3F3F
#define LINF 0x3F3F3F3FFFFFFFFFLL
#define EPS 1e-7

#define FILL(X, V) memset(X, V, sizeof(X))
#define TI(X) __typeof((X).begin())

#define FOR(i, a, b) for(int i = a; i <= b; ++i)
#define RFOR(i, b, a) for(int i = b; i >= a; --i)
#define REP(i, N) FOR(i, 0, N-1)
#define RREP(i, N) RFOR(i, N-1, 0)
#define FORIT(it, i) for(typeof((i).begin()) it = (i).begin(); it != (i).end(); ++it)

#define ALL(V) V.begin(), V.end()
#define S(V) (int)V.size()

#define pb push_back
#define mp make_pair

template<typename T> T inline SQR( const T &a ){ return (a)*(a); }
inline int SGN( double a ){ return ((a > EPS) ? (1) : ((a < -EPS) ? (-1) : (0))); }
inline int CMP( double a, double b ){ return SGN(a - b); }

typedef long long int64;
typedef unsigned long long uint64;

unsigned int mypow(unsigned int a, unsigned int b) {
    unsigned int out = 1;
    while(b) {
        if(b & 1) out *= a;
        a *= a;
        b >>= 1;
    }

    return out;
}

int main(){
    ios::sync_with_stdio( false );
	  string n;
    while(cin>>n) {
        unsigned int ans = 0, sz = (S(n)+1)/2;
        if(S(n) == 1) {
            cout << n << '\n';
            continue;
        }
        
        ans += 10;
        unsigned int inc = 9;
        FOR(i, 2, S(n) - 1) {
            ans += inc;
            if(i % 2 == 0) inc *= 10;
        }

        REP(i, sz) {
            if(i) {
                ans += (n[i] - '0') * mypow(10, sz - i - 1);
            } else {
                ans += (n[i] - '0' - 1) * mypow(10, sz - i - 1);
            }
        }

        cout << ans << '\n';
    }
	  return 0;
}

