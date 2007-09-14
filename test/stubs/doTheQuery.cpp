#include <iostream>
#include <exception>
#include "CoralIface.h"

int main(int, char** )
{


	try {
		CoralIface * cif = new CoralIface("oracle://omds/CMS_TRACKER_DCS");
		cif->doQuery();
		delete cif;
	}
	catch ( std::exception& e ) {
		std::cerr << "C++ Exception : " << e.what() << std::endl;
		return 1;
	}

	return 0;
}


