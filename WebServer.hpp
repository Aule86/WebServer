
#ifndef WEBSERVER
#define WEBSERVER

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

class WebServer
{
    public:
        void checkfile(const std::string filename);
        void readline(const std::string filename);
        std::string trim(std::string &line);

        
        class InvalidName : public std::exception {
		    public :
			    virtual const char *what() const throw()
				{
					return ("Invalid Name");
				}
	    };

        class CantOpen : public std::exception {
		    public :
			    virtual const char *what() const throw()
				{
					return ("Cant open");
				}
	    };

        class empty : public std::exception {
		    public :
			    virtual const char *what() const throw()
				{
					return ("is empty");
				}
	    };
    private:

};

#endif