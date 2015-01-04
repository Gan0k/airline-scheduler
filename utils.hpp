#ifndef UTILS  
#define UTILS

#include <vector>
#include <limits>
using namespace std;

#define INFINITY numeric_limits<int>::max()

struct Edge {
    int to, cap, flow;
};

typedef vector<vector<int> > Graph;

#endif