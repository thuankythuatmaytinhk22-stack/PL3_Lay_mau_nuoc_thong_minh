# 🌊 Smart Water Sampling System (IoT-Advanced)
> **Giải pháp lấy mẫu nước tự động tích hợp giám sát vị trí GIS và quản lý lịch trình thông minh.**

Hệ thống lấy mẫu nước thông minh được thiết kế nhằm hiện đại hóa quy trình thu thập mẫu nước tại hiện trường. Thay vì vận hành thủ công, người dùng có thể điều khiển từ xa qua Dashboard, thiết lập lịch trình tự động và giám sát trực quan vị trí trạm bơm trên bản đồ vệ tinh.



## 🏗 Kiến trúc hệ thống
Dự án sử dụng mô hình **Client-Server-Edge**:
* **Edge Device (ESP32):** Thu thập dữ liệu cảm biến siêu âm, điều khiển Relay, hiển thị LCD và thực thi thuật toán ngắt an toàn tại chỗ.
* **Cloud/Server (PHP & MySQL):** Trung tâm xử lý dữ liệu, lưu trữ lịch trình (Scheduler) và quản lý nhật ký hệ thống (Logs).
* **Web Dashboard (Frontend):** Giao diện tương tác người dùng, tích hợp **Leaflet.js** cho bản đồ và **AJAX** để cập nhật trạng thái thời gian thực.



## 🚀 Tính năng nổi bật

### 1. Điều khiển đa chế độ (Control Modes)
* **Manual Control:** Bật/Tắt các bơm trực tiếp từ giao diện Web.
* **Smart Scheduler:** Lập lịch chạy bơm theo giờ cố định với thời lượng (giây) tùy chỉnh. 
* **Batch Action:** Hỗ trợ điều khiển đồng thời hoặc riêng lẻ từng bơm.

### 2. Giám sát GIS (Bản đồ số)
* Hiển thị vị trí trạm bơm trên nền tảng **OpenStreetMap**.
* **Drag & Drop:** Cho phép kéo thả marker trên bản đồ để cập nhật tọa độ thực tế của trạm bơm về hệ thống ngay lập tức.

### 3. Cơ chế An toàn & Bảo mật
* **Hardware Failsafe:** ESP32 tự động ngắt bơm khi mực nước chạm ngưỡng an toàn (`MAX_LEVEL`) bất kể lệnh từ Server.
* **Role-based Access:** Phân quyền Admin quản trị để bảo mật hệ thống điều khiển.

## 🔌 Đặc tả kết nối (Pinout)

| Linh kiện | Chân ESP32 | Chức năng |
| :--- | :--- | :--- |
| **Relay 1** | GPIO 5 | Điều khiển Bơm 1 |
| **Relay 2** | GPIO 18 | Điều khiển Bơm 2 |
| **Cảm biến Siêu âm (Trig)** | GPIO 3 | Phát tín hiệu đo mức nước |
| **Cảm biến Siêu âm (Echo)** | GPIO 2 | Nhận tín hiệu phản hồi |
| **LCD 16x2 (SDA/SCL)** | GPIO 6 / 7 | Hiển thị thông số tại chỗ |

## 🌐 API Specifications

* `GET /get_command.php`: ESP32 lấy lệnh điều khiển và ngưỡng MAX từ Server.
* `GET /get_command.php?status=full`: ESP32 báo cáo trạng thái đầy nước để Server đồng bộ giao diện.
* `POST /update_coords.php`: Cập nhật tọa độ (Lat/Lng) từ bản đồ Web vào Database.

## 🛠 Hướng dẫn triển khai

### 1. Yêu cầu hệ thống
* **Hardware:** ESP32 Dev Kit, HC-SR04, Module Relay 2 kênh, LCD I2C.
* **Software:** XAMPP (PHP 7.4+, MySQL), Arduino IDE.

### 2. Cài đặt
1.  **Database:** Import file SQL vào PHPMyAdmin để tạo các bảng `pump_schedule`, `pump_locations`, và `logs`.
2.  **Web Server:** Copy thư mục code vào `htdocs`, cấu hình thông tin trong `database.php`.
3.  **Firmware:** Mở file `.ino`, cập nhật thông tin WiFi và IP Server, sau đó nạp vào ESP32.

## 👥 Thành viên thực hiện
* **Đinh Hoàng Thuận**
* **Nguyễn Quốc Khánh**
* **Trần Kiêm Quang Minh**
* **Hồ Anh Nguyên**

---
*Dự án được phát triển cho mục đích nghiên cứu và quản lý tài nguyên nước.*
