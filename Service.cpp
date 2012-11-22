
#include "Service.h"

CService::CService() {
	specificServiceType = -1;
	serviceType = -1;
}

CService::CService( const CService& otherService ) {
	specificServiceType = otherService.specificServiceType;
	serviceType = otherService.serviceType;
	serviceTime = otherService.serviceTime;
}

CService CService::operator=( const CService& otherService ) {
	specificServiceType = otherService.specificServiceType;
	serviceType = otherService.serviceType;
	serviceTime = otherService.serviceTime;
	return (*this);
}
