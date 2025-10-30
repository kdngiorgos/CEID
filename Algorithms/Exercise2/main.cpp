///////////////////////////////////////////////////////////////////////////////////
//
// ONOMA: Γιώργος 
// ΕΠΩΝΥΜΟ: Κάνδηλας
// ΑΜ: 
// ΕΤΟΣ: 2ο
// ΑΣΚΗΣΗ ΕΡΓΑΣΤΗΡΙΟΥ 2
//
///////////////////////////////////////////////////////////////////////////////////
//
// ΣΧΟΛΙΑΣΜΟΣ ΛΥΣΗΣ
/*
	Έχω φτιάξει τις max() και swap() οι οποίες παίρνουν ως όρισμα το index των στοιχείων και συγκρίνουν ή αλλάζουν τις τιμές τους όμως αντίστοιχα. Η print() απλά εκτυπώνει το vector, 
	έχω χρησιμοποιήσει για ονόματα μεταβλητών numbers αλλά είναι Template. Η AddNumber απλά βάζει το νούμερο στο τέλος της σωρού και έπειτα καλεί της HeapifyUp() και η ExtractRoot() κάνει swap την ρίζα
	με το τελευταίο στοιχείο του πίνακα, διαγράφει το τελευταίο στοιχείο (δλδ την ρίζα) και έπειτα κάνει HeapifyDown(). Η ReplaceRoot() απλά προσθέτει ένα στοιχείο στο τέλος του vector και έπειτα καλεί
	την ExtractRoot()


*/
///////////////////////////////////////////////////////////////////////////////////
//
// ΚΩΔΙΚΑΣ ΛΥΣΗΣ

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;


template <typename T>
class answer{
	public:
	
	vector<T> numbers;

	
	void ReplaceRoot(T num){
		cout << "\n\n -- Changing Root with:" << num;
		numbers.push_back(num);
		this->ExtractRoot();
	}

	T ExtractRoot(void){
		cout << "\n\n - Extracting root: \n";
		this->print();
		T root = this->at(1);
		swap(1, numbers.size());
		numbers.pop_back();
		this->HeapifyDown(1);
		cout << "\n";
		cout << "Final form: ";
		this->print();
		return root;
	}
	
	void AddNumber(T num){
		cout << "\n\n - Adding number:" << num <<" \n";
		numbers.push_back(num);
		this->print();
		this->HeapifyUp(numbers.size());
		cout << "\n";
		cout << "Final form: ";
		this->print();
	}
	
	


	void HeapifyUp(int i){
		int j = i/2; 
		
		if(i<2){
			return;
		}
		else if(this->at(i)>this->at(j)){
			swap(i, j);
			this->HeapifyUp(j);
		}
	}
	
	
	void HeapifyDown(int i) {
	    int n = numbers.size();
	    int left = 2 * i;
	    int right = 2 * i + 1;
	    int j;
	
	    // no children → stop
	    if (left > n)
	        return;
	
	    // only left child
	    if (right > n)
	        j = left;
	    else
	        j = max(left, right);  // uses your custom max()
	
		//this->print();
	    if (this->at(j) > this->at(i)) {
	        swap(j, i);
	        this->HeapifyDown(j);
    	}

	}
	
	
	
	
	void ReadFile(string file){
        int current_number;
        
        ifstream inputFile = ifstream(file);
        
        if(inputFile.good()){
		    while(inputFile >> current_number)
		    	numbers.push_back(current_number);
			inputFile.close();
			
            cout << "Read " << numbers.size() << " numbers - " << " The numbers are: ";
            
            for(int icount = 0; icount < numbers.size(); icount++){ 
                cout << numbers[icount] << " ";
            }   
            
            cout << "\n\n";
        }
    }
    
    void print(void){
    	for(int icount = 0; icount < numbers.size(); icount++){ 
                cout << numbers[icount] << " ";
        } 
        
        cout << "\n\n";
	}
    
    void swap(int a, int b){
    	cout << "swapping numbers at indexes " << a << " and " << b << "\t" ;
    	this->print();
		T temp; 
    	temp = this->at(a);
    	numbers[a-1] = this->at(b);
    	numbers[b-1] = temp;
    	
	}
    
    T max(T a, T b){
    	if(this->at(a) > this->at(b)){
    		return a;
		}
		return b;
	}
    
    T& at(int i) {
		if (i <= 0 || i > numbers.size()) {
        	throw out_of_range("Index out of range (1-based access)");
    	}
	return numbers[i - 1]; // shift by one
    }
};

int main(int argc, char** argv) {
	answer<int> myanswer;
	myanswer.ReadFile("Heap.txt");
	
	myanswer.ExtractRoot();
	myanswer.AddNumber(100);
	myanswer.ReplaceRoot(10);
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//
// ΑΠΟΤΕΛΕΜΣΤΑ ΕΚΤΕΛΕΣΗΣ
/*
Read 15 numbers -  The numbers are: 90 85 75 70 80 65 60 50 45 55 70 40 30 25 20



 - Extracting root:
90 85 75 70 80 65 60 50 45 55 70 40 30 25 20

swapping numbers at indexes 1 and 15    90 85 75 70 80 65 60 50 45 55 70 40 30 25 20

swapping numbers at indexes 2 and 1     20 85 75 70 80 65 60 50 45 55 70 40 30 25

swapping numbers at indexes 5 and 2     85 20 75 70 80 65 60 50 45 55 70 40 30 25

swapping numbers at indexes 11 and 5    85 80 75 70 20 65 60 50 45 55 70 40 30 25


Final form: 85 80 75 70 70 65 60 50 45 55 20 40 30 25



 - Adding number:100
85 80 75 70 70 65 60 50 45 55 20 40 30 25 100

swapping numbers at indexes 15 and 7    85 80 75 70 70 65 60 50 45 55 20 40 30 25 100

swapping numbers at indexes 7 and 3     85 80 75 70 70 65 100 50 45 55 20 40 30 25 60

swapping numbers at indexes 3 and 1     85 80 100 70 70 65 75 50 45 55 20 40 30 25 60


Final form: 100 80 85 70 70 65 75 50 45 55 20 40 30 25 60



 -- Changing Root with:10

 - Extracting root:
100 80 85 70 70 65 75 50 45 55 20 40 30 25 60 10

swapping numbers at indexes 1 and 16    100 80 85 70 70 65 75 50 45 55 20 40 30 25 60 10

swapping numbers at indexes 3 and 1     10 80 85 70 70 65 75 50 45 55 20 40 30 25 60

swapping numbers at indexes 7 and 3     85 80 10 70 70 65 75 50 45 55 20 40 30 25 60

swapping numbers at indexes 15 and 7    85 80 75 70 70 65 10 50 45 55 20 40 30 25 60


Final form: 85 80 75 70 70 65 60 50 45 55 20 40 30 25 10

*/