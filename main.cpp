#include "RR - priora.h"
#include "FCFS.h"

int main()
{
	Book* book = xlCreateXMLBook(); // xlCreateXMLBook() for xlsx
	if (book)
	{
		Sheet * sheet = book->addSheet(L"Sheet1");
		if (sheet)
		{
			RRpr(book, sheet);
		}
		Sheet * sheet2 = book->addSheet(L"Sheet2");
		if (sheet2) {
			FCFS(book, sheet2);
		}
		book->save(L"example.xlsx");
		book->release();
	}
	return 0;
}