#include "IOException.h"
#include <sstream>


/*
* IOException.cpp
* Magnus&Gard ris v1.0
*
* Created by: Gard Sandholt and Magnus Østeng
* Date: 20.11.09
* Description:
* This class is a spesialization of runtime_error.
*/
IOException::IOException(std::string what, std::string cause)
:runtime_error(what)
{
	errorCause = cause;
}

std::string IOException::what()
{
	std::stringstream out;

	out << runtime_error::what();
	out << ":";
	out << errorCause;

	return out.str();
}

IOException::~IOException(void)
{
}
