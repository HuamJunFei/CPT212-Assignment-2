#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
    Boyer-Moore String Matching Algorithm
    Implements:
    1. Bad Character Heuristic
    2. Good Suffix Heuristic
*/

//--------------------------------------------------
// Build Bad Character Table
//--------------------------------------------------
void buildBadCharacterTable(const string& pattern, vector<int>& badChar)
{
    int m = static_cast<int>(pattern.length());

    badChar.assign(256, -1);

    for (int i = 0; i < m; i++)
    {
        badChar[(unsigned char)pattern[i]] = i;
    }
}

//--------------------------------------------------
// Helper Function for Good Suffix Rule
//--------------------------------------------------
void preprocessStrongSuffix(vector<int>& shift,vector<int>& borderPos,const string& pattern,int m)
{
    int i = m;
    int j = m + 1;

    borderPos[i] = j;

    while (i > 0)
    {
        while (j <= m &&
               pattern[i - 1] != pattern[j - 1])
        {
            if (shift[j] == 0)
            {
                shift[j] = j - i;
            }

            j = borderPos[j];
        }

        i--;
        j--;

        borderPos[i] = j;
    }
}

//--------------------------------------------------
// Preprocess Case 2 of Good Suffix Rule
//--------------------------------------------------
void preprocessCase2(vector<int>& shift,vector<int>& borderPos,int m)
{
    int j = borderPos[0];

    for (int i = 0; i <= m; i++)
    {
        if (shift[i] == 0)
        {
            shift[i] = j;
        }

        if (i == j)
        {
            j = borderPos[j];
        }
    }
}

//--------------------------------------------------
// Build Good Suffix Table
//--------------------------------------------------
void buildGoodSuffixTable(const string& pattern,vector<int>& shift)
{
    int m = pattern.length();

    vector<int> borderPos(m + 1);

    shift.assign(m + 1, 0);

    preprocessStrongSuffix(shift,borderPos,pattern,m);
    preprocessCase2(shift,borderPos,m);
}

//--------------------------------------------------
// Boyer-Moore Search
//--------------------------------------------------
void boyerMooreSearch(const string& text,const string& pattern)
{
    int n = text.length();
    int m = pattern.length();

    vector<int> badChar;
    vector<int> goodSuffix;

    buildBadCharacterTable(pattern, badChar);
    buildGoodSuffixTable(pattern, goodSuffix);

    int shift = 0;
    bool found = false;

    while (shift <= (n - m))
    {
        int j = m - 1;

        // Compare from right to left
        while (j >= 0 &&
               pattern[j] == text[shift + j])
        {
            j--;
        }

        // Pattern found
        if (j < 0)
        {
            cout << "Pattern found at index "
                 << shift << endl;

            found = true;

            shift += goodSuffix[0];
        }
        else
        {
            int badCharShift = j - badChar[(unsigned char)text[shift + j]];
            int goodSuffixShift = goodSuffix[j + 1];
            shift += max(1,max(badCharShift,goodSuffixShift));
        }
    }

    if (!found)
    {
        cout << "Pattern not found." << endl;
    }
}

//--------------------------------------------------
// Main Function
//--------------------------------------------------
int main()
{
    string text ="AAAAAAAAA";
    string pattern ="AAAA";

    cout << "Text: " << text << endl;
    cout << "Pattern: " << pattern << endl;
    cout << endl;

    boyerMooreSearch(text, pattern);

    return 0;
}
