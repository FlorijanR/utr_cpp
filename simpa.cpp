#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <string>
#include <algorithm>
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

int main(void)
{
    string line;
    vector<string> firstSeven;
    vector<string> otherLines;

    for (int i = 1; i <= 7; i++)
    {
        getline(cin, line);
        firstSeven.push_back(line);
    }

    do
    {
        cin >> line;
        if (line.empty())
            break;
        otherLines.push_back(line);
    } while (getline(cin, line));

    vector<string> allSequences;
    vector<string> states;
    vector<string> symbols;
    vector<string> signs;
    vector<string> okStates;
    string firstState = firstSeven[5];
    string firstSign = firstSeven[6];

    char delim = '|';
    splitString(allSequences, firstSeven[0], delim);
    delim = ',';
    splitString(states, firstSeven[1], delim);
    splitString(symbols, firstSeven[2], delim);
    splitString(signs, firstSeven[3], delim);
    splitString(okStates, firstSeven[4], delim);

    unordered_map<string, int> stateToNumber;
    unordered_map<string, int> symbolToNumber;
    unordered_map<string, int> signToNumber;

    for (long unsigned int i = 0; i < states.size(); i++)
    {
        stateToNumber[states[i]] = i;
    }
    for (long unsigned int i = 0; i < symbols.size(); i++)
    {
        symbolToNumber[symbols[i]] = i;
    }
    symbolToNumber["$"] = symbols.size();
    for (long unsigned int i = 0; i < signs.size(); i++)
    {
        signToNumber[signs[i]] = i;
    }
    signToNumber["$"] = signs.size();

    //[state][symbol][sign]
    vector<vector<vector<vector<string>>>> table;
    vector<string> toInsert;
    vector<vector<string>> toInsert2;
    vector<vector<vector<string>>> toInsert3;
    for (long unsigned int i = 0; i < signs.size() + 1; i++)
    {
        toInsert2.push_back(toInsert);
    }
    for (long unsigned int i = 0; i < symbols.size() + 1; i++)
    {
        toInsert3.push_back(toInsert2);
    }
    for (long unsigned int i = 0; i < states.size(); i++)
    {
        table.push_back(toInsert3);
    }

    vector<string> leftAndRight;
    vector<string> left;
    vector<string> right;
    for (long unsigned int i = 0; i < otherLines.size(); i++)
    {
        delim = '>';
        leftAndRight.clear();
        left.clear();
        right.clear();

        splitString(leftAndRight, otherLines[i], delim);
        leftAndRight[0] = leftAndRight[0].substr(0, leftAndRight[0].length() - 1);

        delim = ',';
        splitString(left, leftAndRight[0], delim);
        splitString(right, leftAndRight[1], delim);

        table[stateToNumber[left[0]]]
             [symbolToNumber[left[1]]]
             [signToNumber[left[2]]]
                 .push_back(right[0]);

        table[stateToNumber[left[0]]]
             [symbolToNumber[left[1]]]
             [signToNumber[left[2]]]
                 .push_back(right[1]);
    }

    vector<string> currentSequence;
    string currentState;
    string currentSymbol;
    string currentSign = firstSign;

    vector<string> myStack;
    vector<string> next;
    string help;
    bool badSeq = false;
    bool found = false;
    bool reduce = false;
    for (long unsigned int i = 0; i < allSequences.size(); i++)
    {
        currentSequence.clear();
        currentState = firstState;
        currentSign = firstSign;
        badSeq = false;
        reduce = false;
        myStack.clear();
        myStack.push_back(firstSign);

        splitString(currentSequence, allSequences[i], delim);

        cout << currentState << "#" << currentSign << "|";
        for (long unsigned int k = 0; k < currentSequence.size();
             reduce ? reduce = false : k++)
        {
            if (myStack.empty())
            {
                currentSign = "$";
            }
            else
            {
                currentSign = myStack.back();
                myStack.pop_back();
            }

            next = table[stateToNumber[currentState]]
                        [symbolToNumber[currentSequence[k]]]
                        [signToNumber[currentSign]];
            if (next.empty())
            {
                reduce = true;
                next = table[stateToNumber[currentState]]
                            [symbolToNumber["$"]]
                            [signToNumber[currentSign]];
                if (next.empty())
                {
                    cout << "fail|0";
                    badSeq = true;
                    break;
                }
            }

            currentState = next[0];
            if (next[1] != "$")
            {
                help = next[1];
                for (int l = help.size() - 1; l >= 0; l--)
                {
                    string help2(1, help.at(l));
                    myStack.push_back(help2);
                }
            }

            cout << currentState << "#";
            if (myStack.empty())
            {
                cout << "$";
            }
            else
            {
                for (long unsigned int m = 0; m < myStack.size(); m++)
                {
                    cout << myStack[myStack.size() - 1 - m];
                }
            }
            cout << "|";
        }

        while (badSeq == false)
        {
            found = false;
            for (long unsigned int j = 0; j < okStates.size(); j++)
            {
                if (okStates[j] == currentState)
                {
                    cout << "1";
                    found = true;
                    break;
                }
            }
            if (found == true)
                break;

            if (myStack.empty())
            {
                currentSign = "$";
            }
            else
            {
                currentSign = myStack.back();
                myStack.pop_back();
            }

            next = table[stateToNumber[currentState]]
                        [symbolToNumber["$"]]
                        [signToNumber[currentSign]];
            if (next.empty())
            {
                cout << "0";
                break;
            }

            currentState = next[0];
            if (next[1] != "$")
            {
                help = next[1];
                for (int n = help.size() - 1; n >= 0; n--)
                {
                    string help2(1, help.at(n));
                    myStack.push_back(help2);
                }
            }

            cout << currentState << "#";
            if (myStack.empty())
            {
                cout << "$";
            }
            else
            {
                for (long unsigned int m = 0; m < myStack.size(); m++)
                {
                    cout << myStack[myStack.size() - 1 - m];
                }
            }
            cout << "|";
        }

        cout << "\n";
    }

    return 0;
}