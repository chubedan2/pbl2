# Thiết kế dữ liệu

## 1. Mô hình dữ liệu

| Entity | Thuộc tính chính | Ghi chú |
|---|---|---|
| `Table` | `id`, `code`, `status`, `isActive` | Trạng thái MVP: Available, Occupied, Maintenance |
| `MenuItem` | `id`, `name`, `price`, `description` | `price` là số nguyên VND |
| `DiningSession` | `id`, `tableId`, `guestCount`, `openedAt`, `closedAt`, `status`, `note` | `guestCount > 0`; `tableId` là liên kết ID |
| `Order` | `id`, `diningSessionId`, `createdAt`, `status`, `items` | Sở hữu các OrderItem |
| `OrderItem` | `id`, `menuItemId`, `itemNameSnapshot`, `unitPriceAtOrder`, `quantity`, `note`, `status` | Snapshot không đổi khi giá thực đơn đổi |
| `Bill` | `id`, `diningSessionId`, `createdAt`, `items`, `subtotal`, `totalAmount`, `paymentStatus` | Tối đa một Bill mỗi session |
| `BillItem` | `id`, `menuItemId`, `itemNameSnapshot`, `unitPriceSnapshot`, `quantity`, `lineTotal` | Snapshot hóa đơn |
| `Payment` | `id`, `billId`, `amount`, `method`, `paidAt`, `referenceCode`, `note` | Tối đa một Payment mỗi Bill |

Tiền dùng kiểu số nguyên `long long` VND, không dùng floating point. `subtotal`
và `totalAmount` bằng tổng `lineTotal`; MVP không có thuế, phí hoặc giảm giá.

## 2. Quan hệ logical

```text
Table 1 ───── 0..* DiningSession
DiningSession 1 ───── 0..* Order
Order 1 ───── 0..* OrderItem
MenuItem 1 ───── 0..* OrderItem
DiningSession 1 ───── 0..1 Bill
Bill 1 ───── 1..* BillItem
MenuItem 0..1 ───── 0..* BillItem
Bill 1 ───── 0..1 Payment
```

Quan hệ tới Table/MenuItem là tham chiếu ID, không phải sở hữu vòng đời. Quan
hệ sở hữu trong bộ nhớ: Order sở hữu OrderItem; Bill sở hữu BillItem. Sơ đồ
Mermaid chi tiết tại [diagrams/README.md](../diagrams/README.md).

## 3. Snapshot và lịch sử

- Khi thêm món vào Order, `OrderItem` sao chép tên và giá hiện tại của MenuItem.
- Khi lập hóa đơn, `BillItem` được dựng từ OrderItem `Served`, sao chép tên, giá
  và số lượng tại thời điểm giao dịch.
- Sửa MenuItem không làm thay đổi các Order cũ hoặc Bill đã tạo.
- OrderItem đã xác nhận không bị xóa vật lý khi hủy; `Cancelled` giữ lịch sử.
- OrderItem thuộc Order Draft có thể bị loại bỏ.

## 4. Vòng đời trạng thái

| Entity | Trạng thái/chuyển đổi |
|---|---|
| Table | Available ↔ Occupied; Available ↔ Maintenance |
| DiningSession | Active → WaitingForPayment → Closed; Active → Cancelled |
| Order | Draft → Pending → InProgress/Completed; Draft/Pending có thể Cancelled nếu điều kiện cho phép |
| OrderItem | Pending → Served hoặc Cancelled |
| Bill | Unpaid → Paid |
| Payment | Bản ghi bất biến sau khi ghi; không có partial/refund lifecycle |

`Order` phản ánh tiến độ các OrderItem. Bill chỉ chứa món Served; món Pending
phải được giải quyết trước khi tạo Bill và món Cancelled bị bỏ qua.

## 5. Bất biến quan trọng

- Một bàn chỉ có một session đang hoạt động tại một thời điểm.
- Session chỉ mở với bàn active và Available; mở thành công chiếm bàn.
- Session hủy chỉ được khi chưa có OrderItem Served.
- Session chuyển sang WaitingForPayment khi lập Bill hợp lệ.
- Session chỉ đóng sau Payment bằng đúng tổng Bill; bàn sau đó trở về Available.
- Không cho Bill thứ hai cho cùng session hoặc Payment thứ hai cho cùng Bill.
- ID OrderItem phải duy nhất trên các Order; ID BillItem tạo theo Bill.

## 6. Giới hạn mô hình lưu trữ

File là persistence cục bộ, không có DBMS, foreign key hoặc transaction nhiều file.
Service phải kiểm tra liên kết ID và điều phối các entity liên quan. Việc cập
nhật nhiều Repository dùng rollback bù trừ, có thể báo lỗi rollback nếu phục hồi
không thành công.
