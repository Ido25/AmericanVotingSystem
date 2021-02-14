#include "../headers/CitizenMap.h"
#include "../headers/Round.h"

using namespace election;

bool CitizenMap::addCitizen(const Citizen &data){
	
	//checking if citizen already exists
	if(this->map.find(data.getId()) == this->map.end())
		this->map[data.getId()] = data;
	else
		return false;
	
	return true;
}
const Citizen &CitizenMap::getCitizen(int idx) const{
	
	auto itr = this->map.begin();
	for(int i = 0; i < idx && itr != this->map.end(); i++)
		itr++;
	
	if(itr == this->map.end())
		throw std::out_of_range("Citizen not found.");
	
	return itr->second;
}
const Citizen &CitizenMap::getCitizen(const string id) const{
	
	auto res = this->map.find(id);
	if(res == this->map.end())
		throw std::out_of_range("Citizen not found.");
	else
		return res->second;
}
bool CitizenMap::save(ofstream &out) const{
	
	short int len = this->map.size();
	out.write(rcastcc(&len), sizeof(len));
	
	auto itr = this->map.begin();
	
	// saving the citizens data.
	while(itr != this->map.end()){
		(itr)->second.save(out);
		itr++;
	}
	
	return out.good();
}
bool CitizenMap::load(ifstream &in){
	
	if(!this->map.empty())
		return false;
	
	short int len;
	in.read(rcastc(&len), sizeof(len));
	
	for(short int i = 0; i < len; i++){
		Citizen temp(in);
		this->map[temp.getId()] = temp;
		auto t = temp;
	}
	
	return in.good();
}
void CitizenMap::printNames() const{
	
	std::for_each(this->map.begin(), this->map.end(),
				  [](std::pair<string, Citizen> pair)->void{ cout << pair.second.getName() << endl; });
}
bool CitizenMap::addCitizen(string name, string id, int birth_year){
	
	Citizen temp(name, id, birth_year);
	return this->addCitizen(temp);
}
