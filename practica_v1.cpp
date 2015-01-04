#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <time.h>
using namespace std;

#define INFINITY numeric_limits<int>::max()

struct Edge {
    int to, cap, flow;
};

typedef vector<vector<int> > Graph;

void add_edge (Graph& G, vector<Edge>& edges, int a, int b, int cap) {
    Edge e1 = {b, cap, 0};
    Edge e2 = {a, 0, 0};
    G[a].push_back((int) edges.size());
    edges.push_back(e1);
    G[b].push_back((int) edges.size());
    edges.push_back(e2);
}

Graph set_up_graph (vector<Edge>& edges, int& source, int& sink, int& edge_s, int& edge_t){
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
            add_edge(ret, edges, ori_src, i, 1);
            add_edge(ret, edges, i, sink, 1);
        }
        else {
            add_edge(ret, edges, source, i, 1);
            add_edge(ret, edges, i, ori_sink, 1);
            for (int j = 0; j < size; j += 2) {
                if (aux[j].first == aux[i].first and 
                    aux[j].second - aux[i].second >= 15){
                    add_edge(ret, edges, i, j, 1);
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

int edmonds_karp (const Graph& g, vector<Edge> edges, int source, int sink) {
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

int push_relabel (const Graph& G, vector<Edge> edges, int s, int t) {
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

int calc_min_k (const Graph& g, vector<Edge>& edges, int s, int t, int s_id, int t_id) {
    int maxflow = push_relabel(g, edges, s, t);

    while (maxflow != (int)g[s].size() - 1 + edges[t_id].cap){
        //the edges where the last ones pushed
        edges[s_id].cap++; //source
        edges[t_id].cap++; //sink
        maxflow = push_relabel(g,edges, s, t);
    }
    return edges[t_id].cap;
}

int main () {
    vector<Edge> edges;
    int s, t, edge_sk, edge_tk;
    Graph g = set_up_graph(edges,s,t, edge_sk, edge_tk);
    const clock_t start = clock();
    cout << calc_min_k(g,edges,s,t,edge_sk,edge_tk) << " ";
    cout << double(clock () - start) / CLOCKS_PER_SEC << endl;
}
