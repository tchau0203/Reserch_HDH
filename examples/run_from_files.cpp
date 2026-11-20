#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "page_table.h"
#include "translate.h"
using namespace std;

int main(int argc, char* argv[]) {
    string ptPath     = (argc > 1) ? argv[1] : "examples/data/page_table_input.txt";
    string laPath     = (argc > 2) ? argv[2] : "examples/data/input_la.txt";
    string resultPath = (argc > 3) ? argv[3] : "examples/data/actual_result.txt";
    string tracePath  = (argc > 4) ? argv[4] : "examples/data/trace.txt";

    ifstream ptFile(ptPath);
    ifstream laFile(laPath);
    if (!ptFile || !laFile) {
        cout << "Khong mo duoc file input." << endl;
        return 1;
    }
    ofstream resultFile(resultPath);
    ofstream traceFile(tracePath);

    int P;
    ptFile >> P;

    PageTable table;
    ostringstream bo_qua;
    try {
        table = loadPageTable(ptFile, bo_qua);
    } catch (const runtime_error& e) {
        cout << "Loi doc page table: " << e.what() << endl;
        return 1;
    }

    traceFile << "Page size P = " << P << " byte" << endl;
    printPageTable(table, traceFile);
    traceFile << endl << "=== TRACE ===" << endl;

    resultFile << "LA status PA" << endl;
    int LA;
    while (laFile >> LA) {
        int PA = -1;
        int kq = translate(LA, P, table, PA, traceFile);

        if (kq == OK)
            resultFile << LA << " OK " << PA << endl;
        else if (kq == OUT_OF_RANGE)
            resultFile << LA << " OUT_OF_RANGE -" << endl;
        else if (kq == PAGE_FAULT)
            resultFile << LA << " PAGE_FAULT -" << endl;
        else
            resultFile << LA << " INVALID_INPUT -" << endl;
    }
    cout << "Xong. Xem " << resultPath << " va " << tracePath << endl;
    return 0;
}

