#include "../headers/District.h"
#include "../headers/UnitedDistrict.h"
#include "../headers/DividedDistrict.h"
#include <cstring>

using namespace election;

int District::idCounter = 0;
District::District(const string _name, int _amount_of_electors) : name(_name), id(this->idCounter++),
																  _dstType(DISTRICT){
	
	this->vote_rate = 0;
	_representors = RepresentorArrray(_amount_of_electors);
}
int District::getAmountOfElectors() const{
	
	return _representors.capacity();
}
District::District(const District &org){
	
	*this = org;
}
const Citizen &District::getCitizen(const string citizen_id){
	
	return this->citizens.getCitizen(citizen_id);
}
District &District::operator=(const District &org){
	
	if(this != &org){
		this->id = org.id;
		const_cast<string &>(this->name) = org.name;
		
		this->vote_rate = 0;
		this->citizens = org.citizens;
		_representors = org._representors;
	}
	
	return *this;
}
bool District::save(ofstream &out) const{
	
	out.write(rcastcc(&_dstType), sizeof(_dstType));
	
	short int temp = this->id;
	out.write(rcastcc(&temp), sizeof(temp));
	
	Byte len = this->name.length();
	out.write(rcastcc(&len), sizeof(len));
	out.write(rcastcc(this->name.c_str()), len);
	
	temp = _representors.capacity();
	out.write(rcastcc(&temp), sizeof(temp));
	
	out.write(rcastcc(&this->vote_rate), sizeof(this->vote_rate));
	
	return out.good() && this->citizens.save(out);
}
bool District::load(ifstream &in){
	
	short int temp;
	in.read(rcastc(&temp), sizeof(temp));
	this->id = temp;
	
	readString(in, this->name);
	
	in.read(rcastc(&temp), sizeof(temp));
	_representors = RepresentorArrray(temp);
	
	in.read(rcastc(&this->vote_rate), sizeof(this->vote_rate));
	
	this->citizens.load(in);
	
	return in.good();
}
District::District(ifstream &in){
	
	this->load(in);
}
void District::clearRepsArray(){
	
	this->_representors.clear();
}
void District::addCitizen(const string _name, const string _id, int _birthYear){
	
	this->citizens.addCitizen(_name, _id, _birthYear);
}
ostream &District::_print(ostream &os) const{
	
	os << "# Name:           " << this->name << endl
	   << "# Type:           " << "District" << endl
	   << "# Amount of reps: " << _representors.capacity() << endl;
	
	return os;
}
void District::addMember(const Citizen &data, int priority){
	
	if(std::any_of(_representors.begin(), _representors.end(), [&](Representor &rep)->bool{ return data == rep.data; }))
		throw std::exception("Representor already allocated to district.");
	
	_representors.push_back({ data, priority });
}
