#ifndef EK_HPP
#define EK_HPP

#include <queue>
#include "utils.hpp"

class Edmonds_Karp {
private:
    Graph G;
    vector<Edge> edges;
    int size;

    int bfs (vector<pair<int,int> >& parent, int start, int end);

public:
    Edmonds_Karp (const Graph& g, const vector<Edge>& edges);
    int get_maxflow (int source, int sink);
};

#endif