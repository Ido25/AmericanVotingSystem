#pragma once

#include <iostream>
#include "configs.h"

using namespace std;

namespace election{
  class Citizen{
	protected:
	  string name;
	  string id;
	  int birth_year;
	  bool load(ifstream &in);
  
	public:
	  Citizen(ifstream &in);
	  Citizen() : name(""), id(""), birth_year(0){}
	  Citizen(const string _name, const string _id, int _birth) : name(_name), id(_id), birth_year(_birth){};
	  Citizen(const Citizen &org) : Citizen(org.name, org.id, org.birth_year){}
	  ~Citizen() = default;
	  //
	  const string getName() const{ return this->name; }
	  const string getId() const{ return this->id; }
	  int getBirthYear() const{ return this->birth_year; }
	  bool save(ofstream &out) const;
	  Citizen &operator=(const Citizen &org);
	  bool operator==(const Citizen &other) const{ return this->id == other.id; };
	  bool operator!=(const Citizen &other) const{ return !(*this == other); }
	  friend ostream &operator<<(ostream &os, const Citizen &ctz){
		
		  const Citizen *temp = &ctz;
		  if(temp == nullptr)
			  return os << endl;
		
		  os << CITIZEN_SEPERATOR() << endl << "# Name:          " << ctz.name << endl << "# ID:            "
			 << ctz.id << endl
			 << "# Birth year:    " << ctz.birth_year << endl;
		
		  return os;
	  }
  };
}