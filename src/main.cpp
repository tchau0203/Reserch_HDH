#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "address_translation.h"
#include "page_table.h"

using namespace std;

// Dữ liệu mẫu chuẩn của nhóm (P = 16, 4 pages)
void loadDefaultConfiguration(int& P, PageTable& table) {
    P = 16;
    table.resize(4);
    table[0] = {0, 5, true};
    table[1] = {1, 3, true};
    table[2] = {2, 7, true};
    table[3] = {3, 0, false};
}

// Chạy bộ 8 test case mẫu và đối chiếu độc lập
void runSelfTest(int P, const PageTable& table) {
    cout << "\n===============================================================\n";
    cout << "          CHAY BO KIEM THU TU DONG (8 TEST CASES MAU)         \n";
    cout << "===============================================================\n";
    cout << "Cau hinh: P = " << P << " byte | So page (PTLR) = " << table.size() << "\n\n";

    struct TestCase {
        int LA;
        string expectedStatus;
        string expectedPA;
        string description;
    };

    vector<TestCase> testCases = {
        {37, "OK", "117", "Dia chi hop le: p=2, d=5, frame=7 => 7*16+5=117"},
        {20, "OK", "52",  "Dia chi hop le: p=1, d=4, frame=3 => 3*16+4=52"},
        {5,  "OK", "85",  "Dia chi hop le: p=0, d=5, frame=5 => 5*16+5=85"},
        {16, "OK", "48",  "Bien duoi (offset = 0): p=1, d=0 => 3*16+0=48"},
        {31, "OK", "63",  "Bien tren (offset = P-1): p=1, d=15 => 3*16+15=63"},
        {64, "OUT_OF_RANGE", "-", "Ngoai bang: p=4 >= PTLR=4 => OUT_OF_RANGE"},
        {48, "PAGE_FAULT",   "-", "Entry khong hop le: p=3, valid=0 => PAGE_FAULT"},
        {63, "PAGE_FAULT",   "-", "Dia chi cuoi pham vi bang nhung valid=0 => PAGE_FAULT"}
    };

    cout << left << setw(4) << "#"
         << setw(8)  << "LA"
         << setw(16) << "Ky vong"
         << setw(16) << "Thuc te"
         << setw(12) << "Ket luan"
         << "Giai thich" << "\n";
    cout << string(80, '-') << "\n";

    int passedCount = 0;
    for (size_t i = 0; i < testCases.size(); ++i) {
        const auto& tc = testCases[i];
        int actualPA = -1;
        ostringstream traceStream;
        int status = translate(tc.LA, P, table, actualPA, traceStream);

        string actStatusStr;
        string actPAStr = "-";
        if (status == OK) {
            actStatusStr = "OK";
            actPAStr = to_string(actualPA);
        } else if (status == OUT_OF_RANGE) {
            actStatusStr = "OUT_OF_RANGE";
        } else if (status == PAGE_FAULT) {
            actStatusStr = "PAGE_FAULT";
        } else {
            actStatusStr = "INVALID_INPUT";
        }

        string expResult = tc.expectedStatus + (tc.expectedPA != "-" ? " (" + tc.expectedPA + ")" : "");
        string actResult = actStatusStr + (actPAStr != "-" ? " (" + actPAStr + ")" : "");
        bool pass = (actStatusStr == tc.expectedStatus && actPAStr == tc.expectedPA);
        if (pass) passedCount++;

        cout << left << setw(4) << (i + 1)
             << setw(8)  << tc.LA
             << setw(16) << expResult
             << setw(16) << actResult
             << setw(12) << (pass ? "[PASS]" : "[FAIL]")
             << tc.description << "\n";
    }

    cout << string(80, '-') << "\n";
    cout << "Ket qua: " << passedCount << "/" << testCases.size()
         << " test PASS (" << (passedCount == (int)testCases.size() ? "100% THANH CONG" : "CO LOI") << ")\n";
    cout << "===============================================================\n\n";
}

// Chạy dịch địa chỉ từ tệp tin hàng loạt
int runBatchFiles(const string& ptPath, const string& laPath,
                  const string& resultPath, const string& tracePath) {
    ifstream ptFile(ptPath);
    ifstream laFile(laPath);
    if (!ptFile.is_open()) {
        cerr << "Loi: Khong the mo tap tin Page Table: " << ptPath << "\n";
        return 1;
    }
    if (!laFile.is_open()) {
        cerr << "Loi: Khong the mo tap tin danh sach LA: " << laPath << "\n";
        return 1;
    }

    ofstream resultFile(resultPath);
    ofstream traceFile(tracePath);
    if (!resultFile.is_open() || !traceFile.is_open()) {
        cerr << "Loi: Khong the tao tap tin ket qua hoac trace.\n";
        return 1;
    }

    int P = 0;
    if (!(ptFile >> P) || P <= 0) {
        cerr << "Loi: Kich thuoc trang P trong tap tin khong hop le.\n";
        return 1;
    }

    PageTable table;
    ostringstream dummyOut;
    try {
        table = loadPageTable(ptFile, dummyOut);
    } catch (const exception& e) {
        cerr << "Loi khi doc Page Table: " << e.what() << "\n";
        return 1;
    }

    traceFile << "Page size P = " << P << " byte\n";
    printPageTable(table, traceFile);
    traceFile << "\n=== TRACE ===\n";

    resultFile << "LA status PA\n";

    cout << "\nDang xu ly dich dia chi tu tệp...\n";
    int count = 0;
    int LA;
    while (laFile >> LA) {
        int PA = -1;
        int status = translate(LA, P, table, PA, traceFile);

        string statusStr = "INVALID_INPUT";
        string paStr = "-";
        if (status == OK) {
            statusStr = "OK";
            paStr = to_string(PA);
        } else if (status == OUT_OF_RANGE) {
            statusStr = "OUT_OF_RANGE";
        } else if (status == PAGE_FAULT) {
            statusStr = "PAGE_FAULT";
        }

        resultFile << LA << " " << statusStr << " " << paStr << "\n";
        count++;
    }

    cout << "Da xu ly thanh cong " << count << " dia chi logic.\n";
    cout << "Ket qua da duoc ghi vao: " << resultPath << "\n";
    cout << "Trace chi tiet da ghi vao: " << tracePath << "\n\n";
    return 0;
}

void printBanner() {
    cout << "===============================================================\n";
    cout << "      CHUONG TRINH MO PHONG DICH DIA CHI QUA BANG TRANG        \n";
    cout << "          (Paging and Page-Table Address Translation)          \n";
    cout << "                   Mon: He Dieu Hanh - Nhom 12                 \n";
    cout << "===============================================================\n";
}

int main(int argc, char* argv[]) {
    // 1. Chế độ tham số dòng lệnh
    if (argc >= 3) {
        string ptPath = argv[1];
        string laPath = argv[2];
        string resultPath = (argc > 3) ? argv[3] : "examples/data/actual_result.txt";
        string tracePath  = (argc > 4) ? argv[4] : "examples/data/trace.txt";
        return runBatchFiles(ptPath, laPath, resultPath, tracePath);
    }

    if (argc == 2 && string(argv[1]) == "--test") {
        int P = 16;
        PageTable table;
        loadDefaultConfiguration(P, table);
        runSelfTest(P, table);
        return 0;
    }

    if (argc == 2 && (string(argv[1]) == "--help" || string(argv[1]) == "-h")) {
        cout << "Su dung:\n";
        cout << "  " << argv[0] << "                                    : Chay che do menu tuong tac\n";
        cout << "  " << argv[0] << " --test                             : Chay kiem thu 8 test cases mau\n";
        cout << "  " << argv[0] << " <pt_file> <la_file> [res] [trace]  : Chay xu ly theo tep hang loat\n";
        return 0;
    }

    // 2. Chế độ Menu tương tác
    printBanner();

    int P = 16;
    PageTable table;
    loadDefaultConfiguration(P, table);
    cout << "[He thong] Da khoi tao cau hinh mau: P = " << P
         << " byte, " << table.size() << " trang.\n";

    while (true) {
        cout << "\n----------------------- MENU CHINH ---------------------------\n";
        cout << " 1. Nhap Page Table moi tu ban phim\n";
        cout << " 2. Cai dat kich thuoc trang P (hien tai: P = " << P << " bytes)\n";
        cout << " 3. Nap lai cau hinh mau mac dinh (P = 16, 4 pages)\n";
        cout << " 4. Hien thi Page Table hien tai\n";
        cout << " 5. Dich mot dia chi logic (in trace chi tiet 6 buoc)\n";
        cout << " 6. Dich nhieu dia chi logic (nhap danh sach LA)\n";
        cout << " 7. Chay bo 8 test case kiem thu tu dong (Expected vs Actual)\n";
        cout << " 8. Chay mo phong doc tu tep tin (examples/data/)\n";
        cout << " 0. Thoat chuong trinh\n";
        cout << "--------------------------------------------------------------\n";
        cout << "Chon chuc nang (0-8): ";

        string choiceLine;
        if (!getline(cin, choiceLine)) break;
        stringstream ss(choiceLine);
        int choice = -1;
        if (!(ss >> choice)) {
            cout << "Lua chon khong hop le. Vui long nhap so tu 0 den 8.\n";
            continue;
        }

        if (choice == 0) {
            cout << "\nCam on ban da su dung chuong trinh. Tam biet!\n";
            break;
        }

        switch (choice) {
            case 1: {
                cout << "\n--- NHAP PAGE TABLE MOI ---\n";
                try {
                    table = loadPageTable(cin, cout);
                    cout << "\n=> Da nhap Page Table thanh cong! So entry: " << table.size() << "\n";
                } catch (const exception& e) {
                    cerr << "Loi khi nhap Page Table: " << e.what() << "\n";
                }
                break;
            }
            case 2: {
                cout << "\nNhap kich thuoc trang P (byte, > 0): ";
                int newP;
                if (cin >> newP && newP > 0) {
                    P = newP;
                    cout << "=> Da cap nhat P = " << P << " byte.\n";
                } else {
                    cout << "Loi: P phai la so nguyen duong.\n";
                    cin.clear();
                }
                cin.ignore(10000, '\n');
                break;
            }
            case 3: {
                loadDefaultConfiguration(P, table);
                cout << "\n=> Da nap lai cau hinh mau: P = " << P << " bytes, 4 pages (0->5, 1->3, 2->7, 3 invalid).\n";
                break;
            }
            case 4: {
                cout << "\nKich thuoc trang P = " << P << " byte | PTLR = " << table.size() << "\n";
                printPageTable(table, cout);
                break;
            }
            case 5: {
                cout << "\nNhap Logical Address (LA): ";
                int LA;
                if (cin >> LA) {
                    cout << "\n--- TRACE CHI TIET DICH DIA CHI ---\n";
                    int PA = -1;
                    int status = translate(LA, P, table, PA, cout);
                    if (status == OK) {
                        cout << "KET QUA CUOI CUNG: Logical Address " << LA
                             << " => Physical Address " << PA << "\n";
                    } else if (status == OUT_OF_RANGE) {
                        cout << "KET QUA CUOI CUNG: OUT_OF_RANGE (Page vuot qua kich thuoc Page Table PTLR = "
                             << table.size() << ")\n";
                    } else if (status == PAGE_FAULT) {
                        cout << "KET QUA CUOI CUNG: PAGE_FAULT (Entry co valid bit = 0)\n";
                    } else {
                        cout << "KET QUA CUOI CUNG: INVALID_INPUT (LA am hoac P <= 0)\n";
                    }
                } else {
                    cout << "Loi nhap so.\n";
                    cin.clear();
                }
                cin.ignore(10000, '\n');
                break;
            }
            case 6: {
                cout << "\nNhap cac dia chi logic (LA) cach nhau boi dau cach (vi du: 37 20 5 16 31 64 48):\n> ";
                string line;
                getline(cin, line);
                stringstream lineStream(line);
                int LA;
                cout << "\n" << left << setw(10) << "LA"
                     << setw(16) << "STATUS"
                     << setw(12) << "PA"
                     << "Ghi chu" << "\n";
                cout << string(60, '-') << "\n";
                while (lineStream >> LA) {
                    int PA = -1;
                    ostringstream dummyTrace;
                    int status = translate(LA, P, table, PA, dummyTrace);
                    string statusStr = "INVALID_INPUT";
                    string paStr = "-";
                    string note = "";
                    if (status == OK) {
                        statusStr = "OK";
                        paStr = to_string(PA);
                        note = "Thanh cong";
                    } else if (status == OUT_OF_RANGE) {
                        statusStr = "OUT_OF_RANGE";
                        note = "Page ngoai pham vi PTLR";
                    } else if (status == PAGE_FAULT) {
                        statusStr = "PAGE_FAULT";
                        note = "Page fault (valid = 0)";
                    }
                    cout << left << setw(10) << LA
                         << setw(16) << statusStr
                         << setw(12) << paStr
                         << note << "\n";
                }
                break;
            }
            case 7: {
                runSelfTest(P, table);
                break;
            }
            case 8: {
                string ptPath = "examples/data/page_table_input.txt";
                string laPath = "examples/data/input_la.txt";
                string resultPath = "examples/data/actual_result.txt";
                string tracePath  = "examples/data/trace.txt";
                cout << "\nThuc thi voi cac tep tin:\n";
                cout << "  Input Page Table : " << ptPath << "\n";
                cout << "  Input LA         : " << laPath << "\n";
                cout << "  Output Ket qua   : " << resultPath << "\n";
                cout << "  Output Trace     : " << tracePath << "\n";
                runBatchFiles(ptPath, laPath, resultPath, tracePath);
                break;
            }
            default:
                cout << "Lua chon khong ton tai. Vui long chon tu 0 den 8.\n";
                break;
        }
    }

    return 0;
}
