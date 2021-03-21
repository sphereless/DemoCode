//
//  Logger_Web.hpp
//  Service_Logger
//
//  Created by Ashish Aring on 17/02/2017.
//  Copyright © 2017 Ashish Aring. All rights reserved.
//

#ifndef Logger_Web_hpp
#define Logger_Web_hpp



// base includes
#include <sstream>

// pepper includes
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/var_dictionary.h"

// own includes
#include "Logger_Base.hpp"




		/**
		 *		Class Logger_Web :	Logger_Base, ILogger
		 *
		 *		Dispatches log-message to Web through pp.
		 *
		 *		Class is specific strategy in Strategy Design-Pattern
		 *		with ServiceLogger as Context.
		 *
		 *		Maintains multiple dictionaries, one for each msg-type,
		 *		and a pointer to web-instance.
		 */

class	LoggerWeb		: public Logger
{
private:
	
	// static members
	static const std::string	_sKeyType,			// identifier of field "type"
								_sKeyMsg,			// identifier of field "message"
								_sKeyFit,			// identifier of field "fitness"
								_sKeyLast,			// identifier of field "isLast"
								_sKeyIter,			// identifier of field "iteration"
								_sKeyMaxIter,		// identifier of field "max iteration"
								_sKeyTargFit,		// identifier of field "target fitness"
								_sKeyBestSol,		// identifier of field "best solution"
								_sKeyKpi,			// identifier of field "kpi"
	
								_sMsgTypeInfo,		// identifier for message-type "info"
								_sMsgTypeFile,		// identifier for message-type "file"
								_sMsgTypeIter,		// identifier for message-type "iteration"
								_sMsgTypeBest;		// identifier for message-type "best"
	
	
	// members
	bool						_hasInstance;		// flags if 'enableLogging()' was called'
	pp::Instance*				_webInst;			// instance set in 'enableLogging()'
	
	pp::VarDictionary			_msgInfo;			// web-message of type info
	pp::VarDictionary			_msgFile;			// web-message of type file
	pp::VarDictionary			_msgIteration;		// web-message of type iteration (updates progress-div, plot-canvas)
	pp::VarDictionary			_msgNewBest;		// web-message of type absBest	 (updates absBest-, ActVsTargets-, Solution-divs)
	pp::VarDictionary			_msgError;			// web-message of type error
	
	// methods
	void						printToWeb			(MessageType log);
	void						raiseErrorMsg		(std::string msg);
	
	void						assignToStatusDict	(std::string	newStatus);
	void						assignToInfoDict	(std::string	user,
													 LogLevel		level,
													 std::string	msg
													);
	void						assignToIterDict	(unsigned int	iter,
													 double			F
													);
	void						assignToBestDict	(double			F,
													 std::string	bestSol,
													 std::string	kpi
													);
	
public:
	
	// constructor
								LoggerWeb			(std::string logOwner);
	virtual						~LoggerWeb			();
	
	
	// sets
	void						enableWebLogging	(std::string user, pp::Instance&);		// Artefact ?
	void						disableWebLogging	(std::string user);						// Artefact ?
	void						setMaxIterations	(unsigned int maxIter);					// Artefact ?
	void						setTargetFitness	(double targetF);						// Artefact ?
	
	
	// overridden methods
	void						sendStatusMsg		(std::string user,
													 std::string	newStatus
													);
	void						sendLogMsg			(std::string	user,
													 LogLevel		level,
													 std::string	logMsg
													);
	void						sendIterMsg			(std::string	user,
													 LogLevel		level,
													 unsigned int	iter,
													 double			F,
													 bool			isNewBest = false
													);
	void						sendNewBestMsg		(std::string user,
													 LogLevel	 level,
													 double		 F,
													 std::string bestSol,
													 std::string kpi
													);
};

#endif /* Logger_Web_hpp */
