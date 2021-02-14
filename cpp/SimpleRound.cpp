#include "../headers/SimpleRound.h"
#include "../headers/DividedDistrict.h"

using namespace election;

void SimpleRound::addCitizen(const string _name, const string _id, int _birthYear, int _dstrictId){
	
	if(_id.length() != 9)
		throw std::exception("the id is not 9 digits");
	
	for(int i = 0; i < _id.length(); i++)
		if(!('0' <= _id[i] && _id[i] <= '9'))
			throw std::exception("the id is not only digits");
	
	if(this->y - _birthYear < 18)
		throw std::exception("the citizen is underage");
	
	// check if citizen already exist.
	bool found = false;
	try{
		_dst_array[0]->getCitizen(_id);
		found = true;
	}
	catch(std::exception &err){
		_dst_array[0]->addCitizen(_name, _id, _birthYear);
	}
	if(found)
		throw std::runtime_error("the id you inserted already exists");
}
bool SimpleRound::isRelatedToParty(const string id){
	
	for(int i = 0; i < _prt_array.size(); i++)
		try{
			// getCitizen will throw if citizen does not found.
			_prt_array[i].getPartyMembersArr()[0].getCitizen(id);
			return true;
		}
		catch(std::exception &ex){
			continue;
		}
	
	return false;
}
void SimpleRound::calcResults(){
	
	auto compare_func = [](VotePair &a, VotePair &b)->int{ return a.amount - b.amount; };
	auto end = _votes.end();
	end--;
	_MergeSort<VotesColArr>(_votes.begin(), --end, compare_func);
	
	_votes[_prt_array.size()].amount = 0;
	for(int i = 0; i < _prt_array.size(); i++)
		_votes[_prt_array.size()].amount += _votes[i].amount;
	
	this->calcElectorsAllocation();
	
	_dst_array[0]->clearRepsArray();
	for(int i = 0; i < _prt_array.size(); i++){
		int prt_idx = _votes[i].prt_id;
		const CitizenMapArray membersArr = _prt_array[prt_idx].getPartyMembersArr();
		int reps_needed = _votes[i].amount_of_reps_needed;
		for(int j = 0; j < reps_needed; j++){
			if(j >= membersArr[0].getSize())
				break;
			
			try{
				auto temp = membersArr[0].getCitizen(j);
				_dst_array[0]->addMember(temp, reps_needed);
			}
			catch(std::exception &ex){ break; }
		}
	}
}
void SimpleRound::displayResults(){
	
	this->calcResults();
	
	cout << endl << endl;
	cout << _dst_array[0];
	cout << "Total votes amount: " << _votes[_prt_array.size()].amount << endl;
	
	for(int i = 0; i < _prt_array.size(); i++)
		_printPartyResultsPerDst(_votes[i], _votes[_prt_array.size()].amount);
	
	auto repsArray = const_cast<RepresentorArrray &>(_dst_array[0]->getRepresentors());
	auto cmp_func = [](Representor &a, Representor &b)->int{ return a.priority - b.priority; };
	_MergeSort<RepresentorArrray>(repsArray.begin(), repsArray.end(), cmp_func);
	_printReps(repsArray);
	
	cout << WINNER_SEPERATOR() << endl << PARTY_SEPERATOR() << endl << "# Name: "
		 << _prt_array[_votes[0].prt_id].getPartyName()
		 << endl << PRESIDENT_SEPERATOR() << endl << "# Name: "
		 << _prt_array[_votes[0].prt_id].getLeader().getName()
		 << endl << SEPERATOR() <<
		 endl;
}
void SimpleRound::vote(const string id, int partyId){
	
	this->reallocVotes();
	
	try{
		const Citizen &human = this->findCitizen(id);
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
	
	for(int i = 0; i < _votes.size(); i++)
		if(_votes[i].prt_id != -1 && _votes[i].prt_id == partyId){
			_votes[i].amount++;
			break;
		}
	
	this->already_voted.insert(id);
}
void SimpleRound::displayAllCitizens(){
	
	if(_dst_array[0]->getCitizensAmount() > 0){
		cout << const_cast<CitizenMap &>(_dst_array[0]->getCitizens()) << SEPERATOR() << endl;
	}
	else{
		cout << SEPERATOR() << endl << "There are no citizens to display" << endl << SEPERATOR();
	}
}
void SimpleRound::addPartyMember(const string elector_id, int partyId, int dstId){
	
	bool found = false;
	try{
		const Citizen &newMember = this->findCitizen(elector_id);
		found = true;
		
		if(this->isRelatedToParty(elector_id))
			throw std::exception("this party member is already related to another party");
		
		_prt_array[partyId].addMember(newMember, 0);
	}
	catch(std::exception &ex){
		if(found)
			throw ex;
		else
			throw std::exception("party member was not found");
	}
}
SimpleRound::SimpleRound(int _y, int _m, int _d, int amount_of_reps) : Round(_y, _m, _d){
	
	_dst_array = DstPtrArray(1);
	_dst_array.push_back(new DividedDistrict("Ghost District", amount_of_reps));
}
bool SimpleRound::save(ofstream &out) const{
	
	Byte type = SIMPLE_ROUND;
	out.write(rcastcc(&type), sizeof(type));
	
	Round::save(out);
	
	short int temp = _votes.size();
	out.write(rcastcc(&temp), sizeof(temp));
	
	for(int i = 0; i < _votes.size(); i++){
		const VotePair &vPair = _votes[i];
		out.write(rcastcc(&vPair.amount), sizeof(vPair.amount));
		
		const string str = vPair.prt_id == -1 ? this->total_str : _prt_array[vPair.prt_id].getPartyName();
		Byte len = str.size();
		out.write(rcastcc(&len), sizeof(len));
		out.write(rcastcc(str.c_str()), len);
	}
	
	return out.good();
}
SimpleRound::SimpleRound(ifstream &in) : Round(in){
	
	short int temp;
	in.read(rcastc(&temp), sizeof(temp));
	_votes = VotesColArr(temp);
	
	for(int i = 0; i < _votes.size(); i++){
		in.read(rcastc(&_votes[i].amount), sizeof(_votes[i].amount));
		
		string prt_name;
		readString(in, prt_name);
		_votes[i].prt_id =
				prt_name == this->total_str ? -1 : this->_getPartyIdByName(prt_name);
	}
}
void SimpleRound::calcElectorsAllocation(){
	
	for(int i = 0; i < _prt_array.size(); i++)
		_votes[i].amount_of_reps_needed = 0;
	
	int sum = 0, total = _dst_array[0]->getAmountOfElectors();
	while(total > 0){
		if(static_cast<int>(this->calcVoteRateByPrt(0) * total) == 0){
			for(int i = 0; i < _prt_array.size() && total > 0; i++){
				total--;
				_votes[i].amount_of_reps_needed++;
			}
		}
		else{
			for(int i = 0; i < _prt_array.size(); i++){
				
				float rate = this->calcVoteRateByPrt(i);
				_votes[i].amount_of_reps_needed += rate * total;
				sum += _votes[i].amount_of_reps_needed;
			}
			total -= sum;
		}
	}
}
void SimpleRound::reallocVotes(){
	
	static bool first_time = true;
	if(first_time){
		first_time = false;
		_votes.clear();
		
		for(int i = 0; i < _prt_array.size() + 1; i++){
			VotePair obj;
			obj.prt_id = (i != _prt_array.size()) ? i : -1;
			obj.amount = 0;
			obj.amount_of_reps_needed = 0;
			_votes.push_back(obj);
		}
	}
	else{
		if(_prt_array.size() + 1 == _votes.size())
			return;
		
		// set new cols
		int prevAmount = _votes.size() - 1;
		VotesColArr::iterator insertPos = _votes.end();
		insertPos--;
		
		for(int i = prevAmount; i < _prt_array.size(); i++){
			VotePair obj;
			obj.prt_id = i;
			obj.amount = 0;
			obj.amount_of_reps_needed = 0;
			_votes.insert(insertPos++, obj);
		}
	}
}
