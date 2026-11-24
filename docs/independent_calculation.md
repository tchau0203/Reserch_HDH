# Tài Liệu Kiểm Chứng & Tính Toán Độc Lập (QA/Testing)

**Dự án:** Mô phỏng Cơ chế Phân trang & Dịch địa chỉ Bảng trang (Paging & Page-Table Address Translation)  
**Vai trò:** Người 5 — QA & Kiểm thử (Verification & Testing)  
**Mục đích:** Tính toán thủ công độc lập toàn bộ 8 test cases dựa trên mô hình toán học phân trang để làm cơ sở đối chiếu (`Expected Result`) với kết quả thực thi của chương trình (`Actual Result`).

---

## 1. Cấu Hình Hệ Thống & Công Thức Áp Dụng

### 1.1. Tham số bộ nhớ giả lập
* **Không gian địa chỉ logic:** 16-bit
* **Kích thước trang ($P$):** $16 \text{ bytes}$
* **Thanh ghi giới hạn bảng trang ($\text{PTLR}$):** $4$ (Quản lý các trang hợp lệ có chỉ số $p \in \{0, 1, 2, 3\}$)

### 1.2. Bảng trang cấu hình (`page_table_input.txt`)

| Page ($p$) | Frame ($f$) | Valid Bit | Trạng thái ánh xạ |
| :---: | :---: | :---: | :--- |
| **0** | 5 | `1` | Hợp lệ (Nạp tại Frame 5) |
| **1** | 3 | `1` | Hợp lệ (Nạp tại Frame 3) |
| **2** | 7 | `1` | Hợp lệ (Nạp tại Frame 7) |
| **3** | -1 | `0` | Không hợp lệ / Chưa nạp vào RAM (`PAGE_FAULT`) |

### 1.3. Quy trình và công thức tính toán độc lập
Với mỗi địa chỉ logic đầu vào $\text{LA}$ trong danh sách `input_la.txt`:
1. **Tách số hiệu trang ($p$) và độ lệch ($d$):**
   $$p = \lfloor \text{LA} / P \rfloor$$
   $$d = \text{LA} \bmod P$$
2. **Kiểm tra giới hạn bảng trang ($\text{PTLR}$):**
   * Nếu $p < 0$ hoặc $p \ge \text{PTLR}$ ($p \ge 4$) $\rightarrow$ Kết luận: **`OUT_OF_RANGE`**.
3. **Kiểm tra bit hợp lệ (`valid`):**
   * Nếu $\text{PageTable}[p].\text{valid} == 0$ $\rightarrow$ Kết luận: **`PAGE_FAULT`**.
4. **Tính địa chỉ vật lý ($\text{PA}$):**
   * Lấy số hiệu khung trang $f = \text{PageTable}[p].\text{frame}$.
   * Áp dụng công thức:
     $$\text{PA} = f \times P + d$$

---

## 2. Chi Tiết Tính Tay Từng Test Case

### Test Case 1: $\text{LA} = 37$ (Truy cập hợp lệ - Trang 2)
* **Bước 1:** $p = \lfloor 37 / 16 \rfloor = 2$; $d = 37 \bmod 16 = 5$
* **Bước 2:** $p = 2 < 4$ (Thỏa mãn điều kiện $\text{PTLR}$)
* **Bước 3:** Tra bảng trang tại $p = 2 \rightarrow f = 7$, $\text{valid} = 1$ (Hợp lệ)
* **Bước 4:** $\text{PA} = 7 \times 16 + 5 = 112 + 5 = 117$
* $\Rightarrow$ **Expected Result:** `PA = 117`

### Test Case 2: $\text{LA} = 20$ (Truy cập hợp lệ - Trang 1)
* **Bước 1:** $p = \lfloor 20 / 16 \rfloor = 1$; $d = 20 \bmod 16 = 4$
* **Bước 2:** $p = 1 < 4$ (Thỏa mãn điều kiện $\text{PTLR}$)
* **Bước 3:** Tra bảng trang tại $p = 1 \rightarrow f = 3$, $\text{valid} = 1$ (Hợp lệ)
* **Bước 4:** $\text{PA} = 3 \times 16 + 4 = 48 + 4 = 52$
* $\Rightarrow$ **Expected Result:** `PA = 52`

### Test Case 3: $\text{LA} = 5$ (Truy cập hợp lệ - Trang 0)
* **Bước 1:** $p = \lfloor 5 / 16 \rfloor = 0$; $d = 5 \bmod 16 = 5$
* **Bước 2:** $p = 0 < 4$ (Thỏa mãn điều kiện $\text{PTLR}$)
* **Bước 3:** Tra bảng trang tại $p = 0 \rightarrow f = 5$, $\text{valid} = 1$ (Hợp lệ)
* **Bước 4:** $\text{PA} = 5 \times 16 + 5 = 80 + 5 = 85$
* $\Rightarrow$ **Expected Result:** `PA = 85`

### Test Case 4: $\text{LA} = 16$ (Trường hợp biên đầu trang: $d = 0$)
* **Bước 1:** $p = \lfloor 16 / 16 \rfloor = 1$; $d = 16 \bmod 16 = 0$
* **Bước 2:** $p = 1 < 4$ (Thỏa mãn điều kiện $\text{PTLR}$)
* **Bước 3:** Tra bảng trang tại $p = 1 \rightarrow f = 3$, $\text{valid} = 1$ (Hợp lệ)
* **Bước 4:** $\text{PA} = 3 \times 16 + 0 = 48$
* $\Rightarrow$ **Expected Result:** `PA = 48`

### Test Case 5: $\text{LA} = 31$ (Trường hợp biên cuối trang: $d = P - 1 = 15$)
* **Bước 1:** $p = \lfloor 31 / 16 \rfloor = 1$; $d = 31 \bmod 16 = 15$
* **Bước 2:** $p = 1 < 4$ (Thỏa mãn điều kiện $\text{PTLR}$)
* **Bước 3:** Tra bảng trang tại $p = 1 \rightarrow f = 3$, $\text{valid} = 1$ (Hợp lệ)
* **Bước 4:** $\text{PA} = 3 \times 16 + 15 = 48 + 15 = 63$
* $\Rightarrow$ **Expected Result:** `PA = 63`

### Test Case 6: $\text{LA} = 64$ (Vượt giới hạn bảng trang: $p \ge \text{PTLR}$)
* **Bước 1:** $p = \lfloor 64 / 16 \rfloor = 4$; $d = 64 \bmod 16 = 0$
* **Bước 2:** Kiểm tra điều kiện $p \ge \text{PTLR}$ ($4 \ge 4 \rightarrow$ Đúng)
* **Bước 3:** Hệ thống ngắt quá trình dịch địa chỉ để bảo vệ bộ nhớ.
* $\Rightarrow$ **Expected Result:** `OUT_OF_RANGE`

### Test Case 7: $\text{LA} = 48$ (Lỗi thiếu trang: $\text{valid} = 0$)
* **Bước 1:** $p = \lfloor 48 / 16 \rfloor = 3$; $d = 48 \bmod 16 = 0$
* **Bước 2:** $p = 3 < 4$ (Thỏa mãn điều kiện $\text{PTLR}$)
* **Bước 3:** Tra bảng trang tại $p = 3 \rightarrow \text{valid} = 0$ (Trang chưa có trong khung nhớ vật lý)
* $\Rightarrow$ **Expected Result:** `PAGE_FAULT`

### Test Case 8: $\text{LA} = 63$ (Trường hợp biên tự chọn: Giáp ranh trang lỗi)
* **Mục đích chọn:** Kiểm tra byte cuối cùng của trang số 3 ($d = 15$) ngay sát ngưỡng vượt bảng trang ($\text{LA} = 64$).
* **Bước 1:** $p = \lfloor 63 / 16 \rfloor = 3$; $d = 63 \bmod 16 = 15$
* **Bước 2:** $p = 3 < 4$ (Vẫn nằm trong phạm vi quản lý của $\text{PTLR}$)
* **Bước 3:** Tra bảng trang tại $p = 3 \rightarrow \text{valid} = 0$
* $\Rightarrow$ **Expected Result:** `PAGE_FAULT`

---

## 3. Bảng Tổng Hợp Đối Chiếu Kết Quả (Expected vs. Actual)

| STT | Địa chỉ logic (LA) | Phân loại kịch bản | Expected Result (Tính tay) | Actual Result (Chương trình) | Trạng thái |
| :---: | :---: | :--- | :--- | :--- | :---: |
| **1** | **37** | Hợp lệ (Trang 2) | `PA = 117` | `PA = 117` | **PASS** |
| **2** | **20** | Hợp lệ (Trang 1) | `PA = 52` | `PA = 52` | **PASS** |
| **3** | **5** | Hợp lệ (Trang 0) | `PA = 85` | `PA = 85` | **PASS** |
| **4** | **16** | Biên đầu trang ($d = 0$) | `PA = 48` | `PA = 48` | **PASS** |
| **5** | **31** | Biên cuối trang ($d = P - 1$) | `PA = 63` | `PA = 63` | **PASS** |
| **6** | **64** | Vượt bảng trang ($p \ge \text{PTLR}$) | `OUT_OF_RANGE` | `OUT_OF_RANGE` | **PASS** |
| **7** | **48** | Lỗi thiếu trang ($\text{valid} = 0$) | `PAGE_FAULT` | `PAGE_FAULT` | **PASS** |
| **8** | **63** | Biên tự chọn (Giáp ranh trang lỗi) | `PAGE_FAULT` | `PAGE_FAULT` | **PASS** |

---

## 4. Kết Luận Kiểm Chứng
* **Tỷ lệ đạt:** $8/8$ test cases (**100% PASS**).
* **Đánh giá:** Kết quả đầu ra của chương trình trùng khớp tuyệt đối với kết quả tính toán lý thuyết độc lập trên mọi nhóm kịch bản (truy cập hợp lệ, biên đầu/cuối trang, vượt giới hạn $\text{PTLR}$ và ngoại lệ `PAGE_FAULT`).