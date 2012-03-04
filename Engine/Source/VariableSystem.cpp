#include "VariableSystem.h"

VariableSystem::VariableSystem() { }

VariableSystem::VariableSystem(const VariableSystem & v) {
	for(unsigned int i=0;i<v.m_categories.size();i++) {
		m_categories.push_back(strtrimcpy(v.m_categories[i]));
	}

	for(unsigned int i=0;i<v.m_variables.size();i++) {
		m_variables.push_back(new Variable(*(v.m_variables.at(i))));
	}
}

VariableSystem & VariableSystem::operator = (const VariableSystem & v) {
	for(unsigned int i=0;i<m_categories.size();i++) {
		delete [] m_categories[i];
	}
	m_categories.clear();

	for(unsigned int i=0;i<m_variables.size();i++) {
		delete m_variables.at(i);
	}
	m_variables.clear();

	for(unsigned int i=0;i<v.m_categories.size();i++) {
		m_categories.push_back(strtrimcpy(v.m_categories[i]));
	}

	for(unsigned int i=0;i<v.m_variables.size();i++) {
		m_variables.push_back(new Variable(*(v.m_variables.at(i))));
	}

	return *this;
}

VariableSystem::~VariableSystem() {
	for(unsigned int i=0;i<m_categories.size();i++) {
		delete [] m_categories[i];
	}

	for(unsigned int i=0;i<m_variables.size();i++) {
		delete m_variables.at(i);
	}
}

int VariableSystem::addCategory(const char * category) {
	if(category == NULL || strlen(category) == 0) { return Variable::NO_CATEGORY; }
	
	char * temp = strtrimcpy(category);

	if(strlen(temp) == 0) {
		delete [] temp;
		return Variable::NO_CATEGORY;
	}

	int categoryIndex = -1;
	for(int i=0;i<(int) m_categories.size();i++) {
		if(_stricmp(temp, m_categories[i]) == 0) {
			categoryIndex = i;
			break;
		}
	}

	if(categoryIndex == -1) {
		m_categories.push_back(temp);
		return m_categories.size() - 1;
	}
	else {
		delete [] temp;
		return categoryIndex;
	}
}

int VariableSystem::indexOfCategory(const char * category) const {
	if(category == NULL || m_categories.size() == 0) { return -1; }
	
	char * temp = strtrimcpy(category);

	if(strlen(temp) == 0) {
		delete [] temp;
		return -1;
	}

	for(unsigned int i=0;i<m_categories.size();i++) {
		if(_stricmp(temp, m_categories[i]) == 0) {
			delete [] temp;
			return i;
		}
	}
	
	delete [] temp;
	return -1;
}

char * VariableSystem::categoryAt(int index) const {
	if(index < 0 || index >= (int) m_categories.size()) { return NULL; }
	return m_categories[index];
}

int VariableSystem::size() const {
	return m_variables.size();
}

bool VariableSystem::contains(const char * id) const {
	if(id == NULL) { return false; }

	char * tempID = strtrimcpy(id);

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(tempID, m_variables[i]->getID()) == 0) {
			delete [] tempID;
			return true;
		}
	}

	delete [] tempID;
	return false;
}

bool VariableSystem::contains(const char * id, const char * category) const {
	if(id == NULL || category == NULL) { return false; }

	char * tempID = strtrimcpy(id);
	int categoryIndex = indexOfCategory(category);

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(tempID, m_variables[i]->getID()) == 0 &&
		   categoryIndex == m_variables[i]->getCategory()) {
			delete [] tempID;
			return true;
		}
	}

	delete [] tempID;
	return false;
}

bool VariableSystem::contains(const Variable * v) const {
	if(v == NULL) { return false; }

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(v->getID(), m_variables[i]->getID()) == 0 &&
			v->getCategory() == m_variables[i]->getCategory()) {
			return true;
		}
	}

	return false;
}

int VariableSystem::indexOf(const char * id) const {
	if(id == NULL) { return -1; }

	char * tempID = strtrimcpy(id);

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(tempID, m_variables[i]->getID()) == 0) {
			delete [] tempID;
			return i;
		}
	}

	delete [] tempID;
	return -1;
}

int VariableSystem::indexOf(const char * id, const char * category) const {
	if(id == NULL || category == NULL) { return -1; }

	char * tempID = strtrimcpy(id);
	int categoryIndex = indexOfCategory(category);

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(tempID, m_variables[i]->getID()) == 0 &&
		   categoryIndex == m_variables[i]->getCategory()) {
			delete [] tempID;
			return i;
		}
	}

	delete [] tempID;
	return -1;
}

int VariableSystem::indexOf(const Variable * v) const {
	if(v == NULL) { return -1; }

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(v->getID(), m_variables[i]->getID()) == 0 &&
			v->getCategory() == m_variables[i]->getCategory()) {
			return i;
		}
	}

	return -1;
}

const Variable * VariableSystem::variableAt(int index) const {
	if(index < 0 || index >= (int) m_variables.size()) { return NULL; }
	return m_variables[index];
}

const Variable * VariableSystem::getVariable(const char * id) const {
	if(id == NULL) { return NULL; }

	char * tempID = strtrimcpy(id);

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(tempID, m_variables[i]->getID()) == 0) {
			delete [] tempID;
			return m_variables[i];
		}
	}

	delete [] tempID;
	return NULL;
}

const Variable * VariableSystem::getVariable(const char * id, const char * category) const {
	if(id == NULL || category == NULL) { return NULL; }

	char * tempID = strtrimcpy(id);
	int categoryIndex = indexOfCategory(category);

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(tempID, m_variables[i]->getID()) == 0 &&
		   categoryIndex == m_variables[i]->getCategory()) {
			delete [] tempID;
			return m_variables[i];
		}
	}

	delete [] tempID;
	return NULL;
}

const char * VariableSystem::getValue(const char * id) const {
	if(id == NULL) { return NULL; }

	char * tempID = strtrimcpy(id);

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(tempID, m_variables[i]->getID()) == 0) {
			delete [] tempID;
			return m_variables[i]->getValue();
		}
	}

	delete [] tempID;
	return NULL;
}

const char * VariableSystem::getValue(const char * id, const char * category) const {
	if(id == NULL || category == NULL) { return NULL; }

	char * tempID = strtrimcpy(id);
	int categoryIndex = indexOfCategory(category);

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(tempID, m_variables[i]->getID()) == 0 &&
		   categoryIndex == m_variables[i]->getCategory()) {
			delete [] tempID;
			return m_variables[i]->getValue();
		}
	}

	delete [] tempID;
	return NULL;
}

vector<Variable *> * VariableSystem::getVariablesInCategory(const char * category) const {
	if(category == NULL) { return NULL; }

	int categoryIndex = indexOfCategory(category);

	vector<Variable *> * variableCollection = new vector<Variable * >;
	
	for(unsigned int i=0;i<m_variables.size();i++) {
		if(categoryIndex == m_variables[i]->getCategory()) {
			variableCollection->push_back(m_variables[i]);
		}
	}
	return variableCollection;
}

bool VariableSystem::add(const char * id, const char * value, const char * category) {
	if(id == NULL || value == NULL || category == NULL) { return false; }

	if(!contains(id, category)) {
		int categoryIndex = addCategory(category);
		m_variables.push_back(new Variable(id, value, categoryIndex));
		return true;
	}
	return false;
}

bool VariableSystem::add(Variable * v) {
	if(v == NULL) { return false; }

	if(!contains(v) && v->getCategory() < (int) m_categories.size()) {
		m_variables.push_back(v);
		return true;
	}
	return false;
}

bool VariableSystem::addCopy(const Variable * v) {
	if(v == NULL) { return false; }

	if(!contains(v) && v->getCategory() < (int) m_categories.size()) {
		m_variables.push_back(new Variable(*v));
		return true;
	}
	return false;
}

void VariableSystem::setValue(const char * id, const char * value) {
	if(id == NULL || value == NULL) { return; }

	bool valueUpdated = false;

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(id, m_variables[i]->getID()) == 0) {
			m_variables[i]->setValue(value);
			valueUpdated = true;
		}
	}

	// if the variable doesn't exist, add it
	if(!valueUpdated) {
		add(id, value);
	}
}

void VariableSystem::setValue(const char * id, int value) {
	char buffer[32];
	sprintf_s(buffer, 32, "%d", value);
	setValue(id, buffer);
}

void VariableSystem::setValue(const char * id, double value) {
	char buffer[32];
	sprintf_s(buffer, 32, "%f", value);
	setValue(id, buffer);
}

void VariableSystem::setValue(const char * id, bool value) {
	setValue(id, value ? "true" : "false");
}

void VariableSystem::setValue(const char * id, const char * value, const char * category) {
	if(id == NULL || value == NULL || category == NULL) { return; }

	int categoryIndex = indexOfCategory(category);

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(id, m_variables[i]->getID()) == 0 &&
		   categoryIndex == m_variables[i]->getCategory()) {
			m_variables[i]->setValue(value);
			return;
		}
	}

	// if the variable doesn't exist, add it
	add(id, value, category);
}

void VariableSystem::setValue(const char * id, int value, const char * category) {
	char buffer[32];
	sprintf_s(buffer, 32, "%d", value);
	setValue(id, buffer, category);
}

void VariableSystem::setValue(const char * id, double value, const char * category) {
	char buffer[32];
	sprintf_s(buffer, 32, "%f", value);
	setValue(id, buffer, category);
}

void VariableSystem::setValue(const char * id, bool value, const char * category) {
	setValue(id, value ? "true" : "false", category);
}

bool VariableSystem::remove(int index) {
	if(index < 0 || index >= (int) m_variables.size()) { return false; }
	delete m_variables[index];
	m_variables.erase(m_variables.begin() + index);
	return true;
}

bool VariableSystem::remove(const char * id) {
	if(id == NULL) { return false; }

	char * tempID = strtrimcpy(id);

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(tempID, m_variables[i]->getID()) == 0) {
			delete m_variables[i];
			m_variables.erase(m_variables.begin() + i);
			delete [] tempID;
			return true;
		}
	}

	delete [] tempID;
	return false;
}

bool VariableSystem::remove(const char * id, const char * category) {
	if(id == NULL || category == NULL) { return false; }

	char * tempID = strtrimcpy(id);
	int categoryIndex = indexOfCategory(category);

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(tempID, m_variables[i]->getID()) == 0 &&
		   categoryIndex == m_variables[i]->getCategory()) {
			delete m_variables[i];
			m_variables.erase(m_variables.begin() + i);
			delete [] tempID;
			return true;
		}
	}

	delete [] tempID;
	return false;
}

bool VariableSystem::remove(const Variable * v) {
	if(v == NULL) { return false; }

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(_stricmp(v->getID(), m_variables[i]->getID()) == 0 &&
			v->getCategory() == m_variables[i]->getCategory()) {
			delete m_variables[i];
			m_variables.erase(m_variables.begin() + i);
			return true;
		}
	}

	return false;
}

void VariableSystem::removeCategory(const char * data) {
	if(data == NULL) { return; }

	char * category = strtrimcpy(data);

	int categoryIndex = indexOfCategory(category);

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(m_variables[i]->getCategory() == categoryIndex) {
			m_variables.erase(m_variables.begin() + i);
			i--;
		}
	}

	delete [] category;
}

void VariableSystem::clear() {
	for(unsigned int i=0;i<m_variables.size();i++) {
		delete m_variables.at(i);
	}
	m_variables.clear();
}

void VariableSystem::sort() {
	Variable * temp;
	for(unsigned int i=0;i<m_variables.size();i++) {
		for(unsigned int j=i;j<m_variables.size();j++) {
			if(m_variables[i]->getCategory() > m_variables[j]->getCategory()) {
				temp = m_variables[i];
				m_variables[i] = m_variables[j];
				m_variables[j] = temp;
			}
		}
	}
}

VariableSystem * VariableSystem::readFrom(const char * fileName) {
	if(fileName == NULL || strlen(fileName) == 0) { return NULL; }

	char data[1024];
	char * temp;

	// open the file
	ifstream input(fileName);
	if(!input.is_open()) { return NULL; }

	VariableSystem * variables = new VariableSystem();
	char * category = NULL;
	int categoryIndex = -1;

	// read to the end of the file
	while(true) {
		input.getline(data, 1024);
		if(input.eof()) { break; }

		temp = strtrimcpy(data);
		if(strlen(temp) == 0) {
			if(category != NULL) { delete [] category; }
			category = NULL;
			categoryIndex = -1;
			continue;
		}

		// parse a category
		if(strlen(temp) >= 2 && temp[0] == '[' && temp[strlen(temp) - 1] == ']') {
			category = new char[strlen(temp) - 1];
			for(unsigned int i=1;i<strlen(temp) - 1;i++) {
				category[i-1] = temp[i];
			}
			category[strlen(temp)-2] = '\0';
			categoryIndex = variables->addCategory(category);
		}
		// parse a variable
		else {
			Variable * v = Variable::parseFrom(temp);
			if(v != NULL) {
				v->setCategory(categoryIndex);
				variables->add(v);
			}
		}

		delete [] temp;
	}

	if(category != NULL) { delete[] category; }

	input.close();

	return variables;
}

bool VariableSystem::writeTo(const char * fileName) const {
	int lastCategory = Variable::NO_CATEGORY;
	bool firstLine = true;

	ofstream output(fileName);
	if(!output.is_open()) {
		return false;
	}

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(lastCategory == Variable::NO_CATEGORY || lastCategory != m_variables[i]->getCategory()) {
			if(m_variables[i]->getCategory() != Variable::NO_CATEGORY) {
				if(!firstLine) { output << endl; }
				output << "[" << m_categories[m_variables[i]->getCategory()] << "]" << endl;
				firstLine = false;
			}
			lastCategory = m_variables[i]->getCategory();
		}
		output << (*m_variables[i]) << endl;
		firstLine = false;
	}

	output.close();

	return true;
}

bool VariableSystem::operator == (const VariableSystem & v) const {
	if(m_variables.size() != v.m_variables.size()) { return false; }

	for(unsigned int i=0;i<m_variables.size();i++) {
		if(!v.contains(m_variables.at(i))) {
			return false;
		}
	}
	return true;
}

bool VariableSystem::operator != (const VariableSystem & v) const {
	return !operator == (v);
}

void VariableSystem::printOn(ostream & o) const {
	for(unsigned int i=0;i<m_variables.size();i++) {
		o << m_variables.at(i);
		if(i < m_variables.size() - 1) {
			o << ", ";
		}
	}
}

ostream & operator << (ostream & o, const VariableSystem & v) {
	v.printOn(o);
	return o;
}
