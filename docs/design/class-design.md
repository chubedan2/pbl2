# Thiết kế kiến trúc và lớp

## 1. Các tầng

```text
Qt Widgets GUI
      │ gọi nghiệp vụ; hiển thị ServiceResult
      ▼
Services ───────────────► Models
   │                         ▲
   │ phối hợp đọc/cập nhật    │ giá trị và trạng thái
   ▼                         │
Repositories ────────────────┘
   │
   ├── DynamicArray<T> trong RAM
   └── FileRepository<T> ↔ data/*.txt
```

GUI không gọi Repository trực tiếp. Service không trực tiếp đọc/ghi file.
Repository quản lý persistence nhưng không quyết định nghiệp vụ nhà hàng.
`DynamicArray<T>` là cấu trúc dữ liệu generic, không chứa luật miền nghiệp vụ.

## 2. Trách nhiệm class

### GUI

| Class | Trách nhiệm |
|---|---|
| `MainWindow` | Tạo tab, giữ page widgets, phát lệnh refresh liên trang |
| `TableSessionPage` | Hiển thị bàn/phiên; thêm, sửa, đổi trạng thái bàn; mở/hủy phiên |
| `MenuPage` | Xem, thêm và sửa món |
| `OrderPage` | Tạo Order; thêm/xác nhận/hủy Order; quản lý trạng thái món |
| `BillingPage` | Lập Bill và ghi nhận Payment |
| `UiHelpers` | Tạo bảng, chuyển text, status, tiền; hiển thị ServiceResult |

### Services

| Service | Repository phụ thuộc | Trách nhiệm nghiệp vụ chính |
|---|---|---|
| `TableService` | Table | Thêm/sửa/ngừng bàn, đổi status hợp lệ, lọc bàn Available |
| `MenuService` | MenuItem | Thêm/sửa/tìm món |
| `DiningSessionService` | Table, DiningSession, Order | Mở/hủy phiên; phối hợp giải phóng/chiếm bàn |
| `OrderService` | Order, MenuItem, DiningSession | Tạo Order, thêm/xóa/hủy món, xác nhận và cập nhật trạng thái |
| `BillService` | Bill, Order, DiningSession | Kiểm tra điều kiện, chụp món Served vào Bill và chuyển session chờ trả |
| `PaymentService` | Payment, Bill, DiningSession, Table | Kiểm tra amount, ghi Payment, Paid/Closed/Available |

Mọi thao tác nghiệp vụ trả `ServiceResult` để caller nhận trạng thái và thông
điệp. Các Service có API tra cứu phù hợp; GUI chỉ phụ thuộc Services.

### Models

| Model | Quyền sở hữu / vai trò |
|---|---|
| `Table`, `MenuItem`, `DiningSession`, `Payment` | Đối tượng miền đơn |
| `Order` | Sở hữu danh sách `OrderItem` |
| `Bill` | Sở hữu danh sách `BillItem` và phép tính tổng |
| `OrderItem`, `BillItem` | Lưu thông tin dòng món và snapshot giao dịch |

Quan hệ giữa các aggregate được liên kết bằng ID, không dùng con trỏ sở hữu
chéo giữa Repository. `Order` sở hữu OrderItem; `Bill` sở hữu BillItem.

### Repositories

Mỗi Repository chuyên biệt dựa trên `FileRepository<T>` template dùng chung.
Repository cung cấp tra cứu theo ID, thêm/cập nhật/xóa phù hợp loại dữ liệu;
`BillRepository` và `PaymentRepository` còn kiểm tra tính duy nhất theo session
hoặc bill.

## 3. Quan hệ và hướng phụ thuộc

- `DiningSessionService` phối hợp Table và DiningSession khi mở/hủy.
- `OrderService` kiểm tra DiningSession và MenuItem trước khi cập nhật Order.
- `BillService` đọc các Order của session, tạo snapshot BillItem và đổi session
  sang `WaitingForPayment`.
- `PaymentService` cập nhật Bill, DiningSession, Table và Payment theo luồng
  thanh toán.
- GUI refresh các page liên quan sau khi thao tác thành công hoặc thất bại để
  giữ danh sách hiện trên màn hình khớp với dữ liệu Services.

## 4. Khởi tạo ứng dụng

`main.cpp` tạo `QApplication`, sau đó khởi tạo sáu Repository với đường dẫn
`data/*.txt`, tạo Services với các Repository tương ứng, dựng `MainWindow` và
chạy event loop. Lỗi khởi tạo/lưu trữ được bắt ở điểm vào và hiển thị hộp thoại
lỗi thay vì giả lập dữ liệu rỗng thành thành công.

## 5. Build

Makefile tự tìm tất cả `src/**/*.cpp`; thêm trang GUI mới không cần đăng ký
nguồn thủ công. Build yêu cầu compiler C++17, GNU Make, `pkg-config` và Qt 6
Widgets.

Xem [sơ đồ hệ thống](../diagrams/README.md) và [thiết kế dữ liệu](data-design.md).
