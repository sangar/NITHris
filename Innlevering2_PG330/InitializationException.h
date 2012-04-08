#ifndef INITIALIZATION_EXCEPTION_H
#define INITIALIZATION_EXCEPTION_H

#include <stdexcept>
#include <string>
using std::runtime_error;
using std::string;

class InitializationException: public runtime_error
{
private:
	string errorCause;
public:
	InitializationException():runtime_error("Initialization error!"){};
	InitializationException(string what, string cause);
	~InitializationException();
	string what();
};


#endif;