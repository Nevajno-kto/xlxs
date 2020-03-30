#include "RR - priora.h"
#pragma warning (disable : 4996)//предупреждение об использование функции с _s
#pragma warning (disable : 4700)
void addName(Book * book,Sheet * sheet) {
	//Название алгоритма
	Font * name = book->addFont();
	name->setSize(26);
	Format * format = book->addFormat();
	format->setAlignH(ALIGNH_CENTER);
	format->setFont(name);
	sheet->setMerge(2, 3, 1, 6);
	sheet->writeStr(2, 1, L"RR - с приоритетом", format);

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

	sheet->setCol(5, 5, 18);
	sheet->writeStr(6, 5, L"Приоритет P0", general);

	sheet->setCol(6, 6, 22);
	sheet->writeStr(6, 6, L"Время пояления", general);
}

void RRpr(Book * book, Sheet * sheet) {
	//Отобразили стандартные надписи
	addName(book,sheet);

	system("chcp 1251");
	int answ;
	dataUser * answer = new dataUser;
	//Максимальное число хранящиеся в Int
	answer->first = 2147483646;
	do {
		cout << "Алгоритм RR - с приоритетом\n1)Заполнить вручную\n2)Заполнить рандомно" << endl;
		cin >> answ;
	} while (answ != 1 && answ != 2);

	if (answ == 1) {
		ansUser(answer);
	}
	else {
		ranFill(answer);
	}
	displayResult(book, sheet, answer);
}

void ansUser(dataUser * answer) {
	do {
		cout << "Введите кол-во процессов" << endl;
		cin >> answer->process;
	} while (answer->process < 1);
	answer->processIn = new processInfo[answer->process];
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
			cout << "Введите время выполнения процесса " << i + 1<< endl;
			cin >> answer->processIn[i].longTime;
		} while (answer->processIn[i].longTime < 1);
	}
	do
	{
		cout << "Введите dA" << endl;
		cin >> answer->dA;
		cout << "Введите dB" << endl;
		cin >> answer->dB;
	} while (((answer->dB / double(answer->dA)) > 1.0) || ((answer->dB / double(answer->dA)) < 0));
}

void ranFill(dataUser * answer) {
	answer->process = rand() % 6 + 1;

	answer->processIn = new processInfo[answer->process];

	for (int i = 0; i < answer->process; i++)
	{
		answer->processIn[i].bornTime = rand() % 10 + 1;
		if (answer->processIn[i].bornTime < answer->first) {
			answer->first = answer->processIn[i].bornTime;
			answer->Id = i;
		}
		answer->processIn[i].longTime = rand() % 10 + 1;
	}

	do
	{
		answer->dA = rand() % 10 + 1; answer->dB = rand() % 10 + 1;
	} while (((answer->dB / answer->dA) > 1.0) || ((answer->dB / answer->dA) < 0));
}

void displayResult(Book * book, Sheet * sheet,dataUser * answer) {
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
	
	int i,j;
	for (i = 0,j = 7; i < answer->process; i++,j++) {
		/*int cSize = 9;
		string str = "Процесс ";
		std::wstring wc(cSize, L'#');
		mbstowcs(&wc[0], str.c_str(), cSize);*/
		sheet->writeStr(j, 3, L"Процесс", general);
		sheet->writeNum(j, 4, answer->processIn[i].longTime,general);
		sheet->writeNum(j, 5, 0, general);
		sheet->writeNum(j, 6, answer->processIn[i].bornTime, general);
		sheet->writeStr(j + 8, 6, L"Процесс", general);
	}
	sheet->writeStr(j + 2, 3, L"dB", general);
	sheet->writeNum(j + 2, 4, answer->dB, general);
	sheet->writeStr(j + 3, 3, L"dA", general);
	sheet->writeNum(j + 3, 4, answer->dA, general);
	sheet->writeStr(j + 4, 3, L"dB / dA", general);
	sheet->writeNum(j + 4, 4, answer->dB / double(answer->dA), general);

	//7 столбец 6 строка
	bool exit = false;
	int str = 6, time = 0;
	j = 7;
	//Для хранения текущего процесса
	processInfo * currently = &answer->processIn[answer->Id];
	while(!exit) {
		//Нумерация квантов
		sheet->writeNum(str, j, time, general);
			for (i = 0; i < answer->process; i++) {
				if (time == answer->processIn[i].bornTime) {
					if (currently != &answer->processIn[i]) {
						answer->processIn[i].isWork = 2;
					}
					else {
						currently->isWork = 1;
					}
				}
				//Заполнение приоритета
				sheet->writeNum(str + i + 9, j, answer->processIn[i].prioritet, general);
				//Окрашивание ячейки в зависимости от типа
				switch (answer->processIn[i].isWork)
				{
				case 1:
					answer->processIn[i].prioritet += answer->dB;
					answer->processIn[i].workingTime++;
					sheet->writeStr(str + i + 1, j, L"", current);
					break;
				case 2:
					answer->processIn[i].prioritet += answer->dA;
					sheet->writeStr(str + i + 1, j, L"", wait);
					answer->processIn[i].waitingTime++;
					break;
				case 3:
					sheet->writeStr(str + i + 1, j, L"", general);
					break;
				default:
					break;
				}
			}

			//проверяем нет ли процесса с большим приоритетом
			for (int j = 0; j < answer->process; j++) {
				if (answer->processIn[j].isWork != 3 && currently) {
					if (answer->processIn[j].prioritet > currently->prioritet || answer->processEnd == answer->process - 1) {
						//Если время вышло
						if (currently->longTime != currently->workingTime)
							currently->isWork = 2;
						else {
							currently->isWork = 3;
							currently->allTimeInSys = currently->waitingTime + currently->workingTime;
							answer->processEnd++;
						}
						answer->processIn[j].isWork = 1;
						currently = &answer->processIn[j];
					}
					else if (answer->processIn[j].prioritet == currently->prioritet) {
						if (answer->processIn[j].bornTime > currently->bornTime || answer->processEnd == answer->process - 1) {//Если время появления больше, то меняем процессы
							if (currently->longTime != currently->workingTime )
								currently->isWork = 2;
							else {
								currently->isWork = 3;
								currently->allTimeInSys = currently->waitingTime + currently->workingTime;
								answer->processEnd++;
							}
							answer->processIn[j].isWork = 1;
							currently = &answer->processIn[j];
						}
					}
				
				}
			}
			//Если кол-во отработанных процессов равно общему ислу процессов
			if (answer->process == answer->processEnd) {
				exit = true;
			}
			time++;
			j++;
		}
	sheet->writeStr(str, j, L"Общее время в системе", general);
	sheet->setCol(j , j, 23);
	sheet->writeStr(str, j + 1, L"Потерянное время", general);
	sheet->setCol(j + 1, j + 1, 22);
	sheet->writeStr(str, j + 2, L"Отношение реактивности", general);
	sheet->setCol(j + 2, j + 3, 25);
	sheet->writeStr(str, j + 3, L"Штрафное время", general);
	sheet->setCol(j + 3, j + 3, 22);
	for (int k = 0; k < answer->process; k++) {
		sheet->writeNum(str + 1 + k, j, answer->processIn[k].allTimeInSys, general);
		sheet->writeNum(str + 1 + k, j + 1, answer->processIn[k].allTimeInSys - answer->processIn[k].longTime, general);
		sheet->writeNum(str + 1 + k, j + 2, answer->processIn[k].longTime / double(answer->processIn[k].allTimeInSys), general);
		sheet->writeNum(str + 1 + k, j + 3, answer->processIn[k].allTimeInSys / double(answer->processIn[k].longTime), general);
	}
	//Очистка памяти
	currently = nullptr;
	delete[] answer->processIn;
}