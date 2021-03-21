//
//  Logger_Local.cpp
//  Service_LoggerLocal
//
//  Created by Ashish Aring on 17/02/2017.
//  Copyright © 2017 Ashish Aring. All rights reserved.
//

#include "Logger_Local.hpp"



// constructor

LoggerLocal::LoggerLocal	(std::string logOwner)
			:	Logger		(logOwner,	typeid(this).name(), LOCAL)
{
	sendLogMsg(_bLogClass, INFO, "Log created. Owner is " + getLogOwner());
}
LoggerLocal::~LoggerLocal	()
{
	_bStream.str("");
	_bStream << "Deleting local-logger. Statistics: ";
	pumpLogStatistic(_bStream);
	sendLogMsg(_bLogClass, INFO, _bStream.str());
}



// methods

			/**
			 *		Private method
			 *
			 *		Prints content of own-stream to output.
			 */

void		LoggerLocal::printToConsole		(MessageType log)
{
	std::cout << _bStream.str();
	
	bUpdateCounter(log);
}


			/**
			 *		Public overridden method
			 *
			 *		Converts status-message into local log-msg.
			 */

void		LoggerLocal::sendStatusMsg			(std::string user,
												 std::string status)
{
	sendLogMsg(user, INFO, "New status: "+status);
}

			/**
			 *		Public overridden method
			 *
			 *		Sends log-message to local loc.
			 */

void		LoggerLocal::sendLogMsg			(std::string user,
												 LogLevel	 level,
												 std::string logMsg)
{
	_bStream.str("");
	Logger::composeLogEntry(_bStream, user, level, logMsg);
	
	printToConsole(ProcessMsg);
}


			/**
			 *		Public overridden method
			 *
			 *		Converts iteration-messages into local log-msg.
			 */

void		LoggerLocal::sendIterMsg			(std::string	user,
												 LogLevel		level,
												 unsigned int	iter,
												 double			F,
												 bool			isNewBest
												 )
{
	// send log-msg
	_bStream.str("");
	_bStream << std::setw(4)                   << std::right << std::fixed << iter;
	_bStream << std::setw(Constants::nomWidth) << std::right << "iterBestF = ";
	_bStream << std::setw(Constants::nomWidth) << std::right << std::setprecision(3) << (F > Constants::LimitELayout ? std::scientific : std::fixed) << F;
	if (isNewBest)	_bStream << " * new best";
	
	sendLogMsg(user, level, _bStream.str());
}


			/**
			 *		Public overridden method
			 *
			 *		Converts newBest-message into local log-msg.
			 */

void		LoggerLocal::sendNewBestMsg		(std::string	user,
												 LogLevel		level,
												 double			F,
												 std::string	bestSol,
												 std::string	kpi)
{
	// send bestF-message
	_bStream.str("");
	_bStream << std::setw(Constants::nomWidth) << std::right << "New best F = ";
	_bStream << std::setw(Constants::nomWidth) << std::right << std::setprecision(3) << (F > Constants::LimitELayout ? std::scientific : std::fixed) << F;
	sendLogMsg(user, level, _bStream.str());

	// send best-sol message
	_bStream.str("");
	_bStream << "Best solution:\n" << bestSol;
	sendLogMsg(user, level, _bStream.str());

	// send kpi-message
	_bStream.str("");
	_bStream << "KPIs:\n" << kpi;
	sendLogMsg(user, level, _bStream.str());
}


