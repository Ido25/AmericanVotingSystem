#pragma once

#include <iostream>
#include "Citizen.h"
#include "District.h"
#include "configs.h"
#include "Party.h"
#include <set>

namespace election{
  
  enum RoundTypes{
	  ROUND, REGULAR_ROUND, SIMPLE_ROUND
  };
  
  class Round{
	protected:
	  struct VotePair{
		  unsigned int amount = 0;
		  short int prt_id = -1;
		  unsigned int amount_of_reps_needed = 0;
	  };
	  
	  using VotesColArr = DynamicArray<VotePair>;
	  using VotesRowArr = DynamicArray<VotesColArr>;
	  const string total_str = "TOTAL";
	  int y, m, d;
	  DstPtrArray _dst_array;
	  PrtArray _prt_array;
	  set<string> already_voted;
	  //
	  int _getPartyIdByName(const string p_name) const;
	  virtual void calcElectorsAllocation() = 0;
	  virtual void calcResults() = 0;
	  virtual void _printPartyResultsPerDst(VotePair prt, int total_dst_votes) const;
	  virtual void _printReps(RepresentorArrray &repsArray) const;
  
	public:
	  Round(ifstream &in);
	  Round(int _y, int _m, int _d) : y(_y), m(_m), d(_d), _prt_array(2){}
	  virtual ~Round();
	  virtual void addParty(const string _partyName, const string _leaderId);
	  virtual void displayPartyNames();
	  virtual void displayAllParties() const;
	  virtual const Citizen &findCitizen(const string id, int *p_district_id = nullptr);
	  int getDay(){ return this->d; }
	  int getMonth(){ return this->m; }
	  int getYear(){ return this->y; }
	  virtual bool isPartyExist(const string name) const;
	  virtual void vote(const string id, int partyId) = 0;
	  virtual void addDistrict(const string _name, int numOfReps, bool isUnited) = 0;
	  virtual void addCitizen(const string _name, const string _id, int _birthYear, int _dstrictId = 0) = 0;
	  virtual void addPartyMember(const string elector_id, int partyId, int dstId) = 0;
	  virtual void displayDistrictNames() = 0;
	  virtual void displayAllDistricts() = 0;
	  virtual void displayAllCitizens() = 0;
	  virtual void displayResults() = 0;
	  virtual bool isDistrictExist(const string name) = 0;
	  virtual bool isDistrictIdValid(int idx) = 0;
	  virtual bool isRelatedToParty(const string id) = 0;
	  virtual bool save(ofstream &out) const;
	  virtual bool noVotesYet() = 0;
  };
}
