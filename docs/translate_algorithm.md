# Giải thuật dịch địa chỉ (page table một cấp)

Người làm: Tuấn. Code: `include/translate.h`, `src/translate.cpp` (dùng `PageTable` của Hoàng).
Nguồn: slide Chapter 9 – Main Memory: slide 23 (Paging), 24 (Address Translation Scheme), 27 (Paging Example), 30 (PTLR), 35–36 (valid-invalid bit).

## Ký hiệu, đơn vị và giả định
- LA: logical address, P: page size, đơn vị **byte**. P là lũy thừa của 2 (4, 8, 16, 1024, ...) và cố định trong một bộ test.
- p: page number, d: page offset, f: frame number, PA: physical address.
- Page và frame **đánh số từ 0**. `table[p]` là entry của page p (frame + valid bit); các page 0 … `table.size()`−1 đều có entry (đúng với `loadPageTable` của Hoàng).
- PTLR = `table.size()` = số entry của page table (slide 30).

## Công thức (slide 24)
```
p  = LA / P      (chia nguyên)
d  = LA % P
PA = f * P + d
```
Ví dụ đề bài: P = 16, LA = 37 → p = 2, d = 5; page 2 → frame 7 ⇒ PA = 7 × 16 + 5 = **117**.

Đối chiếu với slide 27 (n = 2, m = 4, P = 4; page 0→5, 1→6, 2→1, 3→2): LA 0 → 20, LA 3 → 23, LA 4 → 24, LA 13 → 9. Chương trình cho đúng các kết quả này, khớp hình physical memory trong slide.

## Khi nào địa chỉ bị xem là không hợp lệ?
1. `p >= PTLR`: page number nằm ngoài page table → `OUT_OF_RANGE` (slide 30, 35).
2. Valid bit của entry p bằng 0 → `PAGE_FAULT` (slide 35–36).
3. (Đầu vào sai, ngoài phạm vi đề) LA < 0 hoặc P ≤ 0 → `INVALID_INPUT`, chỉ để chương trình không chia cho 0 hoặc dịch địa chỉ âm.

## Pseudocode
```
FUNCTION Translate(LA, P, PT):
    IF P <= 0 OR LA < 0 THEN RETURN INVALID_INPUT
    p ← LA / P ;  d ← LA MOD P
    IF p >= PTLR THEN
        RETURN OUT_OF_RANGE
    entry ← PT[p]
    IF entry.valid ≠ 1 THEN
        RETURN PAGE_FAULT
    f ← entry.frame
    PA ← f * P + d
    RETURN OK, PA
```

## Giao diện hàm
```cpp
int translate(int LA, int P, const PageTable& table, int &PA, std::ostream &out);
```
Trả về `OK`, `OUT_OF_RANGE`, `PAGE_FAULT` hoặc `INVALID_INPUT`. Trace từng bước được ghi vào `out`.
