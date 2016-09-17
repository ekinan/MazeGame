/*
This is the implementation file for Person.h, containing the definition of the member functions.
*/

#include "Person.h"

#include <cstring>
#include <cstdlib>


Person::Person(const char** personalInfo)
{
	for (int i = 0; i < NUM_INFO; ++i) //Copy all of the information in personalInfo
	{				//Note that default info is the null character
		if (personalInfo == NULL || personalInfo[i] == NULL)
			info[i] = new char[1]; //Default information is the null character (nothing)
		else //There is personal information stored in personalInfo[i]
		{    //so we copy it to info's corresponding index.
			info[i] = new char[std::strlen(personalInfo[i]) + 1];
			std::strcpy(info[i], personalInfo[i]);
		}
	}
}

Person::Person(const Person& other)
{
	copy(other);
}

Person::~Person()
{
	clearInfo();
}

void Person::clearInfo()
{	//Recall that the default, null character is initialized using new []
	for (int i = 0; i < NUM_INFO; ++i) //so info[i] will ALWAYS point to something
		delete [] info[i]; //and that delete [] will correctly free the memory.
}

void Person::copy(const Person& other)
{
	for (int i = 0; i < NUM_INFO; ++i) //Copying the information stored in the info array of other
	{
		info[i] = new char[std::strlen(other.info[i]) + 1];
		std::strcpy(info[i], other.info[i]);
	}
}

const char* Person::getInfo(int index) const
{
	if (0 <= index && index < NUM_INFO) //Check if index is within array bounds
		return info[index]; //Return the info there if it does
	else //Return the null string if the array is out of bounds
		return "";
}

Person& Person::operator=(const Person& other)
{
	if (this != &other) //Avoid redundant assignment.
	{
		clearInfo();
		copy(other);
	}

	return *this;
}
