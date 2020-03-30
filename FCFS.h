#pragma once
#include <iostream>
#include <string>
#include "libxl.h"
using namespace libxl;
using namespace std;

struct processInfoF
{
	int bornTime;
	int longTime;
	int allTimeInSys;
	int isWork = 3;
	int workingTime = 0;
	int waitingTime = 0;
};

struct dataUserF
{
	int process;
	processInfoF * processIn;
	int first, Id;
	int processEnd = 0;
};

void FCFS(Book * book, Sheet * sheet);
void addNameF(Book * book, Sheet * sheet);
void ansUserF(dataUserF * answer);
void ranFillF(dataUserF * answer);
void displayResultF(Book * book, Sheet * sheet, dataUserF * answer);
