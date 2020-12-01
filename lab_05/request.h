#ifndef REQUEST_H
#define REQUEST_H

class Request
{
public:
    Request(int _l);
    ~Request();

    double timeInSecondQ();
    double timeInThirdQ();
    double wholeTime();

    int number;
    double s1, s2, s3;
    double e1, e2, e3;

    char *string;
    int l;
};

#endif // !REQUEST_H