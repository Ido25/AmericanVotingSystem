#include "../headers/UnitedDistrict.h"

election::UnitedDistrict::UnitedDistrict(const string _name, int _amount_of_electors) : District(_name,
																								 _amount_of_electors){
	
	_dstType = UNITED_DISTRICT;
}
ostream &election::UnitedDistrict::_print(ostream &os) const{
	
	os << "# Name:           " << this->name << endl
	   << "# Type:           " << "United District" << endl
	   << "# Amount of reps: " << _representors.capacity() << endl;
	
	return os;
}
