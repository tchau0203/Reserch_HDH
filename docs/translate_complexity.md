# Độ phức tạp của một lần dịch địa chỉ

| Đại lượng | Độ phức tạp | Giải thích |
|---|---|---|
| Thời gian | O(1) | 1 phép chia, 1 phép lấy dư, 1 lần truy cập mảng theo chỉ số, 1 phép nhân, 1 phép cộng, vài phép so sánh. Không có vòng lặp, không phụ thuộc số page. |
| Bộ nhớ phụ | O(1) | Chỉ dùng vài biến p, d, f, PA. |
| Bộ nhớ của page table | O(N) | N = số page của tiến trình, mỗi page một entry. Là điểm yếu khi không gian địa chỉ lớn. |
| Truy cập bộ nhớ | hằng số (2 lần) | Page table nằm trong bộ nhớ chính: 1 lần đọc page table + 1 lần đọc dữ liệu (không có TLB; TLB nằm ngoài phạm vi đề tài). |
