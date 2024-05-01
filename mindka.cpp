#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <string>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <iterator>
using namespace std;

void splitString(vector<string> &result, string &source, char &delim)
{
    stringstream ss(source);
    string intermediate;
    while (getline(ss, intermediate, delim))
    {
        result.push_back(intermediate);
    }
}

void availableStates(vector<string> &alphabet, string &state,
                     set<string> &result,
                     vector<vector<set<string>>> &table,
                     unordered_map<string, int> &stateToNumber,
                     unordered_map<string, int> &symbolToNumber)
{
    set<string> nextState;
    string nextStateString;
    result.insert(state);

    for (auto i = alphabet.begin(); i != alphabet.end(); i++)
    {
        nextState = table[stateToNumber[state]][symbolToNumber[*i]];
        for (auto j = nextState.begin(); j != nextState.end(); j++)
        {
            nextStateString = *j;
            if (result.find(nextStateString) == result.end())
                availableStates(alphabet, nextStateString, result, table, stateToNumber, symbolToNumber);
        }
    }
}

void removeState(string &stateToRemove, vector<string> &states,
                 vector<string> &okStates,
                 vector<vector<set<string>>> &table,
                 unordered_map<string, int> &stateToNumber)
{
    vector<string>::iterator itr = find(okStates.begin(), okStates.end(), stateToRemove);
    if (itr != okStates.cend())
        okStates.erase(okStates.begin() + distance(okStates.begin(), itr));

    states.erase(states.begin() + stateToNumber[stateToRemove]);
    table.erase(table.begin() + stateToNumber[stateToRemove]);

    stateToNumber.clear();
    for (long unsigned int i = 0; i < states.size(); i++)
    {
        stateToNumber[states[i]] = i;
    }
}

int main(void)
{
    string line;
    vector<string> firstFour;
    vector<string> otherLines;

    for (int i = 1; i <= 4; i++)
    {
        getline(cin, line);
        firstFour.push_back(line);
    }

    do
    {
        cin >> line;
        if (line.empty())
            break;
        otherLines.push_back(line);
    } while (getline(cin, line));

    vector<string> states;
    vector<string> alphabet;
    vector<string> okStates;
    string firstState = firstFour[3];

    char delim = ',';
    splitString(states, firstFour[0], delim);
    splitString(alphabet, firstFour[1], delim);
    splitString(okStates, firstFour[2], delim);

    unordered_map<string, int> stateToNumber;
    unordered_map<string, int> symbolToNumber;

    for (long unsigned int i = 0; i < states.size(); i++)
    {
        stateToNumber[states[i]] = i;
    }
    for (long unsigned int i = 0; i < alphabet.size(); i++)
    {
        symbolToNumber[alphabet[i]] = i;
    }

    vector<vector<set<string>>> table;
    set<string> setTemp;
    vector<set<string>> vectorTemp;
    for (long unsigned int i = 0; i < alphabet.size() + 1; i++)
    {
        vectorTemp.push_back(setTemp);
    }
    for (long unsigned int i = 0; i < states.size(); i++)
    {
        table.push_back(vectorTemp);
    }

    vector<string> help;
    vector<string> stateSign;
    for (long unsigned int i = 0; i < otherLines.size(); i++)
    {
        delim = '>';
        help.clear();
        stateSign.clear();

        splitString(help, otherLines[i], delim);
        help[0] = help[0].substr(0, help[0].length() - 1);

        delim = ',';
        splitString(stateSign, help[0], delim);

        table[stateToNumber[stateSign[0]]]
             [symbolToNumber[stateSign[1]]]
                 .insert(help[1]);
    }

    set<string> result;
    set<string> allStatesSet;
    set<string> unavailableStates;
    availableStates(alphabet, firstState, result,
                    table, stateToNumber, symbolToNumber);

    for (auto i = states.begin(); i != states.end(); i++)
        allStatesSet.insert(*i);

    set_difference(allStatesSet.begin(), allStatesSet.end(), result.begin(), result.end(),
                   inserter(unavailableStates, unavailableStates.end()));

    string currentState;
    for (auto i = unavailableStates.begin(); i != unavailableStates.end(); i++)
    {
        currentState = *i;
        removeState(currentState, states, okStates, table, stateToNumber);
    }

    int size = states.size();
    int array[size][size];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            array[i][j] = 0;
        }
    }

    int sum = 0;
    int sum2 = 0;
    vector<string>::iterator itr;
    vector<string>::iterator itr2;

    unordered_map<int, string> numberToState;
    for (long unsigned int i = 0; i < states.size(); i++)
    {
        numberToState[i] = states[i];
    }

    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            itr = find(okStates.begin(), okStates.end(), numberToState[i]);
            itr2 = find(okStates.begin(), okStates.end(), numberToState[j]);
            if ((itr != okStates.cend() && itr2 == okStates.cend()) ||
                (itr2 != okStates.cend() && itr == okStates.cend()))
            {
                array[i][j] = 1;
            }
        }
    }

    string state1, state2;

    do
    {
        sum = 0;
        sum2 = 0;
        for (int i = 0; i < size - 1; i++)
        {
            for (int j = i + 1; j < size; j++)
            {
                sum += array[i][j];
            }
        }

        for (int i = 0; i < size - 1; i++)
        {
            for (int j = i + 1; j < size; j++)
            {
                if (array[i][j] == 0)
                {
                    for (auto k = alphabet.begin(); k != alphabet.end(); k++)
                    {
                        state1 = *table[i][symbolToNumber[*k]].begin();
                        state2 = *table[j][symbolToNumber[*k]].begin();

                        if (array[stateToNumber[state2]][stateToNumber[state1]] ||
                            array[stateToNumber[state1]][stateToNumber[state2]])
                        {
                            array[i][j] = 1;
                            break;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < size - 1; i++)
        {
            for (int j = i + 1; j < size; j++)
            {
                sum2 += array[i][j];
            }
        }
    } while (sum != sum2);

    vector<set<string>> equ;
    set<string> setEqu;
    bool done = false;
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (array[i][j] == 0)
            {
                done = false;
                for (auto k = equ.begin(); k != equ.end(); k++)
                {
                    if (((*k).find(numberToState[i]) != (*k).end()) ||
                        ((*k).find(numberToState[j]) != (*k).end()))
                    {
                        (*k).insert(numberToState[i]);
                        (*k).insert(numberToState[j]);
                        done = true;
                    }
                }
                if (done == false)
                {
                    equ.push_back(setEqu);
                    equ.back().insert(numberToState[i]);
                    equ.back().insert(numberToState[j]);
                }
            }
        }
    }

    string toRemove;
    for (auto i = equ.begin(); i != equ.end(); i++)
    {
        for (auto j = (*i).begin(); j != (*i).end(); j++)
        {
            if (j != (*i).begin())
            {
                toRemove = *j;
                if (toRemove == firstState)
                    firstState = *(*i).begin();
                removeState(toRemove, states, okStates, table, stateToNumber);

                for (auto k = table.begin(); k != table.end(); k++)
                {
                    for (auto l = (*k).begin(); l != (*k).end(); l++)
                    {
                        if (*(*l).begin() == toRemove)
                        {
                            (*l).clear();
                            (*l).insert(*(*i).begin());
                        }
                    }
                }
            }
        }
    }

    for (auto i = states.begin(); i != states.end(); i++)
    {
        if (i + 1 == states.end())
        {
            cout << *i;
        }
        else
        {
            cout << *i << ",";
        }
    }
    cout << "\n";

    for (auto i = alphabet.begin(); i != alphabet.end(); i++)
    {
        if (i + 1 == alphabet.end())
        {
            cout << *i;
        }
        else
        {
            cout << *i << ",";
        }
    }
    cout << "\n";

    for (auto i = okStates.begin(); i != okStates.end(); i++)
    {
        if (i + 1 == okStates.end())
        {
            cout << *i;
        }
        else
        {
            cout << *i << ",";
        }
    }
    cout << "\n";

    cout << firstState << "\n";

    for (auto i = states.begin(); i != states.end(); i++)
    {
        for (auto j = alphabet.begin(); j != alphabet.end(); j++)
        {
            cout << *i << "," << *j << "->"
                 << *(table[stateToNumber[*i]][symbolToNumber[*j]].begin())
                 << "\n";
        }
    }

    return 0;
}