///////////////////////////////////////////////////////////////////////////////////
//
// ONOMA: Γιώργος 
// ΕΠΩΝΥΜΟ: Κάνδηλας
// ΑΜ: 
// ΕΤΟΣ: 2ο
// ΑΣΚΗΣΗ ΕΡΓΑΣΤΗΡΙΟΥ 3
//
///////////////////////////////////////////////////////////////////////////////////
//
// ΣΧΟΛΙΑΣΜΟΣ ΛΥΣΗΣ
/*
	Για καλύτερο debugging και πιο εύκολο composition έχω χωρίσει τις συναρτήσεις Search και BinarySearch. Όλα είναι φτιαγμένα όσο γίνεται με templates για να είναι επαναχρησιμοποιήσημα. Η print() απλά μου εκτυπώνει όλο το vector πάλι για debugging λόγους.
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

	// Solve -> Call MergeSort and print before - after for debugging
	void Solve(void){
		this->print();
		numbers = MergeSort(numbers, 1, numbers.size());
		this->print();
	}


	// Search -> Call BinarySearch
	bool Search(T number){
			
		int index = this->BinarySearch(numbers, number, 0, numbers.size()-1);
        //cout << index;   //debugging
		if(index == -1) 
			return false;
		else
			return numbers[index] == number;	
	}
	
	int BinarySearch(vector<T>& L, T num, int imin, int imax){
		
		if (imin > imax) 
        	return -1;
		
		int imid = (imin+imax)/2;
		if(L[imid]<num){
			return	BinarySearch(L,num,imin, imid-1);
		}
		else if(L[imid]>num){
			return BinarySearch(L,num,imid+1,imax);
		}
		else
			return imid; 
	}



	//	MergeSort
	vector<T> MergeSort(vector<T>& L,int start,int end){
		if(start>=end){
			return vector<T>{L[start]};
		}
		
		int q = (end+start)/2;
		
		vector<T> A = MergeSort(L, start, q);
		vector<T> B = MergeSort(L, q+1, end);
		return Merge(A, B);
	}
	
	//	Merge
	vector<T> Merge(vector<T> A, vector<T> B){
		int i=0;
		int j=0;
		int n = A.size()-1;	
		int m = B.size()-1;		
		vector<T> Temp;
		
		while(i<=n && j<=m){
			if(A[i]>B[j]){
				Temp.push_back(A[i]);
				i++;
			}
			else{
				Temp.push_back(B[j]);
				j++;
			}
		}	
		if(i>n){
			for(;j<=m;j++){
				Temp.push_back(B[j]);
			}
		}
		else{
			for(;i<=n;i++){
				Temp.push_back(A[i]);
			}
		}
	return Temp;
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

};

int main(int argc, char** argv) {
	answer<int> myanswer;
	myanswer.ReadFile("data.txt");
	myanswer.Solve();
	
	cout << "\nSearch for a number: ";
	int number;
	cin >> number ;
	
	if(myanswer.Search(number)){
		cout << "\n FOUND! :)";
		return 0;
	}
	cout << "\n NOT FOUND :(";
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////
//
// ΑΠΟΤΕΛΕΜΣΤΑ ΕΚΤΕΛΕΣΗΣ
/*
Read 129 numbers -  The numbers are: -842 527 91 -374 639 -215 308 764 -951 122 473 -688 15 399 -560 831 -102 245 716 -389 587 -734 302 913 -176 447 -609 278 965 -511 -124 679 -342 190 823 -257 534 -901 112 460 -785 371 -698 254 803 -467 119 692 -333 540 271 -845 398 -179 726 -512 63 954 -221 387 -630 145 809 -274 532 -913 207 680 -459 311 874 -98 426 -761 195 642 -334 517 -208 733 -571 84 299 -420 658 -137 923 -286 471 105 -749 332 -610 188 795 -423 260 617 -95 842 173 -538 409 -312 586 -174 931 -267 354 720 -401 239 -689 514 87 -756 623 -322 468 191 -834 275 -599 340 782 -158 429 -691 57


-842 527 91 -374 639 -215 308 764 -951 122 473 -688 15 399 -560 831 -102 245 716 -389 587 -734 302 913 -176 447 -609 278 965 -511 -124 679 -342 190 823 -257 534 -901 112 460 -785 371 -698 254 803 -467 119 692 -333 540 271 -845 398 -179 726 -512 63 954 -221 387 -630 145 809 -274 532 -913 207 680 -459 311 874 -98 426 -761 195 642 -334 517 -208 733 -571 84 299 -420 658 -137 923 -286 471 105 -749 332 -610 188 795 -423 260 617 -95 842 173 -538 409 -312 586 -174 931 -267 354 720 -401 239 -689 514 87 -756 623 -322 468 191 -834 275 -599 340 782 -158 429 -691 57


965 954 931 923 913 874 842 831 823 809 803 795 782 764 733 726 720 716 692 680 679 658 642 639 623 617 587 586 540 534 532 527 517 514 473 471 468 460 447 429 426 409 399 398 387 371 354 340 332 311 308 302 299 278 275 271 260 254 245 239 207 195 191 190 188 173 145 122 119 112 105 91 87 84 63 57 15 0 -95 -98 -102 -124 -137 -158 -174 -176 -179 -208 -215 -221 -257 -267 -274 -286 -312 -322 -333 -334 -342 -374 -389 -401 -420 -423 -459 -467 -511 -512 -538 -560 -571 -599 -609 -610 -630 -688 -689 -691 -698 -734 -749 -756 -761 -785 -834 -845 -901 -913 -951


Search for a number: 119

 FOUND! :)
--------------------------------
Process exited after 2.949 seconds with return value 0
Press any key to continue . . .
*/