#include "page_table.h"

#include <cstddef>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

int readInteger(std::istream& input, std::ostream& output,
                const char* prompt, int minimum, int maximum) {
    for (;;) {
        output << prompt;
        std::string line;
        if (!std::getline(input, line)) {
            throw std::runtime_error("Ket thuc dau vao khi dang nhap page table.");
        }

        std::istringstream parser(line);
        int value = 0;
        if ((parser >> value) && (parser >> std::ws).eof() &&
            value >= minimum && value <= maximum) {
            return value;
        }
        output << "Loi: nhap mot so nguyen tu " << minimum << " den "
               << maximum << ". Hay nhap lai.\n";
    }
}

} // namespace

PageTable loadPageTable(std::istream& input, std::ostream& output) {
    // Also work after a menu has read its choice using operator>>.
    input >> std::ws;
    const int count = readInteger(input, output, "Nhap so page: ", 0,
                                 std::numeric_limits<int>::max());
    PageTable table(static_cast<std::size_t>(count));
    std::vector<bool> entered(static_cast<std::size_t>(count), false);

    for (int index = 0; index < count; ++index) {
        output << "Entry " << index + 1 << "/" << count << ":\n";
        int page = 0;
        for (;;) {
            page = readInteger(input, output, "  Page: ", 0, count - 1);
            if (!entered[static_cast<std::size_t>(page)]) {
                break;
            }
            output << "Loi: page " << page << " da duoc nhap. Hay nhap page khac.\n";
        }

        PageTableEntry entry;
        entry.page = page;
        entry.frame = readInteger(input, output, "  Frame: ", 0,
                                  std::numeric_limits<int>::max());
        entry.valid = readInteger(input, output, "  Valid (0/1): ", 0, 1) == 1;
        table[static_cast<std::size_t>(page)] = entry;
        entered[static_cast<std::size_t>(page)] = true;
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
