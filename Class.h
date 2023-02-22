#pragma once
#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<fstream>
#include<string>
#include "dirent.h"
#include<vector>
using namespace std;

#define path_info "InfoFiles\\"
#define path_Files "Files\\"
#define max_ROW 34
#define max_COL 60
#define BLACK 0
#define BROWN 12
#define WHITE 7
#define RED 4
#define YELL 6
#define LBLUE 9
#define BLUE 1
#define LAQUA 11
#define LYELL 14
#define LPUR 13
#define LGREEN 10
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77


class Line;
class Document;
class TextEditor;
class Line
{
	friend Document;
	friend TextEditor;
	int size;
	char* Cs;
	static bool isAlphabet(char) ;
	static bool isUpper(char );
	static bool isLower(char);
	bool compareStr(int si,string toFind, bool Case, bool word);
	void printSubstr(int si, int ei);
public:
	Line();
	void insertCharAt(int, char);
	void removeAt(int);
	void toUpper(int, int);
	void toLower(int, int);
	int* substringSearch(string toFind, bool Case, bool word);
	void AddPrefix(string ToFind, string Prefix, bool Case, bool word);
	void AddPostfix(string ToFind, string Postfix, bool Case, bool word);
	int FindNextOnly(string Tofind,int,bool Case,bool word);
	int FindPrevOnly(string Tofind, int, bool Case,bool word);
	int* FindNextAll(string Tofind, int, bool Case, bool word);
	int* FindPrevAll(string Tofind, int, bool Case, bool word);
	bool ReplaceFirst(string toFind, string toReplace, bool Case, bool word);
	void ReplaceAll(string toFind, string toReplace, bool Case, bool word);
	bool FindAndReplaceNextOnly(string toFind, string toReplace, int, bool Case, bool word);
	bool FindAndReplacePrevOnly(string toFind, string toReplace, int, bool Case, bool word);
	void FindAndReplaceNextAll(string toFind, string toReplace, int, bool Case, bool word);
	void FindAndReplacePrevAll(string toFind, string toReplace, int, bool Case, bool word);



	void writer(ofstream& wtr);
	void encodedWriter(ofstream& wtr);

	void printLine(int=0);
	~Line();
};

//----------------------------------------------------------------------



class Document
{
	friend Line;
	friend TextEditor;
	string filename;
	int noOflines, dri, dci;
	Line* Ls;
	void deepCopyLines(Line&, const Line&);
	void CopyLines(Line&, const Line&);
	void unhighlightWords(int** WordTable, int size);
public:
	Document(const string& fname);
	Document();
	void Load(const string& fname);
	void simpleLoad(const string& fname);
	void LoadEncoded(const string& fname);
	void Updater(const string &fname);
	void encodedUpdater(const string& fname);
	void Enter_AddLine(int, int);
	void backSpace_line_Remover(int);
	void backSpace_LineConcat(int);
	void printLines(int = 0);
	bool regulateLines(int&,int&);
	void FindAllsubStrings(string toFind,bool Case, bool word);
	void FindNextAll(string toFind,bool Case, bool word);
	void FindPrevAll(string toFind,bool Case, bool word);
	void FindPrevOnly(string toFind,bool Case, bool word);
	void FindNextOnly(string toFind,bool Case, bool word);
	void FindSentence(const string& toFind);
	void FindAndReplaceAll(string toFind, string toReplace, bool Case, bool word);
	void FindAndReplaceNextAll(string toFind, string toReplace, bool Case, bool word);
	void FindAndReplacePrevAll(string toFind, string toReplace, bool Case, bool word);
	void FindAndReplacePrevOnly(string toFind, string toReplace, bool Case, bool word);
	void FindAndReplaceNextOnly(string toFind, string toReplace, bool Case, bool word);
	void FindMenu();
	void addPrefix(string toFind, string prefix, bool Case, bool word);
	void addPostFix(string toFind, string postfix, bool Case, bool word);
	void replaceMenu();
	void prefixMenu();
	void postfixMenu();
	int specialCharCount();
	int paraGraphCount();
	int wordCount(int sLi = 0, int eLi = -1);
	int sentenceCount();
	int largestParaWordCount();
	int smallestWordLength();
	int LargestWordLength();
	double avgWordLength();
	void wordGame();
	void displayDocStas();
	void MoveInFindMode(int** wordTable,string&toFind);
	void ExitDocument();
	void lowerCase();
	void upperCase();
	~Document();


};

//----------------------------------------------------------------------


class TextEditor
{
	friend Line;
	friend Document;
	Document* Doc;
	int cci, cri, noOfdoc, cDoci;
	//static const int MAX_COL, MAX_ROW;

public:
	static void EditorFirstMenu(string&fname, char&);
	TextEditor(string fname);
	void TextEditorInterface();
	void openMultipleDoc(const string&fname,char );
	bool closeADoc();
	void printShortcuts();
	void mergeDocuments(const string& fname1, const string& fname2);
	void run();

};