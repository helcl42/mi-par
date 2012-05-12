/* 
 * File:   Exception.h
 * Author: lubos
 *
 * Created on October 18, 2011, 2:42 AM
 */

#ifndef EXCEPTION_H
#define	EXCEPTION_H

#include<iostream>

class Exception : std::exception
{
private:
	void showMessage(std::string msg) const
        {
            std::cout << "ERROR: " << msg << std::endl;
        }

public:
	Exception(std::string msg)
	{
		showMessage(msg);
	}
};


#endif	/* EXTENSION_H */

