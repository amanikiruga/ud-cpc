/*
monoid concept:
if M is a monoid class
1. M::element is the type of it's elements
2. element M::add(element, element) is the monoid's associative binary operation
3. element M::zero() is the identity element for the binary op "add".
   For all a, add(a, zero()) = a = add(zero(), a).
*/

// Complete tree relationships
int Par(int i) { return i/2; } // parent
int LC(int i) { return 2*i; } // left child
int RC(int i) { return 2*i + 1; } // right child

// The Segment tree data structure providing Init, Segsum, Replace.
template<class M> struct ISR { // M must satisfy the monoid concept
  std::vector<M::element> T;
  int n; // size of data set v_0, ..., v_(n-1).

  // public: functions init, replace, segsum.

  void init(int nn) {
    n = nn;
    int m;
    for (m = 1; m < n; m *= 2); // m/2 < n <= m and m is a power of 2.
    T.resize(2*m, M::zero());
  }

  void init(std::vector V) {
    init(V.size());
    for (int i = 0; i < n; ++i) T[m+i] = V[i]; // leaves
    for (int i = n; i < m; ++i) T[m+i] = M::zero(); // fake leaves
    for (int i = m; i > 0; --i) T[i] = M::add(T[LC(i)],T(RC(i)])); // internal nodes
  } // time O(n).

  void replace(int i, monoid::element x) {
    int m = T.size()/2;
    int nodei = m+i;
    T[nodei] = x;
    for (int k = Par(nodei); k > 0; k = Par(k)) 
      T[k] = M::add(T[LC(k)],T(RC(k)])); 
  }

  M::element segsum(int l, int r) { 
    if (l >= r) return M::zero();
    return segsumAtNode(l, r, 0, T.size()/2, 1);
  }

  M::element segsumAtNode(int l, int r, int ls, int rs, int node) { 
  // node must hold the segment sum for v[ls..rs) and ls <= l <= r <= rs.
    int mid = (rs + ls)/2;
    if (l == ls and r == rs) 
      return T[node];
    if (mid <= l) 
      return segsumAtNode(l, r, mid, rs, RC(node));
    if (r <= mid) 
      return segsumAtNode(l, r, ls, mid, LC(node));
    // now l < mid < r
      return M::add(segsumAtNode(l, mid, ls, mid, LC(node));
                      segsumAtNode(mid, r, mid, rs, RC(node)));
  }
}; // struct ISR
