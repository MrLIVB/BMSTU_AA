#include "counter.h"

// void StartCounter()
// {
//     LARGE_INTEGER li;
//     if (!QueryPerformanceFrequency(&li))
//         std::cout << "QueryPerformanceFrequency failed!\n";

//     PCFreq = double(li.QuadPart);

//     QueryPerformanceCounter(&li);
//     CounterStart = li.QuadPart;
// }

// double GetCounter()
// {
//     LARGE_INTEGER li;
//     QueryPerformanceCounter(&li);
//     return double(li.QuadPart - CounterStart) / PCFreq;
// }

double GetTimestamp()
{
    LARGE_INTEGER li;
    !QueryPerformanceFrequency(&li);
    double PCFreq = double(li.QuadPart);

    QueryPerformanceCounter(&li);
    return double(li.QuadPart) / PCFreq * 1000;
}