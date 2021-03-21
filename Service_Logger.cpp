//
//  Service_Logger.cpp
//  Service_Logger
//
//  Created by Ashish Aring on 17/02/2017.
//  Copyright © 2017 Ashish Aring. All rights reserved.
//

#include "Service_Logger.hpp"


// static properties

Service_Logger*		Service_Logger::_sPtrLogger	= nullptr;
LogLevel			Service_Logger::_sMinLevel	= ALL;
bool				Service_Logger::_sIsReady	= false;
std::ostringstream	Service_Logger::_sOs		("");



// constructor

				/**
				 *		Private constructor (Singleton!)
				 */

Service_Logger::Service_Logger	() :
				_cnts			(0),
				_ixFileLog		(-1),
				_hasFileLog		(false),
				_hasNewLogger	(false),
				_log			(0)
{
	/* empty */
}

Service_Logger::~Service_Logger()
{
	// report end-of-service
	Service_Logger::addToLog(typeid(this).name(), INFO, "Closing Service_Logger.");
	if (_sPtrLogger->_hasFileLog)
		Service_FileOps::writeToFile(_sPtrLogger->_ixFileLog, "Closing Service_Logger.", false, true);
	
	// delete logger
	if (_sPtrLogger)
	{
		delete _log;
		_log	= nullptr;
	}
	
	// unpoint static members
	_sPtrLogger	= nullptr;
	_sIsReady	= false;
	
}


// static gets

LogLevel	Service_Logger::getLogLevel			()						  { return _sMinLevel;	}
bool		Service_Logger::isLevelLogged		(LogLevel level)		  { return (level >= _sMinLevel); }



// gets

bool		Service_Logger::isTypeLocal			()					const { return _log->getLogType() == LOCAL;	}
bool		Service_Logger::isTypeWeb			()					const { return _log->getLogType() == WEB;	}
bool		Service_Logger::hasLogFile			()					const { return _hasFileLog;	}



// sets

void		Service_Logger::setMaxIterations(unsigned int maxIter)
{
	// sanity
	if (!Service_Logger::_sIsReady)		return;
	
	_log->setMaxIterations(maxIter);
}
void		Service_Logger::setTargetFitness(double targetF)
{
	// sanity
	if (!Service_Logger::_sIsReady)		return;
	
	_log->setTargetFitness(targetF);
}



// methods

			/**
			 *		Private method
			 *
			 *		Deletes instance of current logger.
			 */

void		Service_Logger::deleteCurrSetup		(bool isNewLocal, bool isNewWeb)
{
	// None logger existing or same required
	if		(_log == nullptr)				return;			// 'applySetup' not called yet
	if		(isNewLocal && isTypeLocal())	return;			// Same logger required
	else if	(isNewWeb	&& isTypeWeb())		return;			// Same logger required
	
	// delete log
	delete _log;
	_log		= nullptr;
	_sPtrLogger = nullptr;
	_sIsReady	= false;
}

			/**
			 *		Private method
			 *
			 *		Instantiates new logger, if new type is different.
			 */

void		Service_Logger::instantiateLogger	(bool isNewLocal, bool isNewWeb, std::string owner)
{
	// logger exists
	if		(_log != nullptr)
	{
		if		(isNewLocal && isTypeLocal())	return;			// Same logger required
		else if	(isNewWeb	&& isTypeWeb())		return;			// Same logger required
	}
	
	// instantiate log
	_hasNewLogger = true;
	if		(isNewLocal)_log = new LoggerLocal(owner);
	else if	(isNewWeb)	_log = new LoggerWeb(owner);
	else
	{
		_hasNewLogger = false;									// should never come here
		return;
	}
	
	// report
	if (isLevelLogged(DBUG))
	{
		std::ostringstream os;
		os << "Logger of type " <<  _log->getLogType() << "instantiated.";
		addToLog(typeid(this).name(), DBUG, os.str());
	}
}
	


			/**
			 *		Private method
			 *
			 *		Updates instance members of object type.
			 */

void		Service_Logger::initialiseObjects	(Setup_LoggerService &setup)
{
	// new logger created
	if (_hasNewLogger) return;
	
	// update owner
	_log->setNewLogOwner(setup.getLogOwner());					// to set only when NO new object was created
	addToLog(typeid(this).name(), DBUG, "LogOwner changed to " + _log->getLogOwner());
}

			/**
			 *		Private method
			 *
			 *		Updates instance members of basic type.
			 */

void		Service_Logger::initialiseFields	(Setup_LoggerService &setup)
{
	_cnts		= 0;
	_ixFileLog	= setup.getIndexLogFile();
	_hasFileLog	= setup.isLogFile() && (_ixFileLog >= 0);
}

			/**
			 *		Private method
			 *
			 *		Updates static members.
			 */

void		Service_Logger::initialiseStatics	(Setup_LoggerService &setup)
{
	_sMinLevel	= setup.getLogLevel();
	_sIsReady	= true;
}



// static methods

			/**
			 *		Private static method
			 *
			 *		Verifies usability of logger-class.
			 *
			 *		Required because 'addToLog' methods are intentionally static
			 *		to be callable from everywhere without instance. Validity of call
			 *		must thus be ensured before (_isReady).
			 */

bool		Service_Logger::validateUsage		(LogLevel& level)
{
	if (!Service_Logger::_sIsReady)			return false;
	if (level < Service_Logger::_sMinLevel)	return false;
	if (!_sPtrLogger->isTypeLocal() && !_sPtrLogger->isTypeWeb())
											return false;

	return true;
}


			/**
			 *		Public static methods
			 *
			 *		Case 0	- Dispatches 'status' message.
			 *
			 *		Web log:	updates dedicated display-fields.
			 *		Local log:	converts to ProcessMsg to append process log.
			 */
void		Service_Logger::addToLog			(std::string	user,
												 LogLevel		level,
												 CppStatus		status)
{
	// sanity
	if (!validateUsage(level))
		return;

	// create status-msg
	_sOs.str("");
	_sOs << status;
	
	// send status-msg
	_sPtrLogger->_log->sendStatusMsg(user, _sOs.str());
	_sPtrLogger->_cnts++;
		
}

			/**
			 *		Public static method
			 *
			 *		Case 1	- Dispatches textual 'log' message.
			 *
			 *		TODO:	- !		Unused arguments
			 */
void		Service_Logger::addToLog			(std::string	user,
												 LogLevel		level,
												 std::string	msg,
												 MessageType	type,
												 bool			skipFile)
{
	// sanity
	if (!validateUsage(level))
		return;
	
	// send status-msg
	_sPtrLogger->_log->sendLogMsg(user, level, msg);
	_sPtrLogger->_cnts++;
}


			/**
			 *		Public static method
			 *
			 *		Case 2	- Dispatches 'iteration' message.
			 *
			 *		For web-log, updates plot(s) and display-field.
			 *		For local-log, converts to ProcessMsg to append process log.
			 */
void		Service_Logger::addToLog			(std::string	user,
												 LogLevel		level,
												 unsigned int	iter,
												 double			F,
												 bool			isNewBest)
{
	// sanity
	if (!validateUsage(level))
		return;
	
	// convert for local log
	_sPtrLogger->_log->sendIterMsg(user, level, iter, F, isNewBest);
	_sPtrLogger->_cnts++;
}

			/**
			 *		Public static method
			 *
			 *		Case 3	- Dispatches 'newbest' message.
			 *
			 *		For web-log, updates dedicated display-fields.
			 *		For local-log, converts to ProcessMsg to append process log.
			 */
void		Service_Logger::addToLog			(std::string	user,
												 LogLevel		level,
												 double			F,
												 std::string	bestSol,
												 std::string	kpi)
{
	// sanity
	if (!validateUsage(level))
		return;
	
	_sPtrLogger->_log->sendNewBestMsg(user, level, F, bestSol, kpi);
	_sPtrLogger->_cnts++;
}




			/**
			 *		Public static method
			 *
			 *		Instantiates logger and returns singleton-instance.
			 */

Service_Logger*		Service_Logger::getInstance			()
{
	if (_sPtrLogger == 0)
		_sPtrLogger = new Service_Logger();
	return _sPtrLogger;
}



// methods

			/**
			 *		Public method
			 *
			 *		Configures Logger with setup-argument.
			 *
			 *		TODO:	- ?		add overwrite-protection
			 */

void		Service_Logger::applySetup			(Setup_LoggerService	setup)
{
	// sanity
	if (!setup.isLogLocal() && !setup.isLogWeb())
	{
		addToLog(typeid(this).name(), ERROR, "Failed to apply new Logger setup. Has no logging configured.");
		return;
	}
	
	// initialise flag
	_hasNewLogger = false;
	
	// prepare
	deleteCurrSetup(setup.isLogLocal(), setup.isLogWeb());
	instantiateLogger(setup.isLogLocal(), setup.isLogWeb(), setup.getLogOwner());
	
	// update members
	initialiseStatics(setup);
	initialiseFields(setup);
	initialiseObjects(setup);
}




