#pragma once
#include <iostream>
#include <string>
#include "libxl.h"
using namespace libxl;
using namespace std;

struct processInfo
{
	int bornTime;
	int longTime;
	int prioritet = 0;
	int isWork = 3;
	int allTimeInSys;
	int workingTime = 0;
	int waitingTime = 0;
};

struct dataUser
{
	int process, dA, dB;
	processInfo * processIn;
	int first,Id;
	int processEnd = 0;
};

void RRpr(Book * book, Sheet * sheet);
void addName(Book * book, Sheet * sheet);
void ansUser(dataUser * answer);
void ranFill(dataUser * answer);
void displayResult(Book * book, Sheet * sheet,dataUser * answer);