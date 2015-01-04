#include "Preflow_push.hpp"

void Preflow_push::Discharge (int v){
    for (int i = 0; excess[v] > 0 and i < (int)G[v].size(); ++i) 
        Push(G[v][i], v);

    if (excess[v] > 0) {
        if (count[dist[v]] == 1) Gap(dist[v]); 
        else Relabel(v);
    }
}

void Preflow_push::Relabel (int v){
    count[dist[v]]--;
    dist[v] = 2*size;
    for (int i = 0; i < (int)G[v].size(); i++) {
        Edge e = edges[G[v][i]];
        if (e.cap - e.flow > 0)
            dist[v] = min(dist[v], dist[e.to] + 1);
    }

    count[dist[v]]++;
    Enqueue(v);
}

void Preflow_push::Gap (int k){
    for (int v = 0; v < size; ++v) {
        if (dist[v] < k) continue;
        count[dist[v]]--;
        dist[v] = max(dist[v], size+1);
        count[dist[v]]++;
        Enqueue(v);
    }
}

void Preflow_push::Push(int id_edge, int from){
    int amt = min(excess[from], edges[id_edge].cap - edges[id_edge].flow);
    if (dist[from] <= dist[edges[id_edge].to] or amt == 0) return;
    edges[id_edge].flow += amt;
    edges[id_edge^1].flow -= amt;
    excess[edges[id_edge].to] += amt;    
    excess[from] -= amt;
    Enqueue(edges[id_edge].to);
}

void Preflow_push::Enqueue (int v){
    if (not active[v] and excess[v] > 0) { 
        active[v] = true; 
        Q.push(v); 
    } 
}

Preflow_push::Preflow_push(const Graph& g, const vector<Edge>& edges) {
    this->G = g;
    this->edges = edges;
    this->size = G.size();
    count.resize(size*2);
    dist.resize(size);
    active.resize(size);
    excess.resize(size,0);
}

int Preflow_push::get_maxflow (int s, int t){
    count[0] = size-1;
    count[size] = 1;
    dist[s] = size;
    active[s] = active[t] = true;

    for (int i = 0; i < (int)G[s].size(); i++) {
        excess[s] += edges[G[s][i]].cap;
        Push(G[s][i], s);
    }

    while (not Q.empty()) {
        int v = Q.front();
        Q.pop();
        active[v] = false;
        Discharge(v);
    }

    int maxflow = 0;
    for (int i = 0; i < (int)G[s].size(); ++i) 
        maxflow += edges[G[s][i]].flow;

    return maxflow;
}