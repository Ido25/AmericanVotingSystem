#include "Menu.h"

Menu::Menu(int _y, int _m, int _d, int round_type, int amount_of_reps) : election(nullptr){
	
	this->keepGo = true;
	
	try{
		checkDate(_y, _m, _d);
		
		if(round_type == REGULAR_ROUND)
			this->election = new RegularRound(_y, _m, _d);
		else
			this->election = new SimpleRound(_y, _m, _d, amount_of_reps);
	} catch(bad_alloc &ex){
		cout << "There was a problem allocating memory, the program will shut down now" << endl;
		exit(1);
	} catch(exception &msg){
		cout << "Error: " << msg.what() << endl;
		exit(1);
	}
}
int Menu::userInput(){
	
	int input;
	
	cout << endl << "Please enter the number of your selection from the options down below:" << endl;
	this->mainMenuDisplay();
	
	cin >> input;
	
	while((!((1 <= input) && (input <= this->amountMenu_Options)))){
		cout << "Invalid input, please enter an integer from the options above" << endl;
		cin.clear();
		cin >> input;
	}
	
	return input;
}
void Menu::mainMenu(){
	
	while(this->keepGo){
		int input = this->userInput();
		this->activateProcess(input);
	}
}
void Menu::activateProcess(int input){
	
	try{
		if(input == ADD_DISTRICT){
			this->addDistrict();
		}
		else if(input == ADD_CITIZEN){
			this->addCitizen();
		}
		else if(input == ADD_PARTY){
			this->addParty();
		}
		else if(input == ADD_PARTY_MEMBER){
			this->addPartyMember();
		}
		else if(input == DISPLAY_DISTRICTS){
			this->displayAllDistricts();
		}
		else if(input == DISPLAY_CITIZENS){
			this->displayAllCitizens();
		}
		else if(input == DISPLAY_PARTIES){
			this->displayAllParties();
		}
		else if(input == DO_VOTE){
			this->vote();
		}
		else if(input == DISPLAY_RESULTS){
			this->displayResults();
		}
		else if(input == LOAD_ELECTION){
			this->loadElection();
		}
		else if(input == SAVE_ELECTION){
			this->saveElection();
		}
		else if(input == EXIT_ELECTION){
			cout << "Thank you for participating" <<
				 endl;
			this->keepGo = false;
			exit(1);
		}
	} catch(bad_alloc &ex){//catching bad_alloc from all the processes
		cout << "Error: " << "failed to allocate memory" << endl;
	} catch(exception &err){//catching messages from all the processes
		cout << "Error: " << err.what() << endl;
	} catch(string &msg){
		cout << "Error: " << msg << endl;
	}
}
void Menu::addDistrict(){
	
	if(typeid(*(this->election)) == typeid(SimpleRound)){
		throw std::exception("there are no districts in a simple election");
	}
	string dstName;
	int numOfReps, type;
	
	cin.ignore();
	cout << "Please enter the district's name:" << endl;
	getline(cin, dstName);
	
	cout << "Please enter the number of representors of the district:" << endl;
	cin >> numOfReps;
	if(numOfReps < 0){
		throw std::exception("District's amount of electors must be greater than 0.");
	}
	
	cout << "Please enter the type of district: 1 - United, 2 - Divided" << endl;
	cin >> type;
	
	bool isUnited;
	if(type == 1)
		isUnited = true;
	else if(type == 2)
		isUnited = false;
	else{
		throw std::exception("There is no such district type.");
	}
	
	this->election->addDistrict(dstName, numOfReps, isUnited);
}
void Menu::addCitizen(){
	
	string citizenName, citizenId;
	int birthYear, districtId;
	
	cin.ignore();
	cout << endl << "Please enter the citizen's name:" << endl;
	getline(cin, citizenName);
	
	cout << "Please enter the citizen's ID:" << endl;
	getline(cin, citizenId);
	
	cout << "Please enter the citizen's birth Year:" << endl;
	cin >> birthYear;
	
	if(typeid(*(this->election)) == typeid(RegularRound)){
		cout << "Please enter the district's ID, choose from the options below: ";
		this->election->displayDistrictNames();
		cin >> districtId;
		if(!isValidDistrict(districtId - 1)){
			throw std::exception("Invalid district Id");
		}
	}
	
	this->election->addCitizen(citizenName, citizenId, birthYear);
}
void Menu::addParty(){
	
	string partyName, leaderId;
	
	cin.ignore();
	cout << "Please enter the party's name:" << endl;
	getline(cin, partyName);
	
	cout << "Please enter the represntor's ID:" << endl;
	getline(cin, leaderId);
	
	this->election->addParty(partyName, leaderId);
}
void Menu::addPartyMember(){
	
	char repId[20];
	int partyId, dstId;
	
	cin.ignore();
	cout << "Please enter the representor ID" << endl;
	cin.getline(repId, 20);
	
	if(this->election->isRelatedToParty(repId))
		throw std::exception("representative is already related to a party.");
	
	cout << "Please enter the party's ID, choose from the options below: ";
	this->election->displayPartyNames();
	cin >> partyId;
	
	if(typeid(*(this->election)) == typeid(RegularRound)){
		cout << "Please enter the district's ID, choose from the options below: ";
		this->election->displayDistrictNames();
		cin >> dstId;
		if(!isValidDistrict(dstId - 1)){
			throw std::exception("Invalid district Id");
		}
	}
	else
		dstId = 1;
	
	this->election->addPartyMember(repId, partyId - 1, dstId - 1);
}
void Menu::displayAllDistricts(){
	
	if(typeid(*(this->election)) == typeid(RegularRound))
		this->election->displayAllDistricts();
}
void Menu::displayAllCitizens(){
	
	this->election->displayAllCitizens();
}
void Menu::displayAllParties(){
	
	this->election->displayAllParties();
}
void Menu::vote(){
	
	string id;
	int partyId;
	
	cout << "Please enter the voter's Id number" << endl;
	getline(cin, id);
	
	this->election->displayPartyNames();
	cout << "Please enter the number of the party the voter selected" << endl;
	cin >> partyId;
	
	this->election->vote(id, partyId - 1);
}
void Menu::displayResults(){
	
	if(this->election->noVotesYet()){
		throw std::exception("there we're no votes yet.");
	}
	
	cout << "these are the results for the elections that occurred on: "
		 << this->election->getDay() << "/" << this->election->getMonth() << "/" << this->election->getYear() << endl;
	
	this->election->displayResults();
}
bool Menu::isValidDistrict(int idx){
	
	return this->election->isDistrictIdValid(idx);
}
void Menu::loadElection(const string f_name){
	
	string file_name;
	if(f_name.empty()){
		cout << "Please enter the file's name to load: " << endl;
		cin.ignore();
		getline(cin, file_name);
	}
	else
		file_name = f_name;
	
	bool isError = false;
	ifstream in;
	
	try{
		in.open(f_name);
	}
	catch(std::exception &ex){
		cout << "There was a problem with file." << endl;
		return;
	}
	
	if(!in.is_open())
		throw std::exception("problem opening file");
	
	if(this->election != nullptr)
		delete this->election;
	
	Byte type;
	in.read(rcastc(&type), sizeof(type));
	
	if(type == REGULAR_ROUND){
		this->election = new RegularRound(in);
	}
	else if(type == SIMPLE_ROUND){
		this->election = new SimpleRound(in);
	}
	else
		isError = true;
	in.close();
	
	if(isError)
		throw std::exception("reading from file.");
}
void Menu::saveElection(const string f_name){
	
	string file_name;
	if(f_name.empty()){
		cout << "Please enter the file's name to save: " << endl;
		cin.ignore();
		getline(cin, file_name);
	}
	else
		file_name = f_name;
	
	ofstream out(file_name);
	if(!out.is_open())
		throw std::exception("problem opening file");
	
	this->election->save(out);
	
	out.close();
}
Menu::Menu(const string f_name){
	
	try{
		this->election = nullptr;
		this->loadElection(f_name);
	}
	catch(std::exception &ex){
		cout << ex.what() << endl;
		exit(1);
	}
}
void Menu::checkDate(int year, int month, int days){
	
	if(year <= 0)
		throw std::exception("invalid year");
	if(!(1 <= month && month <= 12))
		throw std::exception("invalid month");
	if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
		if(days > 31)
			throw std::exception("the month you entered has 31 days");
	if(month == 4 || month == 6 || month == 9 || month == 11)
		if(days > 30)
			throw std::exception("the month you entered has 30 days");
	if(month == 2)
		if(days > 28)
			throw std::exception("February has 28 days");
}
void Menu::special(char str[]){
	
	// for test 3 we need simple round 10 electors
	// for test 4,5 run special twice
	// for test 6 we need simple round 12 electors and run twice
	
	ifstream district, citizen, party, members, vote;
	char def[] = "tests/test5/1/";
	char *path = str == nullptr ? def : str;
	try{
		
		// district
		char dst_path[200];
		sprintf(dst_path, "%s%s", path, "disData.txt");
		
		district.open(dst_path);
		while(!district.eof()){//for divided district we need to insert 0
			string buffer;
			int input, type;
			getline(district, buffer);
			district >> input >> type;
			this->election->addDistrict(buffer, input, static_cast<bool>(type));
			district.get();
		}
		district.close();
		
		// citizen
		char ctz_path[200];
		sprintf(ctz_path, "%s%s", path, "citizenData.txt");
		
		citizen.open(ctz_path);
		while(!citizen.eof()){
			string name_buffer, id_buffer;
			int year, dis;
			
			getline(citizen, name_buffer);
			getline(citizen, id_buffer);
			citizen >> year >> dis;
			citizen.get();
			
			this->election->addCitizen(name_buffer, id_buffer, year, dis);
		}
		citizen.close();
		
		// party
		char prt_path[200];
		sprintf(prt_path, "%s%s", path, "partyData.txt");
		
		party.open(prt_path);
		while(!party.eof()){
			string name_buffer, id_buffer;
			
			getline(party, name_buffer);
			getline(party, id_buffer);
			
			this->election->addParty(name_buffer, id_buffer);
		}
		party.close();
		
		// party members
		char mem_path[200];
		sprintf(mem_path, "%s%s", path, "pmember.txt");
		
		members.open(mem_path);
		while(!members.eof()){
			string id_buffer;
			int party_id, dst_id;
			
			getline(members, id_buffer);
			members >> party_id >> dst_id;
			members.get();
			
			this->election->addPartyMember(id_buffer, party_id, dst_id);
		}
		members.close();
		
		// votes
		char vote_path[200];
		sprintf(vote_path, "%s%s", path, "vote.txt");
		
		vote.open(vote_path);
		while(!vote.eof()){
			string id_buffer;
			int party_id;
			
			getline(vote, id_buffer);
			vote >> party_id;
			vote.get();
			
			this->election->vote(id_buffer, party_id);
		}
		vote.close();
	} catch(exception &msg){
		cout << "Error: " << msg.what() << endl;
	}

//	this->mainMenu();
}