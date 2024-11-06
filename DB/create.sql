-- Tạo bảng Users
CREATE TABLE IF NOT EXISTS Users (
    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    password_hash TEXT NOT NULL,
    email TEXT,
    role TEXT CHECK(role IN ('admin', 'user')) NOT NULL DEFAULT 'user',
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Tạo bảng Sessions
CREATE TABLE IF NOT EXISTS Sessions (
    session_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER,
    token TEXT NOT NULL,
    expires_at DATETIME,
    last_active DATETIME,
    FOREIGN KEY(user_id) REFERENCES Users(user_id)
);

-- Tạo bảng Files
CREATE TABLE IF NOT EXISTS Files (
    file_id INTEGER PRIMARY KEY AUTOINCREMENT,
    owner_id INTEGER,
    file_name TEXT NOT NULL,
    file_size INTEGER,
    file_path TEXT NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    is_directory BOOLEAN NOT NULL DEFAULT 0,
    FOREIGN KEY(owner_id) REFERENCES Users(user_id)
);

-- Tạo bảng Folders
CREATE TABLE IF NOT EXISTS Folders (
    folder_id INTEGER PRIMARY KEY AUTOINCREMENT,
    parent_id INTEGER,
    owner_id INTEGER,
    folder_name TEXT NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(parent_id) REFERENCES Folders(folder_id),
    FOREIGN KEY(owner_id) REFERENCES Users(user_id)
);

-- Tạo bảng FilePermissions
CREATE TABLE IF NOT EXISTS FilePermissions (
    permission_id INTEGER PRIMARY KEY AUTOINCREMENT,
    file_id INTEGER,
    user_id INTEGER,
    can_read BOOLEAN DEFAULT 1,
    can_write BOOLEAN DEFAULT 0,
    can_delete BOOLEAN DEFAULT 0,
    is_owner BOOLEAN DEFAULT 0,
    FOREIGN KEY(file_id) REFERENCES Files(file_id),
    FOREIGN KEY(user_id) REFERENCES Users(user_id)
);

-- Tạo bảng FileTransfers
CREATE TABLE IF NOT EXISTS FileTransfers (
    transfer_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER,
    file_id INTEGER,
    transfer_type TEXT CHECK(transfer_type IN ('upload', 'download')) NOT NULL,
    transfer_status TEXT CHECK(transfer_status IN ('pending', 'completed', 'failed')) DEFAULT 'pending',
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_id) REFERENCES Users(user_id),
    FOREIGN KEY(file_id) REFERENCES Files(file_id)
);

-- Tạo bảng Logs
CREATE TABLE IF NOT EXISTS Logs (
    log_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER,
    action TEXT NOT NULL,
    file_id INTEGER,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    details TEXT,
    FOREIGN KEY(user_id) REFERENCES Users(user_id)
);

-- Tạo bảng SearchHistory
CREATE TABLE IF NOT EXISTS SearchHistory (
    search_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER,
    search_query TEXT NOT NULL,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_id) REFERENCES Users(user_id)
);

-- Tạo bảng SharedFiles
CREATE TABLE IF NOT EXISTS SharedFiles (
    share_id INTEGER PRIMARY KEY AUTOINCREMENT,
    file_id INTEGER,
    owner_id INTEGER,
    shared_with INTEGER,
    share_type TEXT CHECK(share_type IN ('read', 'write')) NOT NULL DEFAULT 'read',
    expiration DATETIME,
    FOREIGN KEY(file_id) REFERENCES Files(file_id),
    FOREIGN KEY(owner_id) REFERENCES Users(user_id),
    FOREIGN KEY(shared_with) REFERENCES Users(user_id)
);
