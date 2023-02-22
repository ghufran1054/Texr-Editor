#include "Class.h"

void gotoRowCol(int , int);
void SetClr(int tcl, int bcl);
void clearScr(int sr, int sc, int rows, int cols);
bool canBeFormed(string Parent, string child);
Document::Document():noOflines(1), dri(0), dci(0),  Ls(new Line[1])
{
}
Document::Document(const string &fname) :noOflines(1),dri(0),dci(0),filename(fname),Ls(new Line[1])
{
}
void Document::CopyLines(Line& NL, const Line&OL)
{
	delete[]NL.Cs;
	NL.Cs = OL.Cs;
	NL.size = OL.size;
}
void Document::LoadEncoded(const string& fname)
{
	//change 1
	ifstream rdr(path_info+fname + ".info");
	rdr.ignore();
	rdr.ignore();
	string password;
	string entered="";
	getline(rdr, password);
	cout << "\nEnter Password for "<<fname<<": ";
	do
	{
		int input=0;
		if (_kbhit())
		{
			input = _getch();
			while (input != 13)
			{
				if (input == 8)
				{
					if (entered.length() != 0)
					{
						entered.pop_back();
						cout << "\b \b";
					}

				}
				else
				{
					cout << "*";
					entered += input;
				}
				input = _getch();
			}
			if (entered == password)
				break;
			else
				cout << "\nWrong Password! enter again: ";
			entered.clear();

		}

		
	} while (true);

	rdr >> noOflines >> dri >> dci;
	this->Ls = new Line[noOflines];
	rdr.close();
	//change 1
	rdr.open(path_Files+fname );
	string input = "";

	for (int i = 0; i < noOflines; i++)
	{
		getline(rdr, input);
		for (int j = 0; j < input.size(); j++)
		{

			this->Ls[i].insertCharAt(j, input[j]-34);
		}
	}
}

void Document::unhighlightWords(int** wordTable, int size)
{
	int maxRow = max_ROW;
	int currentPage = this->dri / maxRow;
	int maxPage = (this->noOflines - 1) / maxRow;
	
		for (int i = currentPage * maxRow, t = 0; t < maxRow && i < this->noOflines; t++, i++)
		{
			for (int j = 1; wordTable[i] != nullptr && j <= wordTable[i][0]; j++)
			{
				gotoRowCol(i % maxRow, wordTable[i][j]);
				SetClr(WHITE, BLACK);
				//cout << toFind;
				this->Ls[i].printSubstr(wordTable[i][j], wordTable[i][j] + size);
			}
		}
}
void Document::simpleLoad(const string& fname)
{
	//change 1
	string docAtt = path_info+fname + ".info";
	ifstream rdr(docAtt);
	rdr.ignore();
	rdr >> noOflines >> dri >> dci;
	this->Ls = new Line[noOflines];
	rdr.close();
	rdr.open(path_Files+fname );
	string input = "";
	for (int i = 0; i < noOflines; i++)
	{
		getline(rdr, input);
		for (int j = 0; j < input.size(); j++)
		{

			this->Ls[i].insertCharAt(j, input[j]);
		}
	}
}
void Document:: Load(const string &fname)
{
	/*change 1
	string docAtt = fname + ".info";
	*/
	string docAtt = path_info + fname + ".info";
	ifstream rdr(docAtt);
	if (!rdr)
		return;
	bool pass;
	rdr >> pass;
	if (pass)
	{
		this->LoadEncoded(fname);
	}
	else
		this->simpleLoad(fname);
	
}

void Document::Updater(const string& fname)
{
	bool isPass;
	//change 1
	ifstream rdr(path_info+fname + ".info");
	rdr >> isPass;
	if (isPass && rdr)
	{
		string password = "";
		rdr.ignore();
		getline(rdr, password);
		rdr.close();
		ofstream wtr(path_info+fname + ".info");
		wtr << 1 << "\n" << password << '\n' << noOflines << "\n" << dri << "\n" << dci;
		wtr.close();
		encodedUpdater(fname);
		return;
	}
	//simple file

	ofstream wtr(path_info+fname + ".info");
	wtr << 0 << '\n' << noOflines << "\n" << dri << "\n" << dci;
	wtr.close();
	wtr.open(path_Files+fname );
	for (int i = 0; i < noOflines; i++)
	{
		this->Ls[i].writer(wtr);
	}
	wtr.close();
}

void Document::encodedUpdater(const string& fname)
{

	ofstream wtr;
	wtr.open(path_Files+fname );
	for (int i = 0; i < noOflines; i++)
	{
		this->Ls[i].encodedWriter(wtr);
	}
	wtr.close();

}
void Document::Enter_AddLine(int Ci, int Ri)
{
	Line* newLines = new Line[this->noOflines + 1];
	//line copying before the enter-pressed line
	for (int i = 0; i < Ri; i++)
	{
		//deepCopyLines(newLines[i], this->Ls[i]);
		CopyLines(newLines[i], this->Ls[i]);
		//newLines[i] = this->Ls[i];
		this->Ls[i].Cs = nullptr;
	}
	//keeping the content before the enter preesed location in the same line
	for (int i = 0; i < Ci; i++)
	{
		newLines[Ri].insertCharAt(i, Ls[Ri].Cs[i]);
	}
	//passing the content after enter to next line
	for (int i = Ci; i < Ls[Ri].size; i++)
	{
		newLines[Ri + 1].insertCharAt(i - Ci, Ls[Ri].Cs[i]);
	}
	//copying the rest of lines
	for (int i = Ri + 1; i < noOflines; i++)
	{
		//deepCopyLines(newLines[i+1], this->Ls[i]);
		CopyLines(newLines[i+1], this->Ls[i]);
		//newLines[i+1] = this->Ls[i];
		Ls[i].Cs = nullptr;
	}
	delete[]this->Ls;
	this->Ls = newLines;
		this->noOflines++;
}

void Document::deepCopyLines(Line& NL, const Line& OL)
{
	delete[]NL.Cs;
	NL.Cs = new char[OL.size + 1];
	for (int i = 0; i <= OL.size; i++)
	{
		NL.Cs[i] = OL.Cs[i];
	}
	NL.size = OL.size;
}

void Document::backSpace_line_Remover(int Li)
{
	Line* newLines = new Line[this->noOflines - 1];
	for (int i = 0,k=0; i < this->noOflines; i++)
	{
		if (i != Li)
		{
			//deepCopyLines(newLines[k], this->Ls[i]);
			CopyLines(newLines[k], this->Ls[i]);
			//newLines[k] = this->Ls[i];
			Ls[i].Cs = nullptr;
			k++;
		}
	}
	delete[]Ls;
	this->Ls = newLines;
	this->noOflines--;
}

void Document::backSpace_LineConcat(int Li)
{
	int spaceInUpperLine = max_COL - Ls[Li - 1].size;
	int eIndex = -1;
	int j;
	for (j = 0; j < spaceInUpperLine && j<Ls[Li].size; j++)
	{
		if (Ls[Li].Cs[j] == ' ')
			eIndex = j;
		
	}
	if (Ls[Li].Cs[j] == '\0')
	{
		eIndex = j - 1;
	}
	Line* newLines = nullptr;
	//case if all the text in the current line can fit into previous
	if (eIndex == Ls[Li].size-1)
	{
		newLines = new Line[this->noOflines - 1];
		Line* newLines = new Line[this->noOflines - 1];
		for (int i = 0, k = 0; i < this->noOflines; i++)
		{
			if (i != Li)
			{
				//deepCopyLines(newLines[k], this->Ls[i]);
				CopyLines(newLines[k], this->Ls[i]);
				//newLines[k] = this->Ls[i];
				this->Ls[i].Cs = nullptr;
				k++;
			}
		}
		for (int i = 0; i < Ls[Li].size; i++)
		{
			newLines[Li - 1].insertCharAt(newLines[Li - 1].size, Ls[Li].Cs[i]);
		}

		delete[]Ls;
		this->Ls = newLines;
		this->noOflines--;
		return;
	}
	//if all text can't fit in previoous no need to reducw no of lines;
	for (int i = 0; i <= eIndex; i++)
	{
		Ls[Li - 1].insertCharAt(Ls[Li - 1].size, Ls[Li].Cs[0]);
		Ls[Li].removeAt(0);
	}

	
}

void Document::printLines(int Li)
{
	static int currentPage = 0;
	if (Li / max_ROW != currentPage)
	{
		currentPage = Li / max_ROW;
		for (int i = 0; i < max_ROW; i++)
		{
			gotoRowCol(i, 0);
			for (int j = 0; j < max_COL; j++)
			{
				cout << ' ';
			}
		}
	}

	for (int i = Li; i < this->noOflines ; i++)
	{
		if (i / max_ROW > currentPage)
			break;
		gotoRowCol(i%max_ROW, 0);
		this->Ls[i].printLine();
	}

	
}
bool Document::regulateLines(int& ri,int& ci)
{
	bool flag = false;
	int start = 0, firstStart = 0;
	//this is to check if the line size is greater than the MAX Allowed columns
	for (int j = ri; j < this->noOflines; j++)
	{
		// if we are on end line it will create a new line
		if ((j == this->noOflines - 1) && (this->Ls[j].size > max_COL))
		{
			this->Enter_AddLine(max_COL + 1, j);
			flag = true;
		}
		//this if checks if size is greater and then pushes the overflowing chars in the next line and the cycle repeats for the next line.
		if (this->Ls[j].size > max_COL)
		{
			int currentLineSize = this->Ls[j].size;
			//this loop checks for the beggining of a word and pushes the index back until a space is found
			for (start = max_COL; start >= 0; start--)
			{
				if (this->Ls[j].Cs[start] == ' ' && this->Ls[j].Cs[start + 1] != '\0' && this->Ls[j].Cs[start + 1] != ' ')
				{
					if (start != max_COL)
						start++;
					break;
				}
			}

			if (start == -1)
				start = max_COL;
			if (j == ri)
			{
				firstStart = start;
			}
			for (int i = start; i < currentLineSize; i++)
			{
				this->Ls[j + 1].insertCharAt(i - start, this->Ls[j].Cs[start]);
				this->Ls[j].removeAt(start);
			}
			flag = true;
		}
	}

	//this condition checks that if the cursor is within bounds of a line
	if (ci >= firstStart && firstStart != 0)
	{
		ri++;
		ci = ci - firstStart;
	}
	return flag;
}

void Document::FindNextAll(string toFind, bool Case, bool word)
{
	int** wordTable = new int* [this->noOflines];
	int WordFound = 0;
	for (int i = 0; i < dri; i++)
	{
		wordTable[i] = nullptr;
	}
	wordTable[dri] = this->Ls[dri].FindNextAll(toFind, dci, Case, word);
	WordFound += wordTable[dri][0];
	for (int i = dri+1; i < this->noOflines; i++)
	{
		wordTable[i] = this->Ls[i].substringSearch(toFind, Case, word);
		WordFound += wordTable[i][0];
	}
	gotoRowCol(10 ,max_COL + 5);
	cout << "Found: " << WordFound;
	if (WordFound)
		MoveInFindMode(wordTable, toFind);
	else
	{
		gotoRowCol(11, max_COL + 5);
		system("Pause");
	}
	for (int i = 0; i < noOflines; i++)
		delete[]wordTable[i];
	delete[]wordTable;
	clearScr(10, max_COL + 5, 2, 40);
}
void Document::FindPrevAll(string toFind, bool Case, bool word)
{
	int** wordTable = new int* [this->noOflines];
	int WordFound = 0;
	for (int i = 0; i < dri; i++)
	{
		wordTable[i] = this->Ls[i].substringSearch(toFind, Case, word);
		WordFound += wordTable[i][0];
	}
	wordTable[dri] = this->Ls[dri].FindPrevAll(toFind, dci, Case, word);
	WordFound += wordTable[dri][0];
	for (int i = dri+1; i < this->noOflines; i++)
	{
		wordTable[i] = nullptr;
	}
	gotoRowCol(10, max_COL + 5);
	cout << "Found: " << WordFound;
	if (WordFound)
		MoveInFindMode(wordTable, toFind);
	else
	{
		gotoRowCol(11, max_COL + 5);
		system("Pause");
	}
	for (int i = 0; i < noOflines; i++)
		delete[]wordTable[i];
	delete[]wordTable;
	clearScr(10, max_COL + 5, 2, 40);
}
void Document::FindPrevOnly(string toFind, bool Case, bool word)
{
	int currentPage = dri/max_ROW;
	int found = this->Ls[dri].FindPrevOnly(toFind, dci, Case, word);
	int ri=dri;
	if (found!=-1)
	{
		gotoRowCol(0, 0);
		gotoRowCol(ri, found);
		SetClr(RED, BLACK);
		this->Ls[ri].printSubstr(found, found + toFind.size());
	}
	else
	{
		ri = dri-1;
		do
		{
			found = this->Ls[ri].FindPrevOnly(toFind, Ls[ri].size, Case, word);
			if (found != -1)
			{
				gotoRowCol(0, 0);
				if (ri / max_ROW != currentPage)
				{
					gotoRowCol(5, max_COL + 5);
					currentPage = dri / max_ROW;
					cout << "PAGE: " << currentPage + 1 << "    \b\b\bof ";
					this->printLines(currentPage * max_ROW);
				}
				gotoRowCol(ri% max_ROW, found);
				SetClr(RED, BLACK);
				this->Ls[ri].printSubstr(found, found + toFind.size());
				break;
			}
			ri--;
		} while (found == -1 && ri>=0);
	}
	if (found != -1)
	{
		dri = ri;
		dci = found;
		system("pause>0");
		SetClr(WHITE, BLACK);
		gotoRowCol(ri%max_ROW, found);
		this->Ls[ri].printSubstr(found, found + toFind.size());
	}
}
void Document::FindNextOnly(string toFind, bool Case, bool word)
{
	int currentPage = dri / max_ROW;
	int found = this->Ls[dri].FindNextOnly(toFind, dci, Case, word);
	int ri=dri;
	if (found != -1)
	{
		gotoRowCol(0, 0);
		gotoRowCol(dri, found);
		SetClr(RED, BLACK);
		this->Ls[ri].printSubstr(found, found+toFind.size());
	}
	else
	{
		ri = dri + 1;
		do
		{
			found = this->Ls[ri].FindNextOnly(toFind, 0, Case, word);
			if (found != -1)
			{
				gotoRowCol(0, 0);
				if (ri / max_ROW != currentPage)
				{
					gotoRowCol(5, max_COL + 5);
					currentPage = ri / max_ROW;
					cout << "PAGE: " << currentPage + 1 << "    \b\b\bof ";
					this->printLines(currentPage * max_ROW);
				}
				gotoRowCol(ri% max_ROW, found);
				SetClr(RED, BLACK);
				this->Ls[ri].printSubstr(found, found + toFind.size());
				break;
			}
			ri++;
		} while (found == -1 && ri < noOflines);
	}
	if (found != -1)
	{
		//gotoRowCol(dri, dci);
		dri = ri;
		dci = found;
		system("pause>0");
		SetClr(WHITE, BLACK);
		gotoRowCol(ri% max_ROW, found);
		this->Ls[ri].printSubstr(found, found + toFind.size());
	}
}
void Document::FindSentence(const string& toFind)
{
	vector<vector<int>>senStart(noOflines);
	int startLine = 0, endLine = 0, startC = 0, endC = 0;
	bool isFound = true;
	for (int li = 0; li < noOflines; li++)
	{
		for (int ci = 0; ci < Ls[li].size; ci++)
		{
			isFound = true;
			int j = li, k = ci;
			for (int i = 0; i < toFind.size(); i++)
			{
				if (toFind[i] != Ls[j].Cs[k])
				{
					isFound = false;
					break;
				}
				k++;
				if (k >= Ls[j].size)
				{
					k = 0;
					j++;
				}
				if (j >= noOflines)
				{
					isFound = false;
					break;
				}
			}
			if (isFound)
			{

			}
		}
	}
}
int Document::specialCharCount()
{
	int sCount=0;
	char c = ' ';
	for (int li = 0; li < noOflines; li++)
	{
		for (int ci = 0; ci < this->Ls[li].size; ci++)
		{
			c=this->Ls[li].Cs[ci];
			if ((c >= '!' && c <= '/') || (c >= ':' && c <= '@') || (c >= '[' && c <= '`') || (c >= '{' && c <= '~'))
				sCount++;
		}
	}
	return sCount;
}
int Document::paraGraphCount()
{
	int pCount = 0;
	bool* isEmpty = new bool[noOflines] {false};
	for	(int li = 0; li < noOflines; li++)
	{
		if (this->Ls[li].size == 0)
			isEmpty[li] = true;
		else
		{
			isEmpty[li] = true;
			for (int ci = 0; ci < this->Ls[li].size; ci++)
			{
				if (this->Ls[li].Cs[ci] != ' ')
				{
					isEmpty[li] = false;
					break;
				}
			}
		}
	}
	for (int i = 0; i + 1 < noOflines; i++)
	{
		if (!isEmpty[i] && isEmpty[i + 1])
			pCount++;
	}
	if (!isEmpty[noOflines - 1])
		pCount++;
	delete[]isEmpty;
	return pCount;
}
int Document::wordCount(int sLi, int eLi)
{
	if (eLi == -1)
	{
		eLi = noOflines;
	}
	int wCount = 0;
	char s = ' ', e = ' ';
	for (int li = sLi; li < eLi; li++)
	{
		for (int ci = 0; ci < this->Ls[li].size; ci++)
		{
			s = this->Ls[li].Cs[ci], e = this->Ls[li].Cs[ci + 1];
			if (!(s == ' ' || s == '.' || s == ','  || s == '?' || s == '!' || s=='\"'|| s==':' || s==';') && (e == ' ' || e == '.' || e == ',' || e == '\0' || e == '?' || e == '!' || e == '\"'||e==':'|| e==';'))
				wCount++;
		}
	}
	return wCount;
}

int Document::sentenceCount()
{
	int sCount = 0;
	char s = ' ', e = ' ';
	for (int li = 0; li < noOflines; li++)
	{
		for (int ci = 0; ci < this->Ls[li].size; ci++)
		{
			s = this->Ls[li].Cs[ci], e = this->Ls[li].Cs[ci + 1];
			if (!( s == '.'|| s == '?' || s == '!') && (e == '.'  || e == '?' || e == '!'))
				sCount++;
		}
	}
	if (this->Ls[noOflines - 1].Cs[this->Ls[noOflines-1].size] == '\0')
		sCount++;
	return sCount;
}
int Document::largestParaWordCount()
{
	int wCount = 0, maxWCount = 0;
	bool* isEmpty = new bool[noOflines] {false};
	for (int li = 0; li < noOflines; li++)
	{
		if (this->Ls[li].size == 0)
			isEmpty[li] = true;
		else
		{
			isEmpty[li] = true;
			for (int ci = 0; ci < this->Ls[li].size; ci++)
			{
				if (this->Ls[li].Cs[ci] != ' ')
				{
					isEmpty[li] = false;
					break;
				}
			}
		}
	}
	int startLine = 0, endLine = 0;
	for (int i = 0; i + 1 < noOflines; i++)
	{
		if (!isEmpty[i] && isEmpty[i + 1])
		{
			wCount = wordCount(startLine, endLine + 1);
			if (wCount > maxWCount)
				maxWCount = wCount;
			startLine = endLine + 1;
		}
		else
		{
			endLine = i + 1;
		}
	}
	if (!isEmpty[noOflines - 1])
	{
		wCount = wordCount(startLine, endLine + 1);
		if (wCount > maxWCount)
			maxWCount = wCount;
	}
	delete[]isEmpty;
	return maxWCount;
}
double Document::avgWordLength()
{
	double alphaCount = 0;
	char s = ' ';
	for (int li = 0; li < noOflines; li++)
	{
		for (int ci = 0; ci < this->Ls[li].size; ci++)
		{
			s = this->Ls[li].Cs[ci];
			if (isalpha(s) || s>='0' && s<='9')
				alphaCount++;
		}
	}
	double words = double(wordCount());
	return alphaCount/words;
}

int Document::smallestWordLength()
{
	int smallestLen = 1000, cWordLen=0;
	char s = ' ', e = ' ';
	for (int li = 0; li < noOflines; li++)
	{
		for (int ci = 0; ci < this->Ls[li].size; ci++)
		{
			s = this->Ls[li].Cs[ci], e = this->Ls[li].Cs[ci + 1];
			if (isalpha(s) || s>='0' && s<='9')
				cWordLen++;
			if (!(s == ' ' || s == '.' || s == ',' || s == '?' || s == '!' || s == '\"' || s == ':' || s == ';') && (e == ' ' || e == '.' || e == ',' || e == '\0' || e == '?' || e == '!' || e == '\"' || e == ':' || e == ';'))
			{
				smallestLen = (smallestLen > cWordLen ? cWordLen : smallestLen);
				cWordLen = 0;
			}
		}
	}
	return smallestLen;
}
int Document::LargestWordLength()
{
	int largestLen = 0, cWordLen = 0;
	char s = ' ', e = ' ';
	for (int li = 0; li < noOflines; li++)
	{
		for (int ci = 0; ci < this->Ls[li].size; ci++)
		{
			s = this->Ls[li].Cs[ci], e = this->Ls[li].Cs[ci + 1];
			if (isalpha(s) || s>='0' && s<='9')
				cWordLen++;
			if (!(s == ' ' || s == '.' || s == ',' || s == '?' || s == '!' || s == '\"' || s == ':' || s == ';') && (e == ' ' || e == '.' || e == ',' || e == '\0' || e == '?' || e == '!' || e == '\"' || e == ':' || e == ';'))
			{
				largestLen = (largestLen < cWordLen ? cWordLen : largestLen);
				cWordLen = 0;
			}
		}
	}
	return largestLen;
}
void Document::displayDocStas()
{
	gotoRowCol(100, 0);
	gotoRowCol(50, 0);
	cout << "No of Words:              " << this->wordCount()<<endl;
	cout << "No of Paragraphs:         " << this->paraGraphCount()<<endl;
	cout << "No of Special Characters: " << this->specialCharCount()<<endl;
	cout << "No of Sentences:          " << this->sentenceCount() << endl;
	cout << "Average Word Length:      " << this->avgWordLength()<<endl;
	cout << "smallest Word Length:     " << this->smallestWordLength()<<endl;
	cout << "largest Word Length:      " << this->LargestWordLength()<<endl;
	cout << "largest Para Word Count:  " << this->largestParaWordCount()<<endl;
	system("pause");
	clearScr(50, 0, 20, 50);
	gotoRowCol(0, 0);
}
void Document::FindAndReplaceAll(string toFind, string toReplace, bool Case, bool word)
{
	for (int li = 0; li < noOflines; li++)
	{
		this->Ls[li].ReplaceAll(toFind, toReplace,Case, word);
	}
	
	if (toFind.size() < toReplace.size())
	{
		int r = 0, c = 0;
		this->regulateLines(r, c);
		dci = Ls[dri].size;
	}
	this->printLines(dri * (dri / max_ROW));
}

void Document::FindAndReplaceNextAll(string toFind, string toReplace, bool Case, bool word)
{
	this->Ls[dri].FindAndReplaceNextAll(toFind, toReplace, dci, Case, word);
	for (int li = dri + 1; li < noOflines; li++)
	{
		this->Ls[li].ReplaceAll(toFind, toReplace, Case, word);
	}
	if (toFind.size() < toReplace.size())
	{
		int r = 0, c = 0;
		this->regulateLines(r, c);
		dri = Ls[dri].size;
	}
	this->printLines(dri * (dri / max_ROW));
}

void Document::FindAndReplacePrevAll(string toFind, string toReplace, bool Case, bool word)
{
	this->Ls[dri].FindAndReplacePrevAll(toFind, toReplace, dci, Case, word);
	for (int li = dri-1; li >=0; li--)
	{
		this->Ls[li].ReplaceAll(toFind, toReplace, Case, word);
	}
	if (toFind.size() < toReplace.size())
	{
		int r = 0, c = 0;
		this->regulateLines(r, c);
		dri = Ls[dri].size;

	}
	this->printLines(dri * (dri / max_ROW));
}

void Document::FindAndReplacePrevOnly(string toFind, string toReplace, bool Case, bool word)
{
	int ri = dri,currentPage=dri/max_ROW;
	if (this->Ls[ri].FindAndReplacePrevOnly(toFind, toReplace, dci, Case, word))
		return;
	ri--;
	while (ri>=0 && !this->Ls[ri].FindAndReplacePrevOnly(toFind, toReplace, this->Ls[ri].size, Case, word))
		ri--;
	if (toFind.size() < toReplace.size())
	{
		int r = 0, c = 0;
		this->regulateLines(r, c);
		dri = Ls[dri].size;

	}
	if (ri >= 0 && ri/max_ROW==currentPage)
	{
		gotoRowCol(ri, 0);
		this->Ls[ri].printLine();
	}
}
void Document::FindAndReplaceNextOnly(string toFind, string toReplace, bool Case, bool word)
{
	int ri = dri,currentPage = dri / max_ROW;
	if (this->Ls[ri].FindAndReplaceNextOnly(toFind, toReplace, dci, Case, word))
		return;
	ri++;
	while (ri < noOflines && !this->Ls[ri].FindAndReplacePrevOnly(toFind, toReplace,0, Case, word))
		ri++;
	if (toFind.size() < toReplace.size())
	{
		int r = 0, c = dci;
		this->regulateLines(r, c);
		dri = r, dci = c;
	}
	if (ri < noOflines && ri / max_ROW == currentPage)
	{
		gotoRowCol(ri, 0);
		this->Ls[ri].printLine();
	}
}
void Document::addPrefix(string toFind,string prefix, bool Case,bool word)
{
	for (int i = 0; i < noOflines; i++)
	{
		if (i == 13)
			i;
		this->Ls[i].AddPrefix(toFind, prefix, Case, word);
	}
	int r = 0, c = 0,currentPage=dri/max_ROW;
	regulateLines(r, c);
	this->printLines(dri * currentPage);
}
void Document::addPostFix(string toFind, string postfix, bool Case, bool word)
{
	for (int i = 0; i < noOflines; i++)
	{
		this->Ls[i].AddPostfix(toFind, postfix, Case, word);
	}
	int r = 0, c = 0, currentPage = dri / max_ROW;
	regulateLines(r, c);
	this->printLines(dri * currentPage);
}
void Document::prefixMenu()
{
	string toFind = "", prefix = "";
	gotoRowCol(220, 0);
	gotoRowCol(max_ROW + 15, 0);
	cout << "Enter String to find: ";
	getline(cin, toFind);
	cout << "Enter String as prefix: ";
	getline(cin, prefix);
	gotoRowCol(max_ROW + 18, 0);
	cout << "Ctrl+U Case sensitivity (Off)  " << endl;
	cout << "Ctrl+W search Whole word (Off)  " << endl;
	cout << "Press Enter to add Prefix " << endl;
	bool word = false, Case = false;
	int input;
	do
	{
		input = _getch();
		switch (input)
		{
		case 21:
			gotoRowCol(max_ROW + 18, 0);
			Case = !Case;
			if (Case)
			{
				cout << "Ctrl+U Case sensitivity (On) " << endl;
			}
			else
			{
				cout << "Ctrl+U Case sensitivity (Off) " << endl;
			}
			break;
		case 23:
			gotoRowCol(max_ROW + 19, 0);
			word = !word;
			if (word)
			{
				cout << "Ctrl+W search Whole word (On)  " << endl;
			}
			else
			{
				cout << "Ctrl+W search Whole word (Off)  " << endl;
			}
			break;
		case 13:
			this->addPrefix(toFind,prefix,Case,word);
			break;
		}
	} while (input!=13 || input == 21 || input == 23);
	clearScr(max_ROW + 15, 0, 10, 40);
	gotoRowCol(0, 0);
}
void Document::postfixMenu()
{
	string toFind = "", postfix = "";
	gotoRowCol(220, 0);
	gotoRowCol(max_ROW + 15, 0);
	cout << "Enter String to find: ";
	getline(cin, toFind);
	cout << "Enter String as postfix: ";
	getline(cin, postfix);
	gotoRowCol(max_ROW + 18, 0);
	cout << "Ctrl+U Case sensitivity (Off)  " << endl;
	cout << "Ctrl+W search Whole word (Off)  " << endl;
	cout << "Press Enter to add Prefix " << endl;
	bool word = false, Case = false;
	int input;
	do
	{
		input = _getch();
		switch (input)
		{
		case 21:
			gotoRowCol(max_ROW + 18, 0);
			Case = !Case;
			if (Case)
			{
				cout << "Ctrl+U Case sensitivity (On) " << endl;
			}
			else
			{
				cout << "Ctrl+U Case sensitivity (Off) " << endl;
			}
			break;
		case 23:
			gotoRowCol(max_ROW + 19, 0);
			word = !word;
			if (word)
			{
				cout << "Ctrl+W search Whole word (On)  " << endl;
			}
			else
			{
				cout << "Ctrl+W search Whole word (Off)  " << endl;
			}
			break;
		case 13:
			this->addPostFix(toFind, postfix, Case, word);
			break;
		}
	} while (input != 13 || input == 21 || input == 23);
	clearScr(max_ROW + 15, 0, 10, 40);
	gotoRowCol(0, 0);
}
void Document::replaceMenu()
{
	string toFind = "";
	string toReplace = "";
	gotoRowCol(220, 0);
	gotoRowCol(max_ROW + 15, 0);
	cout << "Enter String to find: ";
	getline(cin, toFind);
	cout << "Enter String to replace: ";
	getline(cin, toReplace);
	gotoRowCol(max_ROW + 18, 0);
	cout << "Ctrl+U Case sensitivity (Off)  " << endl;
	cout << "Ctrl+W search Whole word (Off)  " << endl;
	cout << "Ctrl+K Find and replace All" << endl;
	cout << "Ctrl+N Find and replace Next All" << endl;
	cout << "Ctrl+P Find and replace Previous All" << endl;
	cout << "Ctrl+O Find and replace Next Only" << endl;
	cout << "Ctrl+B Find and replace Previous Only" << endl;
	bool word = false, Case = false;
	int input;
	do
	{
		input = _getch();
		switch (input)
		{
		case 21:
			gotoRowCol(max_ROW + 18, 0);
			Case = !Case;
			if (Case)
			{
				cout << "Ctrl+U Case sensitivity (On) " << endl;
			}
			else
			{
				cout << "Ctrl+U Case sensitivity (Off) " << endl;
			}
			break;
		case 23:
			gotoRowCol(max_ROW + 19, 0);
			word = !word;
			if (word)
			{
				cout << "Ctrl+W search Whole word (On)  " << endl;
			}
			else
			{
				cout << "Ctrl+W search Whole word (Off)  " << endl;
			}
			break;
		case 11://ctrl+K
			this->FindAndReplaceAll(toFind, toReplace, Case, word);
			break;
		case 14: //ctrl+N
			this->FindAndReplaceNextAll(toFind, toReplace, Case, word);
			break;
		case 16: //ctrl+p
			this->FindAndReplacePrevAll(toFind, toReplace, Case, word);
			break;
		case 15: //ctrl+O
			this->FindAndReplaceNextOnly(toFind, toReplace, Case, word);
			break;
		case 2://ctrl+B
			this->FindAndReplacePrevOnly(toFind, toReplace, Case, word);
			break;

		}

	} while (input == 21 || input == 23);

}
void Document::FindMenu()
{
	string toFind="";
	gotoRowCol(220, 0);
	gotoRowCol(max_ROW + 15, 0);
	cout << "Enter your String: ";
	getline(cin, toFind);
	gotoRowCol(max_ROW + 17, 0);
	cout << "Ctrl+U Case sensitivity (Off)  " << endl;
	cout << "Ctrl+W search Whole word (Off)  " << endl;
	cout << "Ctrl+K Find All" << endl;
	cout << "Ctrl+N Find Next All" << endl;
	cout << "Ctrl+P Find Previous All" << endl;
	cout << "Ctrl+O Find Next Only" << endl;
	cout << "Ctrl+B Find Previous Only" << endl;
	bool word = false, Case = false;
	int input;
	do
	{
		input = _getch();
		switch (input)
		{
		case 21:
			gotoRowCol(max_ROW + 17, 0);
			Case = !Case;
			if (Case)
			{
				cout << "Ctrl+U Case sensitivity (On) " << endl;
			}
			else
			{
				cout << "Ctrl+U Case sensitivity (Off) " << endl;
			}
			break;
		case 23:
			gotoRowCol(max_ROW + 18, 0);
			word = !word;
			if (word)
			{
				cout << "Ctrl+W search Whole word (On)  " << endl;
			}
			else
			{
				cout << "Ctrl+W search Whole word (Off)  " << endl;
			}
			break;
		case 11:
			gotoRowCol(0, 0);
			this->FindAllsubStrings(toFind, Case, word);
			break;
		case 14: //ctrl+N
			this->FindNextAll(toFind, Case, word);
			break;
		case 16: //ctrl+p
			this->FindPrevAll(toFind, Case, word);
			break;
		case 15: //ctrl+O
			this->FindNextOnly(toFind, Case, word);
			break;
		case 2://ctrl+B
			this->FindPrevOnly(toFind, Case, word);
			break;

		}
		
	} while (input==21 || input==23);

	
}
void Document::FindAllsubStrings(string toFind,bool Case, bool word)
{
	int** wordTable = new int*[this->noOflines];
	int WordFound = 0;
	for (int i = 0; i < this->noOflines; i++)
	{
		wordTable[i] = this->Ls[i].substringSearch(toFind,Case,word);
		WordFound += wordTable[i][0];
	}
	gotoRowCol( 10,max_COL + 5);
	cout << "Found: " << WordFound;
	if(WordFound)
		MoveInFindMode(wordTable, toFind);
	else
	{
		gotoRowCol(11,max_COL + 5);
		system("Pause");
	}
	for (int i = 0; i < noOflines; i++)
		delete[]wordTable[i];
	delete[]wordTable;
	clearScr(10, max_COL + 5, 2, 40);
}

void Document::MoveInFindMode(int** wordTable, string& toFind)
{
	int maxRow = max_ROW;
	bool highlightkro = true;
	int input;
	int currentPage=0;
	gotoRowCol(0, 0);
	do
	{
		currentPage = this->dri / maxRow;
		int maxPage = (this->noOflines - 1) / maxRow;
		if (highlightkro)
		{
			for (int i = currentPage * maxRow, t = 0; t < maxRow && i < this->noOflines; t++, i++)
			{
				for (int j = 1; wordTable[i]!=nullptr && j <= wordTable[i][0]; j++)
				{
					//dci = wordTable[dri][j];
					gotoRowCol(i % maxRow, wordTable[i][j]);
					SetClr(RED, BLACK);
					//cout << toFind;
					this->Ls[i].printSubstr(wordTable[i][j], wordTable[i][j] + toFind.size());
				}
			}
			SetClr(WHITE, BLACK);
			gotoRowCol(dri % maxRow, dci);
			highlightkro = false;
		}
		input = _getch();
		if (input == 224)
		{
			input = _getch();
			switch (input)
			{
			case 73: //pg up key
				if (currentPage != 0)
				{
					dri = (currentPage - 1) * maxRow;
					dci = 0;
					this->printLines(dri);
					highlightkro = true;
					currentPage--;
				}
				break;
			case 81: //pg dn key

				if (currentPage != maxPage)
				{
					dri = (currentPage + 1) * maxRow;
					dci = 0;
					this->printLines(dri);
					highlightkro = true;
					currentPage++;
				}
				break;

			case RIGHT:
				for (int i = dri; i < this->noOflines; i++)
				{
					bool isFound = false;
					for (int j = 1; wordTable[i] != nullptr && j <= wordTable[i][0]; j++)
					{
						if (wordTable[i][j] > dci || i > dri)
						{
							dri = i, dci = wordTable[i][j];
							isFound = true;
							break;
						}
					}
					if (isFound)
						break;
				}
				gotoRowCol(dri % maxRow, dci);
				break;
			case LEFT:
				for (int i = dri; i >= 0; i--)
				{
					bool isFound = false;
					if (wordTable[i] != nullptr)
					{
						for (int j = wordTable[i][0]; j >= 1; j--)
						{
							if (wordTable[i][j] < dci || i < dri)
							{
								dri = i, dci = wordTable[i][j];
								isFound = true;
								break;
							}
						}
					}
					if (isFound)
						break;
				}
				gotoRowCol(dri % maxRow, dci);
				break;

			}
		}

		if (dri / maxRow != currentPage)
		{
			gotoRowCol(5, max_COL + 5);
			currentPage = dri / maxRow;
			cout << "PAGE: " << currentPage + 1 << "    \b\b\bof " <<"   \b\b\b"<< maxPage + 1<<"  ";
			this->printLines(currentPage * maxRow);
			highlightkro = true;
		}
	} while (input != 27);
	this->unhighlightWords(wordTable, toFind.size());
}

void Document::wordGame()
{
	int words = this->wordCount(),si=0,ei=0;
	string* allWords = new string[words];
	int allWindex = 0;
	for (int li = 0; li < noOflines; li++)
	{
		si = 0, ei = 0;
		do
		{
			si = ei;
			char s=this->Ls[li].Cs[si];
			while ((s==' ' || s=='.'|| s==','|| s=='?'|| s=='!' || s=='\"' || s==':'|| s==';') && si<this->Ls[li].size)
			{
				si++;
				s = this->Ls[li].Cs[si];
			}
			ei = si;
			s = this->Ls[li].Cs[ei];
			while (!(s == ' ' || s == '.' || s == ',' || s == '?' || s == '!'|| s=='\"'|| s == ':' || s == ';') && ei < this->Ls[li].size)
			{
				ei++;
				s = this->Ls[li].Cs[ei];
			}
			if (si >= this->Ls[li].size)
				break;
			for (int i = si; i < ei; i++)
			{
				allWords[allWindex] += this->Ls[li].Cs[i];
			}
			allWindex++;
		} while (true);
	}
	int highestWords = 0, wordsFormed = 0
		, highestIndex = 0;
	//removing duplicates 
	for (int i = 0; i < words; i++)
	{
		for (int j = 0; j < words; j++)
		{
			if ((i != j) && allWords[i] == allWords[j])
				allWords[i] = "";
		}
	}
	string childWords = "",childWordsOfHighest="";
	for (int i = 0; i < allWindex; i++)
	{
		for (int j = 0; j < allWindex; j++)
		{
			if (!(allWords[i]==allWords[j]) && canBeFormed(allWords[i], allWords[j]))
			{
				wordsFormed++;
				childWords += allWords[j];
				childWords += ' ';
			}
		}
		if (highestWords < wordsFormed)
		{
			highestWords = wordsFormed;
			highestIndex = i;
			childWordsOfHighest = childWords;
		}
		childWords = "";
		wordsFormed = 0;
	}
	gotoRowCol(220, 0);
	gotoRowCol(max_ROW + 30, 0);
	cout << "The Word out of which most words of the current file can be formed is: " << endl<<allWords[highestIndex]<<endl<<endl;
	cout << "The words formed are: " << endl;
	cout << childWordsOfHighest << endl<<endl;
	system("pause");
	clearScr(max_ROW + 30, 0, 8, 80);
	gotoRowCol(0, 0);
	delete[]allWords;
}

void Document::lowerCase()
{
	int c=0,e=0;
	for (c = dci; c >= 0 && this->Ls[dri].Cs[c] != ' '; c--);
	if (c < 0)
		c = 0;
	for (e = dci; e < this->Ls[dri].size && this->Ls[dri].Cs[e] != ' '; e++);
	if (e >= this->Ls[dri].size)
		e = this->Ls[dri].size - 1;
	this->Ls[dri].toLower(c, e);
	gotoRowCol(dri%max_ROW, c);
	this->Ls[dri].printSubstr(c, e);
}
void Document::upperCase()
{
	int c = 0, e = 0;
	for (c = dci; c >= 0 && isalpha(this->Ls[dri].Cs[c]); c--);
	if (c < 0)
		c = 0;
	for (e = dci; e < this->Ls[dri].size && isalpha(this->Ls[dri].Cs[e]); e++);
	if (e >= this->Ls[dri].size)
		e = this->Ls[dri].size - 1;
	this->Ls[dri].toUpper(c, e);
	gotoRowCol(dri%max_ROW, c);
	this->Ls[dri].printSubstr(c, e);
}
void Document::ExitDocument()
{
	
}
Document::~Document()
{
	delete[]Ls;
}