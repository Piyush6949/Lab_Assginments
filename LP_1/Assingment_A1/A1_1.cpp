#include <bits/stdc++.h>
using namespace std;

// ---------- Structures ----------
struct Symbol
{
    string name;
    int address;
};

struct Literal
{
    string value;
    int address;
};

struct Intermediate
{
    int lc;               // Location Counter
    vector<string> parts; // e.g. {"(IS,09)", "(S,02)"}
};

// ---------- Tables ----------
map<string, pair<string, int>> MOT = {
    {"STOP", {"IS", 0}}, {"ADD", {"IS", 1}}, {"SUB", {"IS", 2}}, {"MULT", {"IS", 3}}, {"MOVER", {"IS", 4}}, {"MOVEM", {"IS", 5}}, {"COMP", {"IS", 6}}, {"BC", {"IS", 7}}, {"DIV", {"IS", 8}}, {"READ", {"IS", 9}}, {"PRINT", {"IS", 10}}};

map<string, pair<string, int>> POT = {
    {"START", {"AD", 1}}, {"END", {"AD", 2}}, {"ORIGIN", {"AD", 3}}, {"EQU", {"AD", 4}}};

map<string, pair<string, int>> DL = {
    {"DC", {"DL", 1}}, {"DS", {"DL", 2}}};

vector<Symbol> SYMTAB;
vector<Literal> LITTAB;
vector<int> POOLTAB = {0}; // Pool starts at index 0
vector<Intermediate> IC;

int LC = 0;

// ---------- Helpers ----------
int searchSymbol(const string &sym)
{
    for (int i = 0; i < (int)SYMTAB.size(); i++)
    {
        if (SYMTAB[i].name == sym)
            return i;
    }
    return -1;
}

int searchLiteral(const string &lit)
{
    for (int i = 0; i < (int)LITTAB.size(); i++)
    {
        if (LITTAB[i].value == lit)
            return i;
    }
    return -1;
}

// ---------- PASS 1 ----------
void Pass1(vector<string> source)
{
    for (auto &line : source)
    {
        vector<string> tokens;
        string word;
        stringstream ss(line);
        while (ss >> word)
            tokens.push_back(word);
        if (tokens.empty())
            continue;

        string label = "", opcode = "", operand = "";
        if (tokens.size() == 1)
        {
            opcode = tokens[0];
        }
        else if (tokens.size() == 2)
        {
            opcode = tokens[0];
            operand = tokens[1];
        }
        else if (tokens.size() == 3)
        {
            label = tokens[0];
            opcode = tokens[1];
            operand = tokens[2];
        }

        // Add label to SYMTAB
        if (!label.empty())
        {
            int idx = searchSymbol(label);
            if (idx == -1)
            {
                SYMTAB.push_back({label, LC});
            }
            else
            {
                SYMTAB[idx].address = LC;
            }
        }

        Intermediate icEntry;
        icEntry.lc = LC;

        // ----- Assembler Directives -----
        if (POT.find(opcode) != POT.end())
        {
            auto [cls, code] = POT[opcode];
            icEntry.parts.push_back("(" + cls + "," + to_string(code) + ")");
            if (opcode == "START" && !operand.empty())
            {
                icEntry.parts.push_back("(C," + operand + ")");
                LC = stoi(operand);
            }
            else if (opcode == "END")
            {
                // assign literals
                for (int i = POOLTAB.back(); i < (int)LITTAB.size(); i++)
                {
                    LITTAB[i].address = LC++;
                }
                POOLTAB.push_back(LITTAB.size());
            }
        }

        // ----- Imperative Statements -----
        else if (MOT.find(opcode) != MOT.end())
        {
            auto [cls, code] = MOT[opcode];
            icEntry.parts.push_back("(" + cls + "," + to_string(code) + ")");
            if (!operand.empty())
            {
                if (operand[0] == '=')
                { // literal
                    int idx = searchLiteral(operand);
                    if (idx == -1)
                    {
                        LITTAB.push_back({operand, -1});
                        idx = LITTAB.size() - 1;
                    }
                    icEntry.parts.push_back("(L," + to_string(idx + 1) + ")");
                }
                else
                { // symbol
                    int idx = searchSymbol(operand);
                    if (idx == -1)
                    {
                        SYMTAB.push_back({operand, -1});
                        idx = SYMTAB.size() - 1;
                    }
                    icEntry.parts.push_back("(S," + to_string(idx + 1) + ")");
                }
            }
            LC++;
        }

        // ----- Declarative Statements -----
        else if (DL.find(opcode) != DL.end())
        {
            auto [cls, code] = DL[opcode];
            icEntry.parts.push_back("(" + cls + "," + to_string(code) + ")");
            if (!operand.empty())
            {
                icEntry.parts.push_back("(C," + operand + ")");
                if (opcode == "DS")
                    LC += stoi(operand);
                else
                    LC++;
            }
        }

        if (!icEntry.parts.empty())
            IC.push_back(icEntry);
    }
}

// ---------- MAIN ----------
int main()
{
    vector<string> source = {
        "START 100",
        "READ N",
        "MOVER AREG, N",
        "ADD =5",
        "MOVEM AREG, RESULT",
        "STOP",
        "N DS 1",
        "RESULT DS 1",
        "END"};

    cout << "---- PASS 1 ----\n";
    Pass1(source);

    cout << "\nSYMTAB:\n";
    for (int i = 0; i < (int)SYMTAB.size(); i++)
    {
        cout << i + 1 << ") " << SYMTAB[i].name << " -> " << SYMTAB[i].address << "\n";
    }

    cout << "\nLITTAB:\n";
    for (int i = 0; i < (int)LITTAB.size(); i++)
    {
        cout << i + 1 << ") " << LITTAB[i].value << " -> " << LITTAB[i].address << "\n";
    }

    cout << "\nPOOLTAB:\n";
    for (int i = 0; i < (int)POOLTAB.size(); i++)
    {
        cout << "#" << i << " -> starts at LITTAB index " << POOLTAB[i] + 1 << "\n";
    }

    cout << "\nIntermediate Code:\n";
    for (auto &i : IC)
    {
        cout << i.lc << " ) ";
        for (auto &p : i.parts)
            cout << p << " ";
        cout << "\n";
    }

    return 0;
}
