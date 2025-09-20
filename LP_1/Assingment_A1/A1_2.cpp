#include <bits/stdc++.h>
using namespace std;

// pad integer string to width 3 with leading zeros
string pad3(const string &s) {
    string t = s;
    t.erase(remove_if(t.begin(), t.end(), ::isspace), t.end());
    if (t.empty()) return "000";
    bool allDigits = all_of(t.begin(), t.end(), [](char c){ return isdigit((unsigned char)c); });
    if (allDigits) {
        if (t.size() >= 3) return t.substr(t.size()-3);
        return string(3 - t.size(), '0') + t;
    }
    return t;
}

bool starts_with(const string &s, const string &p) {
    return s.rfind(p, 0) == 0;
}

// extract digits from a token like "S,05" or "L,1" or "C,101"
int extractIndex(const string &tok) {
    string digs;
    for (char c : tok) if (isdigit((unsigned char)c)) digs.push_back(c);
    if (digs.empty()) return -1;
    return stoi(digs);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Load symbol table
    unordered_map<int,string> symAddr;
    ifstream st("symtable.txt");
    if (!st) { cerr << "Error: cannot open symtable.txt\n"; return 1; }
    string line;
    while (getline(st, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        int no; string name, addr;
        if (!(ss >> no >> name >> addr)) continue;
        symAddr[no] = addr;
    }

    // Load literal table
    unordered_map<int,string> litAddr;
    ifstream lt("littable.txt");
    if (!lt) { cerr << "Error: cannot open littable.txt\n"; return 1; }
    while (getline(lt, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        int no; string lit, addr;
        if (!(ss >> no >> lit >> addr)) continue;
        litAddr[no] = addr;
    }

    // Open intermediate code
    ifstream icin("ic.txt");
    if (!icin) { cerr << "Error: cannot open ic.txt\n"; return 1; }
    ofstream mc("machine_code.txt");
    if (!mc) { cerr << "Error: cannot open machine_code.txt\n"; return 1; }

    cout << "\n -- ASSEMBLER PASS-2 OUTPUT --\n";
    cout << "\n LC\t <INTERMEDIATE CODE>\t\t\tLC\t <MACHINE CODE>\n\n";

    regex re(R"(\(([^)]+)\))"); // extract parentheses groups

    while (getline(icin, line)) {
        if (line.empty()) continue;

        string lcStr, icPart;
        size_t tabpos = line.find('\t');
        if (tabpos != string::npos) {
            lcStr = line.substr(0, tabpos);
            size_t pos2 = line.find_first_not_of("\t ", tabpos);
            icPart = (pos2 != string::npos) ? line.substr(pos2) : "";
        } else {
            stringstream ss(line);
            ss >> lcStr;
            getline(ss, icPart);
        }

        auto trim = [](string &s){ size_t a = s.find_first_not_of(" \t\r\n"); if(a==string::npos){s="";return;} size_t b=s.find_last_not_of(" \t\r\n"); s=s.substr(a,b-a+1);};
        trim(lcStr); trim(icPart);

        vector<string> groups;
        auto begin = sregex_iterator(icPart.begin(), icPart.end(), re);
        auto end = sregex_iterator();
        for (auto it = begin; it != end; ++it) groups.push_back((*it)[1].str());

        string icDisplay;
        if (!groups.empty()) for (auto &g : groups) icDisplay += "(" + g + ")";
        else icDisplay = icPart;

        string MC = "-NAN-";

        if (!groups.empty()) {
            string g0 = groups[0];
            size_t comma = g0.find(',');
            string cls = (comma==string::npos) ? g0 : g0.substr(0, comma);
            string code = (comma==string::npos) ? "" : g0.substr(comma+1);

            if (cls == "IS") {
                string opcode = code, reg = "0", mem = "000";

                for (size_t i=1;i<groups.size();++i) {
                    string g = groups[i];
                    if (starts_with(g,"R,")) { int r=extractIndex(g); reg=(r==-1?"0":to_string(r)); }
                    else if (starts_with(g,"C,")) { int v=extractIndex(g); if(opcode=="07"&&reg=="0") reg=(v==-1?"0":to_string(v)); else if(mem=="000") mem=pad3(to_string(v==-1?0:v)); }
                    else if (starts_with(g,"S,")) { int id=extractIndex(g); if(opcode=="07"&&reg=="0"&&id!=-1&&id<=6) reg=to_string(id); else if(id!=-1&&symAddr.count(id)) mem=pad3(symAddr[id]); }
                    else if (starts_with(g,"L,")) { int id=extractIndex(g); if(id!=-1&&litAddr.count(id)) mem=pad3(litAddr[id]); }
                }

                if (opcode=="00") MC="00\t0\t000";
                else { if(opcode.empty()) opcode="00"; MC=opcode+"\t"+reg+"\t"+mem; }
            }
            else if (cls == "DL") {
                if(code=="01"){ string val="0"; if(groups.size()>=2&&starts_with(groups[1],"C,")) val=to_string(extractIndex(groups[1])); MC="00\t0\t"+pad3(val);}
                else if(code=="02") MC="-NAN-";
            }
            else if (cls == "AD") MC="-NAN-";
        }

        cout << " " << (lcStr.empty()?"-":lcStr) << "\t" << icDisplay
             << "\t\t\t" << (lcStr.empty()?"-":lcStr) << "\t" << MC << "\n";

        string outLC = lcStr.empty()?"-":lcStr;
        mc << outLC << "\t" << MC << "\n";
    }

    cout << "\nMachine code written to machine_code.txt\n";
    return 0;
}
