#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Variables.h"

using namespace std;

int main(int argc, char * argv[]) {
	Variables variables;
	variables.parseFrom("settings.ini");

	for(int i=0;i<variables.size();i++) {
		cout << *(variables.elementAt(i)) << endl;
	}
	
	return 0;
}