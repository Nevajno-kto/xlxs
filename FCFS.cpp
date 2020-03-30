#include "FCFS.h"

void FCFS(Book * book, Sheet * sheet){
	addNameF(book, sheet);

	system("chcp 1251");
	int answ;
	dataUserF * answer = new dataUserF;
	//������������ ����� ���������� � Int
	answer->first = 2147483646;
	do {
		cout << "�������� FCFS\n1)��������� �������\n2)��������� ��������" << endl;
		cin >> answ;
	} while (answ != 1 && answ != 2);
	if (answ == 1) {
		ansUserF(answer);
	}
	else {
		ranFillF(answer);
	}
	displayResultF(book, sheet, answer);
}

void addNameF(Book * book, Sheet * sheet) {
	//�������� ���������
	Font * name = book->addFont();
	name->setSize(26);
	Format * format = book->addFormat();
	format->setAlignH(ALIGNH_CENTER);
	format->setFont(name);
	sheet->setMerge(2, 3, 1, 6);
	sheet->writeStr(2, 1, L"FCFS", format);

	//����� ����� ��� ��������
	Format * general = book->addFormat();
	general->setAlignH(ALIGNH_CENTER);
	general->setAlignV(ALIGNV_CENTER);
	general->setBorder(BORDERSTYLE_THIN);
	general->setBorderColor(COLOR_BLACK);

	//���������� ��������
	sheet->setCol(3, 3, 15);
	sheet->writeStr(6, 3, L"��������", general);

	sheet->setCol(4, 4, 22);
	sheet->writeStr(6, 4, L"����� ����������, t", general);

	sheet->setCol(5, 5, 22);
	sheet->writeStr(6, 5, L"����� ��������", general);
}

void ansUserF(dataUserF * answer) {
	do {
		cout << "������� ���-�� ���������" << endl;
		cin >> answer->process;
	} while (answer->process < 1);
	answer->processIn = new processInfoF[answer->process];
	//���������� ������ � ��������
	for (int i = 0; i < answer->process; i++)
	{
		do {
			cout << "������� ����� ��������� �������� " << i + 1 << endl;
			cin >> answer->processIn[i].bornTime;
			if (answer->processIn[i].bornTime < answer->first) {
				answer->first = answer->processIn[i].bornTime;
				answer->Id = i;
			}
		} while (answer->processIn[i].bornTime < 0);

		do {
			cout << "������� ����� ���������� �������� " << i + 1 << endl;
			cin >> answer->processIn[i].longTime;
		} while (answer->processIn[i].longTime < 1);
	}
}

void ranFillF(dataUserF * answer) {
	answer->process = rand() % 6 + 1;

	answer->processIn = new processInfoF[answer->process];

	for (int i = 0; i < answer->process; i++)
	{
		answer->processIn[i].bornTime = rand() % 10 + 1;
		if (answer->processIn[i].bornTime < answer->first) {
			answer->first = answer->processIn[i].bornTime;
			answer->Id = i;
		}
		answer->processIn[i].longTime = rand() % 10 + 1;
	}
}

void displayResultF(Book * book, Sheet * sheet, dataUserF * answer) {
	//����� ����� ��� ��������
	Format * general = book->addFormat();
	general->setAlignH(ALIGNH_CENTER);
	general->setAlignV(ALIGNV_CENTER);
	general->setBorder(BORDERSTYLE_THIN);
	general->setBorderColor(COLOR_BLACK);

	//����� ��� ����� � ��������
	Format * current = book->addFormat();
	current->setFillPattern(FILLPATTERN_SOLID);
	current->setPatternForegroundColor(COLOR_GREEN);
	current->setBorder(BORDERSTYLE_THIN);
	current->setBorderColor(COLOR_BLACK);
	Format * wait = book->addFormat();
	wait->setFillPattern(FILLPATTERN_SOLID);
	wait->setPatternForegroundColor(COLOR_YELLOW);
	wait->setBorder(BORDERSTYLE_THIN);
	wait->setBorderColor(COLOR_BLACK);

	//����������� ������ � ��������
	int i, j;
	for (i = 0, j = 7; i < answer->process; i++, j++) {
		sheet->writeStr(j, 3, L"�������", general);
		sheet->writeNum(j, 4, answer->processIn[i].longTime, general);
		sheet->writeNum(j, 5, answer->processIn[i].bornTime, general);
	}

	bool exit = false;
	int str = 6, time = 0;//������ � ����� � ������� �� �������� ���������� ������ � "�����"
	j = 6;//������� � ������ � �������� �� �������� ���������� ������
	processInfoF * currently = &answer->processIn[answer->Id];//��� �������� �������� ��������
	int * nextProc = new int[answer->process - 1];//���� ������������ �� �������� ���������
	int countNextProc = 0, countCurrentProc = 0;//�������� ��� �������
	while (!exit)
	{
		//��������� �������
		sheet->writeNum(str, j, time, general);
		for (i = 0; i < answer->process; i++) {
			if (time == answer->processIn[i].bornTime) {
				if (currently != &answer->processIn[i]) {
					answer->processIn[i].isWork = 2;
					nextProc[countNextProc] = i;
					countNextProc++;
				}
				else {
					currently->isWork = 1;
				}
			}
			//����������� ������ � ����������� �� ����
			switch (answer->processIn[i].isWork)
			{
			case 1:
				answer->processIn[i].workingTime++;
				sheet->writeStr(str + i + 1, j, L"", current);
				break;
			case 2:
				answer->processIn[i].waitingTime++;
				sheet->writeStr(str + i + 1, j, L"", wait);
				break;
			case 3:
				sheet->writeStr(str + i + 1, j, L"", general);
				break;
			default:
				break;
			}
		}
		//������� �������� �������
		if (currently->workingTime == currently->longTime) {
			answer->processEnd++;
			currently->isWork = 3;
			currently->allTimeInSys = currently->waitingTime + currently->workingTime;
			//������� �� ����� ���� ���������� ��� ��������
			if (answer->processEnd == answer->process) {
				exit = true;
				break;
			}
			//����������� ��������� �� �������� �������
			currently = &answer->processIn[nextProc[countCurrentProc]];
			countCurrentProc++;
			currently->isWork = 1;
		}
		time++;
		j++;
	}
	//
	sheet->writeStr(str, j + 1, L"����� ����� � �������", general);
	sheet->setCol(j + 1, j + 1, 23);
	sheet->writeStr(str, j + 2, L"���������� �����", general);
	sheet->setCol(j + 2, j + 2, 22);
	sheet->writeStr(str, j + 3, L"��������� ������������", general);
	sheet->setCol(j + 3, j + 3, 25);
	sheet->writeStr(str, j + 4, L"�������� �����", general);
	sheet->setCol(j + 4, j + 4, 22);
	for (int k = 0; k < answer->process; k++) {
		sheet->writeNum(str + 1 + k, j + 1,answer->processIn[k].allTimeInSys, general);
		sheet->writeNum(str + 1 + k, j + 2, answer->processIn[k].allTimeInSys - answer->processIn[k].longTime, general);
		sheet->writeNum(str + 1 + k, j + 3, answer->processIn[k].longTime / double(answer->processIn[k].allTimeInSys), general);
		sheet->writeNum(str + 1 + k, j + 4, answer->processIn[k].allTimeInSys / double(answer->processIn[k].longTime), general);
	}
	//������� ������
	currently = nullptr;
	delete[] nextProc;
	delete[] answer->processIn;

}