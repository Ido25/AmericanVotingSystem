#pragma once

#include <iostream>
#include <typeinfo>
#include <fstream>
#include "headers/Round.h"
#include "headers/RegularRound.h"
#include "headers/SimpleRound.h"
#include "headers/UnitedDistrict.h"
#include "headers/DividedDistrict.h"

using namespace election;

class Menu{
  private:
	Round *election;
	bool isValidDistrict(int idx);
	bool keepGo;
	constexpr static int amountMenu_Options = 12;
	void saveElection(const string f_name = "");
	inline void mainMenuDisplay(){
		
		cout << "1.   Add a district " << endl
			 << "2.   Add a citizen" << endl
			 << "3.   Add a party " << endl
			 << "4.   Add a party member" << endl
			 << "5.   Display all districts" << endl
			 << "6.   Display all citizens" << endl
			 << "7.   Display all parties" << endl
			 << "8.   Vote" << endl
			 << "9.   Display election results " << endl
			 << "10.  Save election " << endl
			 << "11.  Load election " << endl
			 << "12.  Exit" << endl;
	}
  
  public:
	void loadElection(const string = "");
	Menu() = delete;
	Menu(const string f_name);
	Menu(int _y, int _m, int _d, int round_type, int amount_of_reps = 0);
	~Menu(){ delete election; }
	void mainMenu();
	int userInput();
	void activateProcess(int input);
	void addDistrict();
	void addCitizen();
	void addParty();
	void addPartyMember();
	void displayAllDistricts();
	void displayAllCitizens();
	void displayAllParties();
	void vote();
	void displayResults();
	void checkDate(int year, int month, int days);
	enum MenuOptions{
		ADD_DISTRICT = 1, ADD_CITIZEN, ADD_PARTY, ADD_PARTY_MEMBER,
		DISPLAY_DISTRICTS, DISPLAY_CITIZENS, DISPLAY_PARTIES, DO_VOTE,
		DISPLAY_RESULTS, SAVE_ELECTION, LOAD_ELECTION, EXIT_ELECTION
	};
	// TODO : remove before submission.
	void special(char str[] = nullptr);
};