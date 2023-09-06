#include "LBaseGraphAlgorithm.h"

std::string Create32Code(int Lenth)
{
    using namespace std;
    vector<int>  State(32, 0);
    vector<char> Code(32, '0');
    std::string  Code32 = "";
    int          sum    = 0;
    for (int j = 0; j != 32; ++j)
    {
        State[j] = rand() % 2;
        sum += State[j];
    }
    if (sum == 32 || sum == 0)
    {
        int r    = rand() % 32 + 1;
        State[r] = 1 - State[r];
    }
    for (int i = 0; i != 32; ++i)
    {
        if (State[i] == 0)
            Code[i] = rand() % 10 + '0';
        else
            Code[i] = rand() % 26 + 'a';
    }
    for (vector<char>::iterator iter = Code.begin(); iter != Code.end(); ++iter)
    {
        Code32 += *iter;
    }
    return Code32;
}