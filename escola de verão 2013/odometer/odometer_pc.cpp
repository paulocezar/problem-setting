/* 
 * Problem: Awesome Odometer
 * Solution: ad-hoc, math
 * Complexity: O(min(log10 N, 65)) per test case
 */

#include <iostream>

using namespace std;

typedef unsigned int uint;

#define MAXLEN 65
uint palin[MAXLEN+1];
uint zpalin[MAXLEN+1];

int main() {
    ios::sync_with_stdio(false);
    
    uint p10 = 1u;
    palin[0] = 1u;
    zpalin[0] = 1u;

    for (int i = 1; i <= MAXLEN; ++i) {
        palin[i] = 9u * p10;
        zpalin[i] = 10u * p10;
        if (!(i & 1)) p10 = 10u * p10;
    }
    palin[1]++;

    string N;
    while (cin >> N) {
        int len = int(N.size()); 
        uint res = 0u;
        for (int i = min(len-1, 65); i > 0; --i) res += palin[i];
        int ntl = (len+1)>>1;
        int at = max(0, (len-66)/2);
        int ilen = max(0, len-2*(at+1));
        while (at < ntl) {
            if (N[at] != '0') {
                uint dig = uint(N[at]-'0');
                if (!at && len > 1) dig--;
                res += dig * zpalin[ilen]; 
            }
            ++at;
            ilen = max(ilen-2, 0);
        }
        cout << res << "\n";
    }

    return 0;
}




