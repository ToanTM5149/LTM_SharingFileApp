#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

int execute_sql_from_file(sqlite3 *db, const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "Không thể mở tệp: %s\n", file_path);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *sql = malloc(length + 1);
    fread(sql, 1, length, file);
    sql[length] = '\0';

    fclose(file);

    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Lỗi SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
        free(sql);
        return rc;
    }

    free(sql);
    return 0;
}

int main() {
    sqlite3 *db;
    int rc = sqlite3_open("file_sharing_app.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Không thể mở cơ sở dữ liệu: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    if (execute_sql_from_file(db, "create.sql") != 0) {
        fprintf(stderr, "Lỗi khi thực thi tệp create.sql\n");
    } else {
        printf("Cơ sở dữ liệu và các bảng đã được tạo thành công.\n");
    }

    sqlite3_close(db);
    return 0;
}
