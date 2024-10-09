#include <iostream>
#include <algorithm>
#include <random>

using std::cout;
using std::endl;

int main()
{
    cout << "{ ";

    int* table = new int[256];

    for(int i = 0; i < 256; i++)
        table[i] = i;

    std::shuffle(table, table + 256, std::default_random_engine(time(NULL)));

    for (int i = 0; i < 256; i++)
    {
        if(i % 16 == 0) cout << endl;
        cout << table[i] << ", ";
    }

    cout << "}" << endl;

    return 0;

}