#ifndef CONVEYOR_H
#define CONVEYOR_H

#include <queue>
#include <list>
#include <thread>
#include <mutex>
#include <bitset>

#include "counter.h"
#include "request.h"

using namespace std;

class Conveyor
{
public:
    Conveyor(int _taskNumber, int _taskLength);
    void run();
    list<Request*> getResult();
    void generateRequests();

protected:
    void part1();
    void part2();
    void part3();

private:
    queue<Request *> startingQ, q2, q3;
    list<Request *> result;
    mutex m1, m2;
    int ft1, ft2, ft3;
    int taskNumber, taskLength;
};

#endif // !CONVEYOR_H