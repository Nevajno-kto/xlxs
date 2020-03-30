#include "FCFS.h"

void FCFS(Book * book, Sheet * sheet){
	addNameF(book, sheet);

	system("chcp 1251");
	int answ;
	dataUserF * answer = new dataUserF;
	//Максимальное число хранящиеся в Int
	answer->first = 2147483646;
	do {
		cout << "Алгоритм FCFS\n1)Заполнить вручную\n2)Заполнить рандомно" << endl;
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
	//Название алгоритма
	Font * name = book->addFont();
	name->setSize(26);
	Format * format = book->addFormat();
	format->setAlignH(ALIGNH_CENTER);
	format->setFont(name);
	sheet->setMerge(2, 3, 1, 6);
	sheet->writeStr(2, 1, L"FCFS", format);

	//Общий стиль для страницы
	Format * general = book->addFormat();
	general->setAlignH(ALIGNH_CENTER);
	general->setAlignV(ALIGNV_CENTER);
	general->setBorder(BORDERSTYLE_THIN);
	general->setBorderColor(COLOR_BLACK);

	//Оформление страницы
	sheet->setCol(3, 3, 15);
	sheet->writeStr(6, 3, L"Процессы", general);

	sheet->setCol(4, 4, 22);
	sheet->writeStr(6, 4, L"Время выполнения, t", general);

	sheet->setCol(5, 5, 22);
	sheet->writeStr(6, 5, L"Время пояления", general);
}

void ansUserF(dataUserF * answer) {
	do {
		cout << "Введите кол-во процессов" << endl;
		cin >> answer->process;
	} while (answer->process < 1);
	answer->processIn = new processInfoF[answer->process];
	//Заполнение данных о процессе
	for (int i = 0; i < answer->process; i++)
	{
		do {
			cout << "Введите время появления процесса " << i + 1 << endl;
			cin >> answer->processIn[i].bornTime;
			if (answer->processIn[i].bornTime < answer->first) {
				answer->first = answer->processIn[i].bornTime;
				answer->Id = i;
			}
		} while (answer->processIn[i].bornTime < 0);

		do {
			cout << "Введите время выполнения процесса " << i + 1 << endl;
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
	//Общий стиль для страницы
	Format * general = book->addFormat();
	general->setAlignH(ALIGNH_CENTER);
	general->setAlignV(ALIGNV_CENTER);
	general->setBorder(BORDERSTYLE_THIN);
	general->setBorderColor(COLOR_BLACK);

	//Стили для ячеек с квантами
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

	//Отображение данных о процессе
	int i, j;
	for (i = 0, j = 7; i < answer->process; i++, j++) {
		sheet->writeStr(j, 3, L"Процесс", general);
		sheet->writeNum(j, 4, answer->processIn[i].longTime, general);
		sheet->writeNum(j, 5, answer->processIn[i].bornTime, general);
	}

	bool exit = false;
	int str = 6, time = 0;//Строка в ексле с которой мы начинаем записывать данные и "время"
	j = 6;//Столбец в екселе с которого мы начинаем записывать данные
	processInfoF * currently = &answer->processIn[answer->Id];//Для хранения текущего процесса
	int * nextProc = new int[answer->process - 1];//Сюда записываются ид следущих процессов
	int countNextProc = 0, countCurrentProc = 0;//Счётчики для массива
	while (!exit)
	{
		//Нумерация квантов
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
			//Окрашивание ячейки в зависимости от типа
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
		//Находим следущий процесс
		if (currently->workingTime == currently->longTime) {
			answer->processEnd++;
			currently->isWork = 3;
			currently->allTimeInSys = currently->waitingTime + currently->workingTime;
			//Выходим из цикла если отработали все процессы
			if (answer->processEnd == answer->process) {
				exit = true;
				break;
			}
			//Присваиваем указатель на следущий процесс
			currently = &answer->processIn[nextProc[countCurrentProc]];
			countCurrentProc++;
			currently->isWork = 1;
		}
		time++;
		j++;
	}
	//
	sheet->writeStr(str, j + 1, L"Общее время в системе", general);
	sheet->setCol(j + 1, j + 1, 23);
	sheet->writeStr(str, j + 2, L"Потерянное время", general);
	sheet->setCol(j + 2, j + 2, 22);
	sheet->writeStr(str, j + 3, L"Отношение реактивности", general);
	sheet->setCol(j + 3, j + 3, 25);
	sheet->writeStr(str, j + 4, L"Штрафное время", general);
	sheet->setCol(j + 4, j + 4, 22);
	for (int k = 0; k < answer->process; k++) {
		sheet->writeNum(str + 1 + k, j + 1,answer->processIn[k].allTimeInSys, general);
		sheet->writeNum(str + 1 + k, j + 2, answer->processIn[k].allTimeInSys - answer->processIn[k].longTime, general);
		sheet->writeNum(str + 1 + k, j + 3, answer->processIn[k].longTime / double(answer->processIn[k].allTimeInSys), general);
		sheet->writeNum(str + 1 + k, j + 4, answer->processIn[k].allTimeInSys / double(answer->processIn[k].longTime), general);
	}
	//Очистка памяти
	currently = nullptr;
	delete[] nextProc;
	delete[] answer->processIn;

}