#pragma once

#include "../headers/District.h"

namespace election{
  class UnitedDistrict : public District{
	protected:
	  virtual ostream &_print(ostream &os) const;
  
	public:
	  UnitedDistrict(const string _name, int _amount_of_electors);
	  UnitedDistrict(ifstream &in) : District(in){ _dstType = UNITED_DISTRICT; }
	  friend ostream &operator<<(ostream &os, const UnitedDistrict *org){ return org->_print(os); }
  };
}