#include <iostream>
using namespace std;

void S();
void A();
void B();
void C();

string sequence = "";
string symbol;

void readNext()
{
    if (sequence.length() > 0)
    {
        symbol = sequence.substr(0, 1);
        sequence = sequence.substr(1, sequence.length() - 1);
    }
    else
    {
        symbol = "";
    }
}

void S()
{
    cout << "S";

    if (symbol == "a")
    {
        readNext();
        A();
        B();
    }
    else if (symbol == "b")
    {
        readNext();
        B();
        A();
    }
    else
    {
        cout << endl
             << "NE" << endl;
        exit(0);
    }
}

void A()
{
    cout << "A";

    if (symbol == "a")
    {
        readNext();
    }
    else if (symbol == "b")
    {
        readNext();
        C();
    }
    else
    {
        cout << endl
             << "NE" << endl;
        exit(0);
    }
}

void B()
{
    cout << "B";

    if (symbol == "c")
    {
        readNext();

        if (symbol != "c")
        {
            cout << endl
                 << "NE" << endl;
            exit(0);
        }
        readNext();

        S();

        if (symbol != "b")
        {
            cout << endl
                 << "NE" << endl;
            exit(0);
        }
        readNext();

        if (symbol != "c")
        {
            cout << endl
                 << "NE" << endl;
            exit(0);
        }
        readNext();
    }
}

void C()
{
    cout << "C";

    A();
    A();
}

int main(void)
{
    getline(cin, sequence);

    readNext();

    S();

    if (symbol == "")
    {
        cout << endl
             << "DA" << endl;
    }
    else
    {
        cout << endl
             << "NE" << endl;
    }

    return 0;
}