#ifndef IOEXCEPTION_H
#define IOEXCEPTION_H

#include <stdexcept>
#include <string>
class IOException : public std::runtime_error
{
private:
	std::string errorCause;
public:
	IOException():std::runtime_error("IOException"){};
	IOException(std::string what, std::string cause);
	std::string what();
	~IOException(void);
};


#endif;