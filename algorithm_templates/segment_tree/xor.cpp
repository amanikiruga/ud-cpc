// xor.cpp Segment evenness and data changes. 
// Relevant to problem E of 2021 MidAtlantic problem set.   BDS, Mar 2022

/* Problem xor: 
    For a string s of n lower case letters, respond to a series of queries.
    Call a substring of s "even" if each letter of the alphabet occurs an even number 
    of times in the substring.  For example "azazbb" is even and "azazza" is not.
    There are two query forms: 
    "1 l r" asks if the segment over the index range [l,r] is even.
    "2 i x" asks to set s_i = x.
    String indexing is 0 based.

  Input: 
    string s on line 1 of length n.  promise 0 < n <= 2*10^5.
    q on line 2. Promise: 0 < q <= 2*10^5.
    1 query on each of the following q lines.
    Promise: 0 <= l < r < n for queries of the form "1 l r"
    Promise: 0 <= i < n for queries of the form "2 i x"
  Output:
    For each query of form "1 l r" make one line 
    containing "even" if the segment is even, otherwise "no".

Example  
Input: 
banana
4
1 1 4
1 0 3
2 2 b
1 0 3
Output:
even
no
even
*/

#include <iostream>
#include <vector>
using namespace std;

#include "ISR.h"

struct X { // meets monoid concept
  typedef unsigned int element;
  element add(element a, element b) { return a xor b; }
  element zero() { return 0; }
};

int main() {
  ISR<X> T;
  string s;
  cin >> s;

  int n = s.size()
  vector<int> Pat(n); // Pat[i] is odd-even pattern of the string s[0..i).
  Pat[0] = 0;`
  for (int i = 1; i <= n; ++i)
    Pat[i] = Pat[i-1] xor pattern(s[i]);
  int p = 0; pattern of first

  T.init(Pat.begin(), Pat.end());
  cin >> q;
  for (int i = 0; i < q; ++i) {
    //process queries
    cin >> qtype >> l >> r;
    if (qtype == 1) 
      cout >> T.segsum(l, r+1);
    else // qtype == 2
      T.replace(l, r);
  }
} // main
