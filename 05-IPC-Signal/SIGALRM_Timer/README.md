Câu hỏi: Điều gì xảy ra nếu không gọi lại alarm(1) trong hàm xử lý?

=> Nếu không gọi lại alarm(1) trong hàm xử lý, chương trình sẽ chỉ thực hiện việc tăng biến đếm second_count và in ra thông báo một lần duy nhất khi tín hiệu SIGALRM được nhận lần đầu tiên.
Sau khi in ra thông báo, chương trình sẽ không thiết lập alarm nữa. Do đó, không có tín hiệu SIGALRM nào được gửi đến chương trình nữa.
