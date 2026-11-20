# Trace mẫu (P = 16)

Page table: page 0 → frame 5 (valid), page 1 → frame 3 (valid), page 2 → frame 7 (valid), page 3 → frame 0 (invalid). PTLR = 4.
Trace lấy từ output chương trình (`examples/data/trace.txt`).

## Trace 1 – hợp lệ, LA = 37
```
LA = 37, P = 16, PTLR = 4
B1: p = 37 / 16 = 2, d = 37 % 16 = 5
B2: p = 2 < PTLR -> nam trong page table
B3: valid[2] = 1 -> hop le
B4: f = table[2].frame = 7
B5: PA = 7 * 16 + 5 = 117
B6: ket qua PA = 117
Ket qua: OK
```
Kết luận: OK, PA = 117 (khớp tính tay: ⌊37/16⌋ = 2, 37 mod 16 = 5, 7×16+5 = 117).

## Trace 2 – entry không hợp lệ, LA = 48
```
LA = 48, P = 16, PTLR = 4
B1: p = 48 / 16 = 3, d = 48 % 16 = 0
B2: p = 3 < PTLR -> nam trong page table
B3: valid[3] = 0 -> entry khong hop le => page fault
Ket qua: PAGE_FAULT
```
Kết luận: PAGE_FAULT, không có physical address.

## Trace 3 – page ngoài page table, LA = 64
```
LA = 64, P = 16, PTLR = 4
B1: p = 64 / 16 = 4, d = 64 % 16 = 0
B2: p = 4 >= PTLR = 4 -> page ngoai page table => khong hop le
Ket qua: OUT_OF_RANGE
```
Kết luận: OUT_OF_RANGE, không có physical address.
