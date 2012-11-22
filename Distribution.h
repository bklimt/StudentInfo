
/*
 *
 */

// keep the file from being included twice
#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include "Date.h"

class CDistribution {
	public:
		// constructors
		CDistribution();
		CDistribution( const CDistribution& other );
		CDistribution operator=( const CDistribution& other );

		CDate distributionDate;
		int documentID;
		int distributionMethodID;
		int initialDistributionID;
};

#endif