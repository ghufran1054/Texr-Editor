
#include "Class.h"

void SetClr(int tcl, int bcl)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (tcl + (bcl * 16)));
}

void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}

void clearScr(int sr, int sc, int rows, int cols)
{
	
	for (int i = sr; i < sr + rows; i++)
	{
		for (int j = sc; j < sc + cols; j++)
		{
			gotoRowCol(i, j);
			cout << ' ';
		}
	}
	gotoRowCol(sr, sc);
}
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_ECHO_INPUT | ENABLE_INSERT_MODE | ENABLE_LINE_INPUT | ENABLE_EXTENDED_FLAGS| ENABLE_PROCESSED_INPUT);
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
		}
}
bool canBeFormed(string Parent, string child)
{
	//trimming the words from any character other than alphabets
	for (int i = 0; i < Parent.size(); i++)
	{
		if (!isalpha(Parent[i]))
			Parent.erase(Parent.begin() + i);
	}
	for (int i = 0; i < child.size(); i++)
	{
		if (!isalpha(child[i]))
			child.erase(child.begin() + i);
	}
	if (child.size() == 0)
		return false;
	
	if (Parent.size() < child.size())
		return false;
	bool match = false;
	for (int i = 0; i < child.size(); i++)
	{
		match = false;
		for (int j = 0; j < Parent.size(); j++)
		{
			if (child[i] == Parent[j])
			{
				Parent.erase(Parent.begin() + j);
				match = true;
				break;
			}
		}
		if (!match)
			return false;
	}
	return true;
}
int main()
{
	char option=0; 
	string fname = "";
	TextEditor::EditorFirstMenu(fname,option);
	
	system("cls");
	TextEditor TE(fname);
	
	TE.run();

	return 0;
}