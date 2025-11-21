#include <iostream>
#include <list>
#include <iterator>
#include <vector>
#include <algorithm>
using namespace std;

vector<pair<int, int>> treeEdges;
vector<pair<int, int>> backEdges;
vector<pair<int, int>> forwardEdges;
vector<pair<int, int>> crossEdges;

void displayGraph(list<int> graph[], int v) {
    for (int i = 1; i < v; i++) { // start from 1 if it's 1-based
        cout << i << " ---> ";
        for (auto it = graph[i].begin(); it != graph[i].end(); ++it) {
            cout << *it << " ";
        }
        cout << endl;
    }
}

void insert_edge(list<int> graph[], int u, int v) {
    graph[u].push_back(v);  // Add edge u -> v
}

void printEdges(string name, vector<pair<int,int>> &edges) {
    cout << name << endl;
    for(auto p : edges) {
        cout << p.first << " -> " << p.second << endl;
    }
}

class answer{
	int timer =0;

public:	
	void DFS(int u,list<int> graph[], vector<int>& discover, vector<int>& finish){
		discover[u] = ++timer;
		vector<int> neighbors(graph[u].begin(), graph[u].end());
		sort(neighbors.begin(),neighbors.end());
		
		for(int v: neighbors){
			
			if(discover[v]==0){
				// v is child to u
				treeEdges.push_back({u,v});
				DFS(v,graph,discover,finish);
			}
			else{
				if (finish[v]==0){
					backEdges.push_back({u,v});
				}
				else{
					if(discover[u]<discover[v]){
						forwardEdges.push_back({u,v}); 
					}
					else{
						crossEdges.push_back({u,v});
					}
				}
			}
		}
		
		finish[u]=++timer;
	}
};



int main(int argc, char *argv[]) {
    int v = 10; // Number of vertices (+1 if 1-based indexing)

    // Create an adjacency list representation of the graph
    list<int> graph[v];
	vector<bool> visited[v];
	
	vector<int> discover(v, 0);
    vector<int> finish(v, 0);
	
    // Insert edges
insert_edge(graph, 1, 5);
insert_edge(graph, 1, 9);
insert_edge(graph, 1, 2);
insert_edge(graph, 1, 3);

insert_edge(graph, 2, 1);
insert_edge(graph, 2, 6);
insert_edge(graph, 2, 4);

insert_edge(graph, 3, 7);
insert_edge(graph, 3, 5);
insert_edge(graph, 3, 2);
insert_edge(graph, 3, 8);

insert_edge(graph, 4, 9);
insert_edge(graph, 4, 3);
insert_edge(graph, 4, 1);

insert_edge(graph, 5, 2);
insert_edge(graph, 5, 6);
insert_edge(graph, 5, 4);
insert_edge(graph, 5, 8);

insert_edge(graph, 6, 3);
insert_edge(graph, 6, 5);

insert_edge(graph, 7, 1);
insert_edge(graph, 7, 9);
insert_edge(graph, 7, 8);
insert_edge(graph, 7, 6);

insert_edge(graph, 8, 5);
insert_edge(graph, 8, 4);
insert_edge(graph, 8, 7);

insert_edge(graph, 9, 2);
insert_edge(graph, 9, 8);
insert_edge(graph, 9, 6);
insert_edge(graph, 9, 7);
		
    // Display the graph
    displayGraph(graph, v);
	
	cout << "\n\n";
	answer myanswer;
	myanswer.DFS(1, graph, discover, finish);
	
	printEdges("Tree Edges", treeEdges);
    printEdges("Forward Edges", forwardEdges);
    printEdges("Back Edges", backEdges);
    printEdges("Cross Edges", crossEdges);

    return 0;
}
/* answer
Tree Edges
1 2
2 4
4 3
3 5
5 6
5 8
8 7
7 9
Forward Edges
1 3
1 5
1 9
2 6
3 7
3 8
4 9
Back Edges
2 1
3 2
4 1
5 2
5 4
6 3
6 5
7 1
7 8
8 4
8 5
9 2
9 7
9 8
Cross Edges
7 6
9 6 */