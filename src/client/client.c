#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"  // Địa chỉ IP của server (localhost)
#define PORT 8080              // Cổng server đang lắng nghe

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    
    // Tạo socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Chuyển đổi địa chỉ IP từ text sang binary
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported \n");
        return -1;
    }

    // Kết nối tới server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed \n");
        return -1;
    }

    printf("Đã kết nối tới server.\n");

    // Vòng lặp để gửi và nhận dữ liệu
    while (1) {
        printf("Nhập tin nhắn để gửi tới server (gõ 'exit' để thoát): ");
        fgets(buffer, sizeof(buffer), stdin);
        
        // Xóa ký tự xuống dòng
        buffer[strcspn(buffer, "\n")] = 0;

        // Kiểm tra điều kiện thoát
        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        // Gửi dữ liệu tới server
        send(sock, buffer, strlen(buffer), 0);

        // Nhận phản hồi từ server
        int valread = read(sock, buffer, sizeof(buffer) - 1);
        if (valread > 0) {
            buffer[valread] = '\0'; // Kết thúc chuỗi
            printf("Phản hồi từ server: %s\n", buffer);
        } else {
            printf("Không nhận được phản hồi từ server.\n");
            break;
        }

        // Xóa buffer
        memset(buffer, 0, sizeof(buffer));
    }

    printf("Đã ngắt kết nối khỏi server.\n");
    close(sock);
    return 0;
}
