#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <windows.h>

using namespace std;

typedef struct
{
    int *array;
    int length;
} array_t;

typedef void (*sortingAlgorithm)(array_t array);

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart)/1000.;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}

void bubbleSort(array_t array)
{
    bool changed = false;
    for (int i = 1; i < array.length; i++)
    {
        for (int j = 0; j < array.length - i; j++)
            if (array.array[j + 1] < array.array[j])
            {
                const int temp = array.array[j];
                array.array[j] = array.array[j + 1];
                array.array[j + 1] = temp;
                changed = true;
            }
        if (!changed)
            break;
    }
}

void selectionSort(array_t array)
{
    for(int i=0; i<array.length-1; i++)
    {
        int min_i = i;
        for(int j=i+1;j<array.length;j++)
        {
            if (array.array[j] < array.array[min_i])
                min_i = j;
        }
        if (min_i != i)
        {
            int tmp = array.array[i];
            array.array[i] = array.array[min_i];
            array.array[min_i] = tmp;
        }        
    }
}

void siftDown(array_t array, int i)
{
    int nMax = i;
    int value = array.array[i];
    while (true)
    {
        i = nMax;
        int childN = i*2 + 1;
        if ((childN < array.length) && (array.array[childN] > value))
            nMax = childN;
        childN++;
        if ((childN < array.length) && (array.array[childN] > array.array[nMax]))
            nMax = childN;
        if (nMax == i)
            break;
        array.array[i] = array.array[nMax];
        array.array[nMax] = value;
    }
}

void heapSort(array_t array)
{
    for (int i = array.length / 2 - 1; i >= 0; --i)
        siftDown(array, i);
    while (array.length > 1)
    {
        array.length--;

        int firstElement = array.array[0];
        array.array[0] = array.array[array.length];
        array.array[array.length] = firstElement;

        siftDown(array, 0);
    }
}

void initArray(array_t *array, int length)
{
    array->length = length;
    array->array = new int[length]{0};
}

void fillMatrix(array_t array, int value)
{
    for (int i = 0; i < array.length; i++)
        array.array[i] = value;
}

void fillArrayRandom(array_t array)
{
    for (int i = 0; i < array.length; i++)
        array.array[i] = rand();
}

void fillArraySorted(array_t array, bool ascending = true)
{
    if (ascending)
        for (int i = 0; i < array.length; i++)
            array.array[i] = i;
    else
        for (int i = 0; i < array.length; i++)
            array.array[i] = array.length - i;
}

void copyArray(array_t to, array_t from)
{
    for (int i = 0; i < from.length; i++)
        to.array[i] = from.array[i];
}

void deleteArray(array_t array)
{
    delete[] array.array;
}

void measureSortingTime(int repeats, int max, sortingAlgorithm algortihm, char *name)
{
    srand(time(0));
    ofstream file;
    char fileName[50];
    array_t array;

    strcpy(fileName, name);
    strcat(fileName, "Random.txt");
    file.open(fileName);
    for (int i = max / 10; i <= max; i += max / 10)
    {
        initArray(&array, i);
        fillArrayRandom(array);

        StartCounter();
        for (int j = 0; j < repeats; j++)
            algortihm(array);
        file << i << " " << GetCounter() / repeats << "\n";

        deleteArray(array);
    }
    file.close();

    strcpy(fileName, name);
    strcat(fileName, "SortedA.txt");
    file.open(fileName);
    for (int i = max / 10; i <= max; i += max / 10)
    {
        initArray(&array, i);
        fillArraySorted(array);

        StartCounter();
        for (int j = 0; j < repeats; j++)
            algortihm(array);
        file << i << " " << GetCounter() / repeats << "\n";

        deleteArray(array);
    }
    file.close();

    strcpy(fileName, name);
    strcat(fileName, "SortedD.txt");
    file.open(fileName);
    for (int i = max / 10; i <= max; i += max / 10)
    {
        initArray(&array, i);
        fillArraySorted(array, false);

        StartCounter();
        for (int j = 0; j < repeats; j++)
            algortihm(array);
        file << i << " " << GetCounter() / repeats << "\n";

        deleteArray(array);
    }
    file.close();
}

void inputArray(array_t array)
{
    for (int i = 0; i < array.length; i++)
            cin >> array.array[i];
}

void outputArray(array_t array)
{
    for (int i = 0; i < array.length; i++)
        printf("%d ", array.array[i]);
    printf("\n");
}

bool compareArrays(array_t array1, array_t array2) // ПЛОХОЕ НАЗВАНИЕ
{
    bool similar = true;
    for (int i = 0; i < array1.length; i++)
        if (array1.array[i] != array2.array[i])
        {
            similar = false;
            break;
        }
    return similar;
}

void randomTest()
{
    bool passed = true;
    array_t array1, array2, arrayConst;
    for (int length = 1; length < 10; length++)
    {
        initArray(&array1, length);
        initArray(&array2, length);
        initArray(&arrayConst, length);

        fillArrayRandom(arrayConst);
        copyArray(array1, arrayConst);
        copyArray(array2, arrayConst);

        
        bubbleSort(array1);
        selectionSort(array2);

        if (!compareArrays(array1, array2))
        {
            cout << "Incorrect sorting using selection sort with length=" << length << ";\n";
            passed = false;
        }

        copyArray(array2, arrayConst);

        heapSort(array2);

        if (!compareArrays(array1, array2))
        {
            cout << "Incorrect sorting using heap sort with length=" << length << ";\n";
            passed = false;
        }

        deleteArray(array1);
        deleteArray(array2);
        deleteArray(arrayConst);
    }
    if (passed)
        cout << "All tests are passed!\n\n";
}

void sortedTest()
{
    bool passed = true;
    array_t array1, array2, arrayConst;
    for (int length = 1; length < 10; length++)
    {
        initArray(&array1, length);
        initArray(&array2, length);
        initArray(&arrayConst, length);

        fillArraySorted(arrayConst);
        copyArray(array1, arrayConst);
        copyArray(array2, arrayConst);

        bubbleSort(array1);
        selectionSort(array2);

        if (!compareArrays(array1, array2))
        {
            cout << "Incorrect sorting using selection sort with length=" << length << ";\n";
            passed = false;
        }

        copyArray(array2, arrayConst);

        heapSort(array2);

        if (!compareArrays(array1, array2))
        {
            cout << "Incorrect sorting using heap sort with length=" << length << ";\n";
            passed = false;
        }

        deleteArray(array1);
        deleteArray(array2);
        deleteArray(arrayConst);
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
        cout << "1. Sort array\n";
        cout << "2. Measure time\n";
        cout << "3. Check work\n";
        cout << "4. Exit\n";
        cout << "Input your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            array_t array, res;
            int length;
            cout << "Length of array: ";
            cin >> length;
            if (length <= 0)
            {
                cout << "Length of array is not positive!\n\n";
                continue;
            }
            initArray(&array, length);
            initArray(&res, length);

            cout << "Input array:\n";
            inputArray(array);

            copyArray(res, array);

            bubbleSort(res);
            cout << "Bubble sort:\n";
            outputArray(res);

            copyArray(res, array);

            heapSort(res);
            cout << "Heap sort:\n";
            outputArray(res);

            copyArray(res, array);

            selectionSort(res);
            cout << "Selection sort:\n";
            outputArray(res);

            deleteArray(array);
            deleteArray(res);
        }
        else if (choice == 2)
        {
            int repeats;
            cout << "Input number of repeats: ";
            cin >> repeats;

            char name[30] = "tempRes\\bubbleSort";

            measureSortingTime(repeats, 50000, bubbleSort, name);
            cout << "Measured bubble sort\n";
            strcpy(name, "tempRes\\selectionSort");
            measureSortingTime(repeats, 50000, selectionSort, name);
            cout << "Measured selection sort\n";
            strcpy(name, "tempRes\\heapSort");
            measureSortingTime(repeats, 50000, heapSort, name);
            cout << "Measured heap sort\n\n";
        }
        else if (choice == 3)
        {
            randomTest();
            sortedTest();
        }
        else
        {
            break;
        }
    }
    return 0;
}