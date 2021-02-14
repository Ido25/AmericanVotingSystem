#include <iostream>
#include "Menu.h"

void kill(){
	
	cout << endl << "There was an error." << endl;
}
int main(){
	
	set_terminate(kill);
	
	try{
		Menu elec(1950, 1, 1, REGULAR_ROUND);
		elec.special("tests/test5/1/");
		elec.special("tests/test5/2/");
		elec.mainMenu();
	}
	catch(exception &msg){
		cout << "Main error : " << msg.what() << endl;
		exit(1);
	}
	
	int input;
	
	cout << "Hello and welcome to a very anonymous country's election." << endl
		 << "Please choose one of the following options: " << endl
		 << "1. Create a new round" << endl
		 << "2. Load an old round" << endl
		 << "3. exit" << endl;
	
	cin >> input;
	
	if(input == 1){
		int y, m, d, roundType, amount_of_reps = 0;
		
		cout << "Wonderful choice. Let's start the process by adding the election's date. " << endl
			 << "Please enter the date in the standard format (day month year)" << endl;
		cin >> d >> m >> y;
		
		cout << endl << "Please enter round type: " << endl << "1. Regular Round" << endl << "2. Simple Round" << endl;
		cin >> roundType;
		while(roundType != REGULAR_ROUND && roundType != SIMPLE_ROUND){
			cout << endl << "Please enter a valid round type: " << endl << "1. Regular Round" << endl
				 << "2. Simple Round" << endl;
			cin >> roundType;
		}
		
		if(roundType == SIMPLE_ROUND){
			cout << "Please enter amount of electors: " << endl;
			cin >> amount_of_reps;
			
			while(amount_of_reps < 0){
				cout << endl << "District's amount of electors must not be negative." << endl;
				cout << "Please enter amount of electors: " << endl;
				cin >> amount_of_reps;
			}
		}
		
		Menu electionRound(y, m, d, roundType, amount_of_reps);
		electionRound.mainMenu();
	}
	else if(input == 2){
		string file_name;
		getline(cin, file_name);
		cout << "That's my favorite choice choice. Let's start the process by giving me the file name. " << endl;
		cin >> file_name;
		
		Menu electionRound(file_name);
		electionRound.mainMenu();
	}
	else{
		if(input == 3){
			cout << "So soon?" << endl;
		}
		else{
			cout << "Invalid input" << endl;
		}
	}

	return 0;
}
