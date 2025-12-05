#include <bits/stdc++.h>

using namespace std;

class answer{
public:
	
	vector<int> nodeOutEdges;
	vector<int> graphOutEdges;
	vector<vector<int>> graph;
	
	void sortArrays(){
		for(int i=0;i<graph.size();i++){
			sort(graph[i].begin(), graph[i].end());
		}
	}
	
	void algorithm(){
		nodeOutEdges.push_back(-1);
		for(int i=1;i<graph.size();i++){
			nodeOutEdges.push_back(graphOutEdges.size());
			for(int j=0;j<graph[i].size();j++){
				graphOutEdges.push_back(graph[i][j]);
			}
		}
		
		nodeOutEdges[0] = graphOutEdges.size() - 1; 
	}
	
	void displaygraphOutEdges(){
		cout << "Graph Out Edges" << endl;
		for(int i=0;i<graphOutEdges.size();i++){
			cout << graphOutEdges[i] << " ";
		}
		cout << endl;
	}
	
	void displaynodeOutEdges(){
		cout << "Node Out Edges" << endl;
		for(int i=0;i<nodeOutEdges.size();i++){
			cout << nodeOutEdges[i] << " ";
		}
		cout << endl;
	}
	

	
	void ReadFile(string file) {
			ifstream inputFile(file.c_str());
	
			if (!inputFile.is_open()) {
				cerr << "Error: File '" << file << "' not found!" << endl;
				exit(1); 
			}
	
			int u, v;
			while (inputFile >> u >> v) {
				// Resize vector if we encounter a node index larger than current capacity
				if (u >= graph.size()) {
					graph.resize(u + 1);
				}
				graph[u].push_back(v);
			}
			inputFile.close();
		}
};


int main(){
	answer myanswer;
	myanswer.ReadFile("graph.txt");
	myanswer.sortArrays();
	myanswer.algorithm();
	myanswer.displaynodeOutEdges();
	myanswer.displaygraphOutEdges();
	
	return 0;
}