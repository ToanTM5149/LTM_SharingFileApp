#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user_controller.h"
#include "../../db_access/user_dal/user_dal.h"

// Đăng ký người dùng mới
int register_user(const char *username, const char *password, const char *email) {
    // Kiểm tra xem người dùng đã tồn tại chưa
    if (find_user_by_username(username) != NULL) {
        printf("Tên người dùng đã tồn tại.\n");
        return -1;
    }

    // Mã hóa mật khẩu
    char *hashed_password = hash_password(password);

    // Lưu người dùng mới vào cơ sở dữ liệu
    int result = insert_user(username, hashed_password, email);
    free(hashed_password);
    
    if (result == 0) {
        printf("Đăng ký thành công.\n");
    } else {
        printf("Đăng ký thất bại.\n");
    }
    return result;
}

// Đăng nhập người dùng
int login_user(const char *username, const char *password) {
    User *user = find_user_by_username(username);

    if (user == NULL) {
        printf("Người dùng không tồn tại.\n");
        return -1;
    }

    // Kiểm tra mật khẩu
    if (check_password(password, user->password_hash)) {
        // Tạo phiên đăng nhập
        create_session(user->user_id);
        printf("Đăng nhập thành công.\n");
        free_user(user);
        return 0;
    } else {
        printf("Mật khẩu không đúng.\n");
        free_user(user);
        return -1;
    }
}
