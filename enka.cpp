#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <string>
#include <set>
#include <unordered_map>
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

void epsilonF(set<string> &set1, vector<vector<set<string>>> &table,
              unordered_map<string, int> &stateToNumber,
              unordered_map<string, int> &symbolToNumber)
{
    long unsigned int size = set1.size();

    for (auto i = set1.begin(); i != set1.end(); i++)
    {
        if (table[stateToNumber[*i]]
                 [symbolToNumber["$"]]
                     .size() > 0)
        {
            for (auto j : table[stateToNumber[*i]]
                               [symbolToNumber["$"]])
                set1.insert(j);
        }
    }

    if (set1.size() != size)
    {
        epsilonF(set1, table, stateToNumber, symbolToNumber);
    }
}

void delta(set<string> &set1, vector<vector<set<string>>> &table,
           unordered_map<string, int> &stateToNumber,
           unordered_map<string, int> &symbolToNumber,
           string symbol)
{
    set<string> result;
    for (auto i = set1.begin(); i != set1.end(); i++)
    {
        for (auto j : table[stateToNumber[*i]]
                           [symbolToNumber[symbol]])
        {
            result.insert(j);
        }
    }

    set1.clear();
    for (auto i : result)
    {
        set1.insert(i);
    }
}

void printStates(set<string> states)
{
    int size = states.size();
    int ind = 0;

    if (size == 0)
    {
        cout << "#";
    }
    else
    {
        for (auto i : states)
        {
            ind++;
            if (ind == size)
            {
                cout << i;
            }
            else
            {
                cout << i << ",";
            }
        }
    }
}

int main(void)
{
    string line;
    vector<string> firstFive;
    vector<string> otherLines;

    for (int i = 1; i <= 5; i++)
    {
        cin >> line;
        firstFive.push_back(line);
    }

    while (getline(cin, line))
    {
        cin >> line;
        if (line.empty())
            break;
        otherLines.push_back(line);
    }

    vector<string> sequences;
    vector<string> currentSequence;
    vector<string> states;
    vector<string> alphabet;
    vector<string> okStates;
    string firstState = firstFive[4];

    char delim = '|';
    splitString(sequences, firstFive[0], delim);
    delim = ',';
    splitString(states, firstFive[1], delim);
    splitString(alphabet, firstFive[2], delim);
    splitString(okStates, firstFive[3], delim);

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
    symbolToNumber["$"] = alphabet.size();

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
    vector<string> nextStateSet;
    for (long unsigned int i = 0; i < otherLines.size(); i++)
    {
        delim = '>';
        help.clear();
        stateSign.clear();
        nextStateSet.clear();

        splitString(help, otherLines[i], delim);
        help[0] = help[0].substr(0, help[0].length() - 1);

        delim = ',';
        splitString(stateSign, help[0], delim);
        splitString(nextStateSet, help[1], delim);

        for (long unsigned int j = 0; j < nextStateSet.size(); j++)
        {
            if (nextStateSet[0] != "#")
            {
                table[stateToNumber[stateSign[0]]]
                     [symbolToNumber[stateSign[1]]]
                         .insert(nextStateSet[j]);
            }
        }
    }

    set<string> currentStates;

    for (long unsigned int i = 0; i < sequences.size(); i++)
    {
        currentSequence.clear();
        currentStates.clear();
        currentStates.insert(firstState);
        epsilonF(currentStates, table, stateToNumber, symbolToNumber);

        splitString(currentSequence, sequences[i], delim);

        for (long unsigned int i = 0; i < currentSequence.size(); i++)
        {
            printStates(currentStates);
            delta(currentStates, table, stateToNumber,
                  symbolToNumber, currentSequence[i]);
            epsilonF(currentStates, table, stateToNumber, symbolToNumber);

            cout << "|";
        }

        printStates(currentStates);

        cout << "\n";
    }

    return 0;
}