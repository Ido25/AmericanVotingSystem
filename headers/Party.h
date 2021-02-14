#pragma once

#include <iostream>
#include "configs.h"
#include "CitizenMap.h"

using namespace std;

namespace election{
  class Party{
	private:
	  static int idCounter;
	  int id;
	  string name;
	  Citizen leader;
	  CitizenMapArray _members;
  
	public:
	  Party(ifstream &in);
	  Party() : name(""), leader(), _members(), id(-1){} // empty party
	  Party(const string _name, const Citizen &_leader, int amount_of_districts);
	  Party(const Party &org) = default;
	  ~Party() = default;
	  const string getPartyName() const;
	  const Citizen &getLeader() const;
	  const CitizenMapArray &getPartyMembersArr() const;
	  void addMember(const Citizen &new_member, int dstId);
	  int getId() const;
	  Party &operator=(const Party &org);
	  bool save(ofstream &out) const;
	  bool load(ifstream &in);
	  void enMembersArray(){ _members.push_back(CitizenMap()); }
	  friend ostream &operator<<(ostream &os, const Party &party){
		
		  os << endl << "# Name:   " << party.name << endl
			 << "# Leader: " << party.leader.getName() << endl << SEPERATOR() << endl;
		
		  return os;
	  }
  };
}