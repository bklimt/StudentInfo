
/*
 * Service.h
 *
 * This class contains the data for a single instructional, ESY, or
 *   related service
 *
 */

// keep the file from being included twice
#ifndef SERVICE_H
#define SERVICE_H

#include "Date.h"

class CService {
	public:
		// constructors
		CService();
		CService( const CService& otherService );
		CService operator=( const CService& otherService );

		// the data for this service
		int specificServiceType;
		int serviceType;
		string serviceTime;
};

#endif