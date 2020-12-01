#include "conveyor.h"

int stringGenerator(char *string, int l)
{
    srand(time(0));
    for (int i = 0; i < l; i++)
        string[i] = rand() % 26 + 97;
    return 0;
}

int xorCrpyt(char *string, int l, char key)
{
    for (int i = 0; i < l; i++)
        string[i] ^= key;
    return 0;
}

int caesarCrypt(char *string, int l, int shift)
{
    for (int i = 0; i < l; i++)
        string[i] = (string[i] - 97 + shift) % 26 + 97;
    return 0;
}

Conveyor::Conveyor(int _taskNumber, int _taskLength)
{
    taskNumber = _taskNumber;
    taskLength = _taskLength;
    ft1 = 0;
    ft2 = 0;
    ft3 = 0;
}

list<Request*> Conveyor::getResult()
{
    return result;
}

void Conveyor::part1()
{
    while (this->ft1 < this->taskNumber)
    {
        Request *req;
        if (this->startingQ.size())
        {
            req = this->startingQ.front();
            this->startingQ.pop();
        }
        else
            continue;

        req->s1 = GetTimestamp();

        caesarCrypt(req->string, req->l, 12);

        req->e1 = GetTimestamp();

        this->ft1++;

        this->m1.lock();
        this->q2.push(req);
        this->m1.unlock();
    }
}

void Conveyor::part2()
{
    while (this->q2.size() == 0)
        continue;

    while (this->ft2 < this->taskNumber)
    {
        while (this->q2.size() == 0)
            continue;
        
        Request *req;

        this->m1.lock();
        req = this->q2.front();
        this->q2.pop();
        this->m1.unlock();

        req->s2 = GetTimestamp();
        char key = 'e';

        xorCrpyt(req->string, req->l, key);
        req->e2 = GetTimestamp();

        this->ft2++;

        this->m2.lock();
        this->q3.push(req);
        this->m2.unlock();
    }
}

void Conveyor::part3()
{
    while (this->q3.size() == 0)
        continue;

    while (this->ft3 < this->taskNumber)
    {
        while (this->q3.size() == 0)
            continue;

        Request *req;
        this->m2.lock();
        req = this->q3.front();
        this->q3.pop();
        this->m2.unlock();

        req->s3 = GetTimestamp();

        char key = 'k';
        xorCrpyt(req->string, req->l, key);
        req->e3 = GetTimestamp();

        this->ft3++;
        this->result.push_back(req);
    }
}

void Conveyor::run()
{
    generateRequests();

    thread t1 = thread(&Conveyor::part1, this);
    thread t2 = thread(&Conveyor::part2, this);
    thread t3 = thread(&Conveyor::part3, this);

    t1.join();
    t2.join();
    t3.join();
}

void Conveyor::generateRequests()
{
    for (int i = 0; i < taskNumber; i++)
    {
        Request *req = new Request(taskLength);
        req->number = i;
        stringGenerator(req->string, req->l);
        startingQ.push(req);
    }
    cout << startingQ.size();
}