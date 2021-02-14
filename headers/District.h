#pragma once

#include <iostream>
#include "CitizenMap.h"
#include "configs.h"

using namespace std;

namespace election{
  
  enum DistrictTypes{
	  DIVIDED_DISTRICT, UNITED_DISTRICT, DISTRICT
  };
  struct Representor{
	  Citizen data;
	  int priority;
  };
  
  class District{
	protected:
	  static int idCounter;
	  Byte _dstType;
	  string name;
	  int id;
	  float vote_rate;
	  CitizenMap citizens;
	  RepresentorArrray _representors;
	  bool load(ifstream &in);
	  virtual ostream &_print(ostream &os) const;
	
	public:
	  District(ifstream &in);
	  District() : name(""), vote_rate(0), _dstType(DISTRICT), _representors(),
				   id(-1){} // empty district
	  District(const string _name, int _amount_of_electors);
	  District(const District &org);
	  virtual ~District() = default;
	  virtual int getId() const{ return this->id; }
	  virtual int getAmountOfElectors() const;
	  virtual const string getName() const{ return this->name; }
	  virtual int getCitizensAmount(){ return this->citizens.getSize(); }
	  virtual const Citizen &getCitizen(const string citizen_id);
	  virtual const CitizenMap &getCitizens() const{ return this->citizens; }
	  virtual const RepresentorArrray &getRepresentors() const{ return _representors; }
	  virtual void addCitizen(const string _name, const string _id, int _birthYear);
	  virtual void addMember(const Citizen &data, int priority);
	  virtual District &operator=(const District &org);
	  virtual bool save(ofstream &out) const;
	  virtual void clearRepsArray();
	  friend ostream &operator<<(ostream &os, const District *org){ return org->_print(os); }
  };
}