# Cách chạy mô phỏng và kiểm chứng

## Biên dịch và chạy (từ thư mục gốc repo)
```
g++ -std=c++17 -Wall -Wextra -Iinclude -o run examples/run_from_files.cpp src/translate.cpp src/page_table.cpp
./run examples/data/page_table_input.txt examples/data/input_la.txt examples/data/actual_result.txt examples/data/trace.txt
g++ -std=c++17 -Wall -Wextra -o check examples/check_independent.cpp
./check
```

## File input / output (thư mục `examples/data/`)
| File | Nội dung |
|---|---|
| `page_table_input.txt` | Dòng 1: P. Dòng 2: số page. Sau đó mỗi entry 3 dòng: page, frame, valid (đúng thứ tự `loadPageTable` của Hoàng đọc). |
| `input_la.txt` | Mỗi dòng một logical address. |
| `actual_result.txt` | Kết quả chương trình C++: `LA status PA`. |
| `trace.txt` | Page table và trace 6 bước cho từng LA. |
| `expected_vs_actual.txt` | Bảng expected / actual / pass-fail do chương trình `check_independent.cpp` tính độc lập. |

## 8 test mẫu (P = 16; page 0→5, 1→3, 2→7, 3 invalid)
| # | LA | Loại test | Tính tay (expected) | Actual | Kết luận |
|---|---|---|---|---|---|
| 1 | 37 | hợp lệ, page 2 | p=2, d=5 → 7×16+5 = 117 | 117 | PASS |
| 2 | 20 | hợp lệ, page 1 | p=1, d=4 → 3×16+4 = 52 | 52 | PASS |
| 3 | 5 | hợp lệ, page 0 | p=0, d=5 → 5×16+5 = 85 | 85 | PASS |
| 4 | 16 | offset = 0 | p=1, d=0 → 3×16+0 = 48 | 48 | PASS |
| 5 | 31 | offset = P−1 | p=1, d=15 → 3×16+15 = 63 | 63 | PASS |
| 6 | 64 | page ngoài bảng | p=4 ≥ PTLR=4 | OUT_OF_RANGE | PASS |
| 7 | 48 | entry invalid | p=3, valid=0 | PAGE_FAULT | PASS |
| 8 | 63 | biên tự chọn: địa chỉ cuối trong phạm vi bảng (p = PTLR−1) | p=3, valid=0 | PAGE_FAULT | PASS |

Đây là bộ mẫu để kiểm tra code của Tuấn. Bộ test chính thức do Sơn làm và tính tay độc lập; chương trình `check_independent.cpp` chỉ là cách tính thứ hai bổ sung: nó không dùng `translate` hay `PageTable`, mà duyệt từng page, xem LA nằm trong khoảng [i×P, i×P+P) của page nào rồi tính offset = LA − i×P, khác với cách chia và lấy dư của chương trình chính.
