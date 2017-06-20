#pragma once

#include "BaseApp.h"
#include <vector>
#include <iostream>
using namespace std;

struct Cube
{
	int x;
	int y;
	Cube( int X, int Y )
	{
		x = X;
		y = Y;
	}
};

class MyApp : public BaseApp
{
	typedef BaseApp Parent;

private:
	int workZoneX; // ���������� ��� �������� ������ ������� �������
	int counter; // ������� ������ ������� UpdateF. ��������� ��� ����������� �������� ������� �����.
	int dT; // ���������� �������� ��� ������
	int fT; // ���������� �������� ���� �������� ������
	int nextDT; // ���������� �������� ��� ����. ������
	int nextFT; // ���������� �������� ���� �������� ����. ������
	vector<Cube> arr; // ������ � ������� �������� ���� ������(4 ����. Cube = 1 ������)

	void FigureDown(); // ������� ���������� �������� ���� ��������� ������ �� ������� arr
	void FigureLeft(); // ������� ���������� �������� ����� ��������� ������ �� ������� arr
	void FigureRight(); // ������� ���������� �������� ������ ��������� ������ �� ������� arr
	void RowsErase(); // ������� ���������� �������� ��������� �� ������� ���� ��� ��������� ��� ������ �������
	void DetGen(); // ������� ������� ������� ����� ������ �� ������ ���������� dT � fT � ��������� ����� ������� .
	void DetGen( int baseX, int baseY ); // ���������� ������� ������� ������� ����� ������ �� ������ ���������� dT � fT � �������� ����� ������� .
	void NextDet( int baxeX, int base ); // ������� �������� ����� ����. ������ �� ������ ���������� nextDT � nextFT � �������� ����� ������
	void CollCatch(); // ������� ������������ ����������� ������� ��������.
	void DetPrint(); // ������� �������������� �������� ������� � �������.

public:
	MyApp();
	
	virtual void KeyPressed(int btnCode); 
	virtual void UpdateF(float deltaTime);
};
