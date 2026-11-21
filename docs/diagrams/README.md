# Sơ đồ hệ thống

Các sơ đồ dưới đây dùng Mermaid và phản ánh cấu trúc đang triển khai. Đây là
model miền nghiệp vụ lưu file, không phải schema SQL/database.

## 1. Kiến trúc

```mermaid
flowchart TD
    User["Quản lý nhà hàng"]
    subgraph GUI["Qt Widgets"]
        Main["MainWindow<br/>tabs + refresh coordinator"]
        TablePage["TableSessionPage"]
        MenuPage["MenuPage"]
        OrderPage["OrderPage"]
        BillingPage["BillingPage"]
        Main --> TablePage
        Main --> MenuPage
        Main --> OrderPage
        Main --> BillingPage
    end
    Services["Services<br/>Table / Menu / Session / Order / Bill / Payment"]
    Models["Models"]
    Repositories["Repositories<br/>entity repositories + FileRepository<T>"]
    Memory[("DynamicArray<T><br/>RAM")]
    Files[("data/*.txt")]

    User --> GUI
    GUI -->|"use case calls / ServiceResult"| Services
    Services --> Models
    Services --> Repositories
    Repositories --> Memory
    Models --> Memory
    Repositories -->|"load / persist"| Files
```

GUI chỉ phụ thuộc Services. Repositories không được gọi trực tiếp từ GUI.

## 2. Quan hệ dữ liệu

```mermaid
erDiagram
    TABLE ||--o{ DINING_SESSION : "tableId"
    DINING_SESSION ||--o{ ORDER : "diningSessionId"
    ORDER ||--o{ ORDER_ITEM : owns
    MENU_ITEM ||--o{ ORDER_ITEM : "menuItemId; snapshot name/price"
    DINING_SESSION ||--o| BILL : "diningSessionId"
    BILL ||--|{ BILL_ITEM : owns
    MENU_ITEM o|--o{ BILL_ITEM : "menuItemId; snapshot name/price"
    BILL ||--o| PAYMENT : "billId"
```

ID link không sở hữu entity đích. Order sở hữu OrderItem; Bill sở hữu BillItem.
Snapshot giữ lịch sử giá/tên độc lập với thực đơn hiện tại.

## 3. Luồng mở phiên và gọi món

```mermaid
sequenceDiagram
    actor Manager as Quản lý
    participant GUI
    participant TableService
    participant SessionService as DiningSessionService
    participant TableRepo as TableRepository
    participant SessionRepo as DiningSessionRepository
    participant OrderService

    Manager->>GUI: Chọn bàn, nhập guestCount
    GUI->>SessionService: openSession(sessionId, tableId, guestCount)
    SessionService->>TableRepo: kiểm tra active + Available
    SessionService->>TableRepo: cập nhật Occupied
    SessionService->>SessionRepo: lưu DiningSession
    SessionService-->>GUI: ServiceResult
    Manager->>GUI: Tạo Order, thêm món
    GUI->>OrderService: createOrder / addItem
    OrderService-->>GUI: Order + OrderItem snapshot
```

Việc chọn bàn do Quản lý thực hiện. Service xác thực lại trạng thái bàn khi mở
phiên; `guestCount` không được dùng để chọn bàn hoặc kiểm tra sức chứa.

## 4. Luồng lập hóa đơn và thanh toán

```mermaid
sequenceDiagram
    actor Manager as Quản lý
    participant GUI
    participant BillService
    participant PaymentService
    participant BillRepo as BillRepository
    participant SessionRepo as DiningSessionRepository
    participant TableRepo as TableRepository
    participant PaymentRepo as PaymentRepository

    Manager->>GUI: Lập hóa đơn
    GUI->>BillService: createBill(billId, sessionId, time)
    BillService->>BillRepo: lưu Bill từ món Served
    BillService->>SessionRepo: Active → WaitingForPayment
    BillService-->>GUI: ServiceResult
    Manager->>GUI: Nhập amount đúng tổng
    GUI->>PaymentService: payBill(paymentId, billId, amount, method, time)
    PaymentService->>BillRepo: Unpaid → Paid
    PaymentService->>SessionRepo: WaitingForPayment → Closed
    PaymentService->>TableRepo: Occupied → Available
    PaymentService->>PaymentRepo: lưu duy nhất Payment
    PaymentService-->>GUI: ServiceResult
```

Nếu một bước ghi thất bại, Service cố gắng rollback các bước đã lưu; lỗi rollback
cũng được phản ánh cho GUI.

## 5. Trạng thái tổng quan

```mermaid
stateDiagram-v2
    [*] --> Available
    Available --> Occupied: Mở DiningSession
    Occupied --> Available: Hủy hợp lệ hoặc thanh toán thành công
    Available --> Maintenance: Bảo trì
    Maintenance --> Available: Sẵn sàng sử dụng

    state "DiningSession" as Session {
      [*] --> Active
      Active --> WaitingForPayment: Lập Bill
      Active --> Cancelled: Chưa có món Served
      WaitingForPayment --> Closed: Payment đúng tổng
    }

    state "Bill" as Bill {
      [*] --> Unpaid
      Unpaid --> Paid: Một Payment chính xác
    }
```

Order và OrderItem có trạng thái riêng: Draft/Pending/InProgress/Completed/
Cancelled cho Order; Pending/Served/Cancelled cho OrderItem. Bill chỉ tổng hợp
món Served.
