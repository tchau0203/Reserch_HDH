#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "address_translation.h"
#include "page_table.h"

using namespace std;

void printBanner() {
    cout << "===============================================================\n";
    cout << "      CHUONG TRINH MO PHONG DICH DIA CHI QUA BANG TRANG        \n";
    cout << "          (Paging and Page-Table Address Translation)          \n";
    cout << "                   Mon: He Dieu Hanh - Nhom 12                 \n";
    cout << "===============================================================\n";
}

int main() {
    printBanner();

    int P = 16; // Kích thước trang mặc định (byte)
    PageTable table;
    bool hasTable = false;

    cout << "\nChuong trinh rap noi module Page Table (Hoang) va Address Translation (Tuan).\n";

    while (true) {
        cout << "\n----------------------- MENU CHINH ---------------------------\n";
        cout << " 1. Cai dat kich thuoc trang P (hien tai: P = " << P << " bytes)\n";
        cout << " 2. Nhap Page Table moi tu ban phim (goi loadPageTable)\n";
        cout << " 3. Hien thi Page Table hien tai (goi printPageTable)\n";
        cout << " 4. Dich mot dia chi logic LA (goi translate, in trace 6 buoc)\n";
        cout << " 5. Dich danh sach nhieu dia chi logic LA\n";
        cout << " 0. Thoat chuong trinh\n";
        cout << "--------------------------------------------------------------\n";
        cout << "Chon chuc nang (0-5): ";

        string line;
        if (!getline(cin, line)) break;
        stringstream ss(line);
        int choice = -1;
        if (!(ss >> choice)) {
            cout << "Lua chon khong hop le. Vui long chon so tu 0 den 5.\n";
            continue;
        }

        if (choice == 0) {
            cout << "\nKet thuc chuong trinh. Tam biet!\n";
            break;
        }

        switch (choice) {
            case 1: {
                cout << "\nNhap kich thuoc trang P (byte, phai > 0, nen la luy thua cua 2): ";
                int newP;
                if (cin >> newP && newP > 0) {
                    P = newP;
                    cout << "=> Da cap nhat kich thuoc trang P = " << P << " bytes.\n";
                } else {
                    cout << "Loi: Kich thuoc trang P phai la so nguyen duong.\n";
                    cin.clear();
                }
                cin.ignore(10000, '\n');
                break;
            }
            case 2: {
                cout << "\n--- NHAP BANG TRANG (PAGE TABLE) ---\n";
                try {
                    table = loadPageTable(cin, cout);
                    hasTable = true;
                    cout << "\n=> Da nhap thanh cong Page Table voi " << table.size() << " trang!\n";
                } catch (const exception& e) {
                    cerr << "Loi khi nhap bang trang: " << e.what() << "\n";
                }
                break;
            }
            case 3: {
                if (!hasTable || table.empty()) {
                    cout << "\n[Canh bao] Bang trang hien dang trong. Hay nhap bang trang truoc (chuc nang 2).\n";
                } else {
                    cout << "\nKich thuoc trang P = " << P << " bytes | PTLR = " << table.size() << "\n";
                    printPageTable(table, cout);
                }
                break;
            }
            case 4: {
                if (!hasTable || table.empty()) {
                    cout << "\n[Canh bao] Bang trang chua duoc khoi tao. Hay chon chuc nang 2 de nhap bang trang truoc.\n";
                    break;
                }
                cout << "\nNhap dia chi logic (Logical Address - LA): ";
                int LA;
                if (cin >> LA) {
                    cout << "\n--- TRACE CHI TIET CAC BUOC DICH DIA CHI ---\n";
                    int PA = -1;
                    int status = translate(LA, P, table, PA, cout);
                    if (status == OK) {
                        cout << "=> KET QUA: LA = " << LA << " -> PA = " << PA << " (OK)\n";
                    } else if (status == OUT_OF_RANGE) {
                        cout << "=> KET QUA: OUT_OF_RANGE (Page vuot qua PTLR = " << table.size() << ")\n";
                    } else if (status == PAGE_FAULT) {
                        cout << "=> KET QUA: PAGE_FAULT (Entry co bit hop le valid = 0)\n";
                    } else {
                        cout << "=> KET QUA: INVALID_INPUT (LA < 0 hoac P <= 0)\n";
                    }
                } else {
                    cout << "Loi nhap lieu: Can nhap mot so nguyen.\n";
                    cin.clear();
                }
                cin.ignore(10000, '\n');
                break;
            }
            case 5: {
                if (!hasTable || table.empty()) {
                    cout << "\n[Canh bao] Bang trang chua duoc khoi tao. Hay chon chuc nang 2 de nhap bang trang truoc.\n";
                    break;
                }
                cout << "\nNhap cac dia chi logic (LA) cach nhau boi dau cach (vi du: 37 20 5 16 31 64 48):\n> ";
                string inputLALine;
                getline(cin, inputLALine);
                stringstream streamLA(inputLALine);
                int LA;
                cout << "\nBang ket qua dich danh sach dia chi:\n";
                cout << "--------------------------------------------------------\n";
                cout << "     LA | Trạng thái    |     PA | Ghi chu\n";
                cout << "--------+---------------+--------+----------------------\n";
                while (streamLA >> LA) {
                    int PA = -1;
                    ostringstream dummyTrace;
                    int status = translate(LA, P, table, PA, dummyTrace);
                    string statusStr = "INVALID_INPUT";
                    string paStr = "-";
                    string note = "";
                    if (status == OK) {
                        statusStr = "OK";
                        paStr = to_string(PA);
                        note = "Hop le";
                    } else if (status == OUT_OF_RANGE) {
                        statusStr = "OUT_OF_RANGE";
                        note = "Page ngoai pham vi PTLR";
                    } else if (status == PAGE_FAULT) {
                        statusStr = "PAGE_FAULT";
                        note = "Trang chua nap vao RAM";
                    }
                    cout << " " << LA << "\t| " << statusStr << "\t| " << paStr << "\t| " << note << "\n";
                }
                cout << "--------------------------------------------------------\n";
                break;
            }
            default:
                cout << "Lua chon khong ton tai. Vui long chon tu 0 den 5.\n";
                break;
        }
    }

    return 0;
}
