#ifndef USER_DAL_H
#define USER_DAL_H

// Cấu trúc lưu trữ thông tin người dùng
typedef struct {
    int user_id;
    char *username;
    char *password_hash;
    char *email;
} User;

// Khởi tạo kết nối cơ sở dữ liệu
int init_db();

// Thêm người dùng mới
int insert_user(const char *username, const char *password_hash, const char *email);

// Tìm người dùng theo tên
User *find_user_by_username(const char *username);

// Tao token dang nhap
char *generate_token();

// Tạo phiên đăng nhập cho người dùng
int create_session(int user_id);

// Mã hóa mật khẩu
char *hash_password(const char *password);

// Kiểm tra mật khẩu
int check_password(const char *password, const char *hashed_password);

// Giải phóng bộ nhớ cho người dùng
void free_user(User *user);

#endif // USER_DAL_H
