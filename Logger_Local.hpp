//
//  Logger_Local.hpp
//  Service_Logger
//
//  Created by Ashish Aring on 17/02/2017.
//  Copyright © 2017 Ashish Aring. All rights reserved.
//

#ifndef Logger_Local_hpp
#define Logger_Local_hpp



// own includes
#include "Logger_Base.hpp"



		/**
		 *		Class Logger_Local :	Logger_Base, ILogger
		 *
		 *		Dispatches log-message to Console.
		 *
		 *		Class is specific strategy in Strategy Design-Pattern
		 *		with ServiceLogger as Context.
		 */

class	LoggerLocal	: public Logger
{
private:
	
	// methods
	void						printToConsole	(MessageType log);
	
protected:
	
	
public:
	
	// constructor
								LoggerLocal		(std::string logOwner);
	virtual						~LoggerLocal	();
	
	
	// overridden methods
	void						sendStatusMsg	(std::string user,
												 std::string msg);
	void						sendLogMsg		(std::string user,
												 LogLevel	 level,
												 std::string logMsg
												);
	void						sendIterMsg		(std::string user,
												 LogLevel	 level,
												 unsigned int iter,
												 double		 F,
												 bool		 isNewBest = false
												);
	void						sendNewBestMsg	(std::string user,
												 LogLevel	 level,
												 double		 F,
												 std::string bestSol,
												 std::string kpi
												);
};

#endif /* Logger_Local_hpp */
