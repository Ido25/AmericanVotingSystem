#include "../headers/Round.h"
#include "../headers/UnitedDistrict.h"
#include "../headers/DividedDistrict.h"

using namespace election;

void Round::addParty(const string _partyName, const string _leaderId){
	
	if(this->isPartyExist(_partyName))
		throw std::exception("party already exists");
	
	Citizen leader;
	try{ leader = this->findCitizen(_leaderId); }
	catch(std::exception &err){
		throw std::exception("party leader was not found");
	}
	
	bool ldr_already_assigned = std::any_of(_prt_array.begin(), _prt_array.end(),
											[&](auto c)->bool{ return c.getLeader().getId() == leader.getId(); });
	if(ldr_already_assigned)
		throw std::exception("the id you inserted already is related to a different party leader");
	
	if(this->isRelatedToParty(leader.getId()))
		throw std::exception("party leader is related to a different party");
	
	_prt_array.push_back(Party(_partyName, leader, _dst_array.capacity()));
}
void Round::displayAllParties() const{
	
	int counter = 0;
	auto print_per_dst = [&](const CitizenMap &current)->void{
		if(counter < _dst_array.size() && current.getSize() > 0){
			cout << DISTRICT_SEPERATOR() << endl
				 << "#Name: " << _dst_array[counter]->getName() << endl;
			current.printNames();
		}
		counter++;
	};
	auto print_prt = [&](const Party &p)->void{
		cout << PARTY_SEPERATOR() << endl << "# Number: " << p.getId() + 1 << p;
		cout << DELEGATORS_SEPERATOR() << endl;
		
		auto members = p.getPartyMembersArr();
		std::for_each(members.cbegin(), members.cend(), print_per_dst);
		counter = 0;
	};
	
	std::for_each(_prt_array.cbegin(), _prt_array.cend(), print_prt);
	cout << endl;
}
void Round::displayPartyNames(){
	
	cout << endl << "----------------PARTIES----------------";
	for(int i = 0; i < _prt_array.size(); i++)
		cout << endl << "# " << i + 1 << ". " << _prt_array[i].getPartyName();
	cout << endl << "---------------------------------------" << endl;
}
bool Round::isPartyExist(const string name) const{
	
	return std::any_of(_prt_array.cbegin(), _prt_array.cend(),
					   [&](Party c)->bool{ return c.getPartyName() == name; });
}
const Citizen &Round::findCitizen(const string id, int *p_district_id){
	
	for(int i = 0; i < _dst_array.size(); i++){
		try{
			auto &citizen = _dst_array[i]->getCitizen(id);
			if(citizen != Citizen()){
				if(p_district_id != nullptr)
					*p_district_id = i;
				return citizen;
			}
		}
		catch(std::exception &err){
			continue;
		}
	}
	
	throw std::exception("Citizen not found.");
}
bool Round::save(ofstream &out) const{
	
	Byte temp = this->y;
	out.write(rcastcc(&temp), sizeof(temp));
	temp = this->m;
	out.write(rcastcc(&temp), sizeof(temp));
	temp = this->d;
	out.write(rcastcc(&temp), sizeof(temp));
	
	short int len = _prt_array.size();
	out.write(rcastcc(&len), sizeof(len));
	len = _prt_array.capacity();
	out.write(rcastcc(&len), sizeof(len));
	
	len = _dst_array.size();
	out.write(rcastcc(&len), sizeof(len));
	len = _dst_array.capacity();
	out.write(rcastcc(&len), sizeof(len));
	
	for(int i = 0; i < _dst_array.size(); i++)
		if(!_dst_array[i]->save(out))
			return false;
	
	for(int i = 0; i < _prt_array.size(); i++)
		if(!_prt_array[i].save(out))
			return false;
	
	len = this->already_voted.size();
	out.write(rcastcc(&len), sizeof(len));
	auto itr = this->already_voted.begin();
	while(itr != this->already_voted.end()){
		string str = itr->data();
		Byte str_len = str.size();
		out.write(rcastcc(&str_len), sizeof(str_len));
		out.write(rcastcc(str.c_str()), str_len);
		itr++;
	}
	
	return out.good();
}
Round::Round(ifstream &in){
	
	Byte temp;
	in.read(rcastc(&temp), sizeof(temp));
	this->y = temp;
	in.read(rcastc(&temp), sizeof(temp));
	this->m = temp;
	in.read(rcastc(&temp), sizeof(temp));
	this->d = temp;
	
	short int partyLogLen, partyPhyLen, dstLogLen, dstPhyLen;
	in.read(rcastc(&partyLogLen), sizeof(partyLogLen));
	in.read(rcastc(&partyPhyLen), sizeof(partyPhyLen));
	_prt_array = PrtArray(partyPhyLen);
	
	in.read(rcastc(&dstLogLen), sizeof(dstLogLen));
	in.read(rcastc(&dstPhyLen), sizeof(dstPhyLen));
	_dst_array = DstPtrArray(dstPhyLen);
	
	for(int i = 0; i < dstLogLen && in.good(); i++){
		Byte dstType;
		in.read(rcastc(&dstType), sizeof(dstType));
		if(dstType == UNITED_DISTRICT)
			_dst_array.push_back(new UnitedDistrict(in));
		else if(dstType == DIVIDED_DISTRICT)
			_dst_array.push_back(new DividedDistrict(in));
		else
			throw std::exception("There was a problem reading from file.");
	}
	
	for(int i = 0; i < partyLogLen && in.good(); i++){
		Party tempParty;
		tempParty.load(in);
		_prt_array.push_back(tempParty);
	}
	
	short int set_len;
	in.read(rcastc(&set_len), sizeof(set_len));
	for(int i = 0; i < set_len; i++){
		string str;
		readString(in, str);
		this->already_voted.insert(str);
	}
}
int Round::_getPartyIdByName(const string p_name) const{
	
	PrtArray::const_iterator iter = std::find_if(_prt_array.cbegin(), _prt_array.cend(),
												 [&](Party p)->bool{ return p.getPartyName() == p_name; });
	return iter != _prt_array.cend() ? iter->getId() : -1;
}
Round::~Round(){
	
	std::for_each(_dst_array.begin(), _dst_array.end(), [](auto c)->void{ if(c != nullptr) delete c; });
}
void Round::_printPartyResultsPerDst(Round::VotePair prt, int total_dst_votes) const{
	
	int votes = prt.amount;
	float percentage = static_cast<float>(votes) / total_dst_votes;
	
	cout << PARTY_SEPERATOR() << _prt_array[prt.prt_id]
		 << "# Votes:           " << votes << endl
		 << "# Vote percentage: " << percentage * 100 << "%" << endl
		 << "# representatives: " << prt.amount_of_reps_needed << endl;
}
void Round::_printReps(RepresentorArrray &repsArray) const{
	
	cout << DELEGATORS_SEPERATOR() << endl;
	
	int i = 0;
	std::for_each(repsArray.cbegin(), repsArray.cend(),
				  [&](const Representor &rep)->void{
					  const Citizen &temp = rep.data;
		
					  if((i + 1) < 10)
						  cout << i + 1 << ".  " << temp.getName() << endl;
					  else
						  cout << i + 1 << ". " << temp.getName() << endl;
					  i++;
				  });
}
