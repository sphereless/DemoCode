//
//  Service_Logger.hpp
//  Service_Logger
//
//  Created by Ashish Aring on 17/02/2017.
//  Copyright © 2017 Ashish Aring. All rights reserved.
//

#ifndef Service_Logger_hpp
#define Service_Logger_hpp

// base includes
#include <iostream>

// own includes
#include "Setup_LoggerService.hpp"
#include "Logger_Web.hpp"
#include "Logger_Local.hpp"
#include "Service_FileOps.hpp"


		/**
		 *		Class Service_Logger 
		 *
		 *		Manages logging to different output-targets and
		 *		provides commonly-shared access to logging.
		 *
		 *		Implements a (modified) Strategy DesignPattern, with 
		 *		ServiceLogger being Context and WebLogger,LocalLogger
		 *		being Strategies, set by clients. Argument to modify
		 *		strategy is (still) Setup_ServiceLogger, though, not
		 *		an instantiated Strategy-object.
		 *
		 *		Differs from classical StrategyPattern by its four 
		 *		static methods 'addToLog', not just one. It allows
		 *		different types of msg to issue: iteration-, status-,
		 *		text- or newbest-msg, each requiring diff. arguments.
		 *
		 *		Having static addToLog-methods permits calling them
		 *		without instance-variable in callers.
		 *
		 *		ServiceLogger is a Singleton.
		 */

class	Service_Logger
{
	
private:
	
	// constructor
							Service_Logger		();
	
	// static members
	static Service_Logger*	_sPtrLogger;							// Pointer to logger-instance
	static LogLevel			_sMinLevel;								// Minimum level of logging
	static bool				_sIsReady;								// Flag for service readiness
	static std::ostringstream _sOs;									// Exclusive stream
	
	
	// members
	unsigned int			_cnts;									// Message counter
	int						_ixFileLog;								// Index of log-file
	bool					_hasFileLog;							// Flags if configured to write to log-file
	bool					_hasNewLogger;							// Flags if new Logger-object was created
	ILogger*				_log;									// Pointer to log-class
	
	
	// methods
	void					deleteCurrSetup		(bool isNewLocal, bool isNewWeb);
	void					instantiateLogger	(bool isNewLocal, bool isNewWeb, std::string owner);
	void					initialiseStatics	(Setup_LoggerService& setup);
	void					initialiseFields	(Setup_LoggerService& setup);
	void					initialiseObjects	(Setup_LoggerService& setup);
	
	// static methods
	static bool				validateUsage		(LogLevel& level);
	
	
public:
	
	// constructor
	virtual					~Service_Logger		();
	
	// static gets
	static LogLevel			getLogLevel			();
	static bool				isLevelLogged		(LogLevel level);
	
	
	// gets
	bool					isTypeLocal			()					const;
	bool					isTypeWeb			()					const;
	bool					hasLogFile			()					const;
	
	
	// sets
	void					setMaxIterations	(unsigned int maxIter);
	void					setTargetFitness	(double targetF);
	
	// methods
	void					applySetup			(Setup_LoggerService setup);
	
	
	// static methods
	static Service_Logger*	getInstance			();
	

							//	Case0 - Status update
	static void				addToLog			(std::string	user,
												 LogLevel		level,
												 CppStatus		status
												);
	
							//	Case1 - Log update
	static void				addToLog			(std::string	user,
												 LogLevel		level,
												 std::string	message,
												 MessageType	type	= ProcessMsg,
												 bool			skipFile= false
												);
	
							//	Case2 - Iteration update
	static void				addToLog			(std::string	user,
												 LogLevel		level,
												 unsigned int	iter,
												 double			F,
												 bool			isNewBest = false
												 );
	
							//	Case3 - NewBest update
	static void				addToLog			(std::string	user,
												 LogLevel		level,
												 double			F,
												 std::string	bestSol,
												 std::string	kpi
												 );
};
#endif /* Service_Logger_hpp */





