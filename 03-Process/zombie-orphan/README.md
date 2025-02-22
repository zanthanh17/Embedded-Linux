Giải thích trạng thái zombie và orphan, cùng lý do tại sao chúng xuất hiện trong Linux.

=> Zombie: Con chết, cha không wait(), còn xác trong hệ thống.
Orphan: Cha chết, con còn sống, init nhận nuôi.
Cách xử lý: Zombie có thể gây vấn đề nếu không được dọn dẹp, Orphan thì thường không ảnh hưởng.
