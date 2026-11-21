# Đồ án Hệ điều hành: Mô phỏng Phân trang và Dịch địa chỉ qua Bảng trang (Paging and Page-Table Address Translation)

> **Môn học:** Hệ điều hành  
> **Nhóm thực hiện:** OS_26277105_12 (Nhóm 12)  
> **Repository:** `Reserch_HDH`  
> **Hệ điều hành thử nghiệm:** Windows / Linux / macOS  
> **Ngôn ngữ:** C++ (chuẩn C++17)

---

## 1. Giới thiệu đề tài

Phân trang (Paging) là một trong những cơ chế quản lý bộ nhớ quan trọng nhất của hệ điều hành hiện đại, cho phép không gian địa chỉ vật lý của một tiến trình không cần phải liên tục. Cơ chế này giúp loại bỏ hiện tượng phân mảnh ngoại vi (external fragmentation) và đơn giản hóa việc cấp phát bộ nhớ.

Mục tiêu của dự án là:
1. Xây dựng cấu trúc dữ liệu bảng trang (Page Table) một cấp với các trường thông tin cơ bản: số trang (Page), số khung trang (Frame), và bit hợp lệ (Valid/Invalid bit).
2. Hiện thực giải thuật dịch địa chỉ logic (Logical Address - LA) sang địa chỉ vật lý (Physical Address - PA) theo đúng mô hình chuẩn trong lý thuyết Hệ điều hành (Silberschatz, Chapter 9: Main Memory).
3. Mô phỏng cơ chế bẫy lỗi truy cập bộ nhớ: kiểm tra địa chỉ vượt quá kích thước bảng trang (`OUT_OF_RANGE`) và truy cập trang chưa được nạp/không hợp lệ (`PAGE_FAULT`).
4. Xây dựng bộ công cụ kiểm chứng độc lập và bộ test thực nghiệm đối chiếu kết quả giữa tính toán lý thuyết và chương trình mô phỏng.

---

## 2. Phân công nhiệm vụ nhóm

| Thành viên | Nhiệm vụ đảm nhiệm | Sản phẩm / Đóng góp chính |
| :--- | :--- | :--- |
| **Nguyễn Việt Hoàng** | Thiết kế cấu trúc bảng trang & Nhập liệu | - Khai báo cấu trúc `PageTableEntry`, kiểu `PageTable` (`include/page_table.h`).<br>- Hiện thực hàm nhập bảng trang `loadPageTable` có kiểm tra hợp lệ và hàm in `printPageTable` (`src/page_table.cpp`).<br>- Demo riêng và tài liệu hướng dẫn (`docs/hoang-page-table.md`). |
| **Tuấn** | Giải thuật dịch địa chỉ & Cơ sở lý thuyết | - Hiện thực hàm dịch địa chỉ 6 bước `translate` (`include/address_translation.h`, `src/address_translation.cpp`).<br>- Xây dựng pseudocode, trace chi tiết và phân tích độ phức tạp thuật toán O(1) (`docs/translate_*.md`).<br>- Soạn thảo phần báo cáo cơ sở lý thuyết và giải thuật (`docs/Tuan_phan_bao_cao (1).docx`). |
| **Sơn** | Thiết kế bộ dữ liệu & Kiểm thử độc lập | - Xây dựng bộ 8 test case mẫu đại diện đầy đủ các trường hợp (hợp lệ, biên, lỗi out of range, page fault).<br>- Tính toán giá trị lý thuyết (Expected values) độc lập với mã nguồn mô phỏng. |
| **Châu** | Ráp code, Cấu hình & Viết README | - Sửa đổi và hoàn thiện tương thích header (`include/translate.h` wrapper).<br>- Chuẩn hóa thư mục dữ liệu mẫu (`examples/data/`).<br>- Viết chương trình chính tích hợp `src/main.cpp` (Menu CLI tương tác + chế độ batch file + chế độ tự kiểm thử `--test`).<br>- Xây dựng hệ thống biên dịch: `Makefile`, `CMakeLists.txt`, `build.bat`.<br>- Soạn thảo `README.md` toàn diện cho dự án. |

---

## 3. Cấu trúc thư mục

```text
Reserch_HDH/
│
├── include/                          # Header files
│   ├── page_table.h                  # Khai báo cấu trúc PageTable và các hàm nhập/xuất
│   ├── address_translation.h         # Khai báo hàm translate và các mã trạng thái
│   └── translate.h                   # Header wrapper tương thích ngược
│
├── src/                              # Mã nguồn chính
│   ├── page_table.cpp                # Hiện thực loadPageTable, printPageTable
│   ├── address_translation.cpp       # Hiện thực giải thuật translate 6 bước
│   └── main.cpp                      # Chương trình chính tích hợp (CLI & Batch)
│
├── examples/                         # Ví dụ và kiểm thử
│   ├── data/                         # Thư mục dữ liệu mẫu
│   │   ├── page_table_input.txt      # Bảng trang mẫu (P = 16, 4 pages)
│   │   ├── input_la.txt              # Danh sách 8 địa chỉ logic mẫu
│   │   ├── actual_result.txt         # Kết quả chạy thực tế của chương trình
│   │   └── trace.txt                 # File log trace chi tiết từng bước dịch
│   ├── hoang_page_table_demo.cpp     # Demo kiểm tra riêng phần Page Table của Hoàng
│   ├── run_from_files.cpp            # Demo chạy dịch theo tệp tin
│   └── check_independent.cpp         # Chương trình kiểm chứng độc lập Expected vs Actual
│
├── docs/                             # Tài liệu báo cáo của các thành viên
│   ├── hoang-page-table.md           # Báo cáo phần Hoàng
│   ├── translate_algorithm.md        # Tài liệu giải thuật dịch địa chỉ (Tuấn)
│   ├── translate_complexity.md       # Phân tích độ phức tạp O(1) (Tuấn)
│   ├── translate_trace.md            # Các trace mẫu (Tuấn)
│   ├── translate_run_and_test.md     # Hướng dẫn chạy & bộ test (Tuấn/Sơn)
│   └── Tuan_phan_bao_cao (1).docx    # Báo cáo gốc dạng Word của Tuấn
│
├── CMakeLists.txt                    # Cấu hình biên dịch CMake
├── Makefile                          # Cấu hình biên dịch Make (Linux / MinGW)
├── build.bat                         # Script biên dịch nhanh trên Windows
├── .gitignore                        # Cấu hình bỏ qua tệp sinh ra khi biên dịch
└── README.md                         # Hướng dẫn dự án
```

---

## 4. Cơ sở lý thuyết và Giải thuật dịch địa chỉ

### 4.1. Ký hiệu và quy ước
- **$LA$ (Logical Address):** Địa chỉ logic do CPU phát ra (đơn vị: byte).
- **$P$ (Page Size):** Kích thước của một trang và một khung trang (đơn vị: byte, là lũy thừa của 2).
- **$p$ (Page Number):** Số hiệu trang, đóng vai trò là chỉ số tra cứu vào Page Table.
- **$d$ (Offset):** Độ lệch vị trí trong trang/khung trang.
- **$f$ (Frame Number):** Số hiệu khung trang vật lý tương ứng.
- **$PA$ (Physical Address):** Địa chỉ vật lý trong bộ nhớ chính RAM.
- **$PTLR$ (Page-Table Length Register):** Số lượng phần tử hiện có trong bảng trang (`table.size()`).

### 4.2. Công thức toán học (Silberschatz Slide 24)
$$\begin{aligned}
p &= \lfloor LA / P \rfloor \\
d &= LA \pmod P \\
PA &= f \times P + d
\end{aligned}$$

### 4.3. Các bước thực hiện của giải thuật (`translate`)
Hàm `translate` thực hiện dịch địa chỉ qua 6 bước tuần tự:

```text
+-------------------------------------------------------------+
| Bước 0 (Kiểm tra bảo vệ): Nếu LA < 0 hoặc P <= 0            |
|       => Trả về INVALID_INPUT                               |
+-------------------------------------------------------------+
                              |
                              v
+-------------------------------------------------------------+
| Bước 1 (Tách địa chỉ):                                      |
|       p = LA / P,  d = LA % P                               |
+-------------------------------------------------------------+
                              |
                              v
+-------------------------------------------------------------+
| Bước 2 (Kiểm tra phạm vi):                                  |
|       Nếu p >= PTLR => Trả về OUT_OF_RANGE                  |
+-------------------------------------------------------------+
                              |
                              v
+-------------------------------------------------------------+
| Bước 3 (Kiểm tra tính hợp lệ):                              |
|       Nếu table[p].valid == false => Trả về PAGE_FAULT      |
+-------------------------------------------------------------+
                              |
                              v
+-------------------------------------------------------------+
| Bước 4 (Lấy số hiệu khung trang):                           |
|       f = table[p].frame                                    |
+-------------------------------------------------------------+
                              |
                              v
+-------------------------------------------------------------+
| Bước 5 (Tính địa chỉ vật lý):                               |
|       PA = f * P + d                                        |
+-------------------------------------------------------------+
                              |
                              v
+-------------------------------------------------------------+
| Bước 6 (Trả kết quả thành công):                            |
|       Trả về mã OK và giá trị PA                            |
+-------------------------------------------------------------+
```

### 4.4. Đánh giá độ phức tạp thuật toán
- **Thời gian xử lý một địa chỉ (Time Complexity):** $\mathcal{O}(1)$  
  Chỉ gồm các phép toán số học cơ bản (chia, chia lấy dư, nhân, cộng) và truy xuất mảng trực tiếp bằng chỉ số $p$. Không chứa vòng lặp hay tìm kiếm.
- **Bộ nhớ phụ trợ (Auxiliary Space):** $\mathcal{O}(1)$  
  Chỉ sử dụng một số biến nguyên cục bộ ($p, d, f, PA$).
- **Không gian lưu trữ bảng trang:** $\mathcal{O}(N)$ với $N = PTLR$ là số lượng trang của tiến trình.

---

## 5. Hướng dẫn biên dịch và Cài đặt

### Yêu cầu môi trường
- Trình biên dịch C++ hỗ trợ **C++17** trở lên (`g++` phiên bản $\ge 8.0$, `clang++`, hoặc MSVC 2019/2022).

### 5.1. Biên dịch nhanh bằng script Windows (`build.bat`)
Mở Command Prompt hoặc PowerShell tại thư mục `Reserch_HDH`:
```bat
.\build.bat
```

### 5.2. Biên dịch bằng `g++` trực tiếp
```bash
# Biên dịch chương trình chính tích hợp
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.cpp src/page_table.cpp src/address_translation.cpp -o main.exe

# Biên dịch công cụ chạy theo tệp
g++ -std=c++17 -Wall -Wextra -Iinclude examples/run_from_files.cpp src/page_table.cpp src/address_translation.cpp -o run.exe

# Biên dịch chương trình kiểm chứng độc lập
g++ -std=c++17 -Wall -Wextra examples/check_independent.cpp -o check.exe
```

### 5.3. Biên dịch bằng `Make`
```bash
make        # Biên dịch tất cả mục tiêu
make test   # Biên dịch và chạy bộ kiểm thử tự động
make clean  # Xóa các file thực thi đã sinh ra
```

### 5.4. Biên dịch bằng `CMake`
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

---

## 6. Hướng dẫn sử dụng chương trình

Chương trình chính `main.exe` được thiết kế linh hoạt với 3 chế độ hoạt động:

### 6.1. Chế độ 1: Menu tương tác trực quan (Interactive CLI Mode)
Khởi động bằng lệnh:
```bash
.\main.exe
```
Giao diện hiển thị menu chức năng đầy đủ:
```text
===============================================================
      CHUONG TRINH MO PHONG DICH DIA CHI QUA BANG TRANG        
          (Paging and Page-Table Address Translation)          
                   Mon: He Dieu Hanh - Nhom 12                 
===============================================================
[He thong] Da khoi tao cau hinh mau: P = 16 byte, 4 trang.

----------------------- MENU CHINH ---------------------------
 1. Nhap Page Table moi tu ban phim
 2. Cai dat kich thuoc trang P (hien tai: P = 16 bytes)
 3. Nap lai cau hinh mau mac dinh (P = 16, 4 pages)
 4. Hien thi Page Table hien tai
 5. Dich mot dia chi logic (in trace chi tiet 6 buoc)
 6. Dich nhieu dia chi logic (nhap danh sach LA)
 7. Chay bo 8 test case kiem thu tu dong (Expected vs Actual)
 8. Chay mo phong doc tu tep tin (examples/data/)
 0. Thoat chuong trinh
--------------------------------------------------------------
Chon chuc nang (0-8):
```

### 6.2. Chế độ 2: Tự động kiểm thử nhanh (`--test`)
Dùng để kiểm thử nhanh toàn bộ 8 test case chuẩn:
```bash
.\main.exe --test
```

### 6.3. Chế độ 3: Chạy xử lý hàng loạt theo tệp tin (Batch Mode)
Cú pháp:
```bash
.\main.exe <file_page_table> <file_input_la> [file_output_result] [file_output_trace]
```
Ví dụ:
```bash
.\main.exe examples/data/page_table_input.txt examples/data/input_la.txt examples/data/actual_result.txt examples/data/trace.txt
```

---

## 7. Bảng kết quả kiểm thử thực nghiệm

Cấu hình thử nghiệm chuẩn của nhóm:
- Kích thước trang: $P = 16$ bytes.
- Bảng trang ($PTLR = 4$):
  - Page 0 $\rightarrow$ Frame 5, Valid = 1
  - Page 1 $\rightarrow$ Frame 3, Valid = 1
  - Page 2 $\rightarrow$ Frame 7, Valid = 1
  - Page 3 $\rightarrow$ Frame 0, Valid = 0 (Trang không hợp lệ)

| STT | Logical Address ($LA$) | Phân tích lý thuyết (Expected) | Trạng thái thực tế (Actual) | Địa chỉ vật lý ($PA$) | Kết luận |
| :---: | :---: | :--- | :---: | :---: | :---: |
| 1 | **37** | $p = 2, d = 5$, Frame 7 $\Rightarrow 7 \times 16 + 5 = 117$ | `OK` | **117** | **PASS** |
| 2 | **20** | $p = 1, d = 4$, Frame 3 $\Rightarrow 3 \times 16 + 4 = 52$ | `OK` | **52** | **PASS** |
| 3 | **5** | $p = 0, d = 5$, Frame 5 $\Rightarrow 5 \times 16 + 5 = 85$ | `OK` | **85** | **PASS** |
| 4 | **16** | Biên dưới: $p = 1, d = 0 \Rightarrow 3 \times 16 + 0 = 48$ | `OK` | **48** | **PASS** |
| 5 | **31** | Biên trên: $p = 1, d = 15 \Rightarrow 3 \times 16 + 15 = 63$ | `OK` | **63** | **PASS** |
| 6 | **64** | $p = 4 \ge PTLR = 4 \Rightarrow$ Ngoài bảng | `OUT_OF_RANGE` | `-` | **PASS** |
| 7 | **48** | $p = 3$, `valid = 0` $\Rightarrow$ Trang chưa nạp | `PAGE_FAULT` | `-` | **PASS** |
| 8 | **63** | $p = 3$, địa chỉ cuối bảng nhưng `valid = 0` | `PAGE_FAULT` | `-` | **PASS** |

**Tỷ lệ đạt:** **8/8 test cases PASS (100%)**.


