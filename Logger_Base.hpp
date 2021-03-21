//
//  Logger_Base.hpp
//  Service_Logger
//
//  Created by Ashish Aring on 17/02/2017.
//  Copyright © 2017 Ashish Aring. All rights reserved.
//

#ifndef Logger_Base_hpp
#define Logger_Base_hpp




/* base incldues */
#include <iostream>
#include <sstream>
#include <iomanip>


/* own includes */
#include "ILogger.hpp"
#include "Epoch.hpp"
#include "Constants.hpp"



		/**
		 *		Abstract class 'Logger'
		 *
		 *		Implements counter-functionality for individual logs.
		 *
		 *		Functions as base to every Strategy in DesignPattern.
		 *		Has public method to print log-statistics and a static
		 *		method to create a 'standardised' log-message.
		 *
		 *		Important:
		 *		Inherits ILogger interface, but implements it only partially.
		 *		Specific Logger-strategies must implement addToLog-methods.
		 *
		 */

class	Logger	: public ILogger
{
private:
	
	// static members
	static const unsigned int	_sMaxN;					// max. number of entries
	static const unsigned int	_sWTimeStr;				// time-string formatting
	static const unsigned int	_sWClassStr;			// class-string formatting
	static const unsigned int	_sWLevelStr;			// level-string formatting
	static const unsigned int	_sWMsgStr;				// message-string formatting
	
	
	// members
	std::string					_logOwner;				// owner of this log
	unsigned int				_nSLogs,				// Number of entries in Status-Log
								_nPLogs,				// Number of entries in Process-Log
								_nFLogs,				// Number of entries in File-Log
								_nILogs,				// Number of entries in Iter-Log
								_nBLogs;				// Number of entries in Best-Log
	
protected:
	
	// constructor
								Logger					(std::string logOwner,
														 std::string logClassName,
														 LogType	 type
														);
	
	// members
	const std::string			_bLogClass;				// class of this log
	const LogType				_bLogType;				// Type of log (local, web)
	mutable std::ostringstream	_bStream;				// stream for outputs

	
	// overridden methods
	void						bUpdateCounter			(MessageType logtype);			// from ILogger
	void						bResetCounter			(MessageType logtype);			// from ILogger
	void						bResetAllCounters		();								// from ILogger
	
public:
	
	// constructor
	virtual						~Logger					();
	
	// overridden gets
	std::string					getLogOwner				()					const;
	LogType						getLogType				()					const;
	bool						isFull					(MessageType type)	const;
	
	// gets
	unsigned int				getNumStatusEntries		() const;
	unsigned int				getNumProcessLogEntries	() const;
	unsigned int				getNumFileLogEntries	() const;
	unsigned int				getNumIterLogEntries	() const;
	unsigned int				getNumBestLogEntries	() const;
	virtual std::string			getLog					() const;
	
	// overridden sets
	void						setNewLogOwner			(std::string);
	
	// overridden methods
	void						clearLog				(MessageType type);
	void						pumpLogStatistic		(std::ostringstream& os) const;	// from ILogger
	
	// static methods
	static void					composeLogEntry			(std::ostringstream&	os,
														 std::string			user,
														 LogLevel				level,
														 std::string			msg
														);
	
};

#endif /* Logger_Base_hpp */
