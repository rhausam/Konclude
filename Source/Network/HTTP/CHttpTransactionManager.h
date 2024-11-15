/*
 *		Copyright (C) 2013-2015, 2019 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public
 *		License (LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KONCLUDE_NETWORK_HTTP_CHTTPTRANSACTIONMANAGER_H
#define KONCLUDE_NETWORK_HTTP_CHTTPTRANSACTIONMANAGER_H

// Namespace includes
#include "HttpSettings.h"
#include "CHttpRequest.h"
#include "CHttpResponse.h"

// Library includes


// Other includes


// Logger includes
#include "Logger/CLogger.h"


namespace Konclude {

	namespace Network {

		namespace HTTP {


			/*! 
			*
			*		\class		CHttpTransactionManager
			*		\author		Andreas Steigmiller
			*		\version	0.1
			*		\brief		TODO
			*
			*/
			class CHttpTransactionManager {
				// public methods
				public:
					//! Constructor
					CHttpTransactionManager();

					//! Destructor
					virtual ~CHttpTransactionManager();

					enum ABORT_REASON {
						ABORT_NO_REASON = 1,
						ABORT_DOWNLOAD_SIZE_LIMIT_REACHED = 2
					};


					virtual CHttpResponse* getResponse(CHttpRequest* request) = 0;
					virtual bool abort(CHttpRequest* request, CHttpResponse* response, ABORT_REASON abortReason = ABORT_NO_REASON) = 0;


				// protected methods
				protected:

				// protected variables
				protected:
				// private methods
				private:

				// private variables
				private:
			};


		}; // end namespace HTTP

	}; // end namespace Network

}; // end namespace Konclude


#endif // KONCLUDE_NETWORK_HTTP_CHTTPTRANSACTIONMANAGER_H
