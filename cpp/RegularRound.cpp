#include "../headers/RegularRound.h"
#include "../headers/UnitedDistrict.h"
#include "../headers/DividedDistrict.h"

using namespace election;

void RegularRound::displayAllDistricts(){
	
	for(int i = 0; i < _dst_array.size(); i++){
		cout << DISTRICT_SEPERATOR() << endl
			 << "# ID:             " << i + 1 << endl <<
			 _dst_array[i] << endl;
	}
	cout << endl;
}
void RegularRound::displayAllCitizens(){
	
	for(int i = 0; i < _dst_array.size(); i++)
		if(_dst_array[i]->getCitizensAmount() > 0){
			cout << DISTRICT_SEPERATOR() << endl << "# District ID:   " << i + 1 << endl << "# District name: "
				 << _dst_array[i]->getName() << endl << const_cast<CitizenMap &>(_dst_array[i]->getCitizens())
				 << SEPERATOR() << endl;
		}
		else{
			cout << DISTRICT_SEPERATOR() << endl << "# District ID:   " << i + 1 << endl << "# District name: "
				 << _dst_array[i]->getName() << endl << SEPERATOR() << endl << "There are no Citizens in this district"
				 << endl << SEPERATOR();
		}
	
	cout << endl;
}
void RegularRound::displayDistrictNames(){
	
	cout << endl << "----------------DISTRICTS----------------" << endl;
	for(int i = 0; i < _dst_array.size(); i++)
		cout << "# " << i + 1 << ". " << _dst_array[i]->getName() << endl;
	cout << "-----------------------------------------" << endl;
}
void RegularRound::displayResults(){
	
	this->calcResults();
	
	cout << endl << endl;
	
	int partiesLen = _prt_array.size();
	for(int i = 0; i < _dst_array.size(); i++){
		cout << endl << endl << DISTRICT_SEPERATOR() << endl << (_dst_array[i]);
		cout << "# Chosen leader: " <<
			 _prt_array[_votesArray[i][0].prt_id].getLeader().getName() << endl;
		
		for(int j = 0; j < partiesLen; j++)
			_printPartyResultsPerDst(_votesArray[i][j], _votesArray[i][partiesLen].amount);
		
		auto repsArray = const_cast<RepresentorArrray &>(_dst_array[i]->getRepresentors());
		auto cmp_func = [](Representor &a, Representor &b)->int{ return a.priority - b.priority; };
		_MergeSort<RepresentorArrray>(repsArray.begin(), repsArray.end(), cmp_func);
		_printReps(repsArray);
		
		cout << SEPERATOR() << endl;
	}
	
	this->displayTotalCalc();
}
void RegularRound::displayTotalCalc(){
	
	// sum electors will contain how many electors each party won in election.
	DynamicArray<int> sumElectors(_prt_array.size());
	for(int i = 0; i < _prt_array.size(); i++)
		sumElectors.push_back(0);
	
	for(int i = 0; i < _dst_array.size(); i++)
		//calculating for a united district electors
		if(typeid(*_dst_array[i]) == typeid(UnitedDistrict)){
			int id = _votesArray[i][0].prt_id;
			sumElectors[id] += _dst_array[i]->getAmountOfElectors();
		}
		else{
			//calculating for a divided district electors
			for(int j = 0; j < _prt_array.size(); j++){
				int id = _votesArray[i][j].prt_id;
				sumElectors[id] += _votesArray[i][j].amount_of_reps_needed;
			}
		}
	
	cout << SEPERATOR() << endl << TOTAL_VOTES_SEPERATOR() << endl;
	for(int i = 0; i < _prt_array.size(); i++){
		cout << PARTY_SEPERATOR() << endl << "# Name:                     " << _prt_array[i].getPartyName() << endl
			 << "# Total amount of votes:    " << _votesArray[_dst_array.size()][i].amount << endl
			 << "# Total amount of electors: " << sumElectors[i] << endl << SEPERATOR() << endl;
	}
	
	int winnerIdx = winner(sumElectors);
	cout << WINNER_SEPERATOR() << endl << PARTY_SEPERATOR() << endl << "# Name: "
		 << _prt_array[winnerIdx].getPartyName()
		 << endl << PRESIDENT_SEPERATOR() << endl << "# Name: " << _prt_array[winnerIdx].getLeader().getName()
		 << endl << SEPERATOR() << endl;
}
//
void RegularRound::addCitizen(const string _name, const string _id, int _birthYear, int _dstrictId){
	
	if(_id.length() != 9)
		throw std::exception("the id is not 9 digits");
	
	for(auto iter = _id.cbegin(); iter != _id.cend(); iter++)
		if(!std::isdigit(*iter))
			throw std::exception("the id is not only digits");
	
	if(this->y - _birthYear < 18)
		throw std::exception("the citizen is underage");
	
	bool found = false;
	try{
		// find citizen throw if citizen not found.
		this->findCitizen(_id);
		found = true;
	}
	catch(std::exception &ex){
		_dst_array[_dstrictId]->addCitizen(_name, _id, _birthYear);
	}
	if(found)
		throw std::exception("the id you inserted already exists");
}
void RegularRound::addPartyMember(const string elector_id, int partyId, int dstId){
	
	bool found = false;
	try{
		const Citizen &newMember = this->findCitizen(elector_id);
		found = true;
		
		if(this->isRelatedToParty(newMember.getId()))
			throw std::exception("party member is related to a different party");
		
		//	if(dstId != this->getCitizensDistrict(elector_id))
		//		throw std::exception("party member is not related to the district that was inserted");
		
		_prt_array[partyId].addMember(newMember, dstId);
	}
	catch(std::exception &ex){
		if(found)
			throw ex;
		else
			throw std::exception("party member was not found");
	}
}
void RegularRound::addDistrict(const string _name, int numOfReps, bool isUnited){
	
	if(this->isDistrictExist(_name))
		throw std::exception("District with the same name already exists");
	
	if(isUnited)
		_dst_array.push_back(new UnitedDistrict(_name, numOfReps));
	else
		_dst_array.push_back(new DividedDistrict(_name, numOfReps));
	
	std::for_each(_prt_array.begin(), _prt_array.end(), [](Party &p)->void{ p.enMembersArray(); });
}
//
int RegularRound::winner(DynamicArray<int> &sum_electors){
	
	int max = sum_electors[0], winnerIdx = 0;
	for(int i = 1; i < sum_electors.size(); i++){
		if(sum_electors[i] > max){
			max = sum_electors[i];
			winnerIdx = i;
		}
	}
	
	return winnerIdx;
}
bool RegularRound::isDistrictExist(const string name){
	
	for(int i = 0; i < _dst_array.size(); i++)
		if(_dst_array[i]->getName() == name)
			return true;
	
	return false;
}
bool RegularRound::isRelatedToParty(const string id){
	
	for(int i = 0; i < _prt_array.size(); i++){
		for(int j = 0; j < _dst_array.size(); j++)
			try{
				_prt_array[i].getPartyMembersArr()[j].getCitizen(id);
				return true;
			}
			catch(std::exception &ex){
				continue;
			}
	}
	
	return false;
}
//
void RegularRound::vote(const string id, int partyId){
	
	this->manageMemVotesArray();
	
	int district_id;
	try{
		const Citizen &human = this->findCitizen(id, &district_id);
		
		if(this->y - human.getBirthYear() < 18)
			throw std::exception("citizen is underage therefore is not allowed to vote");
	}
	catch(std::exception &ex){
		throw std::exception("voter was not found");
	}
	
	//checking if the citizen already voted
	if(this->already_voted.find(id) != this->already_voted.end())
		throw std::exception("voter has already voted");
	
	if(partyId > _prt_array.size())
		throw std::exception("party elected does not exist");
	
	for(int i = 0; i < _votesArray[0].size(); i++)
		if(_votesArray[district_id][i].prt_id != -1 &&
		   _votesArray[district_id][i].prt_id == partyId){
			_votesArray[district_id][i].amount++;
			break;
		}
	
	this->already_voted.insert(id);
}
void RegularRound::sumVotesArray(){
	
	// sum per party
	for(int i = 0; i < _prt_array.size(); i++)
		_votesArray[_dst_array.size()][i].amount = 0;
	
	for(int i = 0; i < _prt_array.size(); i++)
		for(int j = 0; j < _dst_array.size(); j++){
			int id = _votesArray[j][i].prt_id;
			_votesArray[_dst_array.size()][id].amount += _votesArray[j][i].amount;
		}
	
	// sum per district
	for(int i = 0; i < _dst_array.size(); i++)
		_votesArray[i][_prt_array.size()].amount = 0;
	
	auto compare_func = [](VotePair &a, VotePair &b)->int{ return a.amount - b.amount; };
	for(int i = 0; i < _dst_array.size(); i++){
		auto end = _votesArray[i].end();
		_MergeSort<VotesColArr>(_votesArray[i].begin(), --end, compare_func);
		
		for(int j = 0; j < _prt_array.size(); j++)
			_votesArray[i][_prt_array.size()].amount += _votesArray[i][j].amount;
	}
}
void RegularRound::calcElectorsAllocation(){
	
	for(int i = 0; i < _dst_array.size(); i++)
		for(int j = 0; j < _prt_array.size(); j++)
			_votesArray[i][j].amount_of_reps_needed = 0;
	
	for(int i = 0; i < _dst_array.size(); i++){
		int sum = 0, total = _dst_array[i]->getAmountOfElectors();
		while(total > 0){
			if(static_cast<int>(this->calcVoteRateByDstAndPrt(i, 0) * total) == 0){
				for(int j = 0; j < _prt_array.size() && total > 0; j++){
					total--;
					_votesArray[i][j].amount_of_reps_needed++;
				}
			}
			else{
				for(int j = 0; j < _prt_array.size(); j++){
					float rate = this->calcVoteRateByDstAndPrt(i, j);
					_votesArray[i][j].amount_of_reps_needed += rate * total;
					sum += _votesArray[i][j].amount_of_reps_needed;
				}
				total -= sum;
			}
		}
	}
}
void RegularRound::manageMemVotesArray(){
	
	static bool first_time = true;
	if(first_time){
		first_time = false;
		
		_votesArray = VotesRowArr(_dst_array.size() + 1);
		for(int i = 0; i < _dst_array.capacity(); i++){
			_votesArray.push_back(VotesColArr(_prt_array.size() + 1));
			for(int j = 0; j < _votesArray[i].capacity(); j++){
				VotePair obj;
				obj.prt_id = (j != _prt_array.size()) ? j : -1;
				obj.amount = 0;
				obj.amount_of_reps_needed = 0;
				_votesArray[i].push_back(obj);
			}
		}
	}
	else
		this->reallocVotesArray();
}
void RegularRound::calcResults(){
	
	this->sumVotesArray();
	this->calcElectorsAllocation();
	
	// calculating votes and assigning representatives
	for(int row = 0; row < _dst_array.size(); row++){
		_dst_array[row]->clearRepsArray();
		for(int col = 0; col < _prt_array.size(); col++){
			int prt_idx = _votesArray[row][col].prt_id;
			const CitizenMapArray membersArr = _prt_array[prt_idx].getPartyMembersArr();
			int reps_needed = _votesArray[row][col].amount_of_reps_needed;
			for(int i = 0; i < reps_needed; i++){
				if(i >= membersArr[row].getSize())
					break;
				
				try{
					const Citizen &temp = membersArr[row].getCitizen(i);
					_dst_array[row]->addMember(temp, reps_needed);
				}
				catch(std::exception &ex){ break; }
			}
		}
	}
}
void RegularRound::reallocVotesArray(){
	
	int prevRows = _votesArray.size(), prevCols = _votesArray[0].size();
	if(prevRows == _dst_array.size() + 1 && prevCols == _prt_array.size() + 1)
		return;
	
	// set new cols
	for(int i = 0; i < prevRows; i++){
		VotesColArr::iterator insertPos = _votesArray[i].end();
		insertPos--;
		for(short int j = prevRows - 2; j < _prt_array.size(); j++){
			VotePair obj = { 0, j, 0 };
			_votesArray[i].insert(insertPos++, obj);
		}
	}
	
	VotesColArr temp(_prt_array.size() + 1);
	for(int j = 0; j < temp.capacity(); j++){
		VotePair obj;
		obj.prt_id = (j != _prt_array.size()) ? j : -1;
		obj.amount = 0;
		obj.amount_of_reps_needed = 0;
		temp.push_back(obj);
	}
	
	// set new rows
	for(int i = prevRows - 1; i < _dst_array.size(); i++){
		VotesRowArr::iterator insertPos = _votesArray.end();
		_votesArray.insert(--insertPos, temp);
	}
}
//
bool RegularRound::save(ofstream &out) const{
	
	Byte type = REGULAR_ROUND;
	out.write(rcastcc(&type), sizeof(type));
	
	Round::save(out);
	
	short int temp = _votesArray.size();
	out.write(rcastcc(&temp), sizeof(temp));
	
	temp = _votesArray[0].size();
	out.write(rcastcc(&temp), sizeof(temp));
	
	for(int i = 0; i < _votesArray.size(); i++)
		for(int j = 0; j < _votesArray[i].size(); j++){
			const VotePair &vPair = _votesArray[i][j];
			out.write(rcastcc(&vPair.amount), sizeof(vPair.amount));
			
			const string str = vPair.prt_id == -1 ? this->total_str : _prt_array[vPair.prt_id].getPartyName();
			Byte len = str.size();
			out.write(rcastcc(&len), sizeof(len));
			out.write(rcastcc(str.c_str()), len);
		}
	
	return out.good();
}
RegularRound::RegularRound(ifstream &in) : Round(in){
	
	short int votesArrayRows, votesArrayCols;
	in.read(rcastc(&votesArrayRows), sizeof(votesArrayRows));
	in.read(rcastc(&votesArrayCols), sizeof(votesArrayCols));
	
	VotePair obj = { 0, 0, 0 };
	_votesArray = VotesRowArr(votesArrayRows);
	for(int i = 0; i < votesArrayRows; i++){
		_votesArray.push_back(VotesColArr(votesArrayCols));
		for(int j = 0; j < votesArrayCols; j++)
			_votesArray[i].push_back(obj);
	}
	
	for(int i = 0; i < _votesArray.size(); i++)
		for(int j = 0; j < _votesArray[0].size(); j++){
			VotePair &vPair = _votesArray[i][j];
			in.read(rcastc(&vPair.amount), sizeof(vPair.amount));
			
			string prt_name;
			readString(in, prt_name);
			vPair.prt_id = prt_name != this->total_str ? this->_getPartyIdByName(prt_name) : -1;
		}
}

