# Đồ án Hệ điều hành: Mô phỏng Phân trang và Dịch địa chỉ qua Bảng trang (Paging and Page-Table Address Translation)

> **Môn học:** Hệ điều hành  
> **Nhóm thực hiện:** OS_26277105_12 (Nhóm 12)  
> **Hạn nộp:** 23/11/2026  
> **Ngôn ngữ:** C++ (chuẩn C++17)

---

## 1. Giới thiệu đề tài

Phân trang (Paging) là một trong những cơ chế quản lý bộ nhớ quan trọng nhất của hệ điều hành hiện đại, cho phép không gian địa chỉ vật lý của một tiến trình không cần phải liên tục trong RAM. Cơ chế này giúp loại bỏ hiện tượng phân mảnh ngoại vi (external fragmentation) và đơn giản hóa việc cấp phát bộ nhớ.

Mục tiêu của đề tài:

1. Xây dựng cấu trúc dữ liệu bảng trang (Page Table) một cấp với các trường thông tin cơ bản: số trang (Page), số khung trang (Frame), và bit hợp lệ (Valid/Invalid bit).
2. Hiện thực giải thuật dịch địa chỉ logic (Logical Address - LA) sang địa chỉ vật lý (Physical Address - PA) theo đúng mô hình chuẩn trong lý thuyết Hệ điều hành (Silberschatz, Chapter 9: Main Memory).
3. Mô phỏng cơ chế bẫy lỗi truy cập bộ nhớ: kiểm tra địa chỉ vượt quá kích thước bảng trang (`OUT_OF_RANGE`) và truy cập trang chưa được nạp/không hợp lệ (`PAGE_FAULT`).
4. Ráp nối các module thành một chương trình chạy được hoàn chỉnh và kiểm chứng độc lập.

---

## 2. Cơ sở lý thuyết và Giải thuật dịch địa chỉ

### 2.1. Ký hiệu và quy ước

- **$LA$ (Logical Address):** Địa chỉ logic do CPU phát ra (đơn vị: byte).
- **$P$ (Page Size):** Kích thước của một trang và một khung trang (đơn vị: byte, là lũy thừa của 2).
- **$p$ (Page Number):** Số hiệu trang, dùng làm chỉ số tra cứu vào Page Table.
- **$d$ (Offset):** Độ lệch vị trí bên trong trang/khung trang.
- **$f$ (Frame Number):** Số hiệu khung trang vật lý tương ứng trong RAM.
- **$PA$ (Physical Address):** Địa chỉ vật lý sau khi dịch ($PA = f \times P + d$).
- **$PTLR$ (Page-Table Length Register):** Số lượng phần tử trong bảng trang (`table.size()`).

### 2.2. Các bước thực hiện của giải thuật (`translate`)

Hàm `translate` thực hiện dịch địa chỉ qua các bước:

1. **Kiểm tra đầu vào:** Nếu $LA < 0$ hoặc $P \le 0 \Rightarrow$ Báo lỗi `INVALID_INPUT`.
2. **Tách địa chỉ logic:**
   $$p = \lfloor LA / P \rfloor, \quad d = LA \pmod P$$
3. **Kiểm tra phạm vi:** Nếu $p \ge PTLR \Rightarrow$ Báo lỗi `OUT_OF_RANGE`.
4. **Kiểm tra tính hợp lệ:** Nếu `table[p].valid == false` $\Rightarrow$ Báo lỗi `PAGE_FAULT`.
5. **Lấy khung trang vật lý:** $f = \text{table}[p].\text{frame}$.
6. **Tính địa chỉ vật lý:** $PA = f \times P + d$. Trả về trạng thái `OK` và kết quả $PA$.

---

## 3. Hướng dẫn biên dịch và Chạy chương trình

### Yêu cầu môi trường

- Trình biên dịch C++ hỗ trợ **C++17** trở lên (`g++`, `clang++`, hoặc MSVC).

### 3.1. Biên dịch nhanh bằng `build.bat` (Windows)

Mở Command Prompt hoặc PowerShell tại thư mục `Reserch_HDH`:

```bat
.\build.bat
```

### 3.2. Biên dịch bằng `g++` trực tiếp

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.cpp src/page_table.cpp src/address_translation.cpp -o main.exe
```

### 3.3. Biên dịch bằng `Make`

```bash
make        # Biên dịch main.exe và hoang_demo.exe
make clean  # Xóa các file thực thi
```

---

## 4. Hướng dẫn sử dụng chương trình chính (`main.cpp`)

Khởi động chương trình:

```bash
.\main.exe
```

Giao diện Menu hiển thị:

```text
===============================================================
      CHUONG TRINH MO PHONG DICH DIA CHI QUA BANG TRANG
          (Paging and Page-Table Address Translation)
                   Mon: He Dieu Hanh - Nhom 12
===============================================================

----------------------- MENU CHINH ---------------------------
 1. Cai dat kich thuoc trang P (hien tai: P = 16 bytes)
 2. Nhap Page Table moi tu ban phim (goi loadPageTable)
 3. Hien thi Page Table hien tai (goi printPageTable)
 4. Dich mot dia chi logic LA (goi translate, in trace 6 buoc)
 5. Dich danh sach nhieu dia chi logic LA
 0. Thoat chuong trinh
--------------------------------------------------------------
Chon chuc nang (0-5):
```

### Các chức năng chính:

- **Chức năng 1:** Cài đặt kích thước trang $P$ (ví dụ: 16 bytes, 4096 bytes,...).
- **Chức năng 2:** Gọi hàm `loadPageTable` để nhập số lượng trang và từng entry (page, frame, valid bit).
- **Chức năng 3:** Gọi hàm `printPageTable` để xuất bảng trang hiện tại ra màn hình.
- **Chức năng 4:** Nhập 1 địa chỉ logic $LA$, gọi hàm `translate` để in toàn bộ trace các bước $B_1 \rightarrow B_6$ và kết quả $PA$ (hoặc báo lỗi tương ứng).
- **Chức năng 5:** Dịch danh sách nhiều địa chỉ logic cùng lúc và hiển thị bảng kết quả tổng hợp.

---

## 5. Phần kiểm thử (Testing)

Theo phân công trong nhóm, **Sơn** phụ trách xây dựng bộ 8 test case kiểm chứng, tính tay đối chiếu lý thuyết và ghi nhận kết luận pass/fail
