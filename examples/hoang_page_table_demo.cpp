#include "page_table.h"

#include <exception>
#include <iostream>

int main() {
    try {
        const PageTable table = loadPageTable();
        printPageTable(table);
    } catch (const std::exception& error) {
        std::cerr << "Loi: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
