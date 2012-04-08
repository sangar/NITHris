#include "InitializationException.h"
#include <sstream>

using std::stringstream;

/*
* InitializationException.cpp
* Magnus&Gard ris v1.0
*
* Created by: Gard Sandholt and Magnus Østeng
* Date: 20.11.09
* Description:
* This class is a specialization of runtime_error.
*/
InitializationException::InitializationException(string what, string cause)
: runtime_error(what)
{
	errorCause = cause;
}
string InitializationException::what()
{
	stringstream out;

	out << runtime_error::what();
	out << ": ";
	out << errorCause;

	return out.str();
}
InitializationException::~InitializationException(void)
{
	
}
