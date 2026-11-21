# Định dạng file TXT và vòng đời dữ liệu

## 1. Bộ file

Ứng dụng khởi tạo mỗi Repository với một file tương đối từ thư mục chạy:

| File | Entity được lưu |
|---|---|
| `data/tables.txt` | Table |
| `data/menu_items.txt` | MenuItem |
| `data/dining_sessions.txt` | DiningSession |
| `data/orders.txt` | Order kèm OrderItem |
| `data/bills.txt` | Bill kèm BillItem |
| `data/payments.txt` | Payment |

Chạy ứng dụng từ thư mục gốc dự án để các đường dẫn tương đối trỏ đúng `data/`.

## 2. Quy ước serialization hiện tại

`FileRepository<T>` ghi số record ở dòng đầu. Mỗi record nằm trên một dòng
tiếp theo; trường chuỗi được ghi bằng `std::quoted`, các trường được phân cách
bằng whitespace. Đây **không phải CSV**. Enum được lưu dưới dạng số nguyên theo
thứ tự enum trong code; thời gian được lưu dạng số nguyên epoch seconds.

| Entity | Thứ tự trường record |
|---|---|
| Table | quoted `id`, quoted `code`, `status`, `isActive` |
| MenuItem | quoted `id`, quoted `name`, `price`, quoted `description` |
| DiningSession | quoted `id`, quoted `tableId`, `guestCount`, `openedAt`, `hasClosedAt`, `closedAt`, `status`, quoted `note` |
| Order | quoted `id`, quoted `diningSessionId`, `createdAt`, `status`, `itemCount`, sau đó mỗi OrderItem: quoted `id`, quoted `menuItemId`, quoted `itemNameSnapshot`, `unitPriceAtOrder`, `quantity`, quoted `note`, `status` |
| Bill | quoted `id`, quoted `diningSessionId`, `createdAt`, `paymentStatus`, `itemCount`, sau đó mỗi BillItem: quoted `id`, quoted `menuItemId`, quoted `itemNameSnapshot`, `unitPriceSnapshot`, `quantity` |
| Payment | quoted `id`, quoted `billId`, `amount`, quoted `method`, `paidAt`, quoted `referenceCode`, quoted `note` |

Tổng tiền/line total được tính từ dữ liệu BillItem; không cần lưu bản sao các số
tổng trong record Bill.

## 3. Nạp dữ liệu

Khi Repository khởi tạo:

1. Nếu file chưa tồn tại, Repository bắt đầu với collection rỗng.
2. Nếu file tồn tại, đọc header là số lượng record.
3. Decode đúng số record theo codec entity, kiểm tra ID trùng và dữ liệu thừa.
4. Nếu file sai định dạng, mở không được hoặc chứa dữ liệu không hợp lệ, ném
   `std::runtime_error`; ứng dụng báo lỗi khởi động.

Không bỏ qua record lỗi để tiếp tục với dữ liệu thiếu. Các codec cũng kiểm tra
giới hạn enum và một số bất biến quan trọng của Order, Bill, Session, Payment.
Các liên kết ID xuyên nhiều Repository chủ yếu được xác thực ở Service.

## 4. Ghi dữ liệu

Khi thêm/cập nhật/xóa, `FileRepository<T>` tạo collection mới trong RAM, ghi
`<file>.tmp`, flush và kiểm tra lỗi ghi trước khi thay file đích bằng
`std::filesystem::rename`. Collection trong RAM chỉ được thay sau khi persist
thành công. Nếu thao tác ghi/thay file lỗi, Repository ném exception và không
báo thành công.

Các thao tác Service cập nhật nhiều Repository là một chuỗi write cùng rollback
bù trừ; file system không cung cấp transaction nguyên tử cho toàn bộ các file.

## 5. Giới hạn và vận hành

- File không có schema version, checksum hoặc migration; thay đổi thứ tự trường
  cần cân nhắc tương thích dữ liệu cũ.
- Đây là định dạng nội bộ của ứng dụng, không phải định dạng import/export.
- Không có database lock hay hỗ trợ nhiều tiến trình ghi đồng thời.
- Backup các file trong `data/` trước khi sửa tay hoặc nâng cấp schema.
- Không chỉnh sửa enum numeric trong code nếu không cân nhắc các file đã lưu.
