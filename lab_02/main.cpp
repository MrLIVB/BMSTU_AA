#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <windows.h>


using namespace std;

typedef struct{
    int **matrix;
    int rows;
    int cols;
} matrix_t;

typedef void(* multiplicationAlgorithm)(int** m1, int m, int n, int** m2, int q, int** r);

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart);

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}

void multiplyMatricesStandard(int** matrix1, int m, int n, int** matrix2, int q, int** result)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < q; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < n; k++)
            {
                result[i][j] = result[i][j] + matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

void multiplyMatricesWinograd(int **matrix1, int m, int n, int **matrix2, int q, int **result)
{
    int *mulH = new int[m];
    for (int i = 0; i < m; i++)
    {
        mulH[i] = 0;
        for (int k = 0; k < n / 2; k++){
            mulH[i] = mulH[i] + matrix1[i][2 * k] * matrix1[i][2 * k + 1];
        }
    }

    int *mulV = new int[q];
    for (int i = 0; i < q; i++)
    {
        mulV[i] = 0;
        for (int k = 0; k < n / 2; k++)
            mulV[i] = mulV[i] + matrix2[2*k][i] * matrix2[2*k+1][i];
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < q; j++)
        {
            result[i][j] = -mulH[i] - mulV[j];
            for (int k = 0; k < n/2; k++)
                result[i][j] = result[i][j] + (matrix1[i][2 * k] + matrix2[2 * k + 1][j])
                * (matrix1[i][2 * k + 1] + matrix2[2 * k][j]);
        }
    }

    if (n % 2 == 1)
    {
        for (int i = 0; i < m; i++)
            for (int j = 0; j < q; j++)
                result[i][j] = result[i][j] + matrix1[i][n - 1] * matrix2[n-1][j];
    }
    delete[] mulH;
    delete[] mulV;
}

// +=
// заранее выч. конст.
// k < n/2; k++ -> k < n; k += 2
// объединил 3 и 4
// mulh mulv уже негативные
// Ввёл buf
void multiplyMatricesWinogradOptimized(int **matrix1, int m, int n, int **matrix2, int q, int **result)
{
    int buf;
    const int isOdd = n % 2;
    const int t = n - 1;
    if (isOdd)
        n -= 1;
        
    int *mulH = new int[m];
    for (int i = 0; i < m; i++)
    {
        buf = 0;
        for (int k = 0; k < n; k += 2)
            buf -= matrix1[i][k] * matrix1[i][k + 1];
        mulH[i] = buf;
    }

    int *mulV = new int[q];
    for (int i = 0; i < q; i++)
    {
        buf = 0;
        for (int k = 0; k < n; k += 2)
            buf -= matrix2[k][i] * matrix2[k+1][i];
        mulV[i] = buf;
    }
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < q; j++)
        {
            buf = mulH[i] + mulV[j];
            for (int k = 0; k < n; k += 2)
                buf += (matrix1[i][k] + matrix2[k + 1][j]) * (matrix1[i][k + 1] + matrix2[k][j]);
            if (isOdd==1)
                buf += matrix1[i][t] * matrix2[t][j];
            result[i][j] = buf;
        }
    }
    delete[] mulH;
    delete[] mulV;  
}

void initMatrix(matrix_t *matr, int rows, int cols)
{
    matr->rows = rows;
    matr->cols = cols;
    matr->matrix = new int*[matr->rows];
    for (int i = 0; i < matr->rows; i++)
        matr->matrix[i] = new int[matr->cols]{0};
}

void fillMatrix(matrix_t matr, int value){
    for (int i = 0; i < matr.rows; i++)
        for (int j = 0; j < matr.cols; j++)
            matr.matrix[i][j] = value;
}

void fillMatrixRandom(matrix_t matr){
    for (int i = 0; i < matr.rows; i++)
        for (int j = 0; j < matr.cols; j++)
            matr.matrix[i][j] = rand();
}

void deleteMatrix(matrix_t matr){
    for (int i = 0; i < matr.rows; i++)
        delete []matr.matrix[i];
    delete []matr.matrix;
}

void measureMatricesMultiplication(int repeats, multiplicationAlgorithm algortihm, char *name)
{
    srand(time(0));
    ofstream file;
    char fileName[30];
    matrix_t matr1, matr2, res;

    strcpy(fileName, name);
    strcat(fileName, "Even.txt");
    file.open(fileName);
    for (int i = 100; i < 1001; i += 100)
    {
        initMatrix(&matr1, i, i);
        initMatrix(&matr2, i, i);
        initMatrix(&res, i, i);
        fillMatrixRandom(matr1);
        fillMatrixRandom(matr2);

        StartCounter();
        for (int j = 0; j < repeats; j++)
            algortihm(matr1.matrix, i, i, matr2.matrix, i, res.matrix);
        file << i << " " << GetCounter()/repeats << "\n";
        
        deleteMatrix(matr1);
        deleteMatrix(matr2);
        deleteMatrix(res);
    }
    file.close();

    strcpy(fileName, name);
    strcat(fileName, "Odd.txt");
    file.open(fileName);
    for (int i = 101; i <= 1001; i += 100)
    {
        initMatrix(&matr1, i, i);
        initMatrix(&matr2, i, i);
        initMatrix(&res, i, i);
        fillMatrixRandom(matr1);
        fillMatrixRandom(matr2);

        StartCounter();
        for (int j = 0; j < repeats; j++)
            algortihm(matr1.matrix, i, i, matr2.matrix, i, res.matrix);
        file << i << " " << GetCounter() / repeats << "\n";

        deleteMatrix(matr1);
        deleteMatrix(matr2);
        deleteMatrix(res);
    }
    file.close();
}

void inputMatrix(matrix_t matr)
{
    for (int i = 0; i < matr.rows; i++)
        for (int j = 0; j < matr.cols; j++)
            cin>>matr.matrix[i][j];
}

void outputMatrix(matrix_t matr){
    for (int i = 0; i < matr.rows; i++){
        for (int j = 0; j < matr.cols; j++)
            printf("%d ", matr.matrix[i][j]);
        printf("\n");
    }
}

bool compareMatrices(matrix_t matr1, matrix_t matr2) // ПЛОХОЕ НАЗВАНИЕ
{
    bool similar = true;
    for (int i = 0; i < matr1.rows; i++)
        for (int j = 0; j < matr1.cols; j++)
            if (matr1.matrix[i][j] != matr2.matrix[i][j])
            {
                similar = false;
                break;
            }
    return similar;
}

void randomTest()
{
    bool passed = true;
    matrix_t matrix1, matrix2, result1, result2;
    for (int rows = 1; rows < 10; rows++)
    {
        for (int m = 1; m < 10; m++)
        {
            initMatrix(&matrix1, rows, m);
            for (int cols = 1; cols < 10; cols++)
            {
                initMatrix(&matrix2, m, cols);
                initMatrix(&result1, rows, cols);
                initMatrix(&result2, rows, cols);

                fillMatrixRandom(matrix1);
                fillMatrixRandom(matrix2);

                multiplyMatricesStandard(matrix1.matrix, matrix1.rows, matrix1.cols, matrix2.matrix, matrix2.cols, result1.matrix);
                multiplyMatricesWinograd(matrix1.matrix, matrix1.rows, matrix1.cols, matrix2.matrix, matrix2.cols, result2.matrix);

                if (!compareMatrices(result1, result2))
                {
                    cout << "Incorrect multiplication using Winograd with rows=" << rows << "; m=" << m << "; cols=" << cols << "\n";
                    passed = false;
                }

                multiplyMatricesWinogradOptimized(matrix1.matrix, matrix1.rows, matrix1.cols, matrix2.matrix, matrix2.cols, result2.matrix);
                if (!compareMatrices(result1, result2))
                {
                    cout << "Incorrect multiplication using Winograd optimized with rows=" << rows << "; m=" << m << "; cols=" << cols << "\n";
                    passed = false;
                }

                deleteMatrix(matrix2);
                deleteMatrix(result1);
                deleteMatrix(result2);
            }
            deleteMatrix(matrix1);
        }
    }
    if (passed)
        cout << "All tests are passed!\n\n";
}

int main(){
    while (1)
    {
        int choice = 3;
        cout<<"Menu:\n";
        cout<<"1. Multiply matrices\n";
        cout<<"2. Measure time\n";
        cout<<"3. Check work\n";
        cout<<"4. Exit\n";
        cout<<"Input your choice: ";
        cin>>choice;

        if (choice == 1)
        {
            matrix_t matr1, matr2, res;
            int rows1, rows2;
            int cols1, cols2;
            cout << "Number of rows for first matrix: ";
            cin>>rows1;
            cout << "Number of columns for first matrix: ";
            cin>>cols1;
            cout << "Number of columns for second matrix: ";
            cin >> cols2;
            initMatrix(&matr1, rows1, cols1);
            initMatrix(&matr2, cols1, cols2);
            initMatrix(&res, rows1, cols2);

            cout << "Matrix 1:\n";
            inputMatrix(matr1);
            
            cout << "Matrix 2:\n";
            inputMatrix(matr2);

            multiplyMatricesStandard(matr1.matrix, matr1.rows, matr1.cols, matr2.matrix, matr2.cols, res.matrix);
            cout << "Standart:\n";
            outputMatrix(res);

            multiplyMatricesWinograd(matr1.matrix, matr1.rows, matr1.cols, matr2.matrix, matr2.cols, res.matrix);
            cout << "Winograd:\n";
            outputMatrix(res);

            multiplyMatricesWinogradOptimized(matr1.matrix, matr1.rows, matr1.cols, matr2.matrix, matr2.cols, res.matrix);
            cout << "Winograd optimized:\n";
            outputMatrix(res);

            deleteMatrix(matr1);
            deleteMatrix(matr2);
            deleteMatrix(res);
        }
        else if (choice == 2)
        {
            int repeats;
            cout << "Input number of repeats: ";
            cin >>repeats;

            char name[9] = "standard";

            measureMatricesMultiplication(repeats, multiplyMatricesStandard, name);
            cout << "Measured standard\n";
            strcpy(name, "winograd");
            measureMatricesMultiplication(repeats, multiplyMatricesWinograd, name);
            cout << "Measured Winograd\n";
            strcpy(name, "winoptim");
            measureMatricesMultiplication(repeats, multiplyMatricesWinogradOptimized, name);
            cout << "Measured Winograd optimized\n\n";
        }
        else if (choice == 3)
        {
            randomTest();
        }
        else
        {
            break;
        }
    }
    return 0;
}