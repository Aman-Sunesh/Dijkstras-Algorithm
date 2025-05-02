#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <algorithm>    // std::sort

const int Inf = 1<<30;


using std::cout;
using std::string;
using std::ostream;


struct Node
{
    string name;
    std::map<string, int> edges;
    bool on = false;

    // dijkstra data
    bool visited;
    int distance;
    std::vector<string> path;
};


ostream & operator<<(ostream & os, const Node & n)
{
    os << n.distance << " :";
    for ( auto i : n.path ) os << ' ' << i;
    os << " " << n.name;
    return os;
}



class Graph
{
        std::map<string, Node> m;
        string root;
        bool state = false;

    public:
        void setRoot(string r) { root = r; m[root]; }
        string getRoot() const { return root; }
        void addEdge(string a, string b, int dist)
        {
            if ( root.empty() ) root = a;
            m[a].edges[b] = dist;
            m[b].edges[a] = dist;
            m[a].name = a;
            m[b].name = b;
        }

        bool isState(bool on) { return on == state; }
        void flipState() { state = !state; }
        Node & node(string name) { return m[name]; }

        void dijkstra();
};

template<class T> void visit(Graph & g, T & f, string name)
{
    Node & n = g.node(name);
    f(n);

    n.on = !n.on;
    for ( auto snb : n.edges )
    {
        auto & nb = g.node(snb.first);
        if ( !g.isState(nb.on) ) continue;
        visit(g, f, snb.first);
    }
}
template<class T> void visit(Graph & g, T & f) { visit<T>(g, f, g.getRoot()); g.flipState(); }



template <class T, class U> void sortc(T & c, U & u) { std::sort(c.begin(), c.end(), u); }
template <class T> void print(Graph & g, T & c)
{ for ( auto i : c ) cout << g.node(i) << '\n'; cout << '\n'; }


int main()
{
    Graph g;  // undirected rooted graph

    {
        int d;
        string a, b;
        while (std::cin >> a >> b >> d && (a != "end" || b != "of" || d != -99)) g.addEdge(a, b, d);
    }

    std::vector<string> v;
    auto collectNames = [&v](const Node & n) { v.push_back(n.name); };
    ::visit(g, collectNames);

    auto order = [&g](string a, string b) -> bool
    {
        return g.node(a).distance < g.node(b).distance;
    };

    g.dijkstra();
    ::sortc(v, order);
    ::print(g, v);

    g.setRoot("Dubai");

    g.dijkstra();
    ::sortc(v, order);
    ::print(g, v);

    g.setRoot("Tokyo");

    g.dijkstra();
    ::sortc(v, order);
    ::print(g, v);
}

void Graph::dijkstra()
{
    for (auto& n : m)
    {
        Node& node = n.second;
        node.visited  = false;   // Mark all nodes as unvisited.
        node.path.clear();
    
        // Mark the initially selected node with the current distance of 0 and the rest with infinity.
        if (node.name == root)
        {
            node.distance = 0;
        }

        else
        {
            node.distance = Inf;
        }
    }

    // Set the initial node as the current node.
    string current = root;
    m[root].path = {};


    // (*) For the current node, consider all of its unvisited neighbors
    // and calculate their distances by adding the current distance of the current node
    // to the weight of the edge that connects the current node to the neighboring node.
    while (true)
    {
        for (const auto& [neighbour_name, weight] : m[current].edges)
        {
            Node& neighbour = m[neighbour_name];
    
            if (neighbour.visited == false)
            {
                int new_dist = m[current].distance + weight;
    
                if (new_dist < neighbour.distance)
                {
                    neighbour.distance = new_dist;
                    neighbour.path = m[current].path;
                    neighbour.path.push_back(current);
                }
            }
        }
    
        // When you're done considering all of the unvisited neighbors of the current node,
        // mark the current node as visited.
        m[current].visited = true;

        // Select the unvisited node that is marked with the smallest distance, set it as
        // the new current node, and go back to step (*)
        Node* min = nullptr;
        int min_dist = Inf;
    
        for (auto& n : m)
        {   
            Node& node = n.second;

            if (node.visited == false && node.distance < min_dist)
            {
                min_dist = node.distance;
                min = &node;
            }
        }

        if (!min)
        {
            break;
        }

        current = min->name;
    }    
}
