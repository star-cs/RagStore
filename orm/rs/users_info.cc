#include "users_info.h"
#include "sylar/core/log/log.h"

namespace rs {

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("orm");

UsersInfo::UsersInfo()
    :m_status(1)
    ,m_id()
    ,m_totalStorage("0")
    ,m_usedStorage("0")
    ,m_username()
    ,m_passwordHash()
    ,m_email()
    ,m_createdAt(time(0))
    ,m_updatedAt(time(0)) {
}

std::string UsersInfo::toJsonString() const {
    Json::Value jvalue;
    jvalue["id"] = std::to_string(m_id);
    jvalue["username"] = m_username;
    jvalue["password_hash"] = m_passwordHash;
    jvalue["email"] = m_email;
    jvalue["total_storage"] = std::to_string(m_totalStorage);
    jvalue["used_storage"] = std::to_string(m_usedStorage);
    jvalue["status"] = m_status;
    jvalue["created_at"] = sylar::Time2Str(m_createdAt);
    jvalue["updated_at"] = sylar::Time2Str(m_updatedAt);
    return sylar::JsonUtil::ToString(jvalue);
}

void UsersInfo::setId(const int64_t& v) {
    m_id = v;
}

void UsersInfo::setUsername(const std::string& v) {
    m_username = v;
}

void UsersInfo::setPasswordHash(const std::string& v) {
    m_passwordHash = v;
}

void UsersInfo::setEmail(const std::string& v) {
    m_email = v;
}

void UsersInfo::setTotalStorage(const int64_t& v) {
    m_totalStorage = v;
}

void UsersInfo::setUsedStorage(const int64_t& v) {
    m_usedStorage = v;
}

void UsersInfo::setStatus(const int32_t& v) {
    m_status = v;
}

void UsersInfo::setCreatedAt(const int64_t& v) {
    m_createdAt = v;
}

void UsersInfo::setUpdatedAt(const int64_t& v) {
    m_updatedAt = v;
}


int UsersInfoDao::Update(UsersInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "update users set username = ?, password_hash = ?, email = ?, total_storage = ?, used_storage = ?, status = ?, created_at = ?, updated_at = ? where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, info->m_username);
    stmt->bindString(2, info->m_passwordHash);
    stmt->bindString(3, info->m_email);
    stmt->bindInt64(4, info->m_totalStorage);
    stmt->bindInt64(5, info->m_usedStorage);
    stmt->bindInt32(6, info->m_status);
    stmt->bindTime(7, info->m_createdAt);
    stmt->bindTime(8, info->m_updatedAt);
    stmt->bindInt64(9, info->m_id);
    return stmt->execute();
}

int UsersInfoDao::Insert(UsersInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "insert into users (username, password_hash, email, total_storage, used_storage, status, created_at, updated_at) values (?, ?, ?, ?, ?, ?, ?, ?)";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, info->m_username);
    stmt->bindString(2, info->m_passwordHash);
    stmt->bindString(3, info->m_email);
    stmt->bindInt64(4, info->m_totalStorage);
    stmt->bindInt64(5, info->m_usedStorage);
    stmt->bindInt32(6, info->m_status);
    stmt->bindTime(7, info->m_createdAt);
    stmt->bindTime(8, info->m_updatedAt);
    int rt = stmt->execute();
    if(rt == 0) {
        info->m_id = conn->getLastInsertId();
    }
    return rt;
}

int UsersInfoDao::InsertOrUpdate(UsersInfo::ptr info, sylar::IDB::ptr conn) {
    if(info->m_id == 0) {
        return Insert(info, conn);
    }
    std::string sql = "replace into users (id, username, password_hash, email, total_storage, used_storage, status, created_at, updated_at) values (?, ?, ?, ?, ?, ?, ?, ?, ?)";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_id);
    stmt->bindString(2, info->m_username);
    stmt->bindString(3, info->m_passwordHash);
    stmt->bindString(4, info->m_email);
    stmt->bindInt64(5, info->m_totalStorage);
    stmt->bindInt64(6, info->m_usedStorage);
    stmt->bindInt32(7, info->m_status);
    stmt->bindTime(8, info->m_createdAt);
    stmt->bindTime(9, info->m_updatedAt);
    return stmt->execute();
}

int UsersInfoDao::Delete(UsersInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "delete from users where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_id);
    return stmt->execute();
}

int UsersInfoDao::DeleteById( const int64_t& id, sylar::IDB::ptr conn) {
    std::string sql = "delete from users where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, id);
    return stmt->execute();
}

int UsersInfoDao::DeleteByUsername( const std::string& username, sylar::IDB::ptr conn) {
    std::string sql = "delete from users where username = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, username);
    return stmt->execute();
}

int UsersInfoDao::DeleteByEmail( const std::string& email, sylar::IDB::ptr conn) {
    std::string sql = "delete from users where email = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, email);
    return stmt->execute();
}

int UsersInfoDao::QueryAll(std::vector<UsersInfo::ptr>& results, sylar::IDB::ptr conn) {
    std::string sql = "select id, username, password_hash, email, total_storage, used_storage, status, created_at, updated_at from users";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    auto rt = stmt->query();
    if(!rt) {
        return stmt->getErrno();
    }
    while (rt->next()) {
        UsersInfo::ptr v(new UsersInfo);
        v->m_id = rt->getInt64(0);
        v->m_username = rt->getString(1);
        v->m_passwordHash = rt->getString(2);
        v->m_email = rt->getString(3);
        v->m_totalStorage = rt->getInt64(4);
        v->m_usedStorage = rt->getInt64(5);
        v->m_status = rt->getInt32(6);
        v->m_createdAt = rt->getTime(7);
        v->m_updatedAt = rt->getTime(8);
        results.push_back(v);
    }
    return 0;
}

UsersInfo::ptr UsersInfoDao::Query( const int64_t& id, sylar::IDB::ptr conn) {
    std::string sql = "select id, username, password_hash, email, total_storage, used_storage, status, created_at, updated_at from users where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return nullptr;
    }
    stmt->bindInt64(1, id);
    auto rt = stmt->query();
    if(!rt) {
        return nullptr;
    }
    if(!rt->next()) {
        return nullptr;
    }
    UsersInfo::ptr v(new UsersInfo);
    v->m_id = rt->getInt64(0);
    v->m_username = rt->getString(1);
    v->m_passwordHash = rt->getString(2);
    v->m_email = rt->getString(3);
    v->m_totalStorage = rt->getInt64(4);
    v->m_usedStorage = rt->getInt64(5);
    v->m_status = rt->getInt32(6);
    v->m_createdAt = rt->getTime(7);
    v->m_updatedAt = rt->getTime(8);
    return v;
}

UsersInfo::ptr UsersInfoDao::QueryByUsername( const std::string& username, sylar::IDB::ptr conn) {
    std::string sql = "select id, username, password_hash, email, total_storage, used_storage, status, created_at, updated_at from users where username = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return nullptr;
    }
    stmt->bindString(1, username);
    auto rt = stmt->query();
    if(!rt) {
        return nullptr;
    }
    if(!rt->next()) {
        return nullptr;
    }
    UsersInfo::ptr v(new UsersInfo);
    v->m_id = rt->getInt64(0);
    v->m_username = rt->getString(1);
    v->m_passwordHash = rt->getString(2);
    v->m_email = rt->getString(3);
    v->m_totalStorage = rt->getInt64(4);
    v->m_usedStorage = rt->getInt64(5);
    v->m_status = rt->getInt32(6);
    v->m_createdAt = rt->getTime(7);
    v->m_updatedAt = rt->getTime(8);
    return v;
}

UsersInfo::ptr UsersInfoDao::QueryByEmail( const std::string& email, sylar::IDB::ptr conn) {
    std::string sql = "select id, username, password_hash, email, total_storage, used_storage, status, created_at, updated_at from users where email = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return nullptr;
    }
    stmt->bindString(1, email);
    auto rt = stmt->query();
    if(!rt) {
        return nullptr;
    }
    if(!rt->next()) {
        return nullptr;
    }
    UsersInfo::ptr v(new UsersInfo);
    v->m_id = rt->getInt64(0);
    v->m_username = rt->getString(1);
    v->m_passwordHash = rt->getString(2);
    v->m_email = rt->getString(3);
    v->m_totalStorage = rt->getInt64(4);
    v->m_usedStorage = rt->getInt64(5);
    v->m_status = rt->getInt32(6);
    v->m_createdAt = rt->getTime(7);
    v->m_updatedAt = rt->getTime(8);
    return v;
}

int UsersInfoDao::CreateTableSQLite3(sylar::IDB::ptr conn) {
    return conn->execute("CREATE TABLE users("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "username TEXT NOT NULL DEFAULT '',"
            "password_hash TEXT NOT NULL DEFAULT '',"
            "email TEXT NOT NULL DEFAULT '',"
            "total_storage INTEGER NOT NULL DEFAULT 0,"
            "used_storage INTEGER NOT NULL DEFAULT 0,"
            "status INTEGER NOT NULL DEFAULT 1,"
            "created_at TIMESTAMP NOT NULL DEFAULT current_timestamp,"
            "updated_at TIMESTAMP NOT NULL DEFAULT current_timestamp);"
            "CREATE UNIQUE INDEX users_username ON users(username);"
            "CREATE UNIQUE INDEX users_email ON users(email);"
            );
}

int UsersInfoDao::CreateTableMySQL(sylar::IDB::ptr conn) {
    return conn->execute("CREATE TABLE users("
            "`id` bigint AUTO_INCREMENT COMMENT '用户ID',"
            "`username` varchar(128) NOT NULL DEFAULT '' COMMENT '用户名',"
            "`password_hash` varchar(128) NOT NULL DEFAULT '' COMMENT '密码哈希',"
            "`email` varchar(128) NOT NULL DEFAULT '' COMMENT '用户邮箱',"
            "`total_storage` bigint NOT NULL DEFAULT 0 COMMENT '总存储空间(字节)',"
            "`used_storage` bigint NOT NULL DEFAULT 0 COMMENT '已使用存储空间(字节)',"
            "`status` int NOT NULL DEFAULT 1 COMMENT '状态(0-禁用,1-正常,2-删除)',"
            "`created_at` timestamp NOT NULL DEFAULT current_timestamp COMMENT '创建时间',"
            "`updated_at` timestamp NOT NULL DEFAULT current_timestamp ON UPDATE current_timestamp  COMMENT '更新时间',"
            "PRIMARY KEY(`id`),"
            "UNIQUE KEY `users_username` (`username`),"
            "UNIQUE KEY `users_email` (`email`))");
}
} //namespace rs
