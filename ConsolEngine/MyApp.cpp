#include "MyApp.h"
#include <ctime>
#include <algorithm>
#include <iterator>

MyApp::MyApp() : Parent(40, 20)
{
	counter = 0;
	workZoneX = X_SIZE*70/100-1; // 70% экрана отводится под стакан

	//Генерация типов первой фигуры, генерация самой фигуры.
	srand(time(NULL));
	nextDT = rand() % 7;
	nextFT = rand() % 4;
	DetGen();
}

void MyApp::KeyPressed( int btnCode )
{
	if( btnCode == 119 ) //w
	{}	
	else if( btnCode == 115 ) //s
	{
		FigureDown();
	}	
	else if( btnCode == 97 ) //a
	{
		FigureLeft();
	}	
	else if( btnCode == 100 ) //d
	{
		FigureRight();
	}
	else if( btnCode == 32 ) //Space
	{
		// Удаление последнего элемента из вектора. С сохранением его текущих координат.
		Cube tempCube(arr[arr.size()-4].x, arr[arr.size()-4].y);
		for( int i = 0; i < 4; i++ )
		{
			arr.pop_back();
		}
		// Изменение угла поворота фигуры.
		fT++;
		if(fT > 3)
			fT = 0;
		// Добавление фигуры с новым углом поворота на место удаленной.
		DetGen( tempCube.x, tempCube.y);
		//Проверка коллизии новой фигуры
		for( int i = arr.size() - 4; i <arr.size(); i++ )
		{
			/*Если пересекает границу стакана удаляем только что созданную фигуру
			возвращаем угол поворота и снов создаем исходную деталь.*/
			if( arr[i].x < 1 || arr[i].y == Y_SIZE - 1 || arr[i].x > workZoneX - 1)
			{
				for( int i = 0; i < 4; i++ )
				{
					arr.pop_back();
				}
				fT--;
				if(fT < 0)
					fT = 3;
				DetGen( tempCube.x, tempCube.y);
			}
			/*Если пересекает другую фигуру удаляем только что созданную фигуру 
			возвращаем угол поворота и снов создаем исходную деталь.*/
			for( int k = 0; k < arr.size()-4; k++ )
			{
				if(arr[i].x == arr[k].x && arr[i].y == arr[k].y)
				{
					for( int i = 0; i < 4; i++ )
					{
						arr.pop_back();
					}
					fT--;
					if(fT < 0)
						fT = 3;
					DetGen( tempCube.x, tempCube.y);
				}
			}
		}
	}
}

void MyApp::UpdateF(float deltaTime)
{
	//Отрисовка границ стакана
	for( int i = 0; i < X_SIZE; i++ )
	{
		for( int k = 0; k < Y_SIZE; k++)
		{
			if( i == 0 || k == 0 || k == Y_SIZE -1 || i == X_SIZE -1 || i == workZoneX || k == 7 && i > workZoneX )
			{
				SetChar(i,k,35);
			}
			else
			{
				SetChar(i,k,'.');
			}
		}
	}
	// Смещаем последнюю фигуру 1 раз за 25 вызовов функции UpdateF
	counter++;
	if(counter % 25 == 0)
	{
		FigureDown();
	}
	// Рисуем фигуры
	DetPrint();
	// Рисуем образ след. фигуры.
	int baseX = workZoneX+4;
	int baseY = 2;
	NextDet( baseX, baseY );
}

void MyApp::RowsErase()
{
	int rowsCheck = 0; // Переменная для посчета количества кубов с одниковой координатой Y
	int rowY = 0; // Переменная для хранения координаты заполненной строки
	/* Если количество кубов в векторе с одинаковой координатой Y равно
	рабочей ширине стакана записываем координату в переменную rowY.*/
	for( int i = 0; i < arr.size(); i++ )
	{
		for( int k = 0; k < arr.size(); k++ )
		{
			if( arr[i].y == arr[k].y )
			{
				rowsCheck++;
			}
		}
		if( rowsCheck == workZoneX - 1)
		{
			rowY = arr[i].y;
		}
		rowsCheck = 0;
	}
	/* Если в переменную попало значение, значит все кубы с данной координатой удаляюся все
	кубы находящиеся выше данной строки смещаются вниз Функция вызывается рекурсивно до тех 
	пор пока не будут уничтожены все заполенные строки*/
	if(rowY)
	{
		for( int i = 0; i < arr.size(); i++ )
		{
			if( arr[i].y == rowY )
			{
				arr.erase( arr.begin() + i );
				i--;

			}
			else if( arr[i].y < rowY )
			{
				arr[i].y++;
			}
		}


		RowsErase();
	}
}
void MyApp::FigureDown()
{
	/* Если последние 4 элемента массива вектор не пересекают Y координату
	других элементов массива или не достигли дна стакана смещаем вниз,
	в противном случае вызываем коллизию*/
	if(arr.size() == 0 || arr.size() < 4 )
	{
		return;
	}
	else
	{
		bool check = true;
		for( int i = arr.size() - 4; i <arr.size(); i++ )
		{
			if( arr[i].y > Y_SIZE-3 )
				check = false;
			for( int k = 0; k < arr.size()-4; k++ )
			{
				if( arr[i].x == arr[k].x && arr[i].y+1 == arr[k].y )
					check = false;
			}
		}
		if(check)
		{
			for( int i = arr.size() - 4; i <arr.size(); i++ )
			{
				arr[i].y++;
			}
		}
		else
		{
			CollCatch();
		}
	}
}

void MyApp::FigureLeft()
{
	/* Если последние 4 элемента массива вектор не пересекают X координату
	других элементов массива или не достигли левой границы стакана смещаем влево*/
	if(arr.size() == 0 || arr.size() < 4 )
	{
		return;
	}
	else
	{
		bool check = true;
		for( int i = arr.size() - 4; i <arr.size(); i++ )
		{
			if( arr[i].x < 2 )
				check = false;
			for( int k = 0; k < arr.size()-4; k++ )
			{
				if( arr[i].x-1 == arr[k].x && arr[i].y == arr[k].y )
					check = false;
			}
		}
		if(check)
		{
			for( int i = arr.size() - 4; i <arr.size(); i++ )
			{
				arr[i].x--;
			}
		}
		else
		{
			return;
		}
	}
}
void MyApp::FigureRight()
{
	/* Если последние 4 элемента массива вектор не пересекают X координату
	других элементов массива или не достигли левой границы стакана смещаем вправо*/
	if(arr.size() == 0 || arr.size() < 4 )
	{
		return;
	}
	else
	{
		bool check = true;
		for( int i = arr.size() - 4; i <arr.size(); i++ )
		{
			if( arr[i].x > workZoneX - 2 )
				check = false;
			for( int k = 0; k < arr.size()-4; k++ )
			{
				if( arr[i].x+1 == arr[k].x && arr[i].y == arr[k].y )
					check = false;
			}
		}
		if(check)
		{
			for( int i = arr.size() - 4; i <arr.size(); i++ )
			{
				arr[i].x++;
			}
		}
	}
}

void MyApp::CollCatch()
{
	// Если фигура вызывет коллизию на вершине станака - стакан переполнен. Игра перезапускается.
	for(int i = 0; i < arr.size(); i++)
	{
		if(arr[i].y == 1)
		{
			arr.clear();
		}
	}
	// Создание след детали.
	DetGen();
	// Проверка наличия заполненных строк.
	// Check for full rows.
	RowsErase();
}

void MyApp::DetPrint() // Функция отрисовывающая элементы вектора в стакане.
{
	for(int i = 0; i < arr.size(); i++)
	{
		SetChar(arr[i].x, arr[i].y, '*');
	}
}


void MyApp::DetGen()
{
	//Генерируем тип след фигуры. Создаем фигуру в вершине стакана на основе прошлой "след. фигуры".
	srand(time(NULL));
	dT = nextDT;
	fT = nextFT;
	nextDT = rand() % 7;
	nextFT = rand() % 4;
	int baseX = workZoneX/2; // центр стакана
	int baseY = 1;
	DetGen( baseX, baseY );
}
 void MyApp::NextDet( int baseX, int baseY ) // Функция Рисующая образ след. детали на основе переменных nextDT и nextFT в заданной точке экрана
{
	switch(nextDT)
	{
	case 0:
		{
			switch(nextFT)
			{
			case 0:
				{
					SetChar( baseX, baseY, '*');
					SetChar( baseX, baseY+1, '*');
					SetChar( baseX+1, baseY+1, '*');
					SetChar( baseX, baseY+2, '*');
					break;
				}
			case 1:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX+1,baseY, '*');
					SetChar(baseX+2,baseY, '*');
					SetChar(baseX+1,baseY+1, '*');
					break;
				}
			case 2:
				{
					SetChar(baseX,baseY+1, '*');
					SetChar(baseX+1,baseY, '*');
					SetChar(baseX+1,baseY+1, '*');
					SetChar(baseX+1,baseY+2, '*');
					break;
				}
			case 3:
				{
					SetChar(baseX,baseY+1, '*');
					SetChar(baseX+1,baseY, '*');
					SetChar(baseX+1,baseY+1, '*');
					SetChar(baseX+2,baseY+1, '*');
					break;
				}
			}
			break;
		}
	case 1:
		{
			if(nextFT == 2)
				nextFT = 0;
			if(nextFT == 3)
				nextFT = 1;
			switch(nextFT)
			{
			case 0:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX+1,baseY, '*');
					SetChar(baseX+2,baseY, '*');
					SetChar(baseX+3,baseY, '*');
					break;
				}
			case 1:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX,baseY+1, '*');
					SetChar(baseX,baseY+2, '*');
					SetChar(baseX,baseY+3, '*');
					break;
				}
			}
			break;
		}
	case 2:
		{
			SetChar(baseX,baseY, '*');
			SetChar(baseX,baseY+1, '*');
			SetChar(baseX+1,baseY, '*');
			SetChar(baseX+1,baseY+1, '*');
			break;
		}
	case 3:
		{
			if(nextFT == 2)
				nextFT = 0;
			if(nextFT == 3)
				nextFT = 1;
			switch(nextFT)
			{
			case 0:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX,baseY+1, '*');
					SetChar(baseX-1,baseY+1, '*');
					SetChar(baseX-1,baseY+2, '*');
					break;
				}
			case 1:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX,baseY+1, '*');
					SetChar(baseX-1,baseY, '*');
					SetChar(baseX+1,baseY+1, '*');
					break;
				}
			}
			break;
		}
	case 4:
		{
			if(nextFT == 2)
				nextFT = 0;
			if(nextFT == 3)
				nextFT = 1;
			switch(nextFT)
			{
			case 0:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX,baseY+1, '*');
					SetChar(baseX+1,baseY+1, '*');
					SetChar(baseX+1,baseY+2, '*');
					break;
				}
			case 1:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX-1,baseY+1, '*');
					SetChar(baseX,baseY+1, '*');
					SetChar(baseX+1,baseY, '*');
					break;
				}
			}
			break;
		}
	case 5:
		{
			switch(nextFT)
			{
			case 0:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX+1,baseY+1, '*');
					SetChar(baseX+1,baseY, '*');
					SetChar(baseX+1,baseY+2, '*');
					break;
				}
			case 1:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX,baseY+1, '*');
					SetChar(baseX+1,baseY, '*');
					SetChar(baseX+2,baseY, '*');
					break;
				}
			case 2:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX,baseY+1, '*');
					SetChar(baseX,baseY+2, '*');
					SetChar(baseX+1,baseY+2, '*');
					break;
				}
			case 3:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX,baseY+1, '*');
					SetChar(baseX-1,baseY+1, '*');
					SetChar(baseX-2,baseY+1, '*');
					break;
				}
			}
			break;
		}
	case 6:
		{
			switch(nextFT)
			{
			case 0:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX+1,baseY, '*');
					SetChar(baseX,baseY+1, '*');
					SetChar(baseX,baseY+2, '*');
					break;
				}
			case 1:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX+1,baseY, '*');
					SetChar(baseX+2,baseY, '*');
					SetChar(baseX+2,baseY+1, '*');
					break;
				}
			case 2:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX,baseY+1, '*');
					SetChar(baseX,baseY+2, '*');
					SetChar(baseX-1,baseY+2, '*');
					break;
				}
			case 3:
				{
					SetChar(baseX,baseY, '*');
					SetChar(baseX,baseY+1, '*');
					SetChar(baseX+1,baseY+1, '*');
					SetChar(baseX+2,baseY+1, '*');
					break;
				}
			}
			break;
		}
	}
}

void MyApp::DetGen( int baseX, int baseY ) 
/* Перегрузка функции которая добавляет 4 элемента с координатами соот. нужной детали
в вектор*/
{
	switch(dT)
	{
	case 0:
		{
			switch(fT)
			{
			case 0:
				{
					arr.push_back(Cube( baseX, baseY));
					arr.push_back(Cube( baseX, baseY+1));
					arr.push_back(Cube( baseX+1, baseY+1));
					arr.push_back(Cube( baseX, baseY+2));
					break;
				}
			case 1:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX+1,baseY));
					arr.push_back(Cube(baseX+2,baseY));
					arr.push_back(Cube(baseX+1,baseY+1));
					break;
				}
			case 2:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX-1,baseY+1));
					arr.push_back(Cube(baseX,baseY+2));
					arr.push_back(Cube(baseX,baseY+1));
					break;
				}
			case 3:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX-1,baseY+1));
					arr.push_back(Cube(baseX,baseY+1));
					arr.push_back(Cube(baseX+1,baseY+1));
					break;
				}
			}
			break;
		}
	case 1:
		{
			if(fT == 2)
				fT = 0;
			if(fT == 3)
				fT = 1;
			switch(fT)
			{
			case 0:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX+1,baseY));
					arr.push_back(Cube(baseX+2,baseY));
					arr.push_back(Cube(baseX+3,baseY));
					break;
				}
			case 1:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX,baseY+1));
					arr.push_back(Cube(baseX,baseY+2));
					arr.push_back(Cube(baseX,baseY+3));
					break;
				}
			}
			break;
		}
	case 2:
		{
			arr.push_back(Cube(baseX,baseY));
			arr.push_back(Cube(baseX,baseY+1));
			arr.push_back(Cube(baseX+1,baseY));
			arr.push_back(Cube(baseX+1,baseY+1));
			break;
		}
	case 3:
		{
			if(fT == 2)
				fT = 0;
			if(fT == 3)
				fT = 1;
			switch(fT)
			{
			case 0:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX,baseY+1));
					arr.push_back(Cube(baseX-1,baseY+1));
					arr.push_back(Cube(baseX-1,baseY+2));
					break;
				}
			case 1:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX,baseY+1));
					arr.push_back(Cube(baseX-1,baseY));
					arr.push_back(Cube(baseX+1,baseY+1));
					break;
				}
			}
			break;
		}
	case 4:
		{
			if(fT == 2)
				fT = 0;
			if(fT == 3)
				fT = 1;
			switch(fT)
			{
			case 0:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX,baseY+1));
					arr.push_back(Cube(baseX+1,baseY+1));
					arr.push_back(Cube(baseX+1,baseY+2));
					break;
				}
			case 1:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX-1,baseY+1));
					arr.push_back(Cube(baseX,baseY+1));
					arr.push_back(Cube(baseX+1,baseY));
					break;
				}
			}
			break;
		}
	case 5:
		{
			switch(fT)
			{
			case 0:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX+1,baseY+1));
					arr.push_back(Cube(baseX+1,baseY));
					arr.push_back(Cube(baseX+1,baseY+2));
					break;
				}
			case 1:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX,baseY+1));
					arr.push_back(Cube(baseX+1,baseY));
					arr.push_back(Cube(baseX+2,baseY));
					break;
				}
			case 2:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX,baseY+1));
					arr.push_back(Cube(baseX,baseY+2));
					arr.push_back(Cube(baseX+1,baseY+2));
					break;
				}
			case 3:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX,baseY+1));
					arr.push_back(Cube(baseX-1,baseY+1));
					arr.push_back(Cube(baseX-2,baseY+1));
					break;
				}
			}
			break;
		}
	case 6:
		{
			switch(fT)
			{
			case 0:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX+1,baseY));
					arr.push_back(Cube(baseX,baseY+1));
					arr.push_back(Cube(baseX,baseY+2));
					break;
				}
			case 1:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX+1,baseY));
					arr.push_back(Cube(baseX+2,baseY));
					arr.push_back(Cube(baseX+2,baseY+1));
					break;
				}
			case 2:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX,baseY+1));
					arr.push_back(Cube(baseX,baseY+2));
					arr.push_back(Cube(baseX-1,baseY+2));
					break;
				}
			case 3:
				{
					arr.push_back(Cube(baseX,baseY));
					arr.push_back(Cube(baseX,baseY+1));
					arr.push_back(Cube(baseX+1,baseY+1));
					arr.push_back(Cube(baseX+2,baseY+1));
					break;
				}
			}
			break;
		}
	}
}

