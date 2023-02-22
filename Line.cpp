#include "Class.h"
using namespace std;


Line::Line() : Cs(new char[1]{'\0'}), size(0) {
}

bool Line::isAlphabet(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}
bool Line::isUpper(char c)
{
	return ((c >= 'A' && c <= 'Z'));
}
bool Line::isLower(char c)
{
	return (c >= 'a' && c <= 'z');
}
void Line::insertCharAt(int index, char c)
{
	char* temp = new char[this->size + 2];
	for (int i = 0, k = 0; i <= this->size;)
	{
		if (i == index)
		{
			temp[k] = c;
			k++;
		}
		temp[k] = this->Cs[i];
		k++;
		i++;
	}
	delete[]this->Cs;
	this->Cs = temp;
	this->size++;
}
bool Line::compareStr(int si, string toFind, bool Case, bool word)
{
	if (Case)
	{
		if (word)
		{
			int i = 0;
			for ( i = 0; i < toFind.length() ; i++)
			{
				if (this->Cs[si + i] == '\0')
					return false;
				
				if (toFind[i] != this->Cs[si + i])
				{
					return false;
				}
			}
			char e = this->Cs[si + toFind.size()];
			if ((si == 0 || this->Cs[si - 1] == ' ' || Cs[si-1]=='.'|| Cs[si-1]==','|| Cs[si-1]=='?' || Cs[si-1]=='!') && (e == '\0' || e == ' ' || e == '.' || e == ',' || e == '?' || e == '!'))
				return true;
			else
				return false;
		}
		else
		{
			int i = 0;
			for ( i = 0; i < toFind.length() ; i++)
			{
				if (this->Cs[si + i] == '\0')
					return false;
				if (toFind[i] != this->Cs[si + i])
				{
					return false;
				}
			}
			return true;
		}
		
	}
	else
	{
		if (word)
		{
			int i = 0;
			for ( i = 0; i < toFind.length() ; i++)
			{
				if (this->Cs[si + i] == '\0')
					return false;

				if (tolower(toFind[i]) != tolower(this->Cs[si + i])) 
				{
					return false;
				}
			}
			char e = this->Cs[si + toFind.size()];
			if ((si == 0 || this->Cs[si - 1] == ' ' || Cs[si - 1] == '.' || Cs[si - 1] == ',' || Cs[si - 1] == '?' || Cs[si - 1] == '!') && (e == '\0' || e == ' ' || e == '.' || e == ',' || e == '?' || e == '!'))
				return true;
			else
				return false;
		}
		else
		{
			for (int i = 0; i < toFind.length(); i++)
			{
				if (this->Cs[si + i] == '\0')
					return false;
				if (tolower(toFind[i]) != tolower(this->Cs[si + i]))
				{
					return false;
				}
			}
			return true;
		}
		
	}
	
}
void Line::removeAt(int index)
{
	char* temp = new char[this->size];

	for (int i = 0, k = 0; i <= this->size; i++)
	{
		if (index != i)
		{
			temp[k] = this->Cs[i];
			k++;
		}
	}
	delete[]this->Cs;
	this->size--;
	this->Cs = temp;
}

void Line::toUpper(int s, int e)
{
	for (int i = s; i <= e; i++)
	{
		if (isLower(this->Cs[i]))
			this->Cs[i] -= 32;
	}
}
void Line::toLower(int s, int e)
{
	for (int i = s; i <= e; i++)
	{
		if (isUpper(this->Cs[i]))
			this->Cs[i] += 32;
	}
}

void Line::printLine(int si)
{
	if(Cs!=nullptr)
		cout << this->Cs+si;
	for (int i = this->size; i < max_COL; i++)
		cout << " ";
}

void Line::printSubstr(int si, int ei)
{
	for (int i = si; i < ei; i++)
	{
		cout << this->Cs[i];
	}
}

int* Line::substringSearch(string toFind,bool Case, bool word)
{
	int toFindSize = toFind.length();
	int* arr = new int[1];
	int count = 1;
	for (int si = 0; si < this->size; si++)
	{
		if (compareStr(si, toFind, Case, word))
		{
			int* HA = new int[count + 1];
			for (int j = 1; j < count; j++)
				HA[j] = arr[j];
			HA[count] = si;
			count++;
			delete[]arr;
			arr = HA;
			si += (toFind.length() - 1); // this line helps to jump over the word which is found
		}
	}
	arr[0] = count-1;
	return arr;
}


void Line::AddPrefix(string ToFind, string Prefix,bool Case,bool word)
{
	int* Found = this->substringSearch(ToFind, Case, word);
	for (int i = Found[0]; i >= 1; i--)
	{
		for(int j=0; j<Prefix.size(); j++)
			this->insertCharAt(Found[i]+j, Prefix[j]);
	}
	delete[]Found;
}
void Line::AddPostfix(string ToFind, string Postfix, bool Case, bool word)
{
	int* Found = this->substringSearch(ToFind, Case, word);

	for (int i = Found[0]; i >= 1; i--)
	{
		for(int j = 0; j<Postfix.size();j++)
			this->insertCharAt(Found[i]+ToFind.size()+j, Postfix[j]);
	}
	delete[]Found;
}
int Line::FindNextOnly(string toFind, int ci, bool Case, bool word)
{
	for (int si = ci; si < this->size; si++)
	{
		if (compareStr(si,toFind,Case,word))
		{
			return si;
		}
	}
	return -1;
}
int Line::FindPrevOnly(string toFind, int ci, bool Case, bool word)
{
	int latestIndex = -1;
	for (int si = 0; si < ci ; si++)
	{
		if (compareStr(si,toFind,Case,word)&& si+toFind.length()<=ci)
		{
			latestIndex = si;
		}
	}
	return latestIndex;
}
int* Line::FindNextAll(string toFind, int ci, bool Case, bool word)
{
	int toFindSize = toFind.length();
	int* arr = new int[1];
	int count = 1;
	for (int si = ci; si < this->size; si++)
	{
		if (compareStr(si,toFind,Case,word))
		{
			
			int* HA = new int[count + 1];
			for (int j = 1; j < count; j++)
				HA[j] = arr[j];
			HA[count] = si;
			count++;
			delete[]arr;
			arr = HA;
			si += (toFind.length() - 1); // this line helps to jump over the word which is found
		}
	}
	arr[0] = count-1;
	return arr;
}

int* Line::FindPrevAll(string toFind, int ci, bool Case, bool word)
{
	int toFindSize = toFind.length();
	int* arr = new int[1];
	int count = 1;
	for (int si = 0; si < ci; si++)
	{
		
		if (compareStr(si,toFind,Case,word)&& si+toFind.length()<=ci)
		{
			
			int* HA = new int[count + 1];
			for (int j = 1; j < count; j++)
				HA[j] = arr[j];
			HA[count] = si;
			count++;
			delete[]arr;
			arr = HA;
			si += (toFind.length() - 1); // this line helps to jump over the word which is found
		}
	}
	arr[0] = count-1;
	return arr;
}


bool Line::ReplaceFirst(string toFind, string toReplace, bool Case, bool word)
{
	for (int si = 0; si < this->size; si++)
	{
		if (compareStr(si,toFind,Case,word))
		{
			if (toFind.length() <= toReplace.length())
			{
				int j = 0;
				for ( j = 0; j < toFind.length(); j++)
				{
					this->Cs[si+j] = toReplace[j];
				}
				for (; j < toReplace.length(); j++)
				{
					this->insertCharAt(si + j, toReplace[j]);
				}
			}
			else
			{
				int j = 0;
				for (j = 0; j < toReplace.length(); j++)
				{
					this->Cs[si + j] = toReplace[j];
				}
				for (int t=j; t < toFind.length(); t++)
				{
					this->removeAt(si + j);
				}
			}
			return true;

		}
	}
	return false;
}

void Line:: ReplaceAll(string toFind, string toReplace, bool Case, bool word)
{
	for (int si = 0; si < this->size; si++)
	{
		if (compareStr(si,toFind,Case,word))
		{
			if (toFind.length() <= toReplace.length())
			{
				int j = 0;
				for (j = 0; j < toFind.length(); j++)
				{
					this->Cs[si + j] = toReplace[j];
				}
				for (; j < toReplace.length(); j++)
				{
					this->insertCharAt(si + j, toReplace[j]);
				}
			}
			else
			{
				int j = 0;
				for (j = 0; j < toReplace.length(); j++)
				{
					this->Cs[si + j] = toReplace[j];
				}
				for (int t = j; t < toFind.length(); t++)
				{
					this->removeAt(si + j);
				}
			}
			si += toReplace.length() - 1;
		}
	}

}

bool Line::FindAndReplaceNextOnly(string toFind, string toReplace, int ci, bool Case, bool word)
{
	for (int si = ci; si < this->size; si++)
	{
		if (compareStr(si, toFind, Case, word))
		{
			if (toFind.length() <= toReplace.length())
			{
				int j = 0;
				for (j = 0; j < toFind.length(); j++)
				{
					this->Cs[si + j] = toReplace[j];
				}
				for (; j < toReplace.length(); j++)
				{
					this->insertCharAt(si + j, toReplace[j]);
				}
			}
			else
			{
				int j = 0;
				for (j = 0; j < toReplace.length(); j++)
				{
					this->Cs[si + j] = toReplace[j];
				}
				for (int t = j; t < toFind.length(); t++)
				{
					this->removeAt(si + j);
				}
			}
			return true;

		}
	}
	return false;
}
bool Line::FindAndReplacePrevOnly(string toFind, string toReplace, int ci, bool Case, bool word)
{
	int si=this->FindPrevOnly(toFind, ci, Case,word);
	if (si != -1)
	{
		if (toFind.length() <= toReplace.length())
		{
			int j = 0;
			for (j = 0; j < toFind.length(); j++)
			{
				this->Cs[si + j] = toReplace[j];
			}
			for (; j < toReplace.length(); j++)
			{
				this->insertCharAt(si + j, toReplace[j]);
			}
		}
		else
		{
			int j = 0;
			for (j = 0; j < toReplace.length(); j++)
			{
				this->Cs[si + j] = toReplace[j];
			}
			for (int t = j; t < toFind.length(); t++)
			{
				this->removeAt(si + j);
			}
		}
		return true;
	}
	return false;
}

void Line::FindAndReplaceNextAll(string toFind, string toReplace, int ci, bool Case, bool word)
{
	for (int si = ci; si < this->size; si++)
	{
		
		if (compareStr(si, toFind, Case, word))
		{
			if (toFind.length() <= toReplace.length())
			{
				int j = 0;
				for (j = 0; j < toFind.length(); j++)
				{
					this->Cs[si + j] = toReplace[j];
				}
				for (; j < toReplace.length(); j++)
				{
					this->insertCharAt(si + j, toReplace[j]);
				}
			}
			else
			{
				int j = 0;
				for (j = 0; j < toReplace.length(); j++)
				{
					this->Cs[si + j] = toReplace[j];
				}
				for (int t = j; t < toFind.length(); t++)
				{
					this->removeAt(si + j);
				}
			}
			si += toReplace.length() - 1;
		}
	}
}

void Line::FindAndReplacePrevAll(string toFind, string toReplace, int ci, bool Case, bool word)
{
	for (int si = 0; si < ci; si++)
	{
		
		if (compareStr(si, toFind, Case, word)&&si+toFind.length()<=ci)
		{
			if (toFind.length() <= toReplace.length())
			{
				int j = 0;
				for (j = 0; j < toFind.length(); j++)
				{
					this->Cs[si + j] = toReplace[j];
				}
				for (; j < toReplace.length(); j++)
				{
					this->insertCharAt(si + j, toReplace[j]);
				}
			}
			else
			{
				int j = 0;
				for (j = 0; j < toReplace.length(); j++)
				{
					this->Cs[si + j] = toReplace[j];
				}
				for (int t = j; t < toFind.length(); t++)
				{
					this->removeAt(si + j);
				}
			}
			si += toReplace.length() - 1;
		}
	}
}
void Line::writer(ofstream& wtr)
{
	wtr << this->Cs << '\n';
}

void Line::encodedWriter(ofstream& wtr)
{
	for (int i = 0; i < this->size; i++)
	{
		wtr << char(this->Cs[i]+34) ;
	}
	wtr << '\n';
}
Line::~Line()
{
	delete[]Cs;
}