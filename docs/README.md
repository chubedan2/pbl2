# Tài liệu dự án

## Mục lục

| Tài liệu | Nội dung |
|---|---|
| [README dự án](../README.md) | Phụ thuộc, build, chạy và test |
| [Yêu cầu phần mềm](requirements/srs.md) | SRS v1.9, phạm vi MVP và truy vết trạng thái triển khai |
| [Thiết kế kiến trúc và lớp](design/class-design.md) | Các tầng, trách nhiệm lớp và luồng phụ thuộc |
| [Thiết kế dữ liệu](design/data-design.md) | Model, liên kết ID, snapshot và trạng thái |
| [Lưu trữ TXT](design/txt-format.md) | Schema file, nạp/lưu và giới hạn |
| [Kiểm thử và xác minh](design/verification.md) | Lệnh build/test và phạm vi kiểm tra |
| [Sơ đồ hệ thống](diagrams/README.md) | Sơ đồ kiến trúc, dữ liệu và luồng nghiệp vụ |

## Cách dùng tài liệu

- Bắt đầu từ [SRS](requirements/srs.md) để hiểu phạm vi và quy tắc nghiệp vụ.
- Dùng tài liệu thiết kế và sơ đồ để hiểu cách các yêu cầu được hiện thực.
- Dùng mã nguồn làm nguồn chính xác cuối cùng cho API và hành vi.
- SRS gốc v1.9 được lưu tại [SRS_nha_hang_services_repositories_v1.9.md](../SRS_nha_hang_services_repositories_v1.9.md).

Các tài liệu trong `design/` và `diagrams/` mô tả phiên bản đã code (as-built).
Những điểm còn thiếu hoặc chỉ có ở tầng Core được ghi rõ trong [bảng truy vết](requirements/srs.md#4-truy-vết-trien-khai).
