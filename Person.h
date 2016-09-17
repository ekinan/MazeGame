#ifndef _PERSON_H
#define _PERSON_H

/*
This is the class containing the generic Person object. Here, a person consists of only their info,
which can correspond to their name, background and other parameters depending on the value of
NUM_INFO below.
*/

#include <cstdlib>

const int NUM_INFO = 2; //Number of pieces of personal information, in case class is used for other projects

class Person
{
public:
	Person(const char** personalInfo = NULL); //Info is stored in a const char array
	Person(const Person&); //Copy constructor
	~Person();

	/*
	Returns the string stored in info[index].
	@param index: The array index of info to be accessed
	If index is out of bounds or less than 0, NULL is returned.
	*/
	const char* getInfo(int index) const;

	/*
	Overriding the assignment operator
	*/
	Person& operator=(const Person&);

private:
	char* info[NUM_INFO]; //Stores personal info. For project, index 0 = name, 1 = background.
	void clearInfo(); //Clears any existing information
	void copy(const Person&); //Used to facilitate copying.
};

#endif
