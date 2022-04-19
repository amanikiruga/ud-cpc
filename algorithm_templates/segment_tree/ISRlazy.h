// ISRlazy.h implements class ISR (see e.cpp) with slow segsum, fast replace. BDS 2022Apr
#include <vector>

class ISR {
  std::vector<float> V;
 public:
  typedef vector<float>::iterator ptr;

  void init(ptr b, ptr e) { 
    V.resize(0); 
    for (ptr p = b; p != e; ++p) 
      V.push_back(*p); 
  } // O(n) time

  void init(int n) { V.resize(n); } // O(1) time

  void replace(int i, float x) { V[i] = x; } // O(1) time

  float segsum(int i, int j) { 
    float ans = 0.0; 
    for (int k = i; k < j; ++k) 
      ans = ans + V[k];
    return ans;
  } // O(n) time

}; // class ISR
