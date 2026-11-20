#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <iostream>
#include <vector>

struct PageTableEntry {
    int page = 0;
    int frame = 0;
    bool valid = false;
};

// Pages are numbered from 0 to table.size() - 1; table[p] describes page p.
using PageTable = std::vector<PageTableEntry>;

// Read count, then page/frame/valid for each entry. Invalid input is retried.
// End of input throws std::runtime_error; no partially filled table is returned.
PageTable loadPageTable(std::istream& input = std::cin,
                        std::ostream& output = std::cout);

void printPageTable(const PageTable& table, std::ostream& output = std::cout);

#endif
