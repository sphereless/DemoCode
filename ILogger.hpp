//
//  ILogger.hpp
//  Optimisers
//
//  Created by Ashish Aring on 04.03.21.
//  Copyright © 2021 Sphereless. All rights reserved.
//

#ifndef ILogger_hpp
#define ILogger_hpp


// own includes
#include "Enums.h"


		/**
		 *		Interface 'ILogger'
		 *
		 *		Defines abstract and default methods for loggers.
		 */

class	ILogger
{
	
protected:
	
	// abstract methods
	virtual void		bUpdateCounter		(MessageType type)		= 0;		// To update message-counter for a specific log.
	virtual void		bResetCounter		(MessageType type)		= 0;		// To reset message-counter for a specific log.
	virtual void		bResetAllCounters	()						= 0;		// To reset message-counters for all logs.
	
public:
	
	// constructor
	virtual				~ILogger			()  {};
	
	
	// abstract gets
	virtual std::string	getLogOwner			() const				= 0;
	virtual LogType		getLogType			() const				= 0;
	virtual bool		isFull				(MessageType type)const = 0;		// Checks if requested log is full
	
	
	// abstract sets
	virtual void		setNewLogOwner		(std::string newOwner)	= 0;
	// default sets
	virtual void		setMaxIterations	(unsigned int maxIter)	{ };
	virtual void		setTargetFitness	(double targetF)		{ };
	
	
	// abstract methods
	virtual	void		clearLog			(MessageType type)		= 0;		// Empty a specific log.
	virtual void		pumpLogStatistic	(std::ostringstream& os) const = 0;	// Print log-statistics to OS.
	virtual void		sendStatusMsg		(std::string user,
											 std::string msg)		= 0;
	virtual void		sendLogMsg			(std::string user,
											 LogLevel	 level,
											 std::string logMsg)	= 0;
	virtual void		sendIterMsg			(std::string user,
											 LogLevel	 level,
											 unsigned int iter,
											 double		 F,
											 bool		 isNewBest = false)	= 0;
	virtual void		sendNewBestMsg		(std::string user,
											 LogLevel	 level,
											 double		 F,
											 std::string best,
											 std::string kpi)		= 0;

	
};

#endif /* ILogger_hpp */
