
#include "Evaluation.h"

CEvaluation::CEvaluation() {
	evaluationType = -1;
	evaluationEligible = -1;
}

CEvaluation::CEvaluation( const CEvaluation& otherEval ) {
	evaluationType = otherEval.evaluationType;
	evaluationEligible = otherEval.evaluationEligible;
	evaluationDate = otherEval.evaluationDate;
	reportDate = otherEval.reportDate;
	dismissalDate = otherEval.dismissalDate;
}

CEvaluation CEvaluation::operator =( const CEvaluation& otherEval ) {
	evaluationType = otherEval.evaluationType;
	evaluationEligible = otherEval.evaluationEligible;
	evaluationDate = otherEval.evaluationDate;
	reportDate = otherEval.reportDate;
	dismissalDate = otherEval.dismissalDate;
	return (*this);
}
