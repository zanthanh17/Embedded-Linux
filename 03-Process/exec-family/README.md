Giải thích điều gì xảy ra với tiến trình ban đầu sau khi exec được gọi.

=> khi execlp() được gọi, tiến trình con sẽ bị thay thế hoàn toàn bởi chương trình mới (ls hoặc date).
Tiến trình con không quay trở lại đoạn mã ban đầu sau exec, trừ khi exec thất bại.
Tiến trình cha vẫn tiếp tục hoạt động và đợi tiến trình con kết thúc.
