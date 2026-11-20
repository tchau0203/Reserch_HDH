#include "translate.h"
using namespace std;

int translate(int LA, int P, const PageTable& table, int &PA, ostream &out) {
    int PTLR = table.size();
    out << "LA = " << LA << ", P = " << P << ", PTLR = " << PTLR << endl;

    if (P <= 0 || LA < 0) {
        out << "Dau vao sai (P <= 0 hoac LA < 0)" << endl;
        out << "Ket qua: INVALID_INPUT" << endl << endl;
        return INVALID_INPUT;
    }

    int p = LA / P;
    int d = LA % P;
    out << "B1: p = " << LA << " / " << P << " = " << p
        << ", d = " << LA << " % " << P << " = " << d << endl;

    if (p >= PTLR) {
        out << "B2: p = " << p << " >= PTLR = " << PTLR
            << " -> page ngoai page table => khong hop le" << endl;
        out << "Ket qua: OUT_OF_RANGE" << endl << endl;
        return OUT_OF_RANGE;
    }
    out << "B2: p = " << p << " < PTLR -> nam trong page table" << endl;

    if (!table[p].valid) {
        out << "B3: valid[" << p << "] = 0 -> entry khong hop le => page fault" << endl;
        out << "Ket qua: PAGE_FAULT" << endl << endl;
        return PAGE_FAULT;
    }
    out << "B3: valid[" << p << "] = 1 -> hop le" << endl;

    int f = table[p].frame;
    out << "B4: f = table[" << p << "].frame = " << f << endl;

    PA = f * P + d;
    out << "B5: PA = " << f << " * " << P << " + " << d << " = " << PA << endl;

    out << "B6: ket qua PA = " << PA << endl;
    out << "Ket qua: OK" << endl << endl;
    return OK;
}

