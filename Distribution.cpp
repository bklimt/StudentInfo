
#include "Distribution.h"

CDistribution::CDistribution() {
	documentID = -1;
	distributionMethodID = -1;
	initialDistributionID = -1;
}

CDistribution::CDistribution( const CDistribution& other ) {
	distributionDate = other.distributionDate;
	documentID = other.documentID;
	distributionMethodID = other.distributionMethodID;
	initialDistributionID = other.initialDistributionID;
}

CDistribution CDistribution::operator =( const CDistribution& other ) {
	distributionDate = other.distributionDate;
	documentID = other.documentID;
	distributionMethodID = other.distributionMethodID;
	initialDistributionID = other.initialDistributionID;
	return (*this);
}
