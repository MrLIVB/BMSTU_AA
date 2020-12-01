#include "request.h"

Request::Request(int _l)
{
    l = _l;
    string = new char[l];
}

Request::~Request()
{
    delete[] string;
}

double Request::timeInSecondQ()
{
    return s2 - e1;
}

double Request::timeInThirdQ()
{
    return s3 - e2;
}

double Request::wholeTime()
{
    return e3 - s1;
}