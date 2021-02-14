
#pragma once

#include "Round.h"
#include "configs.h"

namespace election{
  class SimpleRound : public Round{
	private:
	  VotesColArr _votes;
	  bool isRelatedToParty(const string id);
	  virtual void calcElectorsAllocation();
	  void reallocVotes();
	  virtual void calcResults();
	  inline float calcVoteRateByPrt(int prtIdx){
		
		  return static_cast<float>(_votes[prtIdx].amount) / _votes[_prt_array.size()].amount;
	  }
  
	public:
	  SimpleRound(ifstream &in);
	  SimpleRound(int _y, int _m, int _d, int amount_of_reps);
	  virtual ~SimpleRound() = default;
	  virtual void displayAllCitizens();
	  virtual void vote(const string id, int partyId);
	  virtual void addDistrict(const string _name, int numOfReps, bool isUnited){};
	  virtual void addCitizen(const string _name, const string _id, int _birthYear, int _dstrictId = 0);
	  virtual void addPartyMember(const string elector_id, int partyId, int dstId = 0);
	  virtual void displayResults();
	  virtual bool isDistrictIdValid(int idx){ return idx == 0; };
	  virtual void displayDistrictNames(){ cout << endl; };
	  virtual void displayAllDistricts(){ cout << endl; }
	  virtual bool isDistrictExist(const string name){ return false; };
	  virtual bool save(ofstream &out) const;
	  virtual bool noVotesYet(){ return _votes.empty(); };
  };
}
