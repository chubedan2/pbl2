# Đặc tả yêu cầu phần mềm

## 1. Nguồn và trạng thái

Đây là lối vào yêu cầu của dự án, tóm lược các quyết định trong
[SRS v1.9 gốc](../../SRS_nha_hang_services_repositories_v1.9.md), đồng thời
ghi nhận phạm vi đã triển khai. SRS gốc là tài liệu chuẩn khi cần tra cứu mô tả
đầy đủ, các bảng chi tiết hoặc nguyên văn tiêu chí nghiệp vụ.

| Thuộc tính | Giá trị |
|---|---|
| Phiên bản nguồn | 1.9 |
| Sản phẩm | MVP quản lý nhà hàng phục vụ tại bàn |
| Người dùng | Một Quản lý nhà hàng |
| Nền tảng đã chọn | C++17, Qt 6 Widgets |
| Lưu trữ | File cục bộ |
| Trạng thái | MVP đã triển khai; một số yêu cầu UI và Should chưa hoàn thiện |

## 2. Mục tiêu và phạm vi

Hỗ trợ quy trình từ xếp bàn đến thanh toán: quản lý bàn, thực đơn, phiên ăn,
nhiều lần gọi món, trạng thái phục vụ từng món, hóa đơn và một lần thanh toán
đủ. Dữ liệu quan trọng được lưu để còn sau khi đóng và mở lại ứng dụng.

Không thuộc MVP: đặt bàn trước, tồn kho, giao hàng/mang đi, quản lý ca/nhân sự,
nhiều chi nhánh, đồng bộ mạng, nhiều tài khoản/phân quyền, thanh toán tự động,
thanh toán một phần, thuế/phí/giảm giá và hoàn tiền.

## 3. Yêu cầu chức năng

| ID / phân hệ | Yêu cầu MVP | Trạng thái sau khi code |
|---|---|---|
| FR-TB-01–03 | Thêm, xem, sửa bàn; không trùng ID | Core và GUI đã có |
| FR-TB-04 | Tra cứu bàn theo ID/mã | Tra cứu ID trong Service; chưa có truy vấn mã bàn và ô tìm kiếm GUI |
| FR-TB-05 | Lấy bàn đang hoạt động, trạng thái `Available` | Service đã có; GUI hiện chọn từ danh sách toàn bộ bàn |
| FR-TB-07–08 | Ngừng sử dụng bàn an toàn; đồng bộ trạng thái theo phiên | Core và các thao tác GUI chính đã có |
| FR-MN-01–04 | Thêm, xem, sửa và tìm món theo ID | Core đã có; GUI thêm/xem/sửa, chưa có ô tìm kiếm |
| FR-DS-01–04 | Quản lý chọn bàn; kiểm tra bàn còn Available; lưu guestCount > 0 | Core và GUI mở phiên đã có; cần đưa danh sách Available lên UI |
| FR-DS-06–09 | Xem phiên/lịch sử, hủy, đóng sau khi trả đủ, tra cứu Bill/Payment | Core đã có; GUI hiện thiếu màn hình chi tiết Payment |
| FR-OR-01–07 | Tạo nhiều Order cho session; thêm món hợp lệ; lưu quantity/note/snapshot; cập nhật trạng thái món | Core và thao tác GUI chính đã có |
| FR-OR-08 | Draft được bỏ món; Order xác nhận thì chỉ hủy món chưa phục vụ, giữ lịch sử | Core và GUI đã có |
| FR-OR-09 | Xem Order và món chưa phục vụ theo phiên | Lọc Order theo phiên có; GUI chưa lọc riêng món chưa phục vụ |
| FR-OR-10 (Should) | Sửa quantity/note khi trạng thái cho phép | Service có API; GUI chưa có thao tác sửa |
| FR-BL-01–06 | Bill duy nhất mỗi phiên, có món Served, lưu snapshot, tính và hiển thị tổng | Core và GUI lập/xem hóa đơn đã có |
| FR-PY-01–06 | Một Payment mỗi Bill; amount dương và chính xác bằng total | Core và GUI thanh toán đã có |

FR-DS-05 đã bị loại bỏ trong SRS v1.9. `Reserved`, tìm theo tên món và ngừng
bán món không thuộc MVP.

## 4. Truy vết triển khai

**Core** bao gồm Model/Service/Repository; **UI** là thao tác có thể thực hiện
trực tiếp qua GUI. “Một phần” không đồng nghĩa nghiệp vụ lõi chưa có.

| Mã | Core | UI | Ghi chú |
|---|---|---|---|
| FR-TB-01–03, 07–08 | Đạt | Đạt | Thêm/sửa/ngừng bàn; mở, hủy phiên và thanh toán cập nhật trạng thái |
| FR-TB-04 | Một phần | Một phần | Tra ID được; thiếu truy vấn theo mã và giao diện tìm |
| FR-TB-05 | Đạt | Một phần | `getAvailableTables()` có; màn hình vẫn hiển thị mọi bàn |
| FR-MN-01–03 | Đạt | Đạt | Thêm, xem và sửa thực đơn |
| FR-MN-04 | Đạt theo ID | Một phần | API tìm có; chưa có ô tìm kiếm |
| FR-DS-01–04, 06–08 | Đạt | Đạt một phần | Mở phiên, guestCount và lịch sử có; cần chọn bàn từ danh sách Available |
| FR-DS-09 | Đạt ở API | Một phần | API quan hệ đã có; thiếu màn hình chi tiết thanh toán |
| FR-OR-01–08 | Đạt | Đạt | Tạo Order, thêm/xác nhận/hủy món, phục vụ và giữ lịch sử |
| FR-OR-09 | Đạt một phần | Đạt một phần | Chọn Order theo phiên; chưa lọc riêng món pending |
| FR-OR-10 | Service có API | Chưa có | Yêu cầu mức Should |
| FR-BL-01–06 | Đạt | Đạt | Lập Bill từ món Served, chống Bill trùng và xem tổng |
| FR-PY-01–06 | Đạt | Đạt | Từ chối thanh toán thiếu/thừa, không dương hoặc trùng |

## 5. Quy tắc nghiệp vụ bắt buộc

1. ID duy nhất trong phạm vi mỗi loại đối tượng.
2. `guestCount` là số nguyên dương bắt buộc, chỉ dùng để ghi nhận.
3. Chỉ bàn đang hoạt động và `Available` được mở phiên; Service kiểm tra lại
   trạng thái ngay trước khi tạo phiên.
4. Chỉ session `Active` nhận Order. OrderItem cần món hợp lệ, số lượng dương,
   snapshot tên/giá tại thời điểm gọi.
5. OrderItem `Cancelled` không tính vào hóa đơn. Món đã xác nhận không xóa vật
   lý; chỉ được chuyển sang trạng thái hủy nếu chưa phục vụ.
6. Một session có tối đa một Bill. Bill phải có ít nhất một món `Served`;
   pending items phải được xử lý trước.
7. `lineTotal = unitPriceSnapshot * quantity`; `totalAmount` bằng tổng các dòng.
   Không áp dụng thuế, phí dịch vụ hay giảm giá.
8. Một Bill có tối đa một Payment; số tiền phải lớn hơn 0 và bằng chính xác
   `Bill.totalAmount`.
9. Khi Bill chưa `Paid`, session không đóng và bàn không được giải phóng.
10. Thao tác cập nhật nhiều module cần tránh trạng thái dở dang; khi lỗi, Service
    dùng rollback bù trừ và báo lỗi nếu rollback thất bại.

## 6. Yêu cầu phi chức năng

| ID | Yêu cầu | Hiện trạng |
|---|---|---|
| NFR-01 | Dễ bảo trì, tách giao diện/nghiệp vụ/lưu trữ | Tách `GUI`, `Models`, `Services`, `Repositories`, `Structure`; trang UI riêng |
| NFR-02 | Toàn vẹn dữ liệu | Kiểm tra trong Model/Service; rollback cho luồng nhiều repository |
| NFR-03 | Chính xác tiền tệ | Số nguyên VND, không dùng float/double |
| NFR-04 | Dữ liệu tồn tại sau restart | File repositories tự nạp khi khởi tạo và lưu khi thay đổi |
| NFR-05 | Dễ kiểm thử | Test Core độc lập GUI với file dữ liệu tạm |
| NFR-06 | Thao tác thông thường dưới 1 giây | Chưa có benchmark định lượng |
| NFR-07 | GUI rõ ràng, lỗi dễ hiểu | Qt Widgets; Service hiện trả một số thông báo tiếng Anh |
| NFR-08 | Mở rộng được | Các tầng tách biệt; đa người dùng ngoài MVP |
| NFR-09 | C++17 | Đã cấu hình build C++17 |

## 7. Tiêu chí nghiệm thu rút gọn

- Không mở phiên cho bàn bận/bảo trì/ngừng hoạt động; không nhận guestCount
  không dương.
- Cho phép nhiều Order trên cùng một phiên và lưu snapshot tên/giá.
- Chỉ món đã phục vụ mới được tính Bill; món đã hủy không tính.
- Không tạo Bill thứ hai cho cùng phiên hoặc Bill rỗng.
- Chỉ nhận một Payment khớp tổng; sau đó Bill Paid, session Closed và bàn
  Available.
- Dữ liệu đọc lại chính xác sau khi đóng/mở lại ứng dụng.

Danh sách nghiệm thu đầy đủ ở mục 12 của [SRS gốc](../../SRS_nha_hang_services_repositories_v1.9.md).
