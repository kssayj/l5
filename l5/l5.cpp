#include<iostream> 
#include <fstream>
using namespace std;

int MinPath(int** arr, int N, int M) { // заводим таблицу нужного размера и значения первого столбца первого массива проставляем в первый столбец второго
	int** table = new int* [N]; //массив с данными
	int** prev_x = new int* [N]; //массив индексов строк
	int** prev_y = new int* [N]; //массив индексов столбцов
	for (int i = 0; i < N; ++i) {
		table[i] = new int[M];
		prev_x[i] = new int[M];
		prev_y[i] = new int[M];
	}
	for (int i = 0; i < N; ++i) { //заполняем первый столбец 
		table[i][0] = arr[i][0];
	}
	if (N > 1) {
		table[0][1] = table[1][0] + arr[0][1]; //(для верхнего элемента), для каждой клетки задаем минимум (для верхней клетки - это минимум из правее ниже, левее и та, что левее ниже + значение самой клетки)
		prev_x[0][1] = 1;
		prev_y[0][1] = 0;
		table[N - 1][1] = table[N - 2][0] + arr[N - 1][1]; //выбор между самым нижним элементом и преднижнем
		prev_x[N - 1][1] = N - 2;
		prev_y[N - 1][1] = 0;
	}
	for (int i = 1; i + 1 < N; ++i) { // клетки, которые в середине, минимум для нее состоит из клетки, которая выше левее, левее и ниже левее. берем минимум из этих клеток и прибавляем значение самой клетки 
		table[i][1] = arr[i][1]; // для клеток второго ряда выбираем - с какой из клеток с первого ряда выгоднее идти
		if (table[i - 1][0] < table[i + 1][0]) { // сверху (если путь до клетки сверху содержит меньшую сумму)
			table[i][1] += table[i - 1][0];
			prev_x[i][1] = i - 1;
			prev_y[i][1] = 0;
		}
		else {
			table[i][1] += table[i + 1][0]; // снизу(если путь до клетки снизу содержит меньше сумму)
			prev_x[i][1] = i + 1;
			prev_y[i][1] = 0;
		}
	}
	for (int j = 2; j < M; ++j) { //количество столбцов 
		if (N > 1) {
			table[0][j] = arr[0][j]; //(для верхнего элемента), для каждой клетки задаем минимум (для верхней клетки - это минимум из правее ниже, левее и та, что левее ниже + значение самой клетки)
			if (table[0][j - 2] < table[1][j - 1]) {
				table[0][j] += table[0][j - 2];
				prev_x[0][j] = 0;
				prev_y[0][j] = j - 2;
			}
			else {
				table[0][j] += table[1][j - 1];
				prev_x[0][j] = 1;
				prev_y[0][j] = j - 1;
			}
			table[N - 1][j] = arr[N - 1][j]; //выбор между самым нижним элементом и преднижнем
			if (table[N - 2][j - 1] < table[N - 1][j - 2]) {
				table[N - 1][j] += table[N - 2][j - 1];
				prev_x[N - 1][j] = N - 2;
				prev_y[N - 1][j] = j - 1;
			}
			else {
				table[N - 1][j] += table[N - 1][j - 2];
				prev_x[N - 1][j] = N - 1;
				prev_y[N - 1][j] = j - 2;
			}
		}
		for (int i = 1; i + 1 < N; ++i) { // клетки, которые в середине, минимум для нее состоит из клетки, которая выше левеее, левее и ниже левее. берем минимум из этих клеток и прибавляем значение самой клетки
			table[i][j] = arr[i][j];
			if (table[i - 1][j - 1] <= table[i][j - 2] && table[i - 1][j - 1] <= table[i + 1][j - 1]) { // случай оптимальности верхнедиагональной клетки
				table[i][j] += table[i - 1][j - 1];
				prev_x[i][j] = i - 1;
				prev_y[i][j] = j - 1;
			}
			else if (table[i][j - 2] <= table[i - 1][j - 1] && table[i][j - 2] <= table[i + 1][j - 1]) { // случай оптимальности клетки на нашей строке за 2 столбца до нас
				table[i][j] += table[i][j - 2];
				prev_x[i][j] = i;
				prev_y[i][j] = j - 2;
			}
			else { // случай оптимальности нижнедиагональной клетки
				table[i][j] += table[i + 1][j - 1];
				prev_x[i][j] = i + 1;
				prev_y[i][j] = j - 1;
			}
		}
	}

	int result = table[0][M - 1]; //последний столбец, выбераем минимум
	int x = 0;
	int y = M - 1;
	for (int i = 1; i < N; ++i) { // выбираем клетку с минимальным путём(клетку ответа)
		if (result > table[i][M - 1]) {
			x = i;
			result = table[i][M - 1];
		}
	}
	int* answer_x = new int[M]; // готовим вектор координат пути по х
	int* answer_y = new int[M]; // готовим вектор координат пути по у
	int size = 0;
	while (y != 0) { // пока не первый столбец(востанавливаем путь справа налево)
		answer_x[size] = x; //узнаём, из какой клетки мы пришли в текущую и запоминаем
		answer_y[size] = y;
		++size;
		int new_x = prev_x[x][y]; // переходим в предыдущую
		int new_y = prev_y[x][y];
		x = new_x;
		y = new_y;
	}
	for (int i = 0; i < N; ++i) { //вывод элементов на экран
		for (int j = 0; j < M; ++j) {
			bool flag = false;
			for (int k = 0; k < size; ++k) {
				if (answer_x[size - 1 - k] == i) {
					if (answer_y[size - 1 - k] == j) {
						flag = true;
					}
				}
			}
			if (flag || x == i && y == j) cout << "\033[31m" << arr[i][j] << "\033[0m" << "   "; //выведение элемента красным цветом, если данный элемент является минимальным
			else cout << arr[i][j] << "   "; //в остальных случаях, выведение элемента обычным цветом
		}
		cout << endl;
	}
	for (int i = 0; i < N; ++i) { //очистка памяти
		delete[] table[i];
		delete[] prev_x[i];
		delete[] prev_y[i];
	}
	delete[] table;
	delete[] prev_x;
	delete[] prev_y;
	return result;
}

int main() {
	int N1;
	int N2;
	ifstream f("input.txt");
	if (!f)
	{
		cout << "не возможно открыть файл" << endl;
		return 1;
	}
	//cout << "Enter the size of the array: " << endl;
	f >> N1 >> N2;
	int** arr = new int* [N1]; //одномерный динамический массив, хранящий данные из файла
	for (int i = 0; i < N1; ++i)
	{
		for (int j = 0; j < N2; ++j)
		{
			arr[i] = new int[N2];
		}
	}
	for (int i = 0; i < N1; ++i) // заполнение массива данными из файла
	{
		for (int j = 0; j < N2; ++j)
		{
			f >> arr[i][j];
		}
	}
	cout << "The min path: " << MinPath(arr, N1, N2) << endl; //выводим минимальный путь от левого края до правого
	for (int i = 0; i < N1; ++i) // очищение памяти
	{
		delete[] arr[i];
	}
	delete[] arr;
	f.close();
}




/*#include<iostream> 
#include <fstream>
using namespace std;

int MinPath(int** arr, int N, int M) { // заводим таблицу нужного размера и значения первого столбца первого массива проставляем в первый столбец второго
	int** table = new int* [N]; //массив с данными
	int** prev_x = new int* [N]; //массив индексов строк
	int** prev_y = new int* [N]; //массив индексов столбцов
	for (int i = 0; i < N; ++i) {
		table[i] = new int[M];
		prev_x[i] = new int[M];
		prev_y[i] = new int[M];
	}
	for (int i = 0; i < N; ++i) { //заполняем первый столбец созданного массива элементами первого столбца массива arr
		table[i][0] = arr[i][0];
	}
	if (N > 1) {
		table[0][1] = table[1][0] + arr[0][1]; 
		prev_x[0][1] = 1;
		prev_y[0][1] = 0;
		table[N - 1][1] = table[N - 2][0] + arr[N - 1][1]; 
		prev_x[N - 1][1] = N - 2;
		prev_y[N - 1][1] = 0;
	}
	for (int i = 1; i + 1 < N; ++i) { 
		table[i][1] = arr[i][1];
		if (table[i - 1][0] < table[i + 1][0]) {
			table[i][1] += table[i - 1][0];
			prev_x[i][1] = i - 1;
			prev_y[i][1] = 0;
		}
		else {
			table[i][1] += table[i + 1][0];
			prev_x[i][1] = i + 1;
			prev_y[i][1] = 0;
		}
	}
	for (int j = 2; j < M; ++j) {
		if (N > 1) {
			table[0][j] = arr[0][j]; 
			if (table[0][j - 2] < table[1][j - 1]) {
				table[0][j] += table[0][j - 2];
				prev_x[0][j] = 0;
				prev_y[0][j] = j - 2;
			}
			else {
				table[0][j] += table[1][j - 1];
				prev_x[0][j] = 1;
				prev_y[0][j] = j - 1;
			}
			table[N - 1][j] = arr[N - 1][j]; 
			if (table[N - 2][j - 1] < table[N - 1][j - 2]) {
				table[N - 1][j] += table[N - 2][j - 1];
				prev_x[N - 1][j] = N - 2;
				prev_y[N - 1][j] = j - 1;
			}
			else {
				table[N - 1][j] += table[N - 1][j - 2];
				prev_x[N - 1][j] = N - 1;
				prev_y[N - 1][j] = j - 2;
			}
		}
		for (int i = 1; i + 1 < N; ++i) {  
			table[i][j] = arr[i][j];
			if (table[i - 1][j - 1] <= table[i][j - 2] && table[i - 1][j - 1] <= table[i + 1][j - 1]) {
				table[i][j] += table[i - 1][j - 1];
				prev_x[i][j] = i - 1;
				prev_y[i][j] = j - 1;
			}
			else if (table[i][j - 2] <= table[i - 1][j - 1] && table[i][j - 2] <= table[i + 1][j - 1]) {
				table[i][j] += table[i][j - 2];
				prev_x[i][j] = i;
				prev_y[i][j] = j - 2;
			}
			else {
				table[i][j] += table[i + 1][j - 1];
				prev_x[i][j] = i + 1;
				prev_y[i][j] = j - 1;
			}
		}
	}

	int result = table[0][M - 1]; 
	int x = 0;
	int y = M - 1;
	for (int i = 1; i < N; ++i) {
		if (result > table[i][M - 1]) {
			x = i;
			result = table[i][M - 1];
		}
	}
	int* answer_x = new int[M];
	int* answer_y = new int[M];
	int size = 0;
	while (y != 0) {
		answer_x[size] = x;
		answer_y[size] = y;
		++size;
		int new_x = prev_x[x][y];
		int new_y = prev_y[x][y];
		x = new_x;
		y = new_y;
	}
	for (int i = 0; i < size; ++i) {
		std::cout << answer_x[size - 1 - i] + 1 << " " << answer_y[size - 1 - i] + 1 << " (" << arr[answer_x[size - 1 - i]][answer_y[size - 1 - i]] << ")\n";
	}
	std::cout << x + 1 << " " << y + 1 << " (" << arr[x][y] << ")\n";
	for (int i = 0; i < N; ++i) {
		delete[] table[i];
		delete[] prev_x[i];
		delete[] prev_y[i];
	}
	delete[] table;
	delete[] prev_x;
	delete[] prev_y;
	return result;
}

int main() {
	int N1;
	int N2;
	ifstream f("input.txt");
	if (!f)
	{
		cout << "не возможно открыть файл" << endl;
		return 1;
	}
	//cout << "Enter the size of the array: " << endl;
	f >> N1 >> N2;
	int** arr = new int* [N1]; //одномнрный динамический массив, хранящий данные из файла
	for (int i = 0; i < N1; ++i)
	{
		for (int j = 0; j < N2; ++j)
		{
			arr[i] = new int[N2];
		}
	}
	for (int i = 0; i < N1; ++i) // заполнение массива данными из файла
	{
		for (int j = 0; j < N2; ++j)
		{
			f >> arr[i][j];
		}
	}
	cout << "The min path: " << MinPath(arr, N1, N2) << endl; //выводим минимальный путь от левого края до правого
	for (int i = 0; i < N1; ++i) // очищение памяти
	{
		delete[] arr[i];
	}
	delete[] arr;
	f.close();
}*/