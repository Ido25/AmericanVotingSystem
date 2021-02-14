#pragma once

#include "Round.h"
#include "configs.h"

namespace election{
  class RegularRound : public Round{
	private:
	  VotesRowArr _votesArray; // row - district + 1 (for total by party), col - party + 1 (for total by district)
	  //
	  bool isRelatedToParty(const string id);
	  virtual bool noVotesYet(){ return _votesArray.empty(); };
	  int winner(DynamicArray<int> &sum_electors);
	  void displayTotalCalc();
	  void sumVotesArray();
	  virtual void calcElectorsAllocation();
	  virtual inline float calcVoteRateByDstAndPrt(int dstIdx, int prtIdx){
		
		  return static_cast<float>(_votesArray[dstIdx][prtIdx].amount) /
				 _votesArray[dstIdx][_prt_array.size()].amount;
	  }
	  void manageMemVotesArray();
	  void reallocVotesArray();
	  virtual void calcResults();
  
	public:
	  RegularRound(ifstream &in);
	  RegularRound(int _y, int _m, int _d) : Round(_y, _m, _d){}
	  virtual ~RegularRound() = default;
	  virtual void vote(const string id, int partyId);
	  virtual void addDistrict(const string _name, int numOfReps, bool isUnited);
	  virtual void addCitizen(const string _name, const string _id, int _birthYear, int _dstrictId);
	  virtual void addPartyMember(const string elector_id, int partyId, int dstId);
	  virtual void displayDistrictNames();
	  virtual void displayAllDistricts();
	  virtual void displayAllCitizens();
	  virtual void displayResults();
	  virtual bool isDistrictExist(const string name);
	  virtual bool isDistrictIdValid(int idx){ return idx <= _dst_array.size() && idx >= 0; }
	  virtual bool save(ofstream &out) const;
  };
}