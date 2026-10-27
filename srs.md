SOFTWARE REQUIREMENTS SPECIFICATION (SRS)
HỆ THỐNG ĐẶT MÓN TẠI BÀN CHO NHÀ HÀNG CASUAL DINING
Phiên bản: 2.4
Mục đích: Tài liệu đặc tả yêu cầu phần mềm phục vụ đồ án PBL.
________________________________________
1. GIỚI THIỆU
1.1. Mục đích tài liệu
Tài liệu Software Requirements Specification (SRS) mô tả các yêu cầu chức năng, yêu cầu phi chức năng, quy trình nghiệp vụ, các đối tượng dữ liệu và cấu trúc dữ liệu của hệ thống đặt món tại bàn cho nhà hàng Casual Dining.
Tài liệu được sử dụng làm cơ sở để phân tích, thiết kế, lập trình, kiểm thử và đánh giá sản phẩm của đồ án PBL.
Hệ thống hướng đến việc mô phỏng quy trình phục vụ tại bàn, từ khi khách bắt đầu sử dụng bàn, xem menu, đặt món, chuẩn bị món ăn, giao món cho đến khi thanh toán và đóng phiên phục vụ.
1.2. Mục tiêu hệ thống
Hệ thống được xây dựng nhằm:
•	Số hóa quy trình đặt món và phục vụ tại bàn.
•	Giảm sai sót trong quá trình ghi nhận và xử lý order.
•	Quản lý thông tin bàn và phiên phục vụ.
•	Hỗ trợ tìm kiếm và quản lý món ăn trong menu.
•	Theo dõi tiến trình chuẩn bị và giao món.
•	Hỗ trợ tính hóa đơn và xác nhận thanh toán.
•	Ghi nhận các thao tác nghiệp vụ quan trọng.
•	Vận dụng cấu trúc dữ liệu vào bài toán thực tế.
•	Phân tách các quy tắc nghiệp vụ để chương trình dễ bảo trì và mở rộng.
1.3. Phạm vi hệ thống
Hệ thống tập trung vào quy trình phục vụ khách tại bàn, bao gồm:
•	Quản lý thông tin bàn.
•	Mở và đóng DiningSession.
•	Quản lý menu cơ bản.
•	Tìm kiếm và xem thông tin món.
•	Gợi ý món ăn theo các quy tắc đã xác định.
•	Tạo và quản lý order.
•	Thêm món, xóa món và cập nhật số lượng.
•	Kiểm tra tính hợp lệ của order và tình trạng phục vụ của món.
•	Duyệt order.
•	Phân loại và chuyển công việc đến bếp/bar.
•	Theo dõi trạng thái chuẩn bị món.
•	Thông báo món hoàn thành.
•	Giao món cho khách.
•	Tạo hóa đơn và xác nhận thanh toán.
•	Ghi nhận lịch sử hoạt động.
Các chức năng như kết nối cổng thanh toán thực tế, quản lý kho chuyên sâu, quản lý nhân sự, báo cáo tài chính nâng cao và huấn luyện mô hình trí tuệ nhân tạo không thuộc phạm vi triển khai của phiên bản PBL.
1.4. Thuật ngữ
Thuật ngữ	Ý nghĩa
Table	Bàn phục vụ khách
MenuItem	Món ăn hoặc thức uống trong menu
DiningSession	Phiên phục vụ gắn với một bàn
Order	Đơn gọi món của khách
Order Node	Node lưu thông tin món trong Doubly Linked List của Order
KitchenTask	Công việc chuẩn bị món tại bếp hoặc bar
Bill	Hóa đơn thanh toán
Notification	Thông báo trong hệ thống
AuditAction	Bản ghi thao tác nghiệp vụ
Policy	Thành phần kiểm tra hoặc thực thi quy tắc nghiệp vụ
Deny Code	Mã lý do một yêu cầu bị từ chối
Queue	Cấu trúc hàng đợi theo nguyên tắc FIFO
Priority Queue	Hàng đợi ưu tiên
STL	Standard Template Library của C++
________________________________________
2. TÁC NHÂN HỆ THỐNG
2.1. Khách tại bàn
Khách có thể:
•	Xem menu.
•	Xem thông tin món ăn.
•	Nhận gợi ý món.
•	Lựa chọn món.
•	Thêm món vào order.
•	Thay đổi số lượng hoặc xóa món khi order còn cho phép chỉnh sửa.
•	Gửi order.
•	Yêu cầu thanh toán.
2.2. Cashier/Staff
Cashier hoặc Staff chịu trách nhiệm:
•	Mở bàn và tạo DiningSession.
•	Kiểm tra và duyệt order.
•	Xử lý các yêu cầu hủy món theo quy tắc nghiệp vụ.
•	Lập và kiểm tra hóa đơn.
•	Xác nhận thanh toán.
•	Đóng DiningSession sau khi đáp ứng các điều kiện cần thiết.
2.3. Waiter
Waiter chịu trách nhiệm:
•	Tiếp nhận thông báo món đã hoàn thành.
•	Nhận món từ bếp/bar.
•	Giao món cho khách.
•	Xử lý yêu cầu gọi nhân viên.
2.4. Kitchen/Bar
Kitchen/Bar chịu trách nhiệm:
•	Nhận KitchenTask.
•	Xem công việc đang chờ xử lý.
•	Bắt đầu chuẩn bị món.
•	Cập nhật trạng thái công việc.
•	Xác nhận món đã hoàn thành.
2.5. Manager
Manager có thể:
•	Thêm, sửa và ngừng kinh doanh món trong menu.
•	Cập nhật giá và thông tin món.
•	Xem các thông tin hoạt động cơ bản.
•	Kiểm tra lịch sử thao tác.
•	Điều chỉnh các quy tắc gợi ý món trong phạm vi chương trình.
2.6. System
System là thành phần điều phối xử lý nghiệp vụ, kiểm tra điều kiện, cập nhật trạng thái, tạo notification và ghi audit.
________________________________________
3. KIẾN TRÚC TỔNG QUAN
Hệ thống được tổ chức theo ba nhóm thành phần chính.
3.1. Nhóm dữ liệu nghiệp vụ
Các class dữ liệu chính bao gồm:
•	Table
•	MenuItem
•	DiningSession
•	Order
•	KitchenTask
•	Bill
•	Notification
•	AuditAction
•	User
Mỗi class chịu trách nhiệm lưu trữ thông tin của đối tượng tương ứng. Các class này được cài đặt trong thư mục `Models`; `RestaurantSystem` điều phối các đối tượng có giới hạn sức chứa và các Policy.
3.2. Nhóm cấu trúc dữ liệu
Các cấu trúc dữ liệu được lựa chọn theo đặc điểm của từng nghiệp vụ:
•	Mảng có sức chứa xác định: quản lý các đối tượng Table, DiningSession, User và tập KitchenTask còn tồn tại.
•	Mảng động đã sắp xếp: lưu MenuItem theo menuId và tìm kiếm bằng binary search.
•	Doubly Linked List: quản lý danh sách món bên trong Order.
•	Queue tự cài đặt: lưu taskId của KitchenTask thông thường theo thứ tự FIFO, tách riêng theo khu vực xử lý.
•	std::priority_queue: lưu taskId của KitchenTask ưu tiên, tách riêng theo khu vực xử lý.
•	std::queue: quản lý Notification theo thứ tự tạo.
•	std::stack: lưu AuditAction theo thứ tự phát sinh; truy xuất hoặc hiển thị lịch sử phải không làm mất bản ghi gốc.
3.3. Nhóm xử lý nghiệp vụ
Bao gồm:
•	RestaurantSystem
•	OrderingPolicy
•	InventoryPolicy
•	RecommendationPolicy
•	KitchenRoutingPolicy
•	PaymentPolicy
•	PolicyResult
Các Policy và interface/strategy được đặt trong thư mục `Policies`; PolicyResult trả thống nhất success, denyCode và message.
RestaurantSystem điều phối các thành phần và gọi Policy tương ứng. Những thao tác như tìm kiếm/chèn/xóa món trong Menu hoặc chèn/xóa node trong Doubly Linked List phải được xử lý bên trong class tương ứng, không triển khai lại trong RestaurantSystem.
3.4. Nguyên tắc thiết kế hướng đối tượng
•	Đóng gói: thuộc tính của đối tượng nghiệp vụ được đặt ở phạm vi private hoặc protected phù hợp. Trạng thái chỉ được thay đổi qua các phương thức có kiểm tra điều kiện; không cho phép module bên ngoài sửa trực tiếp dữ liệu nội bộ.
•	Trừu tượng hóa: mỗi class cung cấp các thao tác theo nghiệp vụ của nó và che giấu chi tiết cài đặt. RestaurantSystem điều phối; Policy kiểm tra quy tắc; cấu trúc dữ liệu chịu trách nhiệm về thao tác trên cấu trúc của mình.
•	Đa hình: RecommendationPolicy làm việc với interface trừu tượng IRecommendationStrategy. Tối thiểu có hai cách triển khai cụ thể, chẳng hạn CategoryRecommendationStrategy và PopularityRecommendationStrategy. RecommendationPolicy gọi chúng qua interface, không phụ thuộc vào lớp triển khai cụ thể.
•	Kế thừa chỉ được dùng khi lớp con có quan hệ “là một” hợp lý với lớp cha. Không tạo kế thừa chỉ để tăng số lượng lớp; các đối tượng Table, Order, Bill và DiningSession không kế thừa lẫn nhau.
•	IRecommendationStrategy khai báo phương thức recommend(...) const là phương thức thuần ảo và có destructor ảo. RecommendationPolicy giữ tham chiếu không sở hữu đến interface; RestaurantSystem chịu trách nhiệm bảo đảm vòng đời của chiến lược dài hơn thời gian sử dụng.
•	User lưu vai trò bằng kiểu dữ liệu Role; không bắt buộc tạo lớp con riêng cho từng vai trò nếu các vai trò chỉ khác nhau về quyền nghiệp vụ. Quyền thực hiện thao tác phải được kiểm tra tại lớp điều phối hoặc Policy tương ứng.
•	Không yêu cầu tất cả Policy phải kế thừa chung một lớp cơ sở. Chỉ bổ sung interface chung khi có nhu cầu đa hình thực tế.
________________________________________
4. LUỒNG NGHIỆP VỤ TỔNG QUÁT
Quy trình phục vụ chính được thực hiện như sau:
1.	Cashier/Staff chọn một bàn đang trống.
2.	System kiểm tra trạng thái bàn.
3.	System tạo DiningSession mới và cập nhật trạng thái bàn.
4.	Khách xem menu.
5.	System hiển thị món đang được phục vụ.
6.	RecommendationPolicy cung cấp gợi ý món phù hợp.
7.	Khách chọn món và thêm vào Order.
8.	Khách gửi Order.
9.	OrderingPolicy kiểm tra tính hợp lệ của Order.
10.	InventoryPolicy kiểm tra trạng thái các món.
11.	Cashier/Staff duyệt hoặc từ chối Order.
12.	Khi Order được duyệt, KitchenRoutingPolicy xác định khu vực xử lý.
13.	System tạo KitchenTask và đưa task vào hàng đợi phù hợp.
14.	Kitchen/Bar tiếp nhận và cập nhật trạng thái task.
15.	Khi món hoàn thành, System tạo Notification.
16.	Waiter nhận món và giao cho khách.
17.	Khách có thể tiếp tục gọi món trong phạm vi quy trình đặt món được hỗ trợ.
18.	Khi khách yêu cầu thanh toán, PaymentPolicy kiểm tra điều kiện thanh toán.
19.	System tạo Bill và tính tổng tiền.
20.	Cashier xác nhận thanh toán.
21.	System ghi AuditAction và đóng DiningSession.
22.	Bàn trở về trạng thái sẵn sàng phục vụ khách tiếp theo.
Mỗi lượt gọi món sau khi lượt trước đã được gửi duyệt phải tạo một Order mới thuộc cùng DiningSession. Order đã gửi hoặc được duyệt không được sửa trực tiếp; việc sửa/hủy sau thời điểm đó phải đi theo quy trình được kiểm soát. Bill tổng hợp các Order hợp lệ thuộc DiningSession.
________________________________________
5. YÊU CẦU CHỨC NĂNG
5.1. Quản lý bàn và phiên phục vụ
FR-01: Hiển thị danh sách bàn
System phải cho phép xem danh sách bàn, bao gồm mã bàn, sức chứa và trạng thái hiện tại.
FR-02: Mở bàn
System cho phép Cashier/Staff mở một bàn đang trống.
Khi mở bàn, System phải:
•	Kiểm tra bàn có tồn tại hay không.
•	Kiểm tra bàn chưa có phiên phục vụ đang hoạt động.
•	Tạo DiningSession mới.
•	Lưu mã session vào thông tin bàn.
•	Chuyển trạng thái bàn sang đang sử dụng.
•	Ghi nhận thao tác mở bàn.
FR-03: Tra cứu DiningSession
System cho phép tìm phiên phục vụ theo mã session hoặc mã bàn.
FR-04: Đóng DiningSession
System chỉ cho phép đóng session khi hóa đơn đã được thanh toán và không còn KitchenTask đang chờ, đang chuẩn bị hoặc đã sẵn sàng nhưng chưa giao. Task bị hủy hợp lệ không được tính là công việc chưa xử lý.
Khi đóng session:
•	Trạng thái session chuyển sang CLOSED.
•	Trạng thái bàn chuyển về AVAILABLE.
•	Thông tin lịch sử session được giữ lại trong phạm vi lưu trữ của chương trình.
5.2. Quản lý menu
FR-05: Hiển thị menu
System phải hiển thị các món đang được phục vụ cùng thông tin cơ bản:
•	Mã món.
•	Tên món.
•	Giá món.
•	Nhóm món.
•	Trạng thái phục vụ.
•	Khu vực chuẩn bị.
FR-06: Tìm kiếm món
System cho phép tìm món theo menuId bằng tìm kiếm nhị phân trên mảng MenuItem được sắp xếp tăng dần theo menuId. Kết quả tra cứu phải trả về đúng MenuItem tương ứng.
FR-07: Quản lý món
Manager có thể:
•	Thêm món mới.
•	Tìm món.
•	Cập nhật thông tin hoặc giá.
•	Ngừng phục vụ món.
•	Xóa món theo quy tắc của hệ thống.
Món đã xuất hiện trong order cũ không được làm thay đổi giá đã ghi nhận trong order đó chỉ vì giá menu hiện tại được cập nhật.
5.3. Gợi ý món
FR-08: Đưa ra gợi ý
RecommendationPolicy có thể gợi ý món dựa trên các quy tắc như:
•	Nhóm món.
•	Mức độ phổ biến.
•	Món thường được gọi cùng nhau.
•	Trạng thái đang được phục vụ.
FR-09: Xử lý lựa chọn gợi ý
Khách có thể chọn món được gợi ý để thêm vào Order. Món gợi ý vẫn phải trải qua kiểm tra nghiệp vụ như các món được chọn trực tiếp.
5.4. Quản lý Order
FR-10: Tạo Order
System tạo Order cho từng lượt gọi món trong một DiningSession. Một DiningSession có thể liên kết với nhiều Order; Order đã gửi duyệt được giữ lại để tính hóa đơn và tra cứu lịch sử.
FR-11: Thêm món
Khách có thể thêm món nếu:
•	DiningSession còn hoạt động.
•	Món tồn tại trong menu.
•	Món đang được phục vụ.
•	Số lượng hợp lệ.
FR-12: Cập nhật số lượng
Khách có thể thay đổi số lượng món khi Order còn cho phép chỉnh sửa.
Số lượng phải là số nguyên dương. Nếu khách muốn bỏ món, System sử dụng chức năng xóa món.
FR-13: Xóa món
Khách có thể xóa món trước khi Order được gửi duyệt. Sau khi Order đã gửi hoặc được duyệt, khách không được xóa trực tiếp; yêu cầu hủy phải được Staff/Cashier xử lý theo quy trình có kiểm soát và được ghi AuditAction.
Nếu KitchenTask tương ứng đã chuyển sang PREPARING hoặc READY, yêu cầu hủy món bị từ chối trong phạm vi phiên bản PBL. Chỉ task còn WAITING mới có thể được hủy theo xác nhận của Staff/Cashier.
FR-14: Tính tổng Order
System tự động tính tổng tiền từ các món trong Order:
Tổng tiền là số nguyên 64-bit, tính bằng tổng của (đơn giá nguyên theo đồng tại thời điểm đặt × số lượng). Phải kiểm tra tràn số trước khi cộng hoặc nhân.
FR-15: Gửi Order
System kiểm tra Order trước khi gửi cho nhân viên duyệt.
Order rỗng hoặc có dữ liệu không hợp lệ không được chấp nhận.
FR-16: Duyệt hoặc từ chối Order
Cashier/Staff có thể duyệt Order hợp lệ hoặc từ chối Order không hợp lệ.
Chỉ Order ở trạng thái PENDING mới được duyệt hoặc từ chối. System phải ghi nhận trạng thái và lý do từ chối nếu có. Chỉ Order được duyệt mới được chuyển đến bước tạo KitchenTask.
5.5. Xử lý bếp/bar
FR-17: Tạo KitchenTask
Sau khi Order được duyệt, KitchenRoutingPolicy phân loại các món và tạo công việc tương ứng.
Mỗi KitchenTask gồm:
•	Mã task.
•	Mã Order.
•	Mã món.
•	Tên món tại thời điểm tạo task.
•	Số lượng.
•	Khu vực xử lý.
•	Mức ưu tiên.
•	Trạng thái.
FR-18: Đưa task vào hàng đợi
Mỗi KitchenTask được đưa vào đúng một hàng đợi theo khu vực xử lý và mức ưu tiên: Queue tự cài đặt cho task thông thường hoặc std::priority_queue cho task ưu tiên. Các hàng đợi lưu taskId; thông tin và trạng thái hiện tại của task được quản lý tại một bản ghi KitchenTask duy nhất.
Trong mỗi khu vực xử lý, lấy tối đa ba task ưu tiên liên tiếp khi Queue thường vẫn còn task; sau đó phải lấy một task thường trước khi tiếp tục lấy task ưu tiên. Nếu Queue thường rỗng, có thể tiếp tục lấy task ưu tiên. Task thường được xử lý FIFO; task ưu tiên được sắp theo mức ưu tiên giảm dần, nếu bằng nhau thì theo thứ tự được đưa vào hàng đợi. Mức ưu tiên chỉ được đặt bởi quy tắc nghiệp vụ hoặc thao tác được phép, không tự động suy đoán.
FR-19: Cập nhật trạng thái task
Kitchen/Bar có thể chuyển trạng thái task theo trình tự:
WAITING → PREPARING → READY → SERVED.
System không cho phép chuyển trạng thái không hợp lệ theo quy tắc của chương trình.
FR-20: Thông báo món hoàn thành
Khi task chuyển sang READY, System tạo Notification cho Waiter.
5.6. Giao món
FR-21: Xem món sẵn sàng
Waiter có thể xem những món đã hoàn thành và chờ giao.
FR-22: Xác nhận giao món
Waiter xác nhận món đã được giao cho khách. System chuyển KitchenTask từ READY sang SERVED. Chỉ task READY mới được xác nhận giao.
FR-23: Yêu cầu gọi nhân viên
Khách có thể gửi yêu cầu gọi nhân viên. System tạo Notification cho Waiter hoặc Staff.
5.7. Thanh toán
FR-24: Yêu cầu thanh toán
Khách có thể yêu cầu thanh toán cho DiningSession đang hoạt động.
FR-25: Tạo Bill
PaymentPolicy tính tổng tiền từ các dòng món thuộc Order đã được duyệt trong phiên phục vụ, theo đơn giá được ghi nhận tại thời điểm đặt. Order bị từ chối và các dòng món bị hủy hợp lệ không được tính vào Bill. Chỉ cho phép xác nhận thanh toán khi không còn KitchenTask chưa hoàn tất hoặc chưa giao; nếu chưa đủ điều kiện, System phải thông báo lý do.
Bill lưu các thông tin cơ bản:
•	Mã Bill.
•	Mã DiningSession.
•	Tổng tiền.
•	Phương thức thanh toán.
•	Trạng thái thanh toán.
Một DiningSession chỉ có một Bill cuối cùng trong phiên bản PBL. Tổng tiền phải được tính lại từ dữ liệu Order hợp lệ, không nhận trực tiếp một tổng tiền tùy ý từ giao diện.
FR-26: Xác nhận thanh toán
Cashier/Staff xác nhận thanh toán sau khi kiểm tra hóa đơn.
Bill chỉ chuyển từ UNPAID sang PROCESSING khi bắt đầu mô phỏng thanh toán và chỉ chuyển sang PAID khi thao tác thành công. Nếu thất bại hoặc bị từ chối, Bill trở về UNPAID; DiningSession và Table không được đóng/cập nhật thành trạng thái hoàn tất.
FR-27: Hoàn tất phiên phục vụ
Sau khi thanh toán thành công và các điều kiện đóng session được đáp ứng, System đóng DiningSession và đặt bàn về trạng thái AVAILABLE.
Phiên bản PBL mô phỏng việc thanh toán trong chương trình, không yêu cầu kết nối cổng thanh toán thật.
5.8. Notification và Audit
FR-28: Tạo Notification
System tạo thông báo cho các sự kiện như:
•	Order được duyệt hoặc bị từ chối.
•	Món hoàn thành.
•	Khách gọi nhân viên.
•	Thanh toán thành công.
FR-29: Xử lý Notification
Notification được lưu trong hàng đợi và lấy ra theo thứ tự tạo.
FR-30: Ghi AuditAction
System ghi nhận các thao tác quan trọng như:
•	Mở và đóng bàn.
•	Tạo và duyệt Order.
•	Hủy món.
•	Cập nhật trạng thái KitchenTask.
•	Xác nhận thanh toán.
Mỗi bản ghi có thể chứa mã thao tác, người thực hiện, loại thao tác, mã đối tượng và mô tả.
________________________________________
6. THIẾT KẾ CÁC CLASS CHÍNH
6.1. Table
Table đại diện cho một bàn cụ thể.
Các thuộc tính chính:
•	tableId
•	capacity
•	status
•	currentSessionId
Các thao tác chính:
•	Lấy thông tin bàn.
•	Mở phiên phục vụ nếu bàn đang AVAILABLE và sessionId hợp lệ.
•	Đóng phiên phục vụ nếu sessionId khớp và các điều kiện đóng phiên được thỏa mãn.
Tập hợp Table được lưu trong mảng do RestaurantSystem quản lý. Class Table không cần tự chứa một cấu trúc dữ liệu khác.
capacity phải là số dương. Trạng thái và currentSessionId phải được cập nhật đồng bộ. Không cung cấp thao tác cho phép mã bên ngoài đặt tùy ý một trạng thái không hợp lệ.
6.2. MenuItem
MenuItem lưu thông tin của một món ăn hoặc thức uống.
Các thuộc tính chính:
•	menuId
•	name
•	price: số nguyên 64-bit theo đơn vị đồng Việt Nam
•	category
•	status
•	station
station xác định khu vực chuẩn bị (KITCHEN hoặc BAR). Dữ liệu bắt buộc và các giá trị enum không hợp lệ phải bị từ chối; không biến dữ liệu sai thành một món hợp lệ mặc định.
Menu quản lý các MenuItem trong một mảng động duy nhất, luôn sắp xếp tăng dần theo menuId. menuId là duy nhất và không thay đổi sau khi tạo món. Tìm theo menuId dùng binary search O(log n); tìm theo tên và duyệt hiển thị O(n). Mảng tăng dung lượng theo cấp số nhân khi đầy để chi phí thêm trung bình được khấu hao; chèn/xóa vẫn có chi phí O(n) do phải dịch chuyển phần tử.
6.3. Order
Order lưu mã order, sessionId, trạng thái, lý do từ chối nếu có và danh sách các món được đặt.
Danh sách món được cài đặt bằng Doubly Linked List template `DoublyLinkedList<T>`; khai báo và định nghĩa template nằm chung trong header để compiler có thể sinh mã khi sử dụng. Mỗi node riêng tư lưu một đối tượng `OrderItem` cùng liên kết prev/next; `OrderItem` đóng gói mã món, tên món, đơn giá nguyên 64-bit tại thời điểm đặt và số lượng. Mỗi menuId chỉ có tối đa một node trong cùng Order; thêm lại món đã có sẽ cập nhật số lượng thay vì tạo node trùng.
Các thao tác chính:
•	Thêm món.
•	Tìm món.
•	Cập nhật số lượng.
•	Xóa món.
•	Tính tổng tiền.
•	Hiển thị danh sách món.
Order chỉ cho phép sửa danh sách món khi còn ở trạng thái DRAFT. Chuyển trạng thái phải tuân theo luồng DRAFT → PENDING → APPROVED hoặc REJECTED; Order đã APPROVED không được chỉnh sửa âm thầm.
Order chuyển từ APPROVED sang PROCESSING khi bắt đầu xử lý bếp/bar và chỉ sang COMPLETED sau khi RestaurantSystem xác nhận tất cả KitchenTask của Order đã ở trạng thái SERVED hoặc CANCELLED hợp lệ. REJECTED là trạng thái kết thúc.
6.4. DiningSession
DiningSession lưu thông tin phiên phục vụ, gồm:
•	sessionId
•	tableId
•	status
•	Các Order thuộc phiên
DiningSession chỉ lưu tableId, không cần trực tiếp sở hữu một đối tượng Table hoặc include Table.h chỉ để quản lý quan hệ. RestaurantSystem chịu trách nhiệm kết nối session với bàn tương ứng.
Một DiningSession có thể có nhiều Order để hỗ trợ nhiều lượt gọi món. Order mới được tạo cho lượt gọi tiếp theo; các Order cũ được giữ nguyên để tính Bill và tra cứu. DiningSession quản lý vòng đời các Order của mình bằng một mảng con trỏ sở hữu; không cho tạo Order mới sau khi phiên chuyển sang PAYMENT_PENDING. RestaurantSystem phối hợp cập nhật DiningSession và Table để đảm bảo chúng luôn đồng bộ.
6.5. KitchenTask
KitchenTask lưu thông tin công việc tại bếp/bar, bao gồm mã task, mã order, món, số lượng, khu vực xử lý, mức ưu tiên, thứ tự tạo và trạng thái.
Tập KitchenTask đang tồn tại là nguồn dữ liệu chính do RestaurantSystem quản lý bằng mảng có sức chứa xác định. Queue thường và Priority Queue chỉ lưu taskId, không lưu bản sao task; mỗi task chỉ thuộc một hàng đợi tại một thời điểm. Khi lấy task ra xử lý, trạng thái được cập nhật trên bản ghi chính.
6.6. Bill
Bill lưu thông tin hóa đơn, tổng tiền số nguyên 64-bit theo đồng Việt Nam, phương thức và trạng thái thanh toán.
PaymentPolicy đảm nhận phần kiểm tra điều kiện và quy tắc thanh toán.
6.7. Notification
Notification lưu mã thông báo, loại thông báo, người nhận, nội dung và trạng thái xử lý.
Notification được quản lý theo thứ tự tạo bằng std::queue; khi lấy khỏi hàng đợi, trạng thái được đánh dấu DELIVERED.
6.8. AuditAction
AuditAction lưu thông tin thao tác nghiệp vụ để phục vụ kiểm tra.
Toàn bộ AuditAction trong thời gian chạy được giữ trong std::stack. Muốn xem hoặc tìm lịch sử, chương trình duyệt một bản sao của stack; không pop khỏi stack gốc khi chỉ đọc. Thao tác tìm kiếm lịch sử có độ phức tạp O(n).
6.9. User
User lưu thông tin cơ bản của người dùng, gồm mã người dùng, tên và vai trò. Phiên bản PBL có thể sử dụng mảng để quản lý tập hợp User.
6.10. RestaurantSystem
RestaurantSystem quản lý các tập Table, DiningSession, User và KitchenTask có giới hạn sức chứa; điều phối tạo Order, duyệt Order, định tuyến task, Notification, thanh toán và đóng phiên. Queue và Priority Queue chỉ lưu taskId, còn KitchenTask được giữ trong một tập bản ghi chính. Các Policy riêng thực hiện kiểm tra nghiệp vụ.
6.11. IRecommendationStrategy và các chiến lược gợi ý
IRecommendationStrategy là interface trừu tượng cho các thuật toán gợi ý, có destructor ảo và phương thức recommend(...) const thuần ảo.
CategoryRecommendationStrategy và PopularityRecommendationStrategy là các lớp triển khai cụ thể tối thiểu. Có thể bổ sung PairingRecommendationStrategy nếu dữ liệu và thời gian cho phép.
RecommendationPolicy gọi chiến lược qua IRecommendationStrategy. Mỗi chiến lược chỉ đề xuất món đang được phục vụ; kết quả vẫn phải được kiểm tra lại bởi OrderingPolicy và InventoryPolicy trước khi thêm vào Order.
________________________________________
7. PHÂN TÍCH CẤU TRÚC DỮ LIỆU
Cấu trúc dữ liệu	Hình thức triển khai	Ứng dụng	Thao tác chính	Độ phức tạp
Array	Mảng C++ có sức chứa xác định	Table	Truy cập theo chỉ số, tìm bàn	Truy cập O(1); tìm theo ID O(n) nếu phải duyệt
Array	Mảng C++ có sức chứa xác định	DiningSession, User, KitchenTask	Tìm, thêm, cập nhật bản ghi theo ID	O(1) truy cập theo chỉ số; tìm theo ID O(n)
Array động đã sắp xếp	Mảng MenuItem tăng dung lượng theo cấp số nhân	MenuItem theo menuId	Tìm kiếm nhị phân, chèn, xóa, duyệt	Tìm O(log n); chèn/xóa O(n); thêm cuối khấu hao O(1) khi mã mới lớn hơn mọi mã hiện có
Doubly Linked List	Tự cài đặt	Order Node	Thêm, tìm, cập nhật số lượng, xóa	Thêm đầu/cuối O(1) nếu có head/tail; tìm theo mã món O(n); xóa theo mã O(n)
Queue	Tự cài đặt	taskId thông thường theo khu vực xử lý	Thêm, lấy task kế tiếp, xem task đầu	O(1) nếu quản lý front/rear
Priority Queue	std::priority_queue	taskId ưu tiên theo khu vực xử lý	Thêm task, xem/lấy task ưu tiên cao nhất	Thêm O(log n), top O(1), lấy phần tử O(log n)
Queue	std::queue	Notification	Thêm và lấy thông báo theo thứ tự	O(1) cho các thao tác đầu/cuối theo giao diện container
Stack	std::stack	AuditAction và đọc lịch sử không phá hủy	Push, top, duyệt bản sao	O(1) push/top; đọc toàn bộ O(n)
7.1. Mảng động đã sắp xếp quản lý MenuItem
Mảng MenuItem là nguồn dữ liệu duy nhất của Menu và luôn sắp xếp tăng dần theo menuId. Tìm theo menuId sử dụng binary search O(log n); duyệt theo mã món O(n); tìm theo tên O(n). Chèn hoặc xóa một món cần dịch chuyển phần tử nên O(n). Khi mảng hết dung lượng, cấp phát mảng mới có dung lượng gấp đôi, sao chép phần tử và giải phóng vùng nhớ cũ; chi phí tăng dung lượng được khấu hao qua nhiều lần thêm.
Với menu quy mô nhỏ và tần suất đọc cao hơn cập nhật, phương án này đơn giản và tiết kiệm cấu trúc phụ. Mọi con trỏ/tham chiếu trả về cho MenuItem chỉ còn hợp lệ cho đến lần thêm làm tăng dung lượng hoặc lần xóa tiếp theo; bên gọi không được giữ chúng qua các thao tác đó.
7.2. Doubly Linked List quản lý Order
DoublyLinkedList<T> được cài đặt dạng template header-only; Node là kiểu nội bộ private, còn dữ liệu T được truy cập qua các phương thức có kiểm soát như pushFront, pushBack, popFront, popBack, findIf, removeIf, forEach và clear.
Danh sách hỗ trợ sao chép sâu và di chuyển quyền sở hữu để tránh sao chép nông con trỏ. Chèn đầu/cuối O(1); tìm theo menuId, cập nhật, xóa theo điều kiện O(n).
Order dùng DoublyLinkedList<OrderItem>. Giá được chụp tại thời điểm đặt để thay đổi giá MenuItem không làm thay đổi Order cũ.
Mỗi menuId chỉ xuất hiện một lần trong một Order; thêm lại cùng món cập nhật quantity. Chèn cuối là O(1), nhưng tìm/cập nhật/xóa theo menuId là O(n). Đây là lựa chọn đáp ứng yêu cầu cấu trúc dữ liệu của PBL và quy mô Order nhỏ; không được mô tả thao tác tìm/xóa theo mã là O(1).
Không cần tạo một module quản lý OrderItem độc lập nếu node bên trong Order đã đảm nhiệm đầy đủ việc lưu dữ liệu món.
7.3. Queue xử lý KitchenTask
Queue<T> tự cài đặt dạng template header-only trong `Queue.h`; phần khai báo lớp ở trên và phần định nghĩa phương thức ở dưới cùng file. Node là kiểu private, Queue sở hữu và giải phóng các node; copy thực hiện sao chép sâu, move chuyển quyền sở hữu. Queue dùng hai con trỏ đầu và cuối hàng, chứa taskId thay vì bản sao KitchenTask. Mỗi khu vực xử lý có các hàng đợi riêng để bếp/bar không lấy nhầm task của khu vực khác.
Các thao tác cần có:
•	enqueue()
•	dequeue()
•	front()
•	isEmpty()
•	size()
Không bắt buộc phải cung cấp hàm back() nếu nghiệp vụ không cần xem phần tử cuối hàng. Con trỏ nội bộ rear vẫn cần thiết để thêm phần tử cuối trong O(1).
RestaurantSystem áp dụng quy tắc FR-18 để chọn luân phiên giữa Queue thường và Priority Queue, tránh task thường bị đói khi liên tục có task ưu tiên.
7.4. Priority Queue xử lý task ưu tiên
Hệ thống dùng std::priority_queue của C++ thay vì tự cài Max Heap.
Mỗi phần tử hàng đợi ưu tiên chứa mức ưu tiên, thứ tự tạo tăng dần và taskId. Comparator lấy mức ưu tiên cao hơn trước; nếu bằng nhau, task được đưa vào trước được lấy trước. Priority Queue cũng được tách theo khu vực xử lý.
7.5. Stack quản lý AuditAction
Hệ thống sử dụng std::stack để lưu toàn bộ AuditAction theo LIFO. Khi xem lịch sử, tạo bản sao và pop trên bản sao; không xóa bản ghi khỏi stack gốc khi đọc. Dữ liệu audit được giữ trong phạm vi thời gian chạy của chương trình, không cam kết lưu bền vững sau khi thoát.
________________________________________
8. POLICY VÀ QUY TẮC NGHIỆP VỤ
8.1. OrderingPolicy
Chịu trách nhiệm kiểm tra:
•	DiningSession còn hoạt động.
•	Order có ít nhất một món.
•	Số lượng món hợp lệ.
•	Món tồn tại.
•	Order đang ở trạng thái cho phép gửi hoặc chỉnh sửa.
8.2. InventoryPolicy
Kiểm tra trạng thái phục vụ của từng món.
Món không tồn tại, hết hàng hoặc ngừng phục vụ không được chấp nhận trong Order.
Phiên bản PBL có thể mô phỏng trạng thái sẵn có của món; không bắt buộc xây dựng hệ thống quản lý tồn kho vật lý.
8.3. RecommendationPolicy
Đưa ra gợi ý bằng cách gọi một IRecommendationStrategy cụ thể. Có tối thiểu hai chiến lược triển khai để thể hiện đa hình. Việc gợi ý không được bỏ qua kiểm tra trạng thái phục vụ của món và kiểm tra nghiệp vụ trước khi thêm vào Order.
8.4. KitchenRoutingPolicy
Phân loại món theo trường station của MenuItem và tạo KitchenTask tương ứng cho bếp hoặc bar.
8.5. PaymentPolicy
Tính tổng Bill từ các Order được duyệt và kiểm tra điều kiện thanh toán. Session không được đóng nếu thanh toán thất bại, còn KitchenTask chưa giao hoặc còn công việc bắt buộc chưa hoàn thành.
8.6. PolicyResult
Các Policy trả kết quả theo một định dạng thống nhất, có thể gồm:
•	success
•	denyCode
•	message
Ví dụ mã từ chối:
Deny Code	Ý nghĩa
SESSION_NOT_ACTIVE	Phiên phục vụ không hoạt động
ITEM_NOT_FOUND	Không tìm thấy món
ITEM_UNAVAILABLE	Món không được phục vụ
EMPTY_ORDER	Order không có món
INVALID_QUANTITY	Số lượng không hợp lệ
PAYMENT_FAILED	Thanh toán thất bại
TABLE_UNAVAILABLE	Bàn không thể mở
________________________________________
9. TRẠNG THÁI CÁC ĐỐI TƯỢNG
9.1. Table
•	AVAILABLE: Bàn trống.
•	OCCUPIED: Bàn đang được sử dụng.
9.2. DiningSession
•	ACTIVE: Phiên đang phục vụ.
•	PAYMENT_PENDING: Phiên đang chờ thanh toán.
•	CLOSED: Phiên đã kết thúc.
9.3. Order
•	DRAFT: Order đang được chỉnh sửa.
•	PENDING: Order đã gửi và chờ duyệt.
•	APPROVED: Order đã được duyệt.
•	REJECTED: Order bị từ chối.
•	PROCESSING: Các món đang được chuẩn bị.
•	COMPLETED: Các món của Order đã được xử lý theo quy trình.
9.4. KitchenTask
•	WAITING: Chờ xử lý.
•	PREPARING: Đang chuẩn bị.
•	READY: Đã hoàn thành, chờ giao.
•	SERVED: Đã giao cho khách.
•	CANCELLED: Công việc đã bị hủy hợp lệ.
KitchenTask chỉ được chuyển sang CANCELLED khi còn ở WAITING và có xác nhận hủy hợp lệ. Task PREPARING, READY hoặc SERVED không được hủy trong phạm vi phiên bản PBL.
9.5. Bill
•	UNPAID: Chưa thanh toán.
•	PROCESSING: Đang xử lý thanh toán.
•	PAID: Đã thanh toán thành công.
________________________________________
10. YÊU CẦU PHI CHỨC NĂNG
10.1. Tính chính xác
•	Không cho phép sử dụng mã món không tồn tại.
•	Không chấp nhận số lượng bằng hoặc nhỏ hơn 0.
•	Không cho phép mở một bàn đang được sử dụng.
•	Không được đóng session trước khi thỏa mãn điều kiện thanh toán.
•	Tổng tiền phải được tính từ giá đã ghi nhận trong Order và số lượng tương ứng.
•	Chỉ các Order được duyệt mới được tính vào Bill; không được tính trùng một dòng món hoặc Order.
10.2. Tính nhất quán
•	Các đối tượng phải có trạng thái hợp lệ.
•	Các thao tác bị từ chối phải trả về nguyên nhân phù hợp.
•	Việc mở và đóng session phải cập nhật nhất quán thông tin giữa Table và DiningSession.
•	Việc tạo KitchenTask phải tuân thủ kết quả duyệt Order.
•	Mảng MenuItem luôn được sắp xếp theo menuId và không có mã trùng; sau mọi thao tác thêm/xóa, thứ tự phải được duy trì.
•	Order đã gửi duyệt hoặc đã duyệt không bị thay đổi trực tiếp; mỗi lượt gọi mới tạo Order riêng.
•	Chỉ thanh toán thành công mới cho phép đóng DiningSession và trả Table về AVAILABLE.
10.3. Hiệu năng
Hệ thống cần đáp ứng tốt quy mô dữ liệu kiểm thử của đồ án PBL. Mảng MenuItem đã sắp xếp phải hỗ trợ tìm theo mã O(log n); chèn/xóa O(n) được chấp nhận do quy mô menu nhỏ và ít thay đổi. Doubly Linked List, Queue và Priority Queue phải được sử dụng đúng đặc điểm và độ phức tạp thuật toán của chúng. Mảng có sức chứa cố định phải công bố giới hạn; thao tác thêm khi đầy phải trả lỗi rõ ràng, không ghi vượt giới hạn. Mảng MenuItem động được phép tăng dung lượng theo cấp số nhân và phải kiểm tra lỗi cấp phát.
10.4. Khả năng bảo trì
Chương trình phải được chia thành các file .h và .cpp phù hợp.
Cấu trúc dữ liệu, class dữ liệu, Policy và lớp điều phối phải tách biệt trách nhiệm.
Các phụ thuộc đa hình phải dựa trên interface/lớp trừu tượng thay vì phụ thuộc trực tiếp vào lớp triển khai cụ thể. Không đưa toàn bộ quy tắc nghiệp vụ vào RestaurantSystem.
10.5. Tính dễ sử dụng
Giao diện có thể triển khai bằng console. Menu chức năng phải rõ ràng, thông báo lỗi phải dễ hiểu và trạng thái bàn, Order, KitchenTask, Bill phải có cách hiển thị nhất quán.
10.6. Quản lý bộ nhớ
Các cấu trúc dữ liệu tự cài đặt sử dụng cấp phát động phải giải phóng bộ nhớ đúng cách. Class sở hữu con trỏ phải áp dụng quy tắc quản lý sao chép rõ ràng: hoặc cấm copy/assignment, hoặc triển khai sao chép sâu an toàn; không dùng sao chép nông mặc định. Các hàng đợi chỉ lưu taskId không sở hữu KitchenTask; một bản ghi task chính duy nhất do RestaurantSystem quản lý.
________________________________________
11. RÀNG BUỘC TRIỂN KHAI
•	Ngôn ngữ triển khai: C++.
•	Doubly Linked List và Queue được tự cài đặt.
•	DoublyLinkedList<T> và Queue<T> là template; khai báo lớp đặt phía trên, định nghĩa phương thức đặt phía dưới trong cùng file header tương ứng ở thư mục data_structures.
•	Priority Queue sử dụng std::priority_queue.
•	Stack sử dụng std::stack.
•	Notification có thể sử dụng std::queue.
•	Table và DiningSession dùng mảng trong phiên bản PBL.
•	Menu dùng một mảng động duy nhất, luôn sắp xếp theo menuId; tìm theo mã dùng binary search. Không sử dụng cấu trúc chỉ mục thứ hai cho MenuItem.
•	Mảng quản lý Table, DiningSession, User và KitchenTask phải có giới hạn dung lượng được xác định; mọi thao tác thêm phải kiểm tra đầy/tràn. Trong RestaurantSystem hiện tại, giới hạn lần lượt là 64 Table, 128 DiningSession, 128 User và 512 KitchenTask; hàng đợi Notification tối đa 512 mục.
•	Các cấu trúc STL chỉ được sử dụng khi phù hợp với yêu cầu của giảng viên.
•	Không sử dụng Singly Linked List để thay thế Doubly Linked List của Order.
•	Không triển khai Heap tự cài đặt trong phiên bản hiện tại.
•	Hệ thống thanh toán được mô phỏng trong chương trình, không bắt buộc kết nối dịch vụ bên ngoài.
•	Để tránh sai số dấu phẩy động khi tính tiền, giá và tổng tiền được lưu bằng số nguyên theo đơn vị tiền tệ nhỏ nhất được chọn (với VND có thể dùng đồng nguyên); không dùng số thực để cộng tiền.
•	Các giá trị tiền sử dụng thống nhất kiểu số nguyên 64-bit có dấu; phải từ chối phép tính nếu có nguy cơ tràn.
•	Queue thường tự cài đặt và std::priority_queue chỉ lưu taskId; thứ tự điều phối giữa hai hàng đợi tuân theo FR-18.
________________________________________
12. XỬ LÝ TRƯỜNG HỢP NGOẠI LỆ
Hệ thống phải xử lý ít nhất các trường hợp sau:
Mã	Trường hợp	Xử lý
E-01	Bàn đang được sử dụng	Từ chối mở bàn
E-02	Mã món không tồn tại	Không thêm món và thông báo lỗi
E-03	Món ngừng phục vụ	Từ chối Order có món đó
E-04	Order rỗng	Không cho gửi duyệt
E-05	Số lượng không hợp lệ	Từ chối cập nhật
E-06	Hủy món đã duyệt	Yêu cầu xử lý theo quy trình kiểm soát
E-07	Thanh toán thất bại	Bill giữ trạng thái chưa thanh toán
E-08	Đóng session khi còn công việc chưa hoàn thành	Từ chối đóng session
E-09	Không tìm thấy bản ghi	Trả về kết quả không tìm thấy, không truy cập con trỏ rỗng
E-10	Cấu trúc dữ liệu rỗng	Xử lý an toàn, không truy cập phần tử không tồn tại
E-11	Mảng đã đạt giới hạn	Từ chối thêm bản ghi và trả thông báo dung lượng đầy
E-12	Phép tính tiền có nguy cơ tràn	Từ chối tính/gửi thanh toán và báo lỗi; không ghi tổng tiền sai
E-13	Không tìm thấy taskId trong tập KitchenTask	Không xử lý task, báo lỗi nhất quán và không cập nhật trạng thái
________________________________________
13. KIẾN TRÚC MODULE TỔNG THỂ
                    RestaurantSystem
                            |
          ┌─────────────────┼──────────────────┐
          |                 |                  |
        Table              Menu              Session
       Array        Sorted dynamic array      Array
                            |                  |
                        MenuItem               |
                                               Order
                                                 |
                                      Doubly Linked List
                                                 |
                                            Order Node

                    RestaurantSystem
                            |
                     KitchenTask
                            |
                ┌───────────┴───────────┐
                |                       |
               Queue            std::priority_queue

                     Notification
                            |
                       std::queue

                     AuditAction
                            |
                       std::stack

                       Policy Layer
                            |
         ┌──────────────────┼───────────────────┐
         |                  |                   |
    OrderingPolicy   InventoryPolicy   KitchenRoutingPolicy
         |                                      |
 RecommendationPolicy                    PaymentPolicy
Menu sở hữu một mảng MenuItem đã sắp xếp theo menuId. KitchenTask là dữ liệu chính trong mảng; Queue và Priority Queue chỉ lưu taskId, được tra cứu trong tập KitchenTask khi lấy task để xử lý.
________________________________________
14. KẾ HOẠCH KIỂM THỬ
14.1. Kiểm thử cấu trúc dữ liệu
•	Mảng MenuItem đã sắp xếp: thêm món ở đầu/giữa/cuối, tìm kiếm nhị phân, xóa, duyệt theo mã, từ chối mã trùng và xác nhận thứ tự sau mỗi thao tác.
•	Doubly Linked List: thêm đầu/cuối, tìm món, cập nhật số lượng, xóa đầu/giữa/cuối và xóa danh sách rỗng.
•	Queue: enqueue, dequeue, front và thao tác trên Queue rỗng.
•	Priority Queue: kiểm tra task có ưu tiên cao nhất được lấy trước.
•	Stack: kiểm tra push, top, đọc lịch sử qua bản sao và xác nhận stack gốc không bị thay đổi.
•	Queue/Priority Queue: kiểm tra taskId chỉ xuất hiện ở một hàng đợi, tách theo station, thứ tự FIFO, tie-break ưu tiên và quy tắc tối đa ba task ưu tiên liên tiếp.
•	Mảng có giới hạn: kiểm tra thêm đến sức chứa tối đa, từ chối phần tử tiếp theo và giữ nguyên dữ liệu hiện có.
14.2. Kiểm thử nghiệp vụ
•	Mở bàn thành công và từ chối bàn đang sử dụng.
•	Tạo session cho bàn hợp lệ.
•	Tìm món tồn tại và không tồn tại.
•	Gửi Order hợp lệ và từ chối Order rỗng.
•	Gửi thêm lượt gọi mới sau khi Order trước đã duyệt; xác nhận Order cũ không bị thay đổi.
•	Từ chối món ngừng phục vụ.
•	Duyệt Order và tạo KitchenTask.
•	Cập nhật trạng thái KitchenTask.
•	Từ chối chuyển trạng thái KitchenTask sai trình tự và từ chối đóng session khi có task READY chưa giao.
•	Tạo Notification khi món hoàn thành.
•	Tính Bill chính xác.
•	Từ chối phép nhân/cộng tiền gây tràn số nguyên.
•	Không đóng session khi thanh toán thất bại.
•	Đóng session sau khi thanh toán thành công.
•	Ghi AuditAction cho các thao tác quan trọng.
•	Thanh toán thất bại không làm Bill chuyển sang PAID, không đóng DiningSession và không trả Table về AVAILABLE.
•	Thay đổi chiến lược gợi ý được qua IRecommendationStrategy và hai chiến lược cụ thể trả kết quả phù hợp.
•	Kiểm tra thêm MenuItem, tìm theo mã, duyệt theo mã, xóa, mở rộng dung lượng và xác nhận không có mã trùng hoặc thứ tự bị sai.
14.3. Kiểm thử luồng tích hợp
Một kịch bản kiểm thử đầy đủ:
1.	Staff mở bàn số 5.
2.	System tạo DiningSession.
3.	Khách tìm món Pizza theo mã.
4.	Khách thêm hai Pizza vào Order.
5.	OrderingPolicy và InventoryPolicy kiểm tra Order.
6.	Staff duyệt Order.
7.	KitchenRoutingPolicy tạo KitchenTask.
8.	Kitchen nhận task và chuyển sang PREPARING.
9.	Kitchen chuyển task sang READY.
10.	System tạo Notification cho Waiter.
11.	Waiter xác nhận giao món.
12.	Khách yêu cầu thanh toán.
13.	PaymentPolicy tính Bill.
14.	Cashier xác nhận thanh toán.
15.	System ghi AuditAction và đóng DiningSession.
16.	Bàn số 5 được đánh dấu AVAILABLE.
14.4. Kiểm thử thiết kế hướng đối tượng
•	Kiểm tra không thể thay đổi trực tiếp dữ liệu private của các class nghiệp vụ từ module bên ngoài.
•	Kiểm tra phương thức chuyển trạng thái từ chối các chuyển đổi không hợp lệ.
•	Cấu hình lần lượt CategoryRecommendationStrategy và PopularityRecommendationStrategy qua cùng IRecommendationStrategy; xác nhận RecommendationPolicy gọi đúng cách triển khai đang được cấu hình.
•	Kiểm tra RestaurantSystem điều phối Policy mà không tự cài lại thao tác nội bộ của Menu hoặc Doubly Linked List.
________________________________________
15. KẾT LUẬN
Hệ thống đặt món tại bàn cho nhà hàng Casual Dining được thiết kế nhằm mô phỏng các nghiệp vụ cốt lõi của quy trình phục vụ tại bàn. Kiến trúc hệ thống được phân chia thành các class dữ liệu, cấu trúc dữ liệu, Policy và lớp điều phối.
Trong phiên bản PBL, một mảng động đã sắp xếp là nguồn dữ liệu duy nhất của MenuItem và hỗ trợ tìm theo mã bằng binary search; Doubly Linked List quản lý các món trong Orders; Queue xử lý các công việc thông thường; std::priority_queue xử lý task ưu tiên; std::queue quản lý Notification; và std::stack lưu AuditAction trong thời gian chạy. Table và DiningSession được quản lý bằng mảng có giới hạn để giảm độ phức tạp triển khai.
MenuItem không bị lưu trùng trong các chỉ mục song song; các hàng đợi bếp/bar chỉ lưu taskId và tham chiếu đến tập KitchenTask chính. Queue thường và Priority Queue được điều phối theo quy tắc giới hạn số task ưu tiên liên tiếp. Giá và tổng tiền dùng số nguyên 64-bit; các mảng có sức chứa được công bố và kiểm tra khi thêm. std::stack lưu lịch sử trong thời gian chạy và được duyệt trên bản sao để không làm mất dữ liệu.
Việc tách riêng dữ liệu, cấu trúc dữ liệu và nghiệp vụ giúp chương trình dễ kiểm thử, dễ giải thích khi bảo vệ đồ án và thuận tiện mở rộng. SRS là cơ sở để triển khai các class, tích hợp cấu trúc dữ liệu, xây dựng Policy, hoàn thiện RestaurantSystem và kiểm thử toàn bộ quy trình phục vụ.
