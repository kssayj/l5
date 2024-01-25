#include<iostream> 
#include <fstream>
using namespace std;

int MinPath(int** arr, int N, int M) { 
	int** table = new int* [N];
	int** prev_x = new int* [N]; 
	int** prev_y = new int* [N]; 
	for (int i = 0; i < N; ++i) {
		table[i] = new int[M];
		prev_x[i] = new int[M];
		prev_y[i] = new int[M];
	}
	for (int i = 0; i < N; ++i) { 
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
	for (int i = 0; i < N; ++i) { 
		for (int j = 0; j < M; ++j) {
			bool flag = false;
			for (int k = 0; k < size; ++k) {
				if (answer_x[size - 1 - k] == i) {
					if (answer_y[size - 1 - k] == j) {
						flag = true;
					}
				}
			}
			if (flag || x == i && y == j) cout << "\033[31m" << arr[i][j] << "\033[0m" << "   "; 
			else cout << arr[i][j] << "   "; 
		}
		cout << endl;
	}
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
		cout << "the file cannot be opened" << endl;
		return 1;
	}
	//cout << "Enter the size of the array: " << endl;
	f >> N1 >> N2;
	int** arr = new int* [N1]; 
	for (int i = 0; i < N1; ++i)
	{
		for (int j = 0; j < N2; ++j)
		{
			arr[i] = new int[N2];
		}
	}
	for (int i = 0; i < N1; ++i) 
	{
		for (int j = 0; j < N2; ++j)
		{
			f >> arr[i][j];
		}
	}
	cout << "The min path: " << MinPath(arr, N1, N2) << endl; 
	for (int i = 0; i < N1; ++i) 
	{
		delete[] arr[i];
	}
	delete[] arr;
	f.close();
}
