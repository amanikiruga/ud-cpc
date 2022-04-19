// e.cpp Segment Sums and data changes. 
// Relevant to problem E of 2021 MidAtlantic problem set.   BDS, Mar 2022

/* Problem e: 
    For a sequence s_1,..,s_n of n numbers (floats), respond to a series of queries.
    There are two query forms: 
    "1 l r" asks for the sum over index range [l,r], namely s_l+..+s_r.
    "2 i x" asks to set s_i = x.

  Input: 
    Integers n and q on line 1. Promise: 0 < n,q <= 2*10^5.
    1 float on each of next n lines, the s_1,..,s_n.
    1 query on each of the following q lines.
    Promise: 1 <= l < r <= n for queries of the form "1 l r"
    Promise: 1 <= i < n for queries of the form "2 i x"
  Output:
    For each query of form "1 l r" make one line containing one number, 
    the segment sum s_l+..+s_r.

Example: 
Input: 
5 3
1.1 2.1 3.1 4.1 5.1
1 2 4
2 3 3.2
1 2 4
Output:
9.3
9.4
*/

#include <iostream>
#include <vector>
using namespace std;

#include "ISRlazy.h"
//#include "ISRprefix.h"
//#include "ISRsegtree.h"
/* An object of class ISR object stores data values v_0, v_1, ..., v_(n-1).
 *
 * Note that the ISR system uses 0 based indexing whereas 
 * the problem description uses 1 based indexing.  
 * Thus v_0 = s_1, v_1 = s_2, etc.
 *
 * ISR provides three functions:  
 *   void  init(int n);             // Set up data structure 
 *   float segsum(int l, int r);    // Return v_l  .. + v_(r-1).
 *   void  replace(int i, float x); // set v_i to x. 
 * You can call replace repeatedly to define v_0,..,v_(n-1), 
 * then also use it for replacement queries.
 */

int main(){ int n,q,qtype,l,r; float x;

  // setup
  cin >> n >> q;
  vector<float> S(n);
  for (int i = 0; i < n; ++i) {
    cin >> S[i];
  }
  ISR R;
  R.init(S.begin(), S.end());

  // process queries
  for (int k = 0; k < q; ++k) {
    cin >> qtype; // but read other args only after determining type
    if (qtype == 1) {
      cin >> l >> r;
      cerr << qtype << " " << l << " " << r << ": ";
      cout << R.segsum(l-1,r) << endl;
    } else {// qtype == 2
      cin >> l >> x;
      cerr << qtype << " " << l << " " << x << ": ";
      R.replace(l-1,x);
    }
  } // end process queries
} // end main

/*
 For a sequence of floats v_0, v_1, .., v_(n-1), provide the functions 
 1. segsum(i,j) to return the sum s_i + .. + s_(j-1).
 2. replace(i, x) to set v_i to x, which may replace a previous value.

|---------|------|-----------|-----------|---------------------------|
| version | init |  segsum   |  replace  |  description              |
|---------|------|-----------|-----------|---------------------------|
| lazy    | O(n) |      O(n) |      O(1) | array of values           |
| prefix  | O(n) |      O(1) |      O(n) | array of prefix sums      |
| seg tree| O(n) | O(log(n)) | O(log(n)) | tree of pow2 segment sums |
|---------|------|-----------|-----------|---------------------------|
*/

#if 0
#include <vector>

class ISR {
  std::vector<float> V;
 public:

  void init(int n) { V.resize(n); } // O(1) time

  void replace(int i, float x) { V[i] = x; } // O(1) time

  float segsum(int i, int j) { // O(n) time
    float ans = 0; 
    for (int k = i; k < j; ++k) 
      ans = ans + V[i];
    return ans;
  } // segsum

}; // class ISR

void set (int i, float x) { // set 1 value.
  if (V.size() < i+1) V.resize(i+1);
  V[i] = x; 
} // set runtime O(1)

void reset (int i, float x) { // replace 1 value.
  V[i] = x; 
} // reset runtime O(1)
/********************/
#elif version == 2 // use prefix sums to get fast interval_sum, slow replace.

vector<float> PS; // will be PS[k-1] = s_1+..+s_k.
bool initialized = false;

float interval_sum(int l, int r) {
  if (not initialized) { // Initialize by updating to prefix sums
    for (int i = 1; i < PS.size(); ++i) PS[i] += PS[i-1];
    initialized = true;
  }
  if (l == 0) return PS[r-1];
  else return PS[r-1] - PS[l-1];
} // interval_sum runtime O(1) (except the first time: O(n) to initialize).

void set( int i, float x) {
  if ( PS.size() < i+1 ) PS.resize(i+1);
  PS[i] = x;
}
void reset(int i, float x) { 
  if (not initialized) { set( i, x ); return; }

  // Fix it up to prefix sums again
  float oldx = PS[i];
  if (i > 0) oldx -= PS[i-1]; 
  for (int k = i; k < PS.size(); ++k) 
    PS[k] += x - oldx;
  } // reset runtime is O(n) 

/********************/
#elif version == 3 // pretty fast range_sum, pretty fast replace

// uses complete binary tree of power-of-two interval sums
vector<float> T; // Root is T[1]. T[0] is not used.
bool initialized = false;

int Par(int i) { return i/2; } // Parent
int LC(int i) { return 2*i; } // Left child
int RC(int i) { return 1 + 2*i; } // Right child
/*
           v0:s7
         _/      \_
    v0:v3          v4:v7
    /   \           /   \
v0:v1   v2:v3    v4:v5  v6:v7
 /  \    /  \    /  \    /  \
v0  v1  v2  v3  v4  v5  v6  v7
*/

void set(int i, float x) { 
   if (T.size() < i+1) T.resize(i+1);
   T[i] = x;
} // set runtime is O(1)

void initialize(); // build tree

void reset(int i, float x) { 
  if (not initialized) initialize();
  int m = T.size()/2;
  T[m+i] = x;
  for (int v = Par(m+i); v != 0; v = Par(v))
    T[v] = T[LC(v)] + T[RC(v)];
//for (int k = 1; k < T.size(); ++k) cerr << T[k] << ", "; cerr << endl;
} // reset runtime is O(lg(n))

/* ISN (Interval Sum within Node) 
  Requires: node holds the sum of v_i at indices i in [begin..end) and begin <= l < r <= end).  
  Promises: return value is sum of v_i for i in [l..r).
  Note that end-begin is a power of2.
*/
float ISN (int l, int r, int node, int begin, int end);

float interval_sum(int l, int r) {
  if (not initialized) initialize();
  return ISN(l, r, 1/*root*/, 0, T.size()/2);
}

// using 0 based indexing with right open interval specifications, eg. [l..r)
float ISN (int l, int r, int node, int begin, int end) {
  int mid = (begin+end)/2;
  // done
  if (l == begin and r == end) return T[node];
  // all on one side
  if (l >= mid) return ISN( l, r, RC(node), mid, end);
  if (r <= mid)  return ISN( l, r, LC(node), begin, mid);
  // straddle
  else          return ISN( l, mid, LC(node), begin, mid) // right side pegged
                     + ISN( mid, r, RC(node), mid, end); // left side pegged
}

void initialize() {
  int n = T.size();
  int m = 1; while (m < n) m *= 2; // first power of 2 not less than n.
  T.resize(2*m);
  // leaves
  for (int i = 0; i < n; ++i) T[m+i] = T[i]; // data at leaves
  for (int i = n; i < m; ++i) T[m+i] = 0; // padding 
  // internal nodes
  for (int i = m-1; i > 0; --i) T[i] = T[LC(i)] + T[RC(i)];
  initialized = true;
for (int i = 1; i < 2*m; ++i) cout << T[i] << " "; cout << endl;
  cout << "initialized" << endl;
}

#elif version == 4 // version 3 generalized
typedef float summable;
float add(float a, float b) { return a+b; }
float zero(){ return 0.0; }
#include "isr.h"

#endif // versions
