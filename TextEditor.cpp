#include "Class.h"


void gotoRowCol(int rpos, int cpos);
void clearScr(int sr, int sc, int rows, int cols);
void getRowColbyLeftClick(int& rpos, int& cpos);




void TextEditor::EditorFirstMenu(string &fname, char&option)
{
	system("cls");
	gotoRowCol(3, 20);
	cout << "<-----------------------TEXT EDITOR Pro Version----------------------------->";
	gotoRowCol(8, 34);
	cout << "1) Open A new File";
	gotoRowCol(9, 34);
	cout << "2) Open an existing File";
	gotoRowCol(10, 34);
	cout << "3) Exit" << endl;
	cout << "Existing Files: \n\n";
	//recent files
	DIR* dir; struct dirent* diread;
	vector<string> files{};
	if ((dir = opendir("/Users/ghufr/source/OOP ka Kam idhr he/TextEditor/Files")) != nullptr) {
		while ((diread = readdir(dir)) != nullptr) {
			files.push_back((*diread).d_name);
		}
		closedir(dir);
	}

	for (int i = 2; i < files.size(); i++)
	{
		
		cout <<files[i] <<" | ";
		if (i % 6 == 0)
			cout << endl;
	}
	cout << endl;
	cout << "\nSelect an option ";
	do
	{
		if (_kbhit())
		{
			option = _getch();
			if (option == '1')
			{

				ifstream rdr;
				cout << "\nEnter filename: ";
				getline(cin, fname);
				rdr.open(path_Files+fname );
				if (rdr)
				{
					cout << "\nFile already exists! can't create file";
					rdr.close();
					continue;
				}
				break;
			}
			else if (option == '2')
			{
				ifstream rdr;
				cout << "\nEnter filename: ";
				getline(cin, fname);
				rdr.open(path_Files+fname );
				if (!rdr)
				{
					cout << "\nFile not Found! can't open file ";
					continue;
				}
				else
				{
					rdr.close();
					break;
				}

			}
			else if (option == '3')
				exit(0);
		}

	} while (option!='3');
}

TextEditor::TextEditor(string fname) : cci(0), cri(0), noOfdoc(1), cDoci(0), Doc(new Document[1]{fname}) {

	Doc[cDoci].Load(fname);
	cri=Doc[cDoci].dri,cci=Doc[cDoci].dci;
}
void TextEditor::TextEditorInterface()
{
	system("cls");
	for (int i = 0; i <= max_ROW; i++)
	{
		gotoRowCol(i, max_COL);
		cout << char(186);
	}
	for (int i = 0; i < max_COL; i++)
	{
		gotoRowCol(max_ROW, i);
		cout << char(205);
	}
	gotoRowCol(max_ROW, max_COL);
	cout << char(188);
	gotoRowCol(5, max_COL + 5);
	cout << "PAGE: " << cri / max_ROW + 1 << "    \b\b\bof " << "   \b\b\b" << (Doc[cDoci].noOflines - 1) / max_ROW + 1<<"   ";
	Doc[cDoci].printLines((cri/max_ROW)*max_ROW );
	gotoRowCol(0, max_COL + 5);
	cout << "Document: " << Doc[cDoci].filename<<"                      ";
	gotoRowCol(3, max_COL + 5);
	cout << "DOC: " << cDoci + 1 << "    \b\b\bof " << "   \b\b\b" << noOfdoc <<"  ";
}
void TextEditor::openMultipleDoc(const string& fname, char option)
{
	Document* newDocs = new Document[noOfdoc + 1];
	for (int i = 0; i < noOfdoc; i++)
	{
		delete[] newDocs[i].Ls;
		newDocs[i] = Doc[i];
		Doc[i].Ls = nullptr;
	}
	delete[]Doc;

	newDocs[noOfdoc].Load(fname);
	newDocs[noOfdoc].filename = fname;
	noOfdoc++;
	cDoci = noOfdoc-1;
	Doc=newDocs;
	cri = Doc[cDoci].dri, cci = Doc[cDoci].dci;
	

}
void TextEditor::run()
{
	int input,ptr=0,r=max_ROW,c=max_COL;
	int currentPage = cri / max_ROW;
	int MAX_PAGE = (Doc[cDoci].noOflines - 1) / max_ROW;
	this->TextEditorInterface();
	gotoRowCol(cri%max_ROW, cci);
	while (true)
	{
		if (GetKeyState(VK_LBUTTON)  < 0)
		{
			getRowColbyLeftClick(r, c);
			currentPage = cri / max_ROW;
			if (r < max_ROW && c < max_COL)
			{
				r = r + (currentPage * max_ROW);
				if (r < Doc[cDoci].noOflines )
					cri = r;
				if (c > Doc[cDoci].Ls[cri].size)
					cci = Doc[cDoci].Ls[cri].size;
				else 
					cci = c;
				Doc[cDoci].dri = cri, Doc[cDoci].dci = cci;
			}
			
			gotoRowCol(cri%max_ROW, cci);
		}
		if (_kbhit())
		{
			currentPage = cri / max_ROW;
			MAX_PAGE = (Doc[cDoci].noOflines - 1) / max_ROW;
			input = _getch();

			if (input == 224)
			{
				input = _getch();
				switch (input)
				{
				case UP:
					if (cri != 0)
					{
						cri--;
					}
					break;

				case DOWN:

					if (cri != Doc[cDoci].noOflines - 1)
					{
						cri++;
					}
					break;

				case RIGHT:

					cci++;
					if (cci > Doc[cDoci].Ls[cri].size)
					{
						if (cri != Doc[cDoci].noOflines - 1)
						{
							cri++;
							cci = 0;
						}
					}
					break;

				case LEFT:

					cci--;
					if (cci < 0)
					{
						if (cri != 0)
						{
							cri--;
							cci = Doc[cDoci].Ls[cri].size;
						}
						else
							cci = 0;
					}

					break;

				case 83: //DEl key

					if (cci != Doc[cDoci].Ls[cri].size)
					{
						Doc[cDoci].Ls[cri].removeAt(cci);
						gotoRowCol(cri % max_ROW, 0);
						Doc[cDoci].Ls[cri].printLine();
					}
					break;

				case 71: //home key
					cci = 0;
					break;

				case 79: //end key
					cci = Doc[cDoci].Ls[cri].size;
					break;


				case 73: //pg up key
					if (currentPage != 0)
					{
						cri = (currentPage - 1) * max_ROW;
						cci = 0;
						Doc[cDoci].printLines(cri);
					}
					break;

				case 81: //pg dn key
					if (currentPage != MAX_PAGE)
					{
						cri = (currentPage + 1) * max_ROW;
						cci = 0;
						Doc[cDoci].printLines(cri);
					}
					break;
				case 134://ctrl+pgup
					if (cDoci != 0)
					{
						cDoci--;
						TextEditorInterface();
						cri = Doc[cDoci].dri, cci = Doc[cDoci].dci;
					}
					break;
				case 118: //ctr+pgdn
					if (cDoci != noOfdoc - 1)
					{
						cDoci++;
						TextEditorInterface();
						cri = Doc[cDoci].dri, cci = Doc[cDoci].dci;
					}
					break;

				}
				if (cci > Doc[cDoci].Ls[cri].size) //up and down key cases if the cursor is at a point outside the current line size.
				{
					cci = Doc[cDoci].Ls[cri].size;
				}
			}
			else if (input == 0)//Function keys
			{
				input = _getch();
				switch (input)
				{
				case 59: //F1 find menu
					Doc[cDoci].dri = cri, Doc[cDoci].dci = cci;
					Doc[cDoci].FindMenu();
					clearScr(max_ROW+15, 0, 9, 35);
					cri=Doc[cDoci].dri , cci=Doc[cDoci].dci ;
					gotoRowCol(0, 0);
					break;
				case 60:
					Doc[cDoci].dri = cri, Doc[cDoci].dci = cci;
					Doc[cDoci].replaceMenu();
					clearScr(max_ROW + 15, 0, 13, 50);
					cri = Doc[cDoci].dri, cci = Doc[cDoci].dci;
					gotoRowCol(0, 0);
					break;
				case 61:
					Doc[cDoci].displayDocStas();
					break;
				case 64:
					Doc[cDoci].prefixMenu();
					break;
				case 65:
					Doc[cDoci].postfixMenu();
					break;
				case 63:
					printShortcuts();
					break;
				case 66:
					Doc[cDoci].FindSentence("nothing");
					break;

				}
			}
			else
			{
				int ci = 0, ri =0;
				switch (input)
				{
			
				case 13://	ENTER
					Doc[cDoci].Enter_AddLine(cci, cri);
					Doc[cDoci].printLines(cri);
					cri++;
					cci = 0;
					break;


				case 8: //backspace key
					if (cci != 0) //single char removal
					{
						cci--;
						Doc[cDoci].Ls[cri].removeAt(cci);
						gotoRowCol(cri % max_ROW, cci);
						Doc[cDoci].Ls[cri].printLine(cci);

					}
					else if (cci == 0 && cri != 0 && Doc[cDoci].Ls[cri].size == 0) //case for empty line
					{
						Doc[cDoci].backSpace_line_Remover(cri);
						cri--;

						Doc[cDoci].printLines(cri);
						//cleaning the last line as no of lines are reuced
						for (int i = 0; i < max_COL; i++)
						{
							if (currentPage != MAX_PAGE)
								break;
							gotoRowCol(Doc[cDoci].noOflines % max_ROW, i);
							cout << " ";
						}
						cci = Doc[cDoci].Ls[cri].size;
					}
					else if (cci == 0 && cri != 0 && Doc[cDoci].Ls[cri].size != 0)//case for a line having some characters in it
					{
						cci = Doc[cDoci].Ls[cri - 1].size;
						Doc[cDoci].backSpace_LineConcat(cri);
						cri--;
						Doc[cDoci].printLines(cri);
						//cleaning the last line as no of lines are reuced
						for (int i = 0; i < max_COL; i++)
						{
							if (currentPage != MAX_PAGE)
								break;
							gotoRowCol(Doc[cDoci].noOflines % max_ROW, i);
							cout << " ";
						}

					}
					break;

				
				case 19: //ctrl+S for save
					
					Doc[cDoci].dri = cri, Doc[cDoci].dci = cci;
					Doc[cDoci].Updater(Doc[cDoci].filename);
					break;
				case 5: //ctrl+e encoded save
				{

					string password = "";
					gotoRowCol(max_ROW + 10, 0);
					cout << "Enter Password to set: ";
					getline(cin, password);
					clearScr(max_ROW + 10, 0,1, 23 + password.length());
					gotoRowCol(0, 0);
					ofstream wtr(path_info+Doc[cDoci].filename + ".info");
					wtr << 1 << '\n' << password << '\n' << Doc[cDoci].noOflines << "\n" << cri << "\n" << cci;
					wtr.close();
				}
	
				Doc[cDoci].encodedUpdater(Doc[cDoci].filename);
				break;
				case 15: //ctrl+O for opening a new file
				{
					bool isOpened = false;
					string fname = "";
					char option = '0';
					do
					{
						isOpened = false;
						TextEditor::EditorFirstMenu(fname, option);
						for (int i = 0; i < noOfdoc; i++)
						{
							if (fname == Doc[i].filename)
							{
								cout << "\nAlready opened enter again\n";
								system("pause");
								isOpened = true;
								break;
							}
						}
					} while (isOpened);
					
					this->openMultipleDoc(fname, option);
					this->TextEditorInterface();
					break;
				}
				case 10: //ctrl+j merging documents

				{
					string fname1="",  fname2 = "";
					gotoRowCol(100, 0);
					gotoRowCol(max_ROW + 30, 0);
					ifstream rdr;
					do
					{
						cout << "Enter filename 1: ";
						getline(cin, fname1);
						rdr.open(path_info+fname1 + ".info");
						if (!rdr)
						{
							cout << "\nCan't find file Again ";
						}
					} while (!rdr);
					rdr.close();
					do
					{
						cout << "Enter filename 2: ";
						getline(cin, fname2);
						rdr.open(path_info + fname2 + ".info");
						if (!rdr)
						{
							cout << "\nCan't find file Again ";
						}
					} while (!rdr);
					mergeDocuments(fname1, fname2);
					clearScr(max_ROW + 30, 0, 15, 30);
					gotoRowCol(0, 0);
				}
					break;
				case 27: //esc key exiting documents
					if (noOfdoc > 1)
					{
						if(closeADoc())
							TextEditorInterface();
					}
					else if (noOfdoc == 1)
					{
						if (closeADoc())
						{
							string fname = "";
							char o = 'o';
							TextEditor::EditorFirstMenu(fname, o);
							Doc = new Document[1]{ {fname} };
							Doc[cDoci].filename = fname;
							Doc[cDoci].Load(fname);
							cri = Doc[cDoci].dri, cci = Doc[cDoci].dci;
							TextEditorInterface();
						}
					}
					break;
				case 23: //ctrl+W
					Doc[cDoci].wordGame();
					break;
				case 21://ctrl+U
					Doc[cDoci].upperCase();
					break;
				case 12://ctrl+l
					Doc[cDoci].lowerCase();
					break;
				default:
					Doc[cDoci].Ls[cri].insertCharAt(cci, input);
					cci++;
					ci = cci,ri=cri;
					if (Doc[cDoci].regulateLines(ri, ci))
					{
						Doc[cDoci].printLines(cri);
					}
					else
					{
						gotoRowCol(cri % max_ROW, 0);
						Doc[cDoci].Ls[cri].printLine();
					}
					cci = ci, cri = ri;
					break;
				}
			}
			if (currentPage != cri / max_ROW)
			{
				gotoRowCol(5, max_COL + 5);
				currentPage = cri / max_ROW;
				cout << "PAGE: " << currentPage + 1 << "    \b\b\bof " << "   \b\b\b" << (Doc[cDoci].noOflines - 1) / max_ROW + 1<<"  ";
				Doc[cDoci].printLines(currentPage * max_ROW);
			}
			if (MAX_PAGE != (Doc[cDoci].noOflines - 1) / max_ROW)
			{
				gotoRowCol(5, max_COL + 5);
				MAX_PAGE = (Doc[cDoci].noOflines - 1) / max_ROW;
				cout << "PAGE: " << currentPage + 1 << "    \b\b\bof " <<"   \b\b\b"<< MAX_PAGE+1<<"  ";
			}
			gotoRowCol(cri % max_ROW, cci);
			Doc[cDoci].dri = cri,
				Doc[cDoci].dci = cci;
		
		}
	}
}


void TextEditor::printShortcuts()
{
	string shortcuts = "";
	ifstream rdr("tEditorShortcuts.txt");
	getline(rdr, shortcuts, '!');
	gotoRowCol(220, 0);
	gotoRowCol(max_ROW+20, 0);
	cout << shortcuts;
	system("Pause");
	clearScr(max_ROW + 20, 0, 50, 70);
	gotoRowCol(0, 0);
}


void TextEditor::mergeDocuments(const string& fname1, const string& fname2)
{
	Document doc1,doc2;
	doc1.Load(fname1);
	doc2.Load(fname2);

	string destination="";
	cout << "\nEnter Destination File: ";
	getline(cin, destination);
	ofstream wtr(path_Files+destination );

	for (int i = 0; i < doc1.noOflines; i++)
		doc1.Ls[i].writer(wtr);
	for (int i = 0; i < doc2.noOflines; i++)
		doc2.Ls[i].writer(wtr);
	wtr.close();
	wtr.open(path_info+destination + ".info");
	wtr << 0 << '\n' << doc1.noOflines + doc2.noOflines << '\n' << 0 << '\n' << 0;
	wtr.close();
	gotoRowCol(0, 0);

}


bool TextEditor::closeADoc()
{
	char state = 'y';
	gotoRowCol(100, 0);
	gotoRowCol(max_ROW + 30, 0);
	cout << "Do you want to save and Exit or Cancel(y/n/c): ";

	do
	{
		if (_kbhit())
		{
			state = _getch();
			break;
		}
	} while (true);
	state = tolower(state);
	if (state == 'c')
		return false;
	else if (state == 'y')
		Doc[cDoci].Updater(Doc[cDoci].filename);
	if (noOfdoc == 1)
	{
		delete[] this->Doc;
		cDoci = 0;
		return true;
	}
	Document* newDocs = new Document[noOfdoc - 1];
	for (int ni = 0, i = 0; i < noOfdoc; i++)
	{
		if (i != cDoci)
		{
			delete[] newDocs[ni].Ls;
			newDocs[ni] = Doc[i];
			Doc[i].Ls = nullptr;
			ni++;
		}

	}
	delete[]Doc;
	Doc = newDocs;
	noOfdoc--;
	cDoci = 0;
	cri = Doc[cDoci].dri,
		cci = Doc[cDoci].dci;
	return true;

}