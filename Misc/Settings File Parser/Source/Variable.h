//Created: September 18, 2008
//Revised: January 15, 2009

#ifndef _VARIABLE_H
#define _VARIABLE_H

#include <iostream>
#include <string>
#include <fstream>
#include "Strcasecmp.h"
#include "Methods.h"

using namespace std;

class Variable {
public:
	Variable(const char * _id = "", const char * _value = "");
	Variable(const Variable & x);
	Variable & operator = (const Variable & x);
	~Variable(void);
	
	void id(const char * _id);
	void value(const char * _value);
	
	char * id() const;
	char * value() const;

	bool parseFrom(const char * _data);
	
	bool operator == (const Variable & x) const;
	bool operator != (const Variable & x) const;
	
	void printOn(ostream & o) const;
	
private:
	char * _id;
	char * _value;
};

ostream & operator << (ostream & o, const Variable & x);

#endif