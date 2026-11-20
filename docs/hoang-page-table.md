# Phần Hoàng — Nhập và in page table

Em làm phần nhập bảng trang và in bảng cho đề tài Paging và page-table address translation. Mỗi dòng có số page, số frame và valid bit. Các hàm nằm trong `src/page_table.cpp`, khai báo và kiểu dữ liệu nằm trong `include/page_table.h`.

## Cách hiểu dữ liệu

- Page và frame đánh số từ 0. Với n page, nhập đủ các số page từ 0 đến n−1, mỗi số một lần. Có thể nhập các page theo thứ tự bất kỳ. Hàm lưu lại theo số page để nhóm dùng `table[p]` khi tra bảng.
- Frame là số nguyên không âm trong miền `int`. Hai page được phép có cùng frame. Đề chưa quy định số frame vật lý nên phần nhập chưa kiểm tra cận trên theo dung lượng RAM.
- Valid chỉ nhận 0 hoặc 1. Valid bằng 1 nghĩa là ánh xạ có thể dùng trong mô hình. Valid bằng 0 nghĩa là entry không hợp lệ và phần dịch phải báo invalid/page fault. Frame vẫn được nhập và hiển thị ở dòng invalid, nhưng phần dịch phải bỏ qua giá trị đó.
- Nhập số page bằng 0 sẽ có bảng rỗng. Bảng rỗng có thông báo khi in.
- Page size P do phần chương trình chính quản lý. Bảng nhập chỉ lưu page, frame và valid bit.

## Cách gọi để ghép vào chương trình

```cpp
#include "page_table.h"

PageTable table = loadPageTable();
printPageTable(table);
// Bảng đã lưu theo số page. Hàm dịch của Tuấn có thể tra table[p].
```

Hai hàm có thể nhận luồng vào/ra để đọc dữ liệu từ tệp hoặc kiểm tra kết quả mà không phải sửa code. Ví dụ `loadPageTable(inputFile, std::cout)`.

Hàm đọc từng dòng. Mỗi dòng phải có đúng một số nguyên. Dữ liệu như `-1`, `2abc`, `1 2` hoặc số quá lớn được báo sai và yêu cầu nhập lại. Page ngoài bảng hoặc đã nhập rồi cũng được yêu cầu nhập lại. Hàm dùng `std::ws` ở đầu để xử lý dấu xuống dòng sau menu đọc bằng `>>`.

Nếu hết đầu vào khi đang nhập, hàm ném `std::runtime_error` và không giao bảng nhập dở cho bên gọi. Chương trình chính có thể bắt lỗi để giữ bảng cũ hoặc kết thúc lần nhập. Ví dụ demo đã có phần bắt lỗi này.

## Chạy demo riêng của Hoàng

Với GCC C++17, chạy từ thư mục gốc repo.

```sh
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/page_table.cpp examples/hoang_page_table_demo.cpp -o hoang_demo
./hoang_demo < examples/page_table_input.txt
```

Với Visual Studio, mở Developer Command Prompt rồi chạy từ thư mục gốc repo.

```bat
cl /nologo /std:c++17 /EHsc /W4 /WX /Iinclude src\page_table.cpp examples\hoang_page_table_demo.cpp /Fe:hoang_demo.exe
hoang_demo.exe < examples\page_table_input.txt
```

Tệp mẫu nhập 3 page. Mỗi số nằm trên một dòng, theo thứ tự số page rồi page/frame/valid của từng entry. Kết quả bảng như sau.

| Page | Frame | Valid |
| --- | --- | --- |
| 0 | 3 | 1 |
| 1 | 0 | 0 |
| 2 | 7 | 1 |

## Giải thích để trình bày

Em dùng vector để lưu bảng. Khi người dùng nhập page p, em kiểm tra p có trong khoảng 0 đến n−1 và chưa nhập trước đó. Sau đó em đọc frame và valid bit. Chỉ khi đọc xong cả ba giá trị, em mới lưu entry vào `table[p]`. Khi in, em đi lần lượt qua bảng và hiện ba cột Page, Frame và Valid.

Với một lượt nhập hợp lệ ngay, phần nhập và in đều có thời gian O(n). Vector bảng và danh sách đánh dấu page đã nhập dùng O(n) bộ nhớ. Số lần nhập lại phụ thuộc dữ liệu người dùng.

## Contribution statement

Nguyễn Việt Hoàng phụ trách cấu trúc bảng trang, hàm nhập page/frame/valid bit, kiểm tra dữ liệu và hàm in bảng. Phần đóng góp trong repo có chương trình demo riêng, dữ liệu mẫu và hướng dẫn ghép hàm. Code được hỗ trợ soạn thảo và kiểm tra bằng OpenAI Codex. Người nộp cần đọc hiểu và tự chạy lại trước khi trình bày.

Nguồn yêu cầu là `PhanCong_DonGian.docx` và `2627_0101_Research_Requirement` được chia sẻ trong nhóm Zalo GR12 Hệ điều hành. Dữ liệu mẫu dùng page 2 → frame 7 theo ví dụ của PDF yêu cầu.
