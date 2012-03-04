#ifndef _VARIABLE_SYSTEM_H
#define _VARIABLE_SYSTEM_H

#include <vector>
#include <fstream>
#include "Variable.h"

class VariableSystem {
public:
	VariableSystem();
	VariableSystem(const VariableSystem & v);
	VariableSystem & operator = (const VariableSystem & v);
	~VariableSystem();
	
	int addCategory(const char * category);
	int indexOfCategory(const char * category) const;
	char * categoryAt(int index) const;
	int size() const;
	bool contains(const char * id) const;
	bool contains(const char * id, const char * category) const;
	bool contains(const Variable * v) const;
	int indexOf(const char * id) const;
	int indexOf(const char * id, const char * category) const;
	int indexOf(const Variable * v) const;
	const Variable * variableAt(int index) const;
	const Variable * getVariable(const char * id) const;
	const Variable * getVariable(const char * id, const char * category) const;
	const char * getValue(const char * id) const;
	const char * getValue(const char * id, const char * category) const;
	vector<Variable *> * getVariablesInCategory(const char * category) const;
	bool add(const char * id, const char * value = "", const char * category = "");
	bool add(Variable * v);
	bool addCopy(const Variable * v);
	void setValue(const char * id, const char * value);
	void setValue(const char * id, int value);
	void setValue(const char * id, double value);
	void setValue(const char * id, bool value);
	void setValue(const char * id, const char * value, const char * category);
	void setValue(const char * id, int value, const char * category);
	void setValue(const char * id, double value, const char * category);
	void setValue(const char * id, bool value, const char * category);
	bool remove(int index);
	bool remove(const char * id);
	bool remove(const char * id, const char * category);
	bool remove(const Variable * v);
	void removeCategory(const char * data);
	void clear();
	void sort();

	static VariableSystem * readFrom(const char * fileName);
	bool writeTo(const char * fileName) const;

	bool operator == (const VariableSystem & v) const;
	bool operator != (const VariableSystem & v) const;

	void printOn(ostream & o) const;

private:
	vector<Variable *> m_variables;
	vector<char *> m_categories;
};

ostream & operator << (ostream & o, const VariableSystem & v);

#endif // _VARIABLE_SYSTEM_H
