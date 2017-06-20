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
	int workZoneX; // Переменная для хранения правой границы стакана
	int counter; // Счетчик вызова функции UpdateF. Необходим для регулировки скорости падения фигур.
	int dT; // Переменная хранящая тип фигуры
	int fT; // Переменная хранящая угол поворота фигуры
	int nextDT; // Переменная хранящая тип след. фигуры
	int nextFT; // Переменная хранящая угол поворота след. фигуры
	vector<Cube> arr; // Вектор в котором хранятся наши фигуры(4 элем. Cube = 1 фигура)

	void FigureDown(); // Функция вызывающая смещение вниз последней фигуры из вектора arr
	void FigureLeft(); // Функция вызывающая смещение влево последней фигуры из вектора arr
	void FigureRight(); // Функция вызывающая смещение вправо последней фигуры из вектора arr
	void RowsErase(); // Функция вызывающая удаление элементов из вектора если они заполнили всю ширину стакана
	void DetGen(); // Функция которая создает новую деталь на основе переменных dT и fT в стартовой точке стакана .
	void DetGen( int baseX, int baseY ); // Перегрузка функции которая создает новую деталь на основе переменных dT и fT в заданной точке стакана .
	void NextDet( int baxeX, int base ); // Функция Рисующая образ след. детали на основе переменных nextDT и nextFT в заданной точке экрана
	void CollCatch(); // Функция регистриющая наступление момента коллизии.
	void DetPrint(); // Функция отрисовывающая элементы вектора в стакане.

public:
	MyApp();
	
	virtual void KeyPressed(int btnCode); 
	virtual void UpdateF(float deltaTime);
};
