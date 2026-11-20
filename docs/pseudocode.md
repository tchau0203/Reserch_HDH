# Pseudocode dịch địa chỉ (page table một cấp)

Ký hiệu theo slide Chapter 9: p = page number, d = offset, f = frame number, P = page size, PTLR = số entry của page table.

```
FUNCTION Translate(LA, P, PT):
    IF P <= 0 OR LA < 0 THEN RETURN INVALID_INPUT
    p ← LA / P
    d ← LA MOD P
    IF p >= PTLR THEN
        RETURN OUT_OF_RANGE
    entry ← PT[p]
    IF entry.valid ≠ 1 THEN
        RETURN PAGE_FAULT
    f ← entry.frame
    PA ← f * P + d
    RETURN OK, PA
```

## Đối chiếu với 6 bước của đề

| Bước | Dòng pseudocode |
|---|---|
| 1. Tách page number và offset | `p ← LA / P`, `d ← LA MOD P` |
| 2. Kiểm tra page có trong page table | `IF p >= PTLR THEN RETURN OUT_OF_RANGE` |
| 3. Kiểm tra valid/invalid bit | `IF entry.valid ≠ 1 THEN RETURN PAGE_FAULT` |
| 4. Lấy frame number | `f ← entry.frame` |
| 5. Tính physical address | `PA ← f * P + d` |
| 6. Trả kết quả hoặc báo invalid/page fault | `RETURN OK, PA` hoặc các lệnh `RETURN` mã lỗi ở trên |

Dòng đầu (`INVALID_INPUT`) chỉ là kiểm tra bảo vệ để không chia cho 0 hay dịch địa chỉ âm, không thuộc 6 bước của đề.
