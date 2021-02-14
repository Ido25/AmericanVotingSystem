#pragma once

#include <map>
#include "configs.h"
#include "Citizen.h"

namespace election{
  class Round;
  
  class CitizenMap{
	private:
	  map<string, Citizen> map;
  
	public:
	  CitizenMap() = default;
	  CitizenMap(ifstream &in){ this->load(in); }
	  CitizenMap(const CitizenMap &org){ *this = org; };
	  ~CitizenMap() = default;
	  int getSize() const{ return this->map.size(); };
	  bool addCitizen(const Citizen &data);
	  bool addCitizen(string name, string id, int birth_year);
	  const Citizen &getCitizen(int idx) const;
	  const Citizen &getCitizen(const string id) const;
	  CitizenMap &operator=(const CitizenMap &other) = default;
	  const Citizen &operator[](const string str){ return this->getCitizen(str); };
	  bool save(ofstream &out) const;
	  bool load(ifstream &in);
	  void clear(){ this->map.clear(); }
	  void printNames() const;
	  friend ostream &operator<<(ostream &os, CitizenMap &org){
		
		  auto itr = org.map.begin();
		  while(itr != org.map.end())
			  os << (itr++)->second;
		
		  return os;
	  }
  };
}


