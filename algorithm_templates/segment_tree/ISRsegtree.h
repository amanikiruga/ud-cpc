// ISRsegtree.h implements class ISR (see e.cpp) with fast segsum and replacement.  BDS 2022Apr
#include <vector>

// complete tree node relationships
int Par(int i) { return i/2; } // parent
int LC(int i) { return 2*i; } // left child
int RC(int i) { return 2*i + 1; } // right child

struct ISR {
  std::vector<float> T; // size < 4n
  int n; // size of data set v_0, ..., v_(n-1).

// public:: init(), replace(), segsum()

  // Initialize seg tree using data *b = v_0, ..., v_(n-1), *e.
  void init(std::vector<float>::iterator b, std::vector<float>::iterator e) {
    n = e - b;
    int m;
    for (m = 1; m < n; m *= 2); // m/2 < n <= m and m is a power of 2.
    T.resize(2*m);
    for (int i = 0; i < n; ++i) T[m+i] = b[i]; // leaves
    for (int i = n; i < m; ++i) T[m+i] = 0.0; // fake leaves
    for (int i = m; i > 0; --i) T[i] = T[LC(i)] + T[RC(i)]; // internal nodes
  } // time O(n).

  // Replace v_i by x.
  void replace(int i, float x) {
    int m = T.size()/2;
    T[m+i] = x;
    for (int k = Par(m+i); k > 0; k = Par(k)) 
      T[k] = T[LC(k)] + T[RC(k)];
  }

  // Return l == r ? ident() : binop(v_l, segsum(l+1,r)).
  float segsum(int l, int r) { 
    if (l >= r) return 0.0;
    return segsumAtNode(l, r, 0, T.size()/2, 1);
  }

  float segsumAtNode(int l, int r, int ls, int rs, int node) { 
  // node must hold the segment sum for v[ls..rs) and ls <= l <= r <= rs.
  cerr << "l " << l << ", r " << r << ",ls " << ls << ", rs " << rs << ", node " << node << endl;
    int mid = (ls + rs)/2;
    if (l == ls and r == rs) 
      return T[node];
    if (r <= mid) 
      return segsumAtNode(l, r, ls, mid, LC(node));
    if (mid <= l) 
      return segsumAtNode(l, r, mid, rs, RC(node));
    // now l < mid < r
      return segsumAtNode(l, mid, ls, mid, LC(node))
             +
             segsumAtNode(mid, r, mid, rs, RC(node));
  }
}; // struct ISR
