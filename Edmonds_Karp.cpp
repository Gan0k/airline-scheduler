#include "Edmonds_Karp.hpp"

int Edmonds_Karp::bfs (vector<pair<int,int> >& parent, int start, int end){
    vector<int> path_capacity (size, 0);
    parent[start].first = -2;
    path_capacity[start] = INFINITY;

    queue<int> Q;
    Q.push(start);
     
    while (not Q.empty()) {
        int current = Q.front(); 
        Q.pop();

        for (int i = 0; i < (int)G[current].size(); ++i) {
            Edge e = edges[G[current][i]];
            if (parent[e.to].first == -1 and e.cap - e.flow > 0) {
                parent[e.to].first = current;
                parent[e.to].second = G[current][i];
                path_capacity[e.to] = min(path_capacity[current], e.cap - e.flow);
                if (e.to == end) return path_capacity[end];
                Q.push(e.to);
            }
        }
    }
    return 0;
}

Edmonds_Karp::Edmonds_Karp (const Graph& g, const vector<Edge>& edges){
    this->G = g;
    this->edges = edges;
    this->size = g.size();
}

int Edmonds_Karp::get_maxflow (int source, int sink){
    int max_flow = 0;
    while (true) {
        vector<pair<int,int> > parent (size, pair<int,int> (-1, -1));
        int flow = bfs(parent,source,sink);
        if (flow > 0) {
            max_flow += flow;
            int current = sink;
            while (current != source){
                int previous = parent[current].first;
                int id_edge = parent[current].second;
                edges[id_edge].flow += flow;
                edges[id_edge^1].flow -= flow;
                current = previous;
            }
        }
        else break;
    }
    return max_flow;
}
