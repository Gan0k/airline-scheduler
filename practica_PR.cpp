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


/* Preflow push algorithm*/

void Enqueue (const vector<int>& excess, vector<bool>& active, 
              queue<int>& Q, int v) { 

    if (not active[v] and excess[v] > 0) { 
        active[v] = true; 
        Q.push(v); 
    } 
}

void Push (const Graph& G, vector<Edge>& edges, int id_edge, int from, 
           vector<int>& excess, const vector<int>& dist, vector<bool>& active, 
           queue<int>& Q) {

    int amt = min(excess[from], edges[id_edge].cap - edges[id_edge].flow);
    if (dist[from] <= dist[edges[id_edge].to] or amt == 0) return;
    edges[id_edge].flow += amt;
    edges[id_edge^1].flow -= amt;
    excess[edges[id_edge].to] += amt;    
    excess[from] -= amt;
    Enqueue(excess, active, Q, edges[id_edge].to);
}

void Gap (vector<int>& dist, vector<int>& count, const vector<int>& excess, 
          vector<bool>& active, queue<int>& Q, int k, int size) {

    for (int v = 0; v < size; ++v) {
        if (dist[v] < k) continue;
        count[dist[v]]--;
        dist[v] = max(dist[v], size+1);
        count[dist[v]]++;
        Enqueue(excess, active, Q, v);
    }
}

void Relabel (const Graph& G, vector<Edge>& edges, vector<int>& dist, 
             vector<int>& count, const vector<int>& excess, vector<bool>& active, 
             queue<int>& Q, int v, int size) {
    
    count[dist[v]]--;
    dist[v] = 2*size;
    for (int i = 0; i < (int)G[v].size(); i++) {
        Edge e = edges[G[v][i]];
        if (e.cap - e.flow > 0)
            dist[v] = min(dist[v], dist[e.to] + 1);
    }

    count[dist[v]]++;
    Enqueue(excess, active, Q, v);
}

void Discharge (const Graph& G, vector<Edge>& edges, vector<int>& dist, 
                vector<int>& count, vector<int>& excess, vector<bool>& active, 
                queue<int>& Q, int v, int size) {

    for (int i = 0; excess[v] > 0 and i < (int)G[v].size(); ++i) 
        Push(G, edges, G[v][i], v, excess, dist, active, Q);

    if (excess[v] > 0) {
        if (count[dist[v]] == 1) Gap(dist, count, excess, active, Q, dist[v], size); 
        else Relabel(G, edges, dist, count, excess, active, Q, v, size);
    }
}

int push_relabel (const Graph& G, vector<Edge>& edges, int s, int t) {
    int size = G.size();
    vector<int> count (size*2);
    count[0] = size-1;
    count[size] = 1;
    vector<int> dist (size);
    dist[s] = size;
    vector<bool> active (size);
    active[s] = active[t] = true;
    queue<int> Q;
    vector<int> excess (size,0);

    for (int i = 0; i < (int)G[s].size(); ++i) {
        excess[s] += edges[G[s][i]].cap;
        Push(G, edges, G[s][i], s, excess, dist, active, Q);
    }

    while (not Q.empty()) {
        int v = Q.front();
        Q.pop();
        active[v] = false;
        Discharge(G, edges, dist, count, excess, active, Q, v, size);
    }

    int maxflow = 0;
    for (int i = 0; i < (int)G[s].size(); ++i) 
        maxflow += edges[G[s][i]].flow;

    return maxflow;
}


/* Find and print the result */

int calc_min_k (const Graph& g, vector<Edge>& edges, int s, int t, 
                int s_id, int t_id) {

    int maxflow = push_relabel(g, edges, s, t);

    while (maxflow != (int)g[s].size() - 1 + edges[t_id].cap){

        for (int i = 0; i < (int)edges.size(); ++i)
            edges[i].flow = 0;

        edges[s_id].cap++; 
        edges[t_id].cap++;
        maxflow = push_relabel(g,edges, s, t);
    }

    return edges[t_id].cap;
}

void print_paths (const Graph& g, const vector<Edge>& edges, int s_id){
    int ori_src = edges[s_id].to;
    int k = edges[s_id].cap;
    int i = 0, j = 0;
    while (i < k){
        if (edges[g[ori_src][j]].flow > 0){
            int current = edges[g[ori_src][j]].to;
        
            while (current != (int)g.size()-1){
                cout << (current / 2) +1 << " ";
                ++current;

                int h = 0;
                while (edges[g[current][h]].flow < 1) ++h;
                current = edges[g[current][h]].to;
            }
            cout << endl;
            ++i;
        }
        ++j;
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

