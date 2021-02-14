#pragma once
#include "District.h"

namespace election{
  class DividedDistrict : public District{
	protected:
	  virtual ostream &_print(ostream &os) const;
  
	public:
	  DividedDistrict(const string _name, int _amount_of_electors);
	  DividedDistrict(ifstream &in) : District(in){ _dstType = DIVIDED_DISTRICT; }
	  friend ostream &operator<<(ostream &os, const DividedDistrict *org){ return org->_print(os); }
  };
}

