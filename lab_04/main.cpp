#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <windows.h>
#include <thread>

using namespace std;

#define MAX_THREADS 32

typedef struct
{
    int **matrix;
    int rows;
    int cols;
} matrix_t;

typedef void (*multiplicationLinearAlgorithm)(int **m1, int m, int n, int **m2, int q, int **r);
typedef void (*multiplicationParAlgorithm)(int **m1, int m, int n, int **m2, int q, int **r, int nThreads);

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

void multiplyMatricesWinograd(int **matrix1, int m, int n, int **matrix2, int q, int **result)
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
            buf -= matrix2[k][i] * matrix2[k + 1][i];
        mulV[i] = buf;
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < q; j++)
        {
            buf = mulH[i] + mulV[j];
            for (int k = 0; k < n; k += 2)
                buf += (matrix1[i][k] + matrix2[k + 1][j]) * (matrix1[i][k + 1] + matrix2[k][j]);
            result[i][j] = buf;
        }
    }

    if (isOdd)
    {
        for (int i = 0; i < m; i++)
            for (int j = 0; j < q; j++)
                result[i][j] += matrix1[i][t] * matrix2[t][j];
    }

    delete[] mulH;
    delete[] mulV;
}

void parallelMainCycle(int **matrix1, int **matrix2, int *mulH, int *mulV,  int rs, int re, int columns, int rows, int **result)
{
    int buf;
    for (int i = rs; i < re; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            buf = mulH[i] + mulV[j];
            for (int k = 0; k < columns; k += 2)
                buf += (matrix1[i][k] + matrix2[k + 1][j]) * (matrix1[i][k + 1] + matrix2[k][j]);
            result[i][j] = buf;
        }
    }
}

void multiplyMatricesWinogradP1(int **matrix1, int m, int n, int **matrix2, int q, int **result, int nThreads)
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
            buf -= matrix2[k][i] * matrix2[k + 1][i];
        mulV[i] = buf;
    }

    thread* threads = new thread[nThreads];
    int rows = m / nThreads;
    int rs = 0;
    for (int t = 0; t < nThreads; t++)
    {
        int re = t == nThreads - 1 ? m : (rs + rows);
        threads[t] = thread(parallelMainCycle, matrix1, matrix2, mulH, mulV, rs, re, n, q, result);
        rs = re;
    }

    for (int i = 0; i < nThreads; i++)
        threads[i].join();

    if (isOdd)
    {
        for (int i = 0; i < m; i++)
            for (int j = 0; j < q; j++)
                result[i][j] += matrix1[i][t] * matrix2[t][j];
    }

    delete[] threads;
    delete[] mulH;
    delete[] mulV;
}

void parallelMul(int **matrix1, int vs, int ve, int n, int **matrix2, int hs, int he, int *mulV, int *mulH)
{
    int buf;
    for (int i = vs; i < ve; i++)
    {
        buf = 0;
        for (int k = 0; k < n; k += 2)
            buf -= matrix1[i][k] * matrix1[i][k + 1];
        mulH[i] = buf;
    }

    for (int i = hs; i < he; i++)
    {
        buf = 0;
        for (int k = 0; k < n; k += 2)
            buf -= matrix2[k][i] * matrix2[k + 1][i];
        mulV[i] = buf;
    }
}

void multiplyMatricesWinogradP2(int **matrix1, int m, int n, int **matrix2, int q, int **result, int nThreads)
{
    int buf;
    const int isOdd = n % 2;
    const int t = n - 1;
    if (isOdd)
        n -= 1;

    int *mulH = new int[m];
    int *mulV = new int[q];

    thread *threads = new thread[nThreads];
    int vs = 0;
    int vt = m / nThreads;
    int hs = 0;
    int ht = q / nThreads;
    for (int t = 0; t < nThreads; t++)
    {
        int ve = t == nThreads - 1 ? m : (vs + vt);
        int he = t == nThreads - 1 ? q : (hs + ht);
        threads[t] = thread(parallelMul, matrix1, vs, ve, n, matrix2, hs, he, mulV, mulH);

        vs = ve;
        hs = he;
    }

    for (int i = 0; i < nThreads; i++)
        threads[i].join();

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < q; j++)
        {
            buf = mulH[i] + mulV[j];
            for (int k = 0; k < n; k += 2)
                buf += (matrix1[i][k] + matrix2[k + 1][j]) * (matrix1[i][k + 1] + matrix2[k][j]);
            result[i][j] = buf;
        }
    }

    if (isOdd)
    {
        for (int i = 0; i < m; i++)
            for (int j = 0; j < q; j++)
                result[i][j] += matrix1[i][t] * matrix2[t][j];
    }

    delete[] mulH;
    delete[] mulV;
}

void initMatrix(matrix_t *matr, int rows, int cols)
{
    matr->rows = rows;
    matr->cols = cols;
    matr->matrix = new int *[matr->rows];
    for (int i = 0; i < matr->rows; i++)
        matr->matrix[i] = new int[matr->cols]{0};
}

void fillMatrix(matrix_t matr, int value)
{
    for (int i = 0; i < matr.rows; i++)
        for (int j = 0; j < matr.cols; j++)
            matr.matrix[i][j] = value;
}

void fillMatrixRandom(matrix_t matr)
{
    for (int i = 0; i < matr.rows; i++)
        for (int j = 0; j < matr.cols; j++)
            matr.matrix[i][j] = rand();
}

void deleteMatrix(matrix_t matr)
{
    for (int i = 0; i < matr.rows; i++)
        delete[] matr.matrix[i];
    delete[] matr.matrix;
}

void measureLinear(int repeats, multiplicationLinearAlgorithm algortihm, char *name)
{
    srand(time(0));
    ofstream file;
    char fileName[30];
    matrix_t matr1, matr2, res;

    sprintf(fileName, "%s", name);
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
        file << i << " " << GetCounter() / repeats << "\n";

        deleteMatrix(matr1);
        deleteMatrix(matr2);
        deleteMatrix(res);
    }
    file.close();
}

void measureParallel(int repeats, multiplicationParAlgorithm algortihm, char *name)
{
    srand(time(0));
    ofstream file;
    char fileName[30];
    matrix_t matr1, matr2, res;

    for (int t = 1; t <= 32; t *= 2)
    {
        sprintf(fileName, "%s%d", name, t);
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
                algortihm(matr1.matrix, i, i, matr2.matrix, i, res.matrix, t);
            file << i << " " << GetCounter() / repeats << "\n";

            deleteMatrix(matr1);
            deleteMatrix(matr2);
            deleteMatrix(res);
        }
        file.close();
    }
}

void inputMatrix(matrix_t matr)
{
    for (int i = 0; i < matr.rows; i++)
        for (int j = 0; j < matr.cols; j++)
            cin >> matr.matrix[i][j];
}

void outputMatrix(matrix_t matr)
{
    for (int i = 0; i < matr.rows; i++)
    {
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

                multiplyMatricesWinograd(matrix1.matrix, matrix1.rows, matrix1.cols, matrix2.matrix, matrix2.cols, result1.matrix);

                multiplyMatricesWinogradP1(matrix1.matrix, matrix1.rows, matrix1.cols, matrix2.matrix, matrix2.cols, result2.matrix, 2);
                if (!compareMatrices(result1, result2))
                {
                    cout << "Incorrect multiplication using Winograd P1 with rows=" << rows << "; m=" << m << "; cols=" << cols << "\n";
                    passed = false;
                }

                multiplyMatricesWinogradP2(matrix1.matrix, matrix1.rows, matrix1.cols, matrix2.matrix, matrix2.cols, result2.matrix, 2);
                if (!compareMatrices(result1, result2))
                {
                    cout << "Incorrect multiplication using Winograd P2 with rows=" << rows << "; m=" << m << "; cols=" << cols << "\n";
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

int main()
{
    while (1)
    {
        int choice = 3;
        cout << "Menu:\n";
        cout << "1. Multiply matrices\n";
        cout << "2. Measure time\n";
        cout << "3. Check work\n";
        cout << "4. Exit\n";
        cout << "Input your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            matrix_t matr1, matr2, res;
            int rows1, rows2;
            int cols1, cols2;
            cout << "Number of rows for first matrix: ";
            cin >> rows1;
            cout << "Number of columns for first matrix: ";
            cin >> cols1;
            cout << "Number of columns for second matrix: ";
            cin >> cols2;
            initMatrix(&matr1, rows1, cols1);
            initMatrix(&matr2, cols1, cols2);
            initMatrix(&res, rows1, cols2);

            cout << "Matrix 1:\n";
            inputMatrix(matr1);

            cout << "Matrix 2:\n";
            inputMatrix(matr2);

            multiplyMatricesWinograd(matr1.matrix, matr1.rows, matr1.cols, matr2.matrix, matr2.cols, res.matrix);
            cout << "Winograd:\n";
            outputMatrix(res);

            multiplyMatricesWinogradP1(matr1.matrix, matr1.rows, matr1.cols, matr2.matrix, matr2.cols, res.matrix, 2);
            cout << "Winograd P1:\n";
            outputMatrix(res);

            multiplyMatricesWinogradP2(matr1.matrix, matr1.rows, matr1.cols, matr2.matrix, matr2.cols, res.matrix, 2);
            cout << "Winograd P2:\n";
            outputMatrix(res);

            deleteMatrix(matr1);
            deleteMatrix(matr2);
            deleteMatrix(res);
        }
        else if (choice == 2)
        {
            int repeats;
            cout << "Input number of repeats: ";
            cin >> repeats;

            char name[20] = "tempRes//linear";

            measureLinear(repeats, multiplyMatricesWinograd, name);
            cout << "Measured linear\n";

            strcpy(name, "tempRes//parallel1_");
            measureParallel(repeats, multiplyMatricesWinogradP1, name);
            cout << "Measured P1\n";

            strcpy(name, "tempRes//parallel2_");
            measureParallel(repeats, multiplyMatricesWinogradP2, name);
            cout << "Measured P2\n";
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