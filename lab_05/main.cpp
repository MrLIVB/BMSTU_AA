#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "conveyor.h"

using namespace std;

int main()
{
    int nconv = 20;
    int nreq = 200;
    Conveyor*  conveyors[20];
    double times[200]{0};
    for (int i = 0; i < nconv; i++)
    {
        conveyors[i] = new Conveyor(nreq, 1000000);
        conveyors[i]->run();
    }

    // list<Request *> results1 = conv.getResult();
    list<Request*> results[20];
    for (int i = 0; i < nconv; i++)
        results[i] = conveyors[i]->getResult();

    
    int c = 0;
    ofstream file;
    file.open("src\\timein2Q");
    for (int i = 0; i < nconv; i++)
    {
        c = 0;
        for (auto res : results[i])
        {
            times[c] += res->timeInSecondQ();
            c++;
        }
    }
    for (int i = 0; i < nreq; i++)
    {
        file << i << " " << times[i] / nconv << "\n";
        times[i] = 0;
    }
    file.close();


    file.open("src\\timein3Q");
    for (int i = 0; i < nconv; i++)
    {
        c = 0;
        for (auto res : results[i])
        {
            times[c] += res->timeInThirdQ();
            c++;
        }
    }
    for (int i = 0; i < nreq; i++)
    {
        file << i << " " << times[i] / nconv << "\n";
        times[i] = 0;
    }
    file.close();

    file.open("src\\wholetime");
    for (int i = 0; i < nconv; i++)
    {
        c = 0;
        for (auto res : results[i])
        {
            times[c] += res->wholeTime();
            c++;
        }
    }
    for (int i = 0; i < nreq; i++)
        file << i << " " << times[i] / nconv << "\n";
    file.close();

    for (int i = 0; i < nconv; i++)
        delete conveyors[i];
    return 0;
}