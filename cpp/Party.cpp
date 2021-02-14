#include "../headers/Party.h"
#include "../headers/Round.h"

using namespace election;

int Party::idCounter = 0;
Party::Party(const string _name, const Citizen &_leader, int amount_of_districts) : name(_name),
																					_members(amount_of_districts){
	
	this->id = this->idCounter++;
	this->leader = _leader;
	
	for(int i = 0; i < _members.capacity(); i++)
		_members.push_back(CitizenMap());
}
Party &Party::operator=(const Party &org){
	
	if(this != &org){
		const_cast<string &>(this->name) = org.name;
		this->id = org.id;
		
		this->leader = org.leader;
		this->_members = org._members;
	}
	
	return *this;
}
void Party::addMember(const Citizen &new_member, int dstId){
	
	if(dstId > _members.size())
		return;
	
	_members[dstId].addCitizen(new_member);
}
bool Party::save(ofstream &out) const{
	
	short int id_num = this->id;
	out.write(rcastcc(&id_num), sizeof(id_num));
	
	Byte len = this->name.length();
	out.write(rcastcc(&len), sizeof(len));
	out.write(rcastcc(this->name.c_str()), len);
	
	this->leader.save(out);
	
	short int members_length = _members.size();
	out.write(rcastcc(&members_length), sizeof(members_length));
	
	if(!out.good())
		return false;
	
	std::for_each(_members.cbegin(), _members.cend(),
				  [&](auto curr)->void{
					  if(!curr.save(out))
						  throw std::exception("There was a problem saving.");
				  });
	
	return out.good();
}
bool Party::load(ifstream &in){
	
	if(this->id != -1)
		return false;
	
	idCounter++;
	
	short int id_num;
	in.read(rcastc(&id_num), sizeof(id_num));
	this->id = id_num;
	
	readString(in, this->name);
	
	this->leader = Citizen(in);
	
	short int arr_size;
	in.read(rcastc(&arr_size), sizeof(arr_size));
	_members = CitizenMapArray(arr_size);
	
	for(int i = 0; i < _members.capacity() && in.good(); i++){
		CitizenMap temp;
		temp.load(in);
		_members.push_back(temp);
	}
	
	return in.good();
}
Party::Party(ifstream &in){
	
	this->load(in);
}
const string Party::getPartyName() const{
	
	return this->name;
}
const Citizen &Party::getLeader() const{
	
	return this->leader;
}
const CitizenMapArray &Party::getPartyMembersArr() const{
	
	return _members;
}
int Party::getId() const{
	
	return this->id;
}
