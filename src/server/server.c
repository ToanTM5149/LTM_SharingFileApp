#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10

void *handle_client(void *client_socket);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Tạo socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Thiết lập địa chỉ server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Gán địa chỉ cho socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Lắng nghe kết nối
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server đang lắng nghe trên cổng %d...\n", PORT);

    while (1) {
        // Chấp nhận kết nối từ client
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }
        printf("Kết nối mới từ client\n");

        // Tạo một luồng mới để xử lý client
        pthread_t thread_id;
        int *pclient = malloc(sizeof(int));
        *pclient = new_socket;
        pthread_create(&thread_id, NULL, handle_client, pclient);
        pthread_detach(thread_id); // Tách luồng để tự giải phóng sau khi hoàn thành
    }

    close(server_fd);
    return 0;
}

// Hàm xử lý client
void *handle_client(void *client_socket) {
    int sock = *((int *)client_socket);
    free(client_socket);

    char buffer[1024] = {0};
    int bytes_read;

    // Nhận và gửi dữ liệu từ/to client
    while ((bytes_read = read(sock, buffer, sizeof(buffer))) > 0) {
        printf("Nhận từ client: %s\n", buffer);

        // Gửi phản hồi
        send(sock, buffer, bytes_read, 0);
        memset(buffer, 0, sizeof(buffer)); // Xóa buffer sau mỗi lần gửi
    }

    printf("Client ngắt kết nối\n");
    close(sock);
    return NULL;
}
