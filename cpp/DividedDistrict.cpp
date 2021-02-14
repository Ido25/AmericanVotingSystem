#include "../headers/DividedDistrict.h"

election::DividedDistrict::DividedDistrict(const string _name, int _amount_of_electors) : District(_name,
																								   _amount_of_electors){
	
	_dstType = DIVIDED_DISTRICT;
}
ostream &election::DividedDistrict::_print(ostream &os) const{
	
	os << "# Name:           " << this->name << endl
	   << "# Type:           " << "Divided District" << endl
	   << "# Amount of reps: " << _representors.capacity() << endl;
	
	return os;
}
