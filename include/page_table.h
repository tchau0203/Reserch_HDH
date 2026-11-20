#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <iostream>
#include <vector>

struct PageTableEntry {
    int page = 0;
    int frame = 0;
    bool valid = false;
};

// Page danh so tu 0. table[p] luu thong tin cua page p.
using PageTable = std::vector<PageTableEntry>;

// Nhap so page, sau do nhap page, frame va valid cua tung dong.
// Neu het dau vao giua chung, ham bao loi bang std::runtime_error.
PageTable loadPageTable(std::istream& input = std::cin,
                        std::ostream& output = std::cout);

void printPageTable(const PageTable& table, std::ostream& output = std::cout);

#endif
