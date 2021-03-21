//
//  Logger_Web.cpp
//  Service_Logger
//
//  Created by Ashish Aring on 17/02/2017.
//  Copyright © 2017 Ashish Aring. All rights reserved.
//

#include "Logger_Web.hpp"


//	static members

const std::string				LoggerWeb::_sKeyType		= "type";
const std::string				LoggerWeb::_sKeyMsg		= "msg";
const std::string				LoggerWeb::_sKeyFit		= "F";
const std::string				LoggerWeb::_sKeyLast		= "isLast";
const std::string				LoggerWeb::_sKeyIter		= "iter";
const std::string				LoggerWeb::_sKeyMaxIter	= "maxIter";
const std::string				LoggerWeb::_sKeyTargFit	= "targetF";
const std::string				LoggerWeb::_sKeyBestSol	= "Best";
const std::string				LoggerWeb::_sKeyKpi		= "Targets";

const std::string				LoggerWeb::_sMsgTypeInfo	= "info";
const std::string				LoggerWeb::_sMsgTypeFile	= "file";
const std::string				LoggerWeb::_sMsgTypeIter	= "iter";
const std::string				LoggerWeb::_sMsgTypeBest	= "newbest";




// constructor

LoggerWeb::LoggerWeb			(std::string logOwner)
			:	Logger			(logOwner,	typeid(this).name(), WEB),
				_hasInstance	(false),
				_webInst		(0),
				_msgInfo		(pp::VarDictionary()),
				_msgFile		(pp::VarDictionary()),
				_msgIteration	(pp::VarDictionary()),
				_msgNewBest		(pp::VarDictionary()),
				_msgError		(pp::VarDictionary())
{
	// report
	sendLogMsg(_bLogClass, INFO, "Log owner is " + getLogOwner());
	
	// configure info-message
	_msgInfo.Set	(_sKeyType, _sMsgTypeInfo);
	_msgInfo.Set	(_sKeyMsg,  "");
	_msgInfo.Set	(_sKeyFit,  "0");
	_msgInfo.Set	(_sKeyLast, "false");			// important for closing file
	
	// configure file-message
	_msgFile.Set	(_sKeyType,	_sMsgTypeFile);
	_msgFile.Set	(_sKeyMsg,	"");
		
	// configure iteration-message
	_msgIteration.Set(_sKeyType,	_sMsgTypeIter);
	_msgIteration.Set(_sKeyIter,	"0");
	_msgIteration.Set(_sKeyMaxIter,	"0");
	_msgIteration.Set(_sKeyFit,	"0");
	_msgIteration.Set(_sKeyLast,	"false");
	
	// configure newBest-message
	_msgNewBest.Set	(_sKeyType,	_sMsgTypeBest);
	_msgNewBest.Set	(_sKeyFit,	"0");
	_msgNewBest.Set	(_sKeyBestSol,"");
	_msgNewBest.Set	(_sKeyKpi,	"");
	
	// configure error-message
	_msgError.Set	(_sKeyType, _sMsgTypeInfo);
	_msgError.Set	(_sKeyMsg,  "");
}
LoggerWeb::~LoggerWeb	()
{
	_bStream.str	("");
	_bStream << "Deleting web-logger. Statistics: ";
	pumpLogStatistic(_bStream);
	sendLogMsg		(_bLogClass, INFO, _bStream.str());
}


// sets

			/**
			 *		Public set
			 *
			 *		Assigns instance-argument to member-pointer.
			 *
			 *		Method specific to LoggerWeb to avoid instance-argument at construction.
			 *		Assignee must be LogOwner.
			 */

void		LoggerWeb::enableWebLogging	(std::string user, pp::Instance& instance)
{
	// sanity
	if (_hasInstance)
	{
		raiseErrorMsg("Failed to enable web-logging. Need to disable current assigned first.");
		return;
	}
	if (getLogOwner().compare(user) != 0)
	{
		raiseErrorMsg("Failed to enable web-logging. Assignee must be log-owner.");
		return;
	}
	
	_webInst = &instance;
	_hasInstance = true;
		
	sendLogMsg(_bLogClass, INFO, "Enabled web-logging.");
}

			/**
			 *		Public set
			 *
			 *		Removes assigned instance in member-pointer.
			 *
			 *		Assignee must be LogOwner. 
			 *		Method specific to LoggerWeb.
			 */

void		LoggerWeb::disableWebLogging	(std::string user)
{
	if (!_hasInstance)
	{
		raiseErrorMsg( "Failed to disable logging. It is not enabled.");
		return;
	}
	if (getLogOwner().compare(user) != 0)
	{
		raiseErrorMsg("Failed to disable web-logging. Caller must be log-owner.");
		return;
	}
	
	_webInst = 0;
	_hasInstance = false;
	
	sendLogMsg(_bLogClass, INFO, "Disabled logging.");
}

			/**
			 *		Public set
			 *	
			 *		Assigns max-iter in dictionary 'Update'.
			 */

void		LoggerWeb::setMaxIterations	(unsigned int maxIter)
{
	if (maxIter == 0)
	{
		raiseErrorMsg( "Failed to set maxIterations. Value must be greater than 0.");
		return;
	}
	
	_msgIteration.Set(_sKeyMaxIter,	std::to_string(maxIter));
}

			/**
			 *		Public set
			 *
			 *		Assigns target-fitness in dictionary 'Update'.
			 *
			 *		TODO:	- !		Any other message-type to take 'targetF'?
			 */

void		LoggerWeb::setTargetFitness	(double targetF)
{
	if (targetF != targetF)
	{
		raiseErrorMsg("Failed to set target-fitness. Incompatible value: NaN.");
		return;
	}
}



// methods

			/**
			 *		Private method
			 *
			 *		Dispatches message to browser and updates counter.
			 */

void		LoggerWeb::printToWeb			(MessageType log)
{
	if		(log == StatusMsg)	raiseErrorMsg("Failed to print StatusMsg. No such dictionary defined.");
	else if	(log == ProcessMsg)	_webInst->PostMessage(_msgInfo);
	else if (log == FileMsg)	_webInst->PostMessage(_msgFile);
	else if (log == IterMsg)	_webInst->PostMessage(_msgIteration);
	else if (log == NewBestMsg)	_webInst->PostMessage(_msgNewBest);
	else
	{
		raiseErrorMsg("Failed to print to Web. Message-type ("+std::to_string(log)+") unrecognised.");
		return;
	}
	
	bUpdateCounter(log);
}

			/**
			 *		Private method
			 *
			 *		Sends error-message to Web.
			 */

void		LoggerWeb::raiseErrorMsg		(std::string msg)
{
	_msgError.Set(_sKeyMsg, msg);
	_webInst->PostMessage(_msgError);
}



			/**
			 *		Private method
			 *
			 *		Updates content of status-dictionary with argument.
			 */

void		LoggerWeb::assignToStatusDict	(std::string newStatus)
{
	raiseErrorMsg("Failed to update Status-dictionary. No such dictionary defined.");
}
			/**
			 *		Private method
			 *
			 *		Updates content of info-dictionary with arguments.
			 */

void		LoggerWeb::assignToInfoDict	(std::string	user,
											 LogLevel		level,
											 std::string	msg)
{
	_bStream.str("");
	composeLogEntry(_bStream, user, level, msg);
	
	_msgInfo.Set(_sKeyMsg, _bStream.str());
}

			/**
			 *		Private method
			 *
			 *		Updates content of iteration-dictionary with arguments.
			 */

void		LoggerWeb::assignToIterDict	(unsigned int	iter,
											 double			F)
{
	_msgIteration.Set(_sKeyIter, std::to_string(iter));
	_msgIteration.Set(_sKeyFit,  std::to_string(F));
}


			/**
			 *		Private method
			 *
			 *		Updates content of best-dictionary with arguments.
			 *
			 *		Note:
			 *		Unfortunately, the sender is in charge of composing the strings
			 *		for bestSol and kpi. This, in both cases, is an (unformatted) table.
			 *		Room for improvement !!
			 *
			 *		TODO:	- ?		Have this method compose best-, kpi-tables?
			 */

void		LoggerWeb::assignToBestDict	(double			F,
											 std::string	bestSol,
											 std::string	kpi)
{
	_msgNewBest.Set	(_sKeyFit,  std::to_string(F));
	_msgNewBest.Set	(_sKeyBestSol, bestSol);
	_msgNewBest.Set	(_sKeyKpi, kpi);
}

			/**
			 *		Public overridden method
			 *
			 *		Sends status-update message to web.
			 */

void		LoggerWeb::sendStatusMsg		(std::string user,
											 std::string newStatus)
{
	assignToStatusDict(newStatus);
	printToWeb(StatusMsg);
}

			/**
			 *		Public overridden method
			 *
			 *		Sends log-message to local log.
			 */

void		LoggerWeb::sendLogMsg			(std::string	user,
											 LogLevel		level,
											 std::string	logMsg)
{
	assignToInfoDict(user, level, logMsg);
	printToWeb(ProcessMsg);
}


			/**
			 *		Public overridden method
			 *
			 *		Sends (two) iteration-messages to web.
			 */

void		LoggerWeb::sendIterMsg			(std::string	user,
											 LogLevel		level,
											 unsigned int	iter,
											 double			F,
											 bool			isNewBest
											)
{
	// send iter-message
	assignToIterDict(iter, F);
	printToWeb(IterMsg);
	
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
			 *		Sends (two) newBest-messages to web.
			 */

void		LoggerWeb::sendNewBestMsg		(std::string user,
											 LogLevel	 level,
											 double		 F,
											 std::string bestSol,
											 std::string kpi)
{
	// send best-message
	assignToBestDict(F, bestSol, kpi);
	printToWeb(NewBestMsg);
	
	// send log-msg
	_bStream.str("");
	_bStream << std::setw(Constants::nomWidth) << std::right << "New best F = ";
	_bStream << std::setw(Constants::nomWidth) << std::right << std::setprecision(3) << (F > Constants::LimitELayout ? std::scientific : std::fixed) << F;
	sendLogMsg(user, level, _bStream.str());
	
}





