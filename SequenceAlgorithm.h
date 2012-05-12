/* 
 * File:   SequenceAlgorithm.h
 * Author: lubos
 *
 * Created on October 18, 2011, 2:44 AM
 */

#ifndef SEQUENCEALGORITHM_H
#define	SEQUENCEALGORITHM_H

#include "Exception.h"
#include "Playground.h"

class SequenceAlgorithm
{
private:
	SequenceAlgorithm(const SequenceAlgorithm& seq) {}
	SequenceAlgorithm& operator=(const SequenceAlgorithm& seq);

protected:
	Playground* 	playground;
	Stack<int> 		stck;

public:
	SequenceAlgorithm(Playground* playground);
    	virtual ~SequenceAlgorithm();

    	void run();
};

#endif	/* SEQUENCEALGORITHM_H */
