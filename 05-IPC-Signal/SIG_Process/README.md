Câu hỏi: Tại sao chúng ta cần sử dụng kill() để gửi tín hiệu trong bài này?

=> process cha sử dụng kill(child_pid, SIGUSR1) để gửi tín hiệu SIGUSR1 đến process con.
Tín hiệu này được xử lý bởi hàm signal_handler(), nơi biến đếm signal_count được tăng lên và thông báo sẽ được in ra.
Điều này cho phép process con phản ứng lại với tín hiệu từ process cha.
