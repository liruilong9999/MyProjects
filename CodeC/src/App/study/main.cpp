#include <iostream>
#include <string>
using namespace std;

struct T1
{
    int x;
    int y;
} a = {123, 321};

struct T2
{
    int x;
    int y;
    T2(int, int)
        : x(10)
        , y(20)
    {
		
	}
} b = {123, 321};

int main(void)
{
    cout << "a.x: " << a.x << ", a.y: " << a.y << endl;
    cout << "b.x: " << b.x << ", b.y: " << b.y << endl;

	T2 c(11, 21);
	cout << "c.x: " << c.x << ", c.y: " << c.y << endl;
    return 0;
}