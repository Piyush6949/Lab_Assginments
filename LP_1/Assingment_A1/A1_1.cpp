#include <bits/stdc++.h>
using namespace std;

struct OPtab {
    string opcode, mclass, mnemonic;
};
struct symTable {
    int no;
    string sname, addr;
};
struct litTable {
    int no;
    string lname, addr;
};
struct poolTable {
    int no;
    string lno;
};

OPtab optab[18] = {
    {"STOP", "IS", "00"}, {"ADD", "IS", "01"}, {"SUB", "IS", "02"}, {"MULT", "IS", "03"},
    {"MOVER", "IS", "04"}, {"MOVEM", "IS", "05"}, {"COMP", "IS", "06"}, {"BC", "IS", "07"},
    {"DIV", "IS", "08"}, {"READ", "IS", "09"}, {"PRINT", "IS", "10"}, {"START", "AD", "01"},
    {"END", "AD", "02"}, {"ORIGIN", "AD", "03"}, {"EQU", "AD", "04"}, {"LTORG", "AD", "05"},
    {"DC", "DL", "01"}, {"DS", "DL", "02"}
};

symTable ST[200];
litTable LT[200];
poolTable PT[200];

int scnt = 0, lcnt = 0, nlcnt = 0, pcnt = 0;
int poolStartIndex = 0; 

int getOP(const string &s) {
    for (int i = 0; i < 18; ++i)
        if (optab[i].opcode == s) return i;
    return -1;
}

int getRegID(const string &s) {
    if (s == "AREG") return 1;
    if (s == "BREG") return 2;
    if (s == "CREG") return 3;
    if (s == "DREG") return 4;
    return -1;
}

int getConditionCode(const string &s) {
    if (s == "LT") return 1;
    if (s == "LE") return 2;
    if (s == "EQ") return 3;
    if (s == "GT") return 4;
    if (s == "GE") return 5;
    if (s == "ANY") return 6;
    return -1;
}

bool presentST(const string &s) {
    for (int i = 0; i < scnt; ++i)
        if (ST[i].sname == s) return true;
    return false;
}

int getSymID(const string &s) {
    for (int i = 0; i < scnt; ++i)
        if (ST[i].sname == s) return i;
    return -1;
}

bool presentLTCurrentPool(const string &s) {
    for (int i = poolStartIndex; i < lcnt; ++i)
        if (LT[i].lname == s) return true;
    return false;
}

int getLitID(const string &s) {
    for (int i = 0; i < lcnt; ++i)
        if (LT[i].lname == s) return i;
    return -1;
}

bool isNumber(const string &s) {
    if (s.empty()) return false;
    int i = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    for (; i < (int)s.size(); i++) if (!isdigit(s[i])) return false;
    return true;
}

string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

vector<string> tokenizeLine(const string &line) {
    vector<string> tokens;
    istringstream iss(line);
    string tok;
    while (iss >> tok) {
        while (!tok.empty() && tok.back() == ',') tok.pop_back();
        tokens.push_back(tok);
    }
    return tokens;
}

// assign addresses to literals in pool
void flushLiteralPool(int &LC, ofstream &ic) {
    for (int i = poolStartIndex; i < lcnt; ++i) {
        string litVal = LT[i].lname;
        if (!litVal.empty() && litVal[0] == '=') litVal = litVal.substr(1);
        if (litVal.size() >= 2 && litVal.front() == '\'' && litVal.back() == '\'')
            litVal = litVal.substr(1, litVal.size() - 2);

        string IC = "(DL,01)(C," + litVal + ")";
        LT[i].addr = to_string(LC);
        cout << "\t\t\t\t\t\t\t" << LT[i].addr << "\t\t" << IC << "\n";
        ic << LT[i].addr << "\t" << IC << "\n";
        LC++;
    }
    if (lcnt - poolStartIndex > 0) {
        PT[pcnt++] = {pcnt + 1, "#" + to_string(poolStartIndex + 1)};
    }
    poolStartIndex = lcnt;
    nlcnt = 0;
}

int main() {
    ifstream fin("input3.txt");
    ofstream ic("ic.txt"), st("symtable.txt"), lt("littable.txt");

    if (!fin) {
        cerr << "Cannot open input3.txt\n";
        return 1;
    }

    cout << "\n -- ASSEMBLER PASS-1 OUTPUT --\n";
    cout << "\n<LABEL\t\tOPCODE\t\tOP1\t\tOP2\tLC\t\tINTERMEDIATE CODE>\n";

    string line;
    int LC = 0;

    while (getline(fin, line)) {
        line = trim(line);
        if (line.empty() || line[0] == ';' || line[0] == '#') continue;

        vector<string> tok = tokenizeLine(line);
        if (tok.empty()) continue;

        string label = "NAN", opcode, op1 = "NAN", op2 = "NAN";
        int firstOP = getOP(tok[0]);

        if (firstOP != -1) {
            opcode = tok[0];
            if (tok.size() >= 2) op1 = tok[1];
            if (tok.size() >= 3) op2 = tok[2];
        } else {
            label = tok[0];
            if (tok.size() >= 2) opcode = tok[1];
            if (tok.size() >= 3) op1 = tok[2];
            if (tok.size() >= 4) op2 = tok[3];
        }

        int id = getOP(opcode);
        string IC = "", lc = "---";

        if (id == -1) {
            cout << label << "\t\t" << opcode << "\t\t" << op1 << "\t\t" << op2
                 << "\t" << lc << "\t\t" << "(UNKNOWN OPCODE)\n";
            continue;
        }

        IC = "(" + optab[id].mclass + "," + optab[id].mnemonic + ")";

        if (opcode == "START") {
            if (op1 != "NAN") {
                LC = stoi(op1);
                IC += "(C," + op1 + ")";
            }
            cout << label << "\t\t" << opcode << "\t\t" << op1 << "\t\t" << op2
                 << "\t" << "---" << "\t\t" << IC << "\n";
            ic << "---" << "\t" << IC << "\n";
            continue;
        }

        if (label != "NAN") {
            if (!presentST(label)) ST[scnt++] = {scnt + 1, label, to_string(LC)};
            else ST[getSymID(label)].addr = to_string(LC);
        }

        if (opcode == "ORIGIN") {
            string token1, token2; char op = '+';
            size_t p = op1.find('+'), m = op1.find('-');
            if (p != string::npos) { op = '+'; token1 = op1.substr(0, p); token2 = op1.substr(p + 1); }
            else if (m != string::npos) { op = '-'; token1 = op1.substr(0, m); token2 = op1.substr(m + 1); }
            else { token1 = op1; token2 = "0"; }

            if (!presentST(token1)) ST[scnt++] = {scnt + 1, token1, "0"};
            int baseAddr = stoi(ST[getSymID(token1)].addr);
            int offset = isNumber(token2) ? stoi(token2) : 0;
            LC = (op == '+') ? baseAddr + offset : baseAddr - offset;

            IC += "(S,0" + to_string(ST[getSymID(token1)].no) + ")" + op + token2;
            cout << label << "\t\t" << opcode << "\t\t" << op1 << "\t\t" << op2
                 << "\t" << "---" << "\t\t" << IC << "\n";
            ic << "---" << "\t" << IC << "\n";
            continue;
        }

        if (opcode == "EQU") {
            if (!presentST(op1)) ST[scnt++] = {scnt + 1, op1, "0"};
            string addr = ST[getSymID(op1)].addr;
            if (label != "NAN") {
                if (!presentST(label)) ST[scnt++] = {scnt + 1, label, addr};
                else ST[getSymID(label)].addr = addr;
            }
            IC += "(S,0" + to_string(ST[getSymID(op1)].no) + ")";
            cout << label << "\t\t" << opcode << "\t\t" << op1 << "\t\t" << op2
                 << "\t" << "---" << "\t\t" << IC << "\n";
            ic << "---" << "\t" << IC << "\n";
            continue;
        }

        if (opcode == "LTORG" || opcode == "END") {
            cout << label << "\t\t" << opcode << "\t\t" << op1 << "\t\t" << op2
                 << "\t" << "---" << "\t\t" << IC << "\n";
            ic << "---" << "\t" << IC << "\n";
            flushLiteralPool(LC, ic);
            if (opcode == "END") break;
            else continue;
        }

        lc = to_string(LC);

        if (opcode == "DS") {
            IC += "(C," + op1 + ")";
            LC += stoi(op1);
        }
        else if (opcode == "DC") {
            string val = op1;
            if (val.size() >= 2 && val.front() == '\'' && val.back() == '\'')
                val = val.substr(1, val.size() - 2);
            IC += "(C," + val + ")";
            LC++;
        }
        else if (optab[id].mclass == "IS") {
            if (opcode == "BC") {
                IC += "(S," + to_string(getConditionCode(op1)) + ")";
            } else if (opcode == "READ" || opcode == "PRINT") {
                if (op1 != "NAN" && op1 != "") {
                    if (!presentST(op1)) ST[scnt++] = {scnt + 1, op1, "0"};
                    IC += "(S,0" + to_string(ST[getSymID(op1)].no) + ")";
                } else IC += "(R,-1)";
            } else {
                int reg = getRegID(op1);
                IC += "(R," + (reg == -1 ? string("-1") : to_string(reg)) + ")";
            }

            if (op2 != "NAN" && !op2.empty()) {
                if (op2[0] == '=') {
                    if (!presentLTCurrentPool(op2)) LT[lcnt++] = {lcnt + 1, op2, "0"};
                    IC += "(L,0" + to_string(getLitID(op2) + 1) + ")";
                }
                else if (isNumber(op2)) IC += "(C," + op2 + ")";
                else {
                    if (!presentST(op2)) ST[scnt++] = {scnt + 1, op2, "0"};
                    IC += "(S,0" + to_string(getSymID(op2) + 1) + ")";
                }
            }
            LC++;
        }

        cout << label << "\t\t" << opcode << "\t\t" << op1 << "\t\t" << op2
             << "\t" << lc << "\t\t" << IC << "\n";
        ic << lc << "\t" << IC << "\n";
    }

    if (poolStartIndex < lcnt) flushLiteralPool(LC, ic);

    cout << "\n----------------------------------------------------------------\n";
    cout << " -- SYMBOL TABLE --\n\n<NO.\tSYMBOL\tADDRESS>\n";
    for (int i = 0; i < scnt; ++i) {
        cout << ST[i].no << "\t " << ST[i].sname << "\t  " << ST[i].addr << "\n";
        st << ST[i].no << "\t " << ST[i].sname << "\t  " << ST[i].addr << "\n";
    }

    cout << "\n----------------------------------------------------------------\n";
    cout << " -- LITERAL TABLE --\n\n<NO.\tLITERAL\tADDRESS>\n";
    for (int i = 0; i < lcnt; ++i) {
        cout << LT[i].no << "\t " << LT[i].lname << "\t  " << LT[i].addr << "\n";
        lt << LT[i].no << "\t " << LT[i].lname << "\t  " << LT[i].addr << "\n";
    }

    cout << "\n----------------------------------------------------------------\n";
    cout << " -- POOL TABLE --\n\n<NO.\tLITERAL_START_INDEX>\n";
    ofstream pt("pooltable.txt");
    for (int i = 0; i < pcnt; ++i) {
        cout << PT[i].no << "\t " << PT[i].lno << "\n";
        pt << PT[i].no << "\t " << PT[i].lno << "\n";
    }
    pt.close();

    cout << "\n\nFiles written: ic.txt, symtable.txt, littable.txt, pooltable.txt\n";
    return 0;
}
