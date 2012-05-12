/* 
 * File:   ParallelAlgorithm.h
 * Author: lubos
 *
 * Created on November 14, 2011, 1:00 PM
 */

#ifndef PARALLELALGORITHM_H
#define	PARALLELALGORITHM_H

#include "mpi.h"
#include "Exception.h"
#include "Playground.h"


//urcuje po kolika DFS cyklech se budou kontrolovat zpravy
#define CHECK_MSG_AMOUNT 	    150
#define STACK_DIV 		    2
#define MIN_STACK_SIZE_FOR_DIVIDE 4

//zadost o praci
#define MSG_WORK_REQUEST 	0
//posilani prace
#define MSG_WORK_TRANSMIT     1
//posilani reseni
#define MSG_SOLUTION_TRANSMIT 2
//posilani priznaku o ukonceni
#define MSG_END_WORK          3
//zaslani peska
#define MSG_TOKEN             4
//nemam praci
#define MSG_WORK_NOWORK       5

#define WHITE 0
#define BLACK 1



class ParallelAlgorithm
{
private:
	ParallelAlgorithm(const ParallelAlgorithm& seq) {}
	ParallelAlgorithm& operator=(const ParallelAlgorithm& seq);

	int 			getDonorProcess();
protected:
	Playground* 	playground;
	Stack<int> 		stck;
	
	int 			token;
	int 			myRank;
	int 			ahdRank;
	int 			countOfProcesses;

	MPI_Request 	incomingWorkRequest;
	MPI_Request		outcomingWorkRequest;

	int 			previousIncomingWorkRequestRank;
	int 			previousOutcomingWorkRequestRank;
	MPI_Status 		status;	

	int 			tokenColor;
	int 			processColor;

	bool			startFlag;

public:
	ParallelAlgorithm(Playground* playground, int argc, char** argv);
    	virtual ~ParallelAlgorithm();

	int communicationLoop();

	void checkWorkRequest();
	void requestMoreWork(void);
	bool checkIncommingWork(void);

	void checkToken(void);
	void checkSolution(void);
	int  checkWorkEnd(void);
	void sendSolution(void);
	void transmitWorkEnd(void);

	void mainLoop(void);
	void start(void);
	void startAcceptData();
	int run(void);
};


#endif
