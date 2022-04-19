// ISRprefix.h implements class ISR (see e.cpp) with fast segsum, slow replacement.  BDS 2022Apr
#include <vector>

class ISR {
  std::vector<float> V;
  typedef vector<float>::iterator ptr;
 public:

  void init(ptr b, ptr e) { 
    V.resize(0); 
    for (ptr p = b; p != e; ++p) 
      V.push_back(*p); 

    // now turn them into prefix sums
    for (int i = 1; i < V.size(); ++i) 
      V[i] += V[i-1];
  } // O(n) time

  void replace(int i, float x) { 
    float currentv_i;
    if (i == 0) currentv_i = V[0];
    else currentv_i = V[i] - V[i-1];

    float delta = x - currentv_i;
    for (int k = i; k < V.size(); ++k)
      V[k] += delta;

  } // O(n) time to fix V[i] and all following prefixes

  float segsum(int l, int r) { 
    if (r == 0) return 0.0;
    float prior_prefix = (l == 0 ? 0.0 : V[l-1]);
    return V[r-1] - prior_prefix;
  } // O(1) time to return V[l..r)

}; // class ISR
