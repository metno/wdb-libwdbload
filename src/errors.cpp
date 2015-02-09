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

#include "errors.h"
#include <boost/assign/list_of.hpp>
#include <map>

namespace wdb
{
namespace load
{
namespace
{
ErrorCode currentErrorStatus = Success;

const std::map<ErrorCode, std::string> defaultErrorMessages = boost::assign::map_list_of
		(Success, "Success")
		(UnknownError, "Unknown error - something no one anticipated")

		(UnableToConnectToDatabase, "Unable to connect to database")
		(UnableToReadFromDatabase, "Error when attempting to read from database")
		(UnableToUpdateMetadata, "Error when updating database metadata")
		(NoFieldsLoaded, "Unable to load any fields at all")

		(UnableToReadFile, "Unable to read file")
		(ErrorWhenReadingFile, "Error when reading from file")

		(InvalidCommandLineArguments, "Invalid command line arguments")
		(UnableToReadConfigFile, "Error when attempting to read a configuration file")

		(FieldFailedToLoad, "One or more fields failed to load, but some may have loaded successfully")
		;
}

std::ostream & explainErrors(std::ostream & s)
{
	for ( std::map<ErrorCode, std::string>::const_iterator it = defaultErrorMessages.begin(); it != defaultErrorMessages.end(); ++ it )
		s << int(it->first) << ": " << it->second << '\n';
	return s;
}

bool success()
{
	return Success == currentErrorStatus;
}

ErrorCode errorCode()
{
	return currentErrorStatus;
}

void registerError(ErrorCode error)
{
	currentErrorStatus = error;
}

const std::string & getErrorMessage()
{
	static const std::string noMessage = "No error message available";
	std::map<ErrorCode, std::string>::const_iterator find = defaultErrorMessages.find(currentErrorStatus);
	if ( find != defaultErrorMessages.end() )
		return find->second;

	// Should never happen
	return noMessage;
}

LoadError::LoadError(ErrorCode error)
{
	registerError(error);
	message_ = getErrorMessage();
}

LoadError::LoadError(ErrorCode error, const std::string & message) :
		message_(message)
{
	registerError(error);
}

LoadError::~LoadError() throw ()
{}

const char * LoadError::what() const throw()
{
	return message_.c_str();
}

}
}
