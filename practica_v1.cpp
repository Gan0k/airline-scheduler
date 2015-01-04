#include <iostream>
#include <time.h>
#include "Preflow_push.hpp"
#include "Edmonds_Karp.hpp"
using namespace std;

void add_edge (Graph& G, vector<Edge>& edges, int a, int b, int cap) {
    Edge e1 = {b, cap, 0};
    Edge e2 = {a, 0, 0};
    G[a].push_back((int) edges.size());
    edges.push_back(e1);
    G[b].push_back((int) edges.size());
    edges.push_back(e2);
}

Graph set_up_graph (vector<Edge>& edges, int& source, int& sink){
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

    add_edge(ret,edges, source, ori_src, 1);
    add_edge(ret,edges, ori_sink, sink, 1);

    return ret;
}

int calc_min_k (const Graph& g, vector<Edge>& edges, int s, int t) {
    //Edmonds_Karp ek (g, edges);
    //int maxflow = ek.get_maxflow(s, t);
    Preflow_push pf (g,edges);
    int maxflow = pf.get_maxflow(s,t);
    int size = edges.size();

    while (maxflow != (int)g[s].size() - 1 + edges[size-2].cap){
        edges[size-4].cap++; //source
        edges[size-2].cap++; //sink
        Preflow_push pf (g,edges);
        maxflow = pf.get_maxflow(s,t);
    }
    return edges[size-2].cap;
}

int main() {
    vector<Edge> edges;
    int s, t;
    Graph g = set_up_graph(edges,s,t);
    const clock_t start = clock();
    cout << calc_min_k(g,edges,s,t) << " ";
    cout << double(clock () - start) / CLOCKS_PER_SEC << endl;
}
