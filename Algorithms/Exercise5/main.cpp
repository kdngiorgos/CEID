///////////////////////////////////////////////////////////////////////////////////
//
// ONOMA: Giorgos
// ΕΠΩΝΥΜΟ:	Kandilas
// ΑΜ: 1115510
// ΕΤΟΣ: 2o
// ΑΣΚΗΣΗ ΕΡΓΑΣΤΗΡΙΟΥ 5
//
///////////////////////////////////////////////////////////////////////////////////
//
// ΣΧΟΛΙΑΣΜΟΣ ΛΥΣΗΣ
/*
	H Συνάρτηση search() παιρνει τις 2 κορυφές, ελέγχει αν ξεκινόντας από την πρώτη θα φτάσει στην δεύτερη, αν όχι επιστρέφει -1,
	αν ναι επιστρέφει το επίπεδο της δεύτερης κορυφής (με αρχή την 1η κορυφή). Η BFS λειτουργεί κανονικά όπως συνήθως.
*/
///////////////////////////////////////////////////////////////////////////////////
//
// ΚΩΔΙΚΑΣ ΛΥΣΗΣ

#include <iostream>
#include <list>
#include <iterator>
#include <queue>
#include <vector>

using namespace std;
void displayGraph(list<int> graph[], int v)  // display the Graph
{
   for(int i = 1; i<v; i++) {
      cout << i << "--->";
      list<int> :: iterator it;
      for(it = graph[i].begin(); it != graph[i].end(); ++it) {
         cout << *it << " ";
      }
      cout << endl;
   }
}

void insert_edge(list<int> graph[], int u, int v)  //add vertices (u v), (v u)
{    
   graph[u].push_back(v);
   graph[v].push_back(u);
}







	
const int v = 10;    //number of vertices in graph +1
//create an array of lists whose size is v
list<int> graph[v];




void BFS(int s,vector<bool>& Discovered, vector<int>& Parent, vector<int>& d){
	for(int i=0;i<10;i++){
		Discovered[i] = false;
		Parent[i] =0;
		d[i] =1000000;
	}
	queue<int> L;
	Discovered[s]=true;
	L.push(s); d[s]=0;
	while(!(L.empty())){
		int u= L.front();
		list<int> :: iterator it;
		for(it = graph[u].begin(); it != graph[u].end(); ++it) {
        	if(Discovered[*it]==false){
        		Discovered[*it]=true;
        		d[*it] = d[u] + 1;
        		Parent[*it] =u;
        		L.push(*it);
			}
    	}
    	L.pop();
	}
}


int search(int u,int v,vector<bool>& Discovered, vector<int>& Parent, vector<int>& d){
	BFS(u,Discovered,Parent,d);
	if(Discovered[v]){
		return d[v];}
	else{
		return -1;
	}
	
	
}





int main() 
{
	vector<bool> Discovered(10);
	vector<int> Parent(10);
	vector<int> d(10);
	
	insert_edge(graph, 1, 2);
	insert_edge(graph, 1, 3);

	insert_edge(graph, 2, 4);
	insert_edge(graph, 2, 5);

	insert_edge(graph, 3, 6);

	insert_edge(graph, 4, 7);

	insert_edge(graph, 5, 8);

	insert_edge(graph, 6, 9);

	displayGraph(graph, v);
   
   	int a, b;
   	cout << "Give 2 nodes" << endl;
   	cin >> a >> b;
   	a = search(a, b,Discovered,Parent,d);
   	if(a==-1){
   		cout << "Not discovered";
	   }
	else{
   		cout << "Level " << a;}
   	
   
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//
// ΑΠΟΤΕΛΕΜΣΤΑ ΕΚΤΕΛΕΣΗΣ
/*
1--->2 3
2--->1 4 5
3--->1 6
4--->2 7
5--->2 8
6--->3 9
7--->4
8--->5
9--->6
Give 2 nodes
1 9
Level 3


*/


