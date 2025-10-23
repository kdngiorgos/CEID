///////////////////////////////////////////////////////////////////////////////////
//
// ONOMA: ΓΙΩΡΓΟΣ
// ΕΠΩΝΥΜΟ: ΚΑΝΔΗΛΑΣ
// ΑΜ: 1115510
// ΕΤΟΣ: 2025 (2ο)
// ΑΣΚΗΣΗ ΕΡΓΑΣΤΗΡΙΟΥ 1 
//
///////////////////////////////////////////////////////////////////////////////////
//
// ΣΧΟΛΙΑΣΜΟΣ ΛΥΣΗΣ
/*
max = Μέγιστος Άρτιος
min = Ελάχιστος Περιττός 
imax = η θέση του max
imin = η θέση του min

Η Συνάρτηση ReadFile καταλαβαίνετε τι κάνει όπως και η WriteFile. η Algo() είναι αυτή που μας ενδιαφέρει. 
Αρχικά παίρνουμε 2 integers που είναι -1 και οι δύο και οι 2 πρώτοι που θα συναντήσει
θα γίνουν μέγιστος και ελάχιστος αντίστοιχα (ελέχγοντας άν έχει βρεθεί άλλος ελάχιστος με το imax). 
Τέλος κάνει swap τις τιμές με μία τοπική συνάρτηση της κλάσσης.
*/
///////////////////////////////////////////////////////////////////////////////////
//


// ΚΩΔΙΚΑΣ ΛΥΣΗΣ

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits.h>

using namespace std;

class answer{
    vector<int> numbers;
    int numbersRead=0;
	int temp; 
	
    public:
    void ReadFile(string file){
        int current_number;
        
        ifstream inputFile = ifstream(file);
        
        if(inputFile.good()){
		    while(inputFile >> current_number)
		    	numbers.push_back(current_number);
			inputFile.close();
			numbersRead=numbers.size();
			
            cout << "Read " << numbersRead << " numbers - " << " The numbers are: ";
            
            for(int icount = 0; icount < numbersRead; icount++){ 
                cout << numbers[icount] << " ";
            } 
        }
    }
	
	
	void WriteFile(){
		ofstream outputfile;
		outputfile.open("output.txt"); 
		for(int icount = 0; icount < numbersRead; icount++){ 
			outputfile << numbers[icount] << " ";
			} 
		outputfile.close();
	}

	void Algo(){
		int imin, imax, max=-1;
		int min;
		
		for(int i=0;i<numbersRead;i++){
			temp = numbers[i]; 
			if(temp%2==0 && temp>max){
				max = temp;
				imax=i;
				}
				else if(temp%2==1 && (temp<min || imin==-1)){
					min = temp;
					imin=i;
				}
		}
		
		cout << "\n\nMegistos artios: " << max << " stin 8esi: " << imax+1 << " ---  Elaxistos perittos: " << min << " stin 8esi: "<< imin+1;
		
		
		if(imax==-1){cout<<"\n Den bre8ike megaliteros artios";}
		else{this->swap(&numbers[0],&numbers[imax]);}
		
		if(imin==-1){cout <<"\n Den bre8ike elaxistos perittos";}
		else{this->swap(&numbers[1],&numbers[imin]);}
		
	}
	
	void swap(int* a, int* b){
		temp = *a; 
		*a = *b;
		*b = temp;
	}

};


main ()
{
answer myanswer;
myanswer.ReadFile("input.txt");
myanswer.Algo();
myanswer.WriteFile();
}






////////////////////////////////////////////////////////////////////////////////////
//
// ΑΠΟΤΕΛΕΜΣΤΑ ΕΚΤΕΛΕΣΗΣ
/*
Read 120 numbers -  The numbers are: 45 82 17 94 33 58 71 26 89 12 65 38 91 54 27 76 41 68 15 92 37 84 23 56 79 14 63 98 31 72 49 86 21 64 39 78 13 52 95 28 67 44 81 18 53 96 35 74 29 88 11 62 47 90 25 70 43 85 19 60 51 93 36 77 22 66 99 48 83 16 59 42 87 24 69 34 80 57 97 30 73 46 61 20 75 50 55 32 40 10 55 100 45 88 33 76 21 64 39 50 27 92 18 85 41 68 13 96 25 72 49 81 37 70 15 58 43 90 31 66

Megistos artios: 100 stin 8esi: 92 ---  Elaxistos perittos: 11 stin 8esi: 51
*/