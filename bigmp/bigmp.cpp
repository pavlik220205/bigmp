// bigmp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <ctime>
using namespace std;

#define ROW_SIZE 25

int getRow(int Pos) { return Pos / ROW_SIZE; }
int getCol(int Pos) { return Pos % ROW_SIZE; }

int getItem(int Array[], int ArraySize, int Row, int Col) {
	if (Row < 0 || Row >= ArraySize / ROW_SIZE) return -1;
	if (Col < 0 || Col >= ROW_SIZE) return -1;
	int pos = Row * ROW_SIZE + Col;
	return pos < ArraySize ? pos : -1;
}


void fillArray(int A[], int N, int Min, int Max) {
	for (int i = 0; i < N; i++)
	{
		int rad = rand() % (Max - Min + 1) + Min;
		A[i] = rad;
	}
}

void setTextColor(int Color)
{
	HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(Console, Color);
}

bool isHighlighted(int HlightList[], int ListSize, int Item) {
	for (int i = 0; i < ListSize; i++) if (Item == HlightList[i]) return true;
	return false;
}


void showArray(int Array[], int ArraySize, int HlightList[], int ListSize) {
	int rc = 0;
	for (int i = 0; i < ArraySize; i++) {
		if (isHighlighted(HlightList, ListSize, i)) setTextColor(12);
		else setTextColor(15);
		cout << " " << Array[i];
		if (rc == (ROW_SIZE - 1)) cout << "\n";
		rc = (rc + 1) % ROW_SIZE;
	}
	if (rc) cout << "\n";
	setTextColor(7);
}


bool TableExist(int arr[], int size, int search) {
	for (int i = 0; i < size; i++)
	{
		if (search == arr[i]) return true;
	}
	return false;
}
///////////////// Local Maximum Search //////////


bool isLocalMaximum(int Array[], int ArraySize, int Pos) {
	int r = getRow(Pos), c = getCol(Pos);
	for (int dr = -1; dr <= 1; dr++)
	{
		for (int dc = -1; dc <= 1; dc++) 
		{
			if (dr == 0 && dc == 0) continue;
			if (Array[Pos] <= Array[getItem(Array, ArraySize, r + dr, c + dc)]) return false;
		}
	}
	return true;
}


void collectLocalMaximums(int Array[], int ArraySize, int List[], int &ListSize) {
	ListSize = 0;
	for (int i = 0; i < ArraySize; i++) if (isLocalMaximum(Array, ArraySize, i))
		List[ListSize++] = i;
}

///////////////////////////////////////////////


///////////////// Horizontal Search //////////
int getHorChainLen(int Array[], int ArraySize, int ChainStart)
{
	int as = 1;
	int r = getRow(ChainStart);
	int c = getCol(ChainStart);
	for (int i = 1; i < ROW_SIZE-c; i++)
	{
		int a = ChainStart + i;
		if (r == getRow(a)) {
			if (Array[ChainStart] == Array[a]) as++;
			else break;
		}
	}
	return as;
}


void searchMaxHorChain(int Array[], int ArraySize, int &ChainStart, int &ChainSize) {
	ChainSize = 0;
	for (int i = 0; i < ArraySize; i++)
	{
		int len = getHorChainLen(Array, ArraySize, i);
		if (ChainSize < len) {
			ChainSize = len;
			ChainStart = i;
		}
	}

}
void getHorChain(int Array[], int ArraySize, int Chain[], int ChainStart, int ChainSize) {
	for (int i = 0; i < ChainSize; i++)
	{
		Chain[i] = ChainStart + i;
	}
}

//////////


// Vertial SEARCH

int getVertChainLen(int Array[], int ArraySize, int ChainStart)
{
	int as = 1;
	int r = getRow(ChainStart);
	for (int i = 1; i < getRow(ArraySize) - r; i++)
	{
		int a = ChainStart + (i * ROW_SIZE);
		if (r != getRow(a) && getRow(a) > r) {
			if (Array[ChainStart] == Array[a]) as++;
			else break;
		}
	}
	return as;
}

void searchMaxVertChain(int Array[], int ArraySize, int &ChainStart, int &ChainSize) {
	ChainSize = 0;
	for (int i = 0; i < ArraySize; i++)
	{
		int len = getVertChainLen(Array, ArraySize, i);
		if (ChainSize < len) {
			ChainSize = len;
			ChainStart = i;
		}
	}

}

void getVertChain(int Array[], int ArraySize, int Chain[], int ChainStart, int ChainSize) {
	for (int i = 0; i < ChainSize; i++)
	{
		Chain[i] = ChainStart + (i * ROW_SIZE);
		cout << ChainStart + (i * ROW_SIZE) << endl;
	}
}
//////////////////////////////////
/////////////////Diagonal Search/////////////


int getDiagChainLen(int Array[], int ArraySize, int ChainStart, bool left = false)
{
	int as = 1;
	int r = getRow(ChainStart);
	for (int i = 1; i < getRow(ArraySize) - r; i++)
	{
		int a;
		if (left) a = ChainStart + (i * ROW_SIZE) - i;
		else a = ChainStart + (i * ROW_SIZE) + i;
		if (r != getRow(a) && ((left && getCol(a) < getCol(ChainStart)) || (!left && getCol(a) > getCol(ChainStart)))) {
			if (Array[ChainStart] == Array[a]) as++;
			else break;
		}
	}
	return as;
}

void searchMaxDiagChain(int Array[], int ArraySize, int &ChainStart, int &ChainSize, bool &left) {
	ChainSize = 0;
	for (int i = 0; i < ArraySize; i++)
	{
		int lenr = getDiagChainLen(Array, ArraySize, i);
		int lenl = getDiagChainLen(Array, ArraySize, i, true);
		
		if (ChainSize < lenr) {
			ChainSize = lenr;
			ChainStart = i;
			left = false;
		}else if(ChainSize < lenl) {
			ChainStart = i;
			ChainSize = lenl;
			left = true;
		}
	}

}

void getDiagChain(int Array[], int ArraySize, int Chain[], int ChainStart, int ChainSize, bool left) {
	for (int i = 0; i < ChainSize; i++)
	{
		if (left) Chain[i] = ChainStart + (i * ROW_SIZE) - i;
		else  Chain[i] = ChainStart + (i * ROW_SIZE) + i;
	}
}



//////////////////////////////////////////////

/////////////////SPOT numbers////////////////
void isSpot(int Array[], int ArraySize, int lpost, int Pos, int List[], int &ListSize) {
	int r = getRow(Pos), c = getCol(Pos);
	for (int i = -1; i <= 1; i++) {
		for (int b = -1; b <= 1; b++) {
			if (abs(i) == abs(b)) continue;
			int as = getItem(Array, ArraySize, r + i, c + b);
			if (!TableExist(List, ListSize, as) && Array[Pos] == Array[as]) {
				List[ListSize++] = as;
				isSpot(Array, ArraySize, Pos, as, List, ListSize);
			}
		}
	}
	return;
}

void collectSpotNumber(int Array[], int ArraySize, int List[], int &ListSize) {
	ListSize = 0;
	int tempArr[10000] = {};

	for (int i = 0; i < ArraySize; i++)
	{
		int tempSize = 0;
		isSpot(Array, ArraySize, i, i, tempArr, tempSize);
		if (tempSize > ListSize) {
			ListSize = tempSize;
			cout << tempSize << endl;
			for (int i = 0; i < ListSize; i++)
			{
				List[i] = tempArr[i];
			}
		}
	}


}
//////////////////////////////////////////////////////////////////////
int showChose() {
	int choise;
	cout << "\n1. Mark local maximums\n2. Mark longest horizontal chain ( -- )\n3. Mark longest vertical chain ( | )\n4. Mark longest diagonal chain ( / or \\ )\n5. Mark longest chain in any direction\n6. Mark largest spot\n7. Try another array\n8. Quit the program \n You choise [1..8]: ";
	cin >> choise;
	return choise;
}
int main()
{
	srand(time(NULL));
	int Arr[1100] = {};
	int markArr[1100] = {};
	int ArrSize = 0;
	int markSize = 0;
	bool running = true;
	cout << "Amount of elements ( <1000 ): ";
	cin >> ArrSize;
	fillArray(Arr, ArrSize, 0, 2);
	showArray(Arr, ArrSize, markArr, markSize);
	do {
		int choise = showChose();
		switch (choise)
		{
		case 1: {
			system("CLS");
			collectLocalMaximums(Arr, ArrSize, markArr, markSize);
			showArray(Arr, ArrSize, markArr, markSize);
			break;
		}
		case 2: {
			int chStart = 0;
			system("CLS");
			searchMaxHorChain(Arr, ArrSize, chStart, markSize);
			getHorChain(Arr, ArrSize, markArr, chStart, markSize);
			showArray(Arr, ArrSize, markArr, markSize);
			break;
		}
		case 3: {
			int chStart = 0;
			system("CLS");
			searchMaxVertChain(Arr, ArrSize, chStart, markSize);
			getVertChain(Arr, ArrSize, markArr, chStart, markSize);
			showArray(Arr, ArrSize, markArr, markSize);
			break;
		}
		case 4: {
			int chStart = 0;
			bool left = false;
			system("CLS");
			searchMaxDiagChain(Arr, ArrSize,chStart, markSize, left);
			getDiagChain(Arr, ArrSize, markArr, chStart, markSize, left);
			showArray(Arr, ArrSize, markArr, markSize);
			break;
		}
		case 5: {
			int tempArrSize = 0;
			int chStart = 0, type = 1;
			bool left = false;
			searchMaxHorChain(Arr, ArrSize, chStart, markSize);
			searchMaxVertChain(Arr, ArrSize, chStart, tempArrSize);
			if (markSize < tempArrSize) {
				markSize = tempArrSize;
				type = 2;
			}
			getDiagChain(Arr, ArrSize, markArr, chStart, tempArrSize, left);
			if (markSize < tempArrSize) {
				markSize = tempArrSize;
				type = 3;
			}
			switch (type)
			{
			case 1: {
				getHorChain(Arr, ArrSize, markArr, chStart, markSize);
				break;
			}
			case 2: {
				getVertChain(Arr, ArrSize, markArr, chStart, markSize);
				break;
			}
			case 3: {
				getDiagChain(Arr, ArrSize, markArr, chStart, markSize,left);
				break;
			}
			}
			showArray(Arr, ArrSize, markArr, markSize);
			break;
		}
		case 6: {
			collectSpotNumber(Arr, ArrSize, markArr, markSize);
			system("CLS");
			showArray(Arr, ArrSize, markArr, markSize);
			break;
		}
		case 7: {
			system("cls");
			cout << "Amount of elements ( <1000 ): ";
			cin >> ArrSize;
			fillArray(Arr, ArrSize, 0, 2);
			markSize = 0;
			showArray(Arr, ArrSize, markArr, markSize);
			break;
		}
		case 8: {
			exit(0);
			break;
		}
		default:
			break;
		}
	} while (true);
}