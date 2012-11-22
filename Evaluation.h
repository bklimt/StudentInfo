
/*
 * Evaluation.h
 *
 * This class contains the data for an evaluation record
 *
 */

// keep the file from being included twice
#ifndef EVALUATION_H
#define EVALUATION_H

#include "Date.h"

class CEvaluation {
	public:
		// constructors
		CEvaluation();
		CEvaluation( const CEvaluation& otherEval );
		CEvaluation operator=( const CEvaluation& otherEval );

		// the data for this evaluation
		int evaluationType;
		CDate evaluationDate;
		CDate reportDate;
		int evaluationEligible;
		CDate dismissalDate;
};

#endif