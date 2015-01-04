#ifndef PREFLOW_HPP
#define PREFLOW_HPP

#include <queue>
#include "utils.hpp"

class Preflow_push {
private:
    Graph G;
    vector<Edge> edges;
    int size;
    vector<int> count;
    vector<int> dist;
    vector<bool> active;
    queue<int> Q;
    vector<int> excess;

    void Discharge (int v);
    void Relabel (int v);
    void Gap (int k);
    void Push(int id_edge, int from);
    void Enqueue (int v);

public:
    Preflow_push (const Graph& g, const vector<Edge>& edges);
    int get_maxflow (int s, int t);
};

#endif