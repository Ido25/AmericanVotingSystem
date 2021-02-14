#include "../headers/Citizen.h"

using namespace election;

bool Citizen::save(ofstream &out) const{
	
	Byte len = this->name.size();
	out.write(rcastcc(&len), sizeof(len));
	out.write(rcastcc(this->name.c_str()), len);
	
	len = this->id.size();
	out.write(rcastcc(&len), sizeof(len));
	out.write(rcastcc(this->id.c_str()), len);
	
	short int temp = this->birth_year;
	out.write(rcastcc(&temp), sizeof(temp));
	
	return out.good();
}
bool Citizen::load(ifstream &in){
	
	readString(in, this->name);
	readString(in, this->id);
	
	short int year;
	in.read(rcastc(&year), sizeof(year));
	this->birth_year = year;
	
	return in.good();
}
Citizen::Citizen(ifstream &in){
	
	this->load(in);
}
Citizen &Citizen::operator=(const Citizen &org){
	
	if(this != &org){
		this->name = org.name;
		this->id = org.id;
		this->birth_year = org.birth_year;
	}
	
	return *this;
}
