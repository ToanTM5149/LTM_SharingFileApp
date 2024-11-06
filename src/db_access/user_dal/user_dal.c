#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <time.h>
#include "user_dal.h"

// Kết nối tới cơ sở dữ liệu
static sqlite3 *db;

int init_db() {
    if (sqlite3_open("file_sharing_app.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Không thể mở cơ sở dữ liệu: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    return 0;
}

// Chèn người dùng mới vào cơ sở dữ liệu
int insert_user(const char *username, const char *password_hash, const char *email) {
    const char *sql = "INSERT INTO Users (username, password_hash, email) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Không thể chuẩn bị truy vấn: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password_hash, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Không thể chèn người dùng: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    return 0;
}



// Tìm người dùng theo tên
User *find_user_by_username(const char *username) {
    const char *sql = "SELECT user_id, username, password_hash, email FROM Users WHERE username = ?;";
    sqlite3_stmt *stmt;
    User *user = NULL;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Không thể chuẩn bị truy vấn: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        user = malloc(sizeof(User));
        user->user_id = sqlite3_column_int(stmt, 0);
        user->username = strdup((const char *)sqlite3_column_text(stmt, 1));
        user->password_hash = strdup((const char *)sqlite3_column_text(stmt, 2));
        user->email = strdup((const char *)sqlite3_column_text(stmt, 3));
    }
    sqlite3_finalize(stmt);
    return user;
}

// Tạo một phiên đăng nhập mới
int create_session(int user_id) {
    const char *sql = "INSERT INTO Sessions (user_id, token, expires_at, last_active) VALUES (?, ?, datetime('now', '+1 hour'), datetime('now'));";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Không thể chuẩn bị truy vấn: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, generate_token(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Không thể tạo phiên: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    return 0;
}

// tao token dang nhap
char *generate_token() {
    static char token[21];
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int charset_size = strlen(charset);

    srand(time(NULL));
    for (int i = 0; i < 20; i++) {
        token[i] = charset[rand() % charset_size];
    }
    token[20] = '\0';
    return token;
}

// Hàm giả định để mã hóa mật khẩu
char *hash_password(const char *password) {
    // Mã hóa giả định, bạn có thể thay bằng thuật toán mã hóa thực tế như SHA-256
    return strdup(password);
}

// Kiểm tra mật khẩu
int check_password(const char *password, const char *hashed_password) {
    return strcmp(password, hashed_password) == 0;
}

// Giai phong bo nho
void free_user(User *user) {
    if (user != NULL) {
        free(user->username);
        free(user->password_hash);
        free(user->email);
        free(user);
    }
}
