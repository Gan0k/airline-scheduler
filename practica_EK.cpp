#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <time.h>
#include <cstring>
using namespace std;

#define INFINITY numeric_limits<int>::max()

struct Edge {
    int to, cap, flow;
};

typedef vector<vector<int> > Graph;

bool v2_act = false;

/* Read and create the graph */

void add_edge (Graph& G, vector<Edge>& edges, int a, int b, int cap) {
    Edge e1 = {b, cap, 0};
    Edge e2 = {a, 0, 0};
    G[a].push_back((int) edges.size());
    edges.push_back(e1);
    G[b].push_back((int) edges.size());
    edges.push_back(e2);
}

Graph set_up_graph (vector<Edge>& edges, int& source, int& sink, 
                    int& edge_s, int& edge_t){

    vector<pair<int,int> > aux;
    int d, a, td, ta;
    while (cin >> d >> a >> td >> ta) {
        aux.push_back(pair<int,int>(d,td));
        aux.push_back(pair<int,int>(a,ta));
    }

    int size = aux.size();
    Graph ret (size + 4);
    source = size;
    sink = size+1; 
    int ori_src = size+2, ori_sink = size+3;

    for (int i = 0; i < size; ++i){
        if (i % 2 == 0){
            if (v2_act){
                add_edge(ret, edges, ori_src, i, INFINITY);
                add_edge(ret, edges, i, sink, 1);
                add_edge(ret, edges, i, i+1, INFINITY);
            }
            else {
                add_edge(ret, edges, ori_src, i, 1);
                add_edge(ret, edges, i, sink, 1);
            }
        }
        else {
            add_edge(ret, edges, source, i, 1);
            add_edge(ret, edges, i, ori_sink, 1);
            for (int j = 0; j < size; j += 2) {
                if (aux[j].first == aux[i].first and 
                    aux[j].second - aux[i].second >= 15){
                    if (v2_act) add_edge(ret, edges, i, j, INFINITY);
                    else add_edge(ret, edges, i, j, 1);
                }
            }
        }
    }

    edge_s = edges.size();
    add_edge(ret,edges, source, ori_src, 1);
    edge_t = edges.size();
    add_edge(ret,edges, ori_sink, sink, 1);

    return ret;
}


/* Edmonds Karp algorithm */

int bfs (const Graph& G, const vector<Edge>& edges, 
        vector<pair<int,int> >& parent, int start, int end) {

    int size = G.size();
    vector<int> path_capacity (size, 0);
    parent[start].first = -2;
    path_capacity[start] = INFINITY;

    queue<int> Q;
    Q.push(start);
     
    while (not Q.empty()) {
        int current = Q.front(); 
        Q.pop();
        int s = G[current].size();
        for (int i = 0; i < s; ++i) {
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

int edmonds_karp (const Graph& g, vector<Edge>& edges, int source, int sink) {
    int max_flow = 0;
    while (true) {
        vector<pair<int,int> > parent (g.size(), pair<int,int> (-1, -1));
        int flow = bfs(g,edges,parent,source,sink);
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


/* Find and print the result */

int calc_min_k (const Graph& g, vector<Edge>& edges, int s, int t, 
                int s_id, int t_id) {

    int maxflow = edmonds_karp(g, edges, s, t);

    while (maxflow != (int)g[s].size() - 1 + edges[t_id].cap){

        for (int i = 0; i < (int)edges.size(); ++i)
            edges[i].flow = 0;

        edges[s_id].cap++; 
        edges[t_id].cap++;
        maxflow = edmonds_karp(g,edges, s, t);
    }

    return edges[t_id].cap;
}

void print_paths (const Graph& g, vector<Edge>& edges, int s_id){
    int ori_src = edges[s_id].to;
    int k = edges[s_id].cap;
    int i = 0;
    vector<bool> pilots (g[ori_src].size());
    
    while (i < k){
        int j = 0;
        while (edges[g[ori_src][j]].flow <= 0) ++j;

        int current = edges[g[ori_src][j]].to;
        --edges[g[ori_src][j]].flow;

        while (current != (int)g.size()-1){
            if (not pilots[current/2]) 
                cout << (current / 2) + 1 << " ";

            pilots[current/2] = true;
            ++current;

            int h = 0;
            while (edges[g[current][h]].flow < 1) ++h;
            --edges[g[current][h]].flow;
            current = edges[g[current][h]].to;
        }
        cout << endl;
        ++i;
    }
}

int main (int argc, char **argv) {
    bool ti = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-t") == 0) ti = true;
        else if (strcmp(argv[i], "-v2") == 0) v2_act = true;
    }
    
    vector<Edge> edges;
    int s, t, edge_sk, edge_tk;
    Graph g = set_up_graph(edges,s,t, edge_sk, edge_tk);
    if (ti){
        const clock_t start = clock();
        calc_min_k(g,edges,s,t,edge_sk,edge_tk);
        cout << double(clock () - start) / CLOCKS_PER_SEC << endl;
    }
    else {
        cout << calc_min_k(g,edges,s,t,edge_sk,edge_tk) << endl;
        print_paths(g, edges, edge_sk);
    }
}
