#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

// Đăng ký người dùng
int register_user(const char *username, const char *password, const char *email);

// Đăng nhập người dùng
int login_user(const char *username, const char *password);

#endif // USER_CONTROLLER_H
