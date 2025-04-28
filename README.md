MÔ TẢ HỆ THỐNG GIÁM SÁT VÀ ỔN ĐỊNH NHIỆT ĐỘ

1. Mục đích:

-Hệ thống được thiết kế để liên tục theo dõi nhiệt độ của một đối tượng hoặc một vật thể. Mục tiêu chính là duy trì nhiệt độ của đối tượng không vượt quá một ngưỡng giới hạn đã được xác định trước, bằng cách tự động kích hoạt cơ chế quạt làm mát và đồng thời phát tín hiệu cảnh báo bằng buzzer khi nhiệt độ vượt ngưỡng.

2. Nguyên lý hoạt động:
-Đo lường liên tục: Cảm biến nhiệt độ được đặt gần hoặc tiếp xúc với đối tượng cần giám sát, liên tục đo đạc giá trị nhiệt độ hiện tại.

Gửi dữ liệu: Dữ liệu nhiệt độ đo được sẽ được gửi về xử lí bởi vi điều khiển 

So sánh ngưỡng: Bộ vi điều khiển so sánh giá trị nhiệt độ thực tế (T_actual) với giá trị ngưỡng nhiệt độ an toàn đã được cài đặt trước (T_threshold).

Quyết định và Hành động:

Nếu T_actual > T_threshold (Nhiệt độ vượt ngưỡng):

-Bộ vi điều khiển gửi tín hiệu kích hoạt relay điều khiển quạt

-Quạt làm mát bắt đầu hoạt động, thổi luồng khí để giảm nhiệt độ của đối tượng.

-Đồng thời, bộ vi điều khiển gửi tín hiệu kích hoạt còi báo (buzzer), phát ra âm thanh để cảnh báo người dùng về tình trạng quá nhiệt.

Nếu T_actual ≤ T_threshold (Nhiệt độ trong ngưỡng an toàn):

-Bộ vi điều khiển gửi tín hiệu ngắt (hoặc không kích hoạt) module điều khiển quạt.

-Quạt ngừng hoạt động hoặc không chạy.

-Bộ vi điều khiển gửi tín hiệu ngắt (hoặc không kích hoạt) còi báo.

-Còi báo im lặng.

Chu kỳ lặp lại: Quá trình đo lường, so sánh và điều khiển này được lặp đi lặp lại liên tục theo một chu kỳ thời gian ngắn, đảm bảo hệ thống phản ứng nhanh chóng với sự thay đổi nhiệt độ.

3. Thành phần chính:
-Cảm biến nhiệt độ: Thiết bị đo nhiệt độ (Thermistor NTC)

-Bộ vi điều khiển / Mạch điều khiển: Trung tâm xử lý tín hiệu, so sánh và ra quyết định (PIC 16877A)

-Quạt làm mát: Thiết bị tạo luồng gió để tản nhiệt (quạt DC 12V)

-Còi báo (Buzzer): Thiết bị phát ra âm thanh cảnh báo khi có tín hiệu kích hoạt.

-Nguồn cấp: Sử dụng Adapter AC-DC 220V sang 5V để cấp nguồn cho mạch hoạt động

-Màn hình hiển thị: Hiển thị giá trị nhiệt độ hiện tại và trạng thái hệ thống.

-Nút nhấn (4 nút): Cài đặt nhiệt độ ngưỡng 

4. Chức năng chính:

-Giám sát nhiệt độ thời gian thực.

-So sánh nhiệt độ đo được với ngưỡng cài đặt.

-Tự động bật quạt làm mát khi nhiệt độ vượt ngưỡng.

-Tự động tắt quạt khi nhiệt độ trở về mức an toàn.

-Phát cảnh báo bằng âm thanh (còi báo) khi nhiệt độ vượt ngưỡng.

-Tự động tắt cảnh báo khi nhiệt độ trở về mức an toàn.

5. Ứng dụng tiềm năng:

-Làm mát linh kiện điện tử công suất cao (CPU, GPU, bộ nguồn).

-Ổn định nhiệt độ trong tủ điện, tủ rack server.

-Kiểm soát nhiệt độ trong các dự án DIY, mô hình thí nghiệm.

-Kiểm soát nhiệt độ trong nhà kính hoặc chuồng trại quy mô nhỏ.
