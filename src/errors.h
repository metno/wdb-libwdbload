/*
    wdb-libwdbload

    Copyright (C) 2015 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: post@met.no

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA  02110-1301, USA
*/


#ifndef ERRORS_H_
#define ERRORS_H_

#include <exception>
#include <iosfwd>
#include <string>

namespace wdb
{
namespace load
{
enum ErrorCode
{
	Success = 0,

	// Errors
	UnknownError, // exceptions and stuff that developers did not expect or plan for

	UnableToConnectToDatabase = 10,
	UnableToReadFromDatabase,
	UnableToUpdateMetadata,
	NoFieldsLoaded,

	UnableToReadFile = 20,
	ErrorWhenReadingFile,

	InvalidCommandLineArguments = 30,
	UnableToReadConfigFile,

	FieldFailedToLoad = 100
};

std::ostream & explainErrors(std::ostream & s);


bool success();

ErrorCode errorCode();

inline int exitStatus()
{
	return int(errorCode());
}


void registerError(ErrorCode error);

const std::string & getErrorMessage();



class LoadError : public std::exception
{
public:
	explicit LoadError(ErrorCode error);
	LoadError(ErrorCode error, const std::string & message);
	virtual ~LoadError() throw ();

	virtual const char * what() const throw();
private:
	std::string message_;
};


}
}







#endif /* ERRORS_H_ */
