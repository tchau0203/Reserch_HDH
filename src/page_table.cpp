#include "page_table.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

// Doc ca dong de tranh nhan nham du lieu nhu "2abc" hoac "1 2".
static int readNonNegativeInteger(std::istream& input, std::ostream& output,
                                  const char* prompt) {
    while (true) {
        output << prompt;
        std::string line;
        if (!std::getline(input, line)) {
            throw std::runtime_error("Ket thuc dau vao khi dang nhap page table.");
        }

        std::istringstream parser(line);
        int value = 0;
        if ((parser >> value) && value >= 0 && (parser >> std::ws).eof()) {
            return value;
        }
        output << "Loi: can mot so nguyen khong am hop le. Hay nhap lai.\n";
    }
}

PageTable loadPageTable(std::istream& input, std::ostream& output) {
    // Bo dau xuong dong con lai neu menu vua nhap bang cin >>.
    input >> std::ws;
    int count = readNonNegativeInteger(input, output, "Nhap so page: ");
    PageTable table(count);
    std::vector<bool> entered(count, false);

    for (int index = 0; index < count; ++index) {
        output << "Entry " << index + 1 << "/" << count << ":\n";
        int page = 0;
        while (true) {
            page = readNonNegativeInteger(input, output, "  Page: ");
            if (page >= count) {
                output << "Loi: page phai tu 0 den " << count - 1 << ".\n";
                continue;
            }
            if (entered[page]) {
                output << "Loi: page " << page << " da duoc nhap. Hay nhap page khac.\n";
                continue;
            }
            break;
        }

        int frame = readNonNegativeInteger(input, output, "  Frame: ");
        int valid = readNonNegativeInteger(input, output, "  Valid (0/1): ");
        while (valid > 1) {
            output << "Loi: valid chi nhan 0 hoac 1.\n";
            valid = readNonNegativeInteger(input, output, "  Valid (0/1): ");
        }

        // Chi luu khi da nhap du ba gia tri hop le.
        PageTableEntry entry;
        entry.page = page;
        entry.frame = frame;
        entry.valid = (valid == 1);
        table[page] = entry;
        entered[page] = true;
    }
    return table;
}

void printPageTable(const PageTable& table, std::ostream& output) {
    output << "\n=== PAGE TABLE ===\n";
    if (table.empty()) {
        output << "Bang trang rong.\n";
        return;
    }
    output << "       Page |       Frame | Valid\n"
              "------------+-------------+------\n";
    for (const PageTableEntry& entry : table) {
        output << std::setw(11) << entry.page << " | "
               << std::setw(11) << entry.frame << " | "
               << (entry.valid ? 1 : 0) << '\n';
    }
}
