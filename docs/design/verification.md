# Kiểm thử và xác minh

## 1. Các lệnh

Từ thư mục gốc:

```sh
make
make test
make run
```

Build cần GNU Make, compiler C++17, `pkg-config` và Qt 6 Widgets. `make test`
build/runs core tests và không cần mở cửa sổ GUI. Có thể dùng
`QT_QPA_PLATFORMTHEME=generic` nếu môi trường desktop báo lỗi theme GNOME/
GSettings.

## 2. Phạm vi test lõi

`tests/core_tests.cpp` kiểm tra:

- `DynamicArray`: quản lý kích thước/capacity, copy/move, tìm kiếm và xóa.
- Model invariants: chuyển trạng thái bàn, dữ liệu MenuItem và Bill hợp lệ.
- Service end-to-end: bàn → session → order/item → bill → payment.
- Từ chối dữ liệu sai và chuyển trạng thái không hợp lệ.
- Persistence: tạo lại Repository từ file và xác nhận dữ liệu được khôi phục.
- Lỗi đọc file, ID trùng và tính nhất quán ở các thao tác liên Service.

**Kết quả lần xác minh gần nhất:** `make test` — 87 checks passed.

## 3. Kiểm tra GUI

GUI build cùng target ứng dụng bằng `make`. Smoke test khởi chạy ứng dụng Qt
trong môi trường offscreen đã thành công; lệnh được giữ chạy trong thời gian
ngắn và kết thúc theo timeout vì event loop ứng dụng là liên tục. Kiểm thử tương
tác GUI đầy đủ vẫn cần thực hiện thủ công.

## 4. Giới hạn kiểm chứng

- Chưa có benchmark chứng minh NFR-06 dưới một giây.
- Core test không thay thế kiểm thử usability hoặc tương tác bằng chuột/bàn phím.
- Tính nguyên tử giữa nhiều file dựa trên rollback bù trừ, không có crash
  consistency như transaction database.
