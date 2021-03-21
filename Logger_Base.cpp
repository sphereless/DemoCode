//
//  Logger_Base.cpp
//  Service_Logger
//
//  Created by Ashish Aring on 17/02/2017.
//  Copyright © 2017 Ashish Aring. All rights reserved.
//

#include "Logger_Base.hpp"



const unsigned int Logger::_sMaxN			= 1024;
const unsigned int Logger::_sWTimeStr		= 10;
const unsigned int Logger::_sWClassStr		= 35;
const unsigned int Logger::_sWLevelStr		= 3;
const unsigned int Logger::_sWMsgStr		= 50;


// constructor

Logger::Logger		(std::string logOwner,
								 std::string logClass,
								 LogType	 type
								)
			:	_logOwner		((logOwner != "") ? logOwner : "(no name)"),
				_nPLogs			(0),
				_nFLogs			(0),
				_nILogs			(0),
				_nBLogs			(0),
				_bLogClass		(logClass),
				_bLogType		(type),
				_bStream		("")
{
	/* empty */
}

Logger::~Logger ()
{
	/* empty */
}



// gets

LogType			Logger::getLogType					() const	{	return _bLogType;	}
std::string		Logger::getLogOwner				() const	{	return _logOwner;	}
std::string		Logger::getLog						() const	{	return _bStream.str();}
unsigned int	Logger::getNumStatusEntries		() const	{	return _nSLogs;		}
unsigned int	Logger::getNumProcessLogEntries	() const	{	return _nPLogs;		}
unsigned int	Logger::getNumFileLogEntries		() const	{	return _nFLogs;		}
unsigned int	Logger::getNumIterLogEntries		() const	{	return _nILogs;		}
unsigned int	Logger::getNumBestLogEntries		() const	{	return _nBLogs;		}

				/**
				 *		Public overridden get
				 *
				 *		Determines if requested log is full.
				 */
bool			Logger::isFull						(MessageType type) const
{
	if		(type == ProcessMsg)	return (_nPLogs >= _sMaxN);
	else if	(type == FileMsg)		return (_nFLogs >= _sMaxN);
	else if	(type == IterMsg)		return (_nILogs >= _sMaxN);
	else if	(type == NewBestMsg)	return (_nBLogs >= _sMaxN);
	else if	(type == StatusMsg)		return (_nSLogs >= _sMaxN);
	else return false;
}



// overridden sets

				/*
				 *		Public overridden set
				 *
				 *		Overwrites logOwner with argument.
				 */
void			Logger::setNewLogOwner				(std::string newOwner)
{
	_logOwner = newOwner;
}


// overridden methods

				/**
				 *		Protected overridden method
				 *
				 *		Increments log-counter associated to argument.
				 *
				 *		TODO:	! -		Add message for unhandled logtype.
				 */

void			Logger::bUpdateCounter				(MessageType logtype)
{
	if		(logtype == ProcessMsg) ++_nPLogs;
	else if (logtype == FileMsg)	++_nFLogs;
	else if	(logtype == IterMsg)	++_nILogs;
	else if	(logtype == NewBestMsg)	++_nBLogs;
	else if	(logtype == StatusMsg)	++_nSLogs;
	else ;										// msg??
}
				/**
				 *		Protected overridden method
				 *
				 *		Resets log-counter associated to argument.
				 *
				 *		TODO:	! -		Add message for unhandled logtype.
				 */

void			Logger::bResetCounter				(MessageType logtype)
{
	if		(logtype == ProcessMsg) _nPLogs = 0;
	else if (logtype == FileMsg)	_nFLogs = 0;
	else if	(logtype == IterMsg)	_nILogs = 0;
	else if	(logtype == NewBestMsg)	_nBLogs = 0;
	else if	(logtype == StatusMsg)	_nSLogs = 0;
	else;										// msg??
}
				/**
				 *		Protected overridden method
				 *
				 *		Resets all log-counters.
				 */

void			Logger::bResetAllCounters			()
{
	bResetCounter(StatusMsg);
	bResetCounter(ProcessMsg);
	bResetCounter(FileMsg);
	bResetCounter(IterMsg);
	bResetCounter(NewBestMsg);
}


				/**
				 *		Public overridden method
				 *
				 *		Empties requested log.
				 */

void			Logger::clearLog			(MessageType logType)
{
	bResetCounter(logType);
}

				/**
				 *		Public overridden method
				 *
				 *		Pumps logging-statistics into stream-argument.
				 */

void			Logger::pumpLogStatistic			(std::ostringstream& os) const
{
	os  << "StatusLog entries: "  << _nSLogs << ". "
		<< "ProcessLog entries: " << _nPLogs << ". "
		<< "FileLog entries: "	  << _nFLogs << ". "
		<< "Iteration entries: "  << _nILogs << ". "
		<< "AbsBest entries: "	  << _nBLogs << ". ";
}



// static methods

				/**
				 *		Public static method
				 *
				 *		Creates standardised log-message and writes to argument-os.
				 */

void			Logger::composeLogEntry			(std::ostringstream& stream, std::string user, LogLevel level, std::string message)
{
	stream << std::fixed;
	stream << std::right << std::setfill(' ') << std::setw(_sWTimeStr)  << Epoch::getTime_Standard();
	stream << std::right << std::setfill(' ') << std::setw(_sWClassStr) << user;
	stream << std::right << std::setfill(' ') << std::setw(_sWLevelStr) << "[" << level << "] ";
	stream << std::left						  << std::setw(_sWMsgStr) << message << "\n";
}



