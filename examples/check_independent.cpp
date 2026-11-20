#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    string dir = "examples/data/";

    ifstream ptFile(dir + "page_table_input.txt");
    ifstream laFile(dir + "input_la.txt");
    ifstream actFile(dir + "actual_result.txt");
    if (!ptFile || !laFile || !actFile) {
        cout << "Khong mo duoc file input." << endl;
        return 1;
    }

    int P, count;
    ptFile >> P >> count;
    vector<int> frame(count), valid(count);
    for (int i = 0; i < count; i++) {
        int page;
        ptFile >> page >> frame[page] >> valid[page];
    }

    string header;
    getline(actFile, header);

    ofstream out(dir + "expected_vs_actual.txt");
    out << "LA | expected | actual | ket luan" << endl;
    cout << "LA | expected | actual | ket luan" << endl;

    int total = 0, pass = 0;
    long long LA;
    while (laFile >> LA) {
        long long actLA;
        string actStatus, actPA;
        actFile >> actLA >> actStatus >> actPA;

        string expStatus = "OUT_OF_RANGE";
        string expPA = "-";
        for (int i = 0; i < count; i++) {
            long long start = (long long)i * P;
            if (LA >= start && LA < start + P) {
                if (valid[i] == 0) {
                    expStatus = "PAGE_FAULT";
                } else {
                    expStatus = "OK";
                    expPA = to_string((long long)frame[i] * P + (LA - start));
                }
            }
        }

        bool ok = (actLA == LA && actStatus == expStatus && actPA == expPA);
        total++;
        if (ok) pass++;

        string line = to_string(LA) + " | " + expStatus + " " + expPA + " | " +
                      actStatus + " " + actPA + " | " + (ok ? "PASS" : "FAIL");
        out << line << endl;
        cout << line << endl;
    }

    cout << endl << "Tong: " << total << " test, " << pass << " PASS, "
         << total - pass << " FAIL" << endl;
    return (pass == total) ? 0 : 1;
}
