#include "user_file_relations_info.h"
#include "sylar/core/log/log.h"

namespace rs {

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("orm");

UserFileRelationsInfo::UserFileRelationsInfo()
    :m_permission(1)
    ,m_id()
    ,m_userId()
    ,m_fileId()
    ,m_createdAt(time(0))
    ,m_updatedAt(time(0)) {
}

std::string UserFileRelationsInfo::toJsonString() const {
    Json::Value jvalue;
    jvalue["id"] = std::to_string(m_id);
    jvalue["user_id"] = std::to_string(m_userId);
    jvalue["file_id"] = std::to_string(m_fileId);
    jvalue["permission"] = m_permission;
    jvalue["created_at"] = sylar::Time2Str(m_createdAt);
    jvalue["updated_at"] = sylar::Time2Str(m_updatedAt);
    return sylar::JsonUtil::ToString(jvalue);
}

void UserFileRelationsInfo::setId(const int64_t& v) {
    m_id = v;
}

void UserFileRelationsInfo::setUserId(const int64_t& v) {
    m_userId = v;
}

void UserFileRelationsInfo::setFileId(const int64_t& v) {
    m_fileId = v;
}

void UserFileRelationsInfo::setPermission(const int32_t& v) {
    m_permission = v;
}

void UserFileRelationsInfo::setCreatedAt(const int64_t& v) {
    m_createdAt = v;
}

void UserFileRelationsInfo::setUpdatedAt(const int64_t& v) {
    m_updatedAt = v;
}


int UserFileRelationsInfoDao::Update(UserFileRelationsInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "update user_file_relations set user_id = ?, file_id = ?, permission = ?, created_at = ?, updated_at = ? where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_userId);
    stmt->bindInt64(2, info->m_fileId);
    stmt->bindInt32(3, info->m_permission);
    stmt->bindTime(4, info->m_createdAt);
    stmt->bindTime(5, info->m_updatedAt);
    stmt->bindInt64(6, info->m_id);
    return stmt->execute();
}

int UserFileRelationsInfoDao::Insert(UserFileRelationsInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "insert into user_file_relations (user_id, file_id, permission, created_at, updated_at) values (?, ?, ?, ?, ?)";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_userId);
    stmt->bindInt64(2, info->m_fileId);
    stmt->bindInt32(3, info->m_permission);
    stmt->bindTime(4, info->m_createdAt);
    stmt->bindTime(5, info->m_updatedAt);
    int rt = stmt->execute();
    if(rt == 0) {
        info->m_id = conn->getLastInsertId();
    }
    return rt;
}

int UserFileRelationsInfoDao::InsertOrUpdate(UserFileRelationsInfo::ptr info, sylar::IDB::ptr conn) {
    if(info->m_id == 0) {
        return Insert(info, conn);
    }
    std::string sql = "replace into user_file_relations (id, user_id, file_id, permission, created_at, updated_at) values (?, ?, ?, ?, ?, ?)";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_id);
    stmt->bindInt64(2, info->m_userId);
    stmt->bindInt64(3, info->m_fileId);
    stmt->bindInt32(4, info->m_permission);
    stmt->bindTime(5, info->m_createdAt);
    stmt->bindTime(6, info->m_updatedAt);
    return stmt->execute();
}

int UserFileRelationsInfoDao::Delete(UserFileRelationsInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "delete from user_file_relations where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_id);
    return stmt->execute();
}

int UserFileRelationsInfoDao::DeleteById( const int64_t& id, sylar::IDB::ptr conn) {
    std::string sql = "delete from user_file_relations where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, id);
    return stmt->execute();
}

int UserFileRelationsInfoDao::DeleteByUserId( const int64_t& user_id, sylar::IDB::ptr conn) {
    std::string sql = "delete from user_file_relations where user_id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, user_id);
    return stmt->execute();
}

int UserFileRelationsInfoDao::DeleteByFileId( const int64_t& file_id, sylar::IDB::ptr conn) {
    std::string sql = "delete from user_file_relations where file_id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, file_id);
    return stmt->execute();
}

int UserFileRelationsInfoDao::DeleteByUserIdFileId( const int64_t& user_id,  const int64_t& file_id, sylar::IDB::ptr conn) {
    std::string sql = "delete from user_file_relations where user_id = ? and file_id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, user_id);
    stmt->bindInt64(1, file_id);
    return stmt->execute();
}

int UserFileRelationsInfoDao::QueryAll(std::vector<UserFileRelationsInfo::ptr>& results, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, permission, created_at, updated_at from user_file_relations";
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
        UserFileRelationsInfo::ptr v(new UserFileRelationsInfo);
        v->m_id = rt->getInt64(0);
        v->m_userId = rt->getInt64(1);
        v->m_fileId = rt->getInt64(2);
        v->m_permission = rt->getInt32(3);
        v->m_createdAt = rt->getTime(4);
        v->m_updatedAt = rt->getTime(5);
        results.push_back(v);
    }
    return 0;
}

UserFileRelationsInfo::ptr UserFileRelationsInfoDao::Query( const int64_t& id, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, permission, created_at, updated_at from user_file_relations where id = ?";
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
    UserFileRelationsInfo::ptr v(new UserFileRelationsInfo);
    v->m_id = rt->getInt64(0);
    v->m_userId = rt->getInt64(1);
    v->m_fileId = rt->getInt64(2);
    v->m_permission = rt->getInt32(3);
    v->m_createdAt = rt->getTime(4);
    v->m_updatedAt = rt->getTime(5);
    return v;
}

int UserFileRelationsInfoDao::QueryByUserId(std::vector<UserFileRelationsInfo::ptr>& results,  const int64_t& user_id, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, permission, created_at, updated_at from user_file_relations where user_id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, user_id);
    auto rt = stmt->query();
    if(!rt) {
        return 0;
    }
    while (rt->next()) {
        UserFileRelationsInfo::ptr v(new UserFileRelationsInfo);
        v->m_id = rt->getInt64(0);
        v->m_userId = rt->getInt64(1);
        v->m_fileId = rt->getInt64(2);
        v->m_permission = rt->getInt32(3);
        v->m_createdAt = rt->getTime(4);
        v->m_updatedAt = rt->getTime(5);
        results.push_back(v);
    };
    return 0;
}

int UserFileRelationsInfoDao::QueryByFileId(std::vector<UserFileRelationsInfo::ptr>& results,  const int64_t& file_id, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, permission, created_at, updated_at from user_file_relations where file_id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, file_id);
    auto rt = stmt->query();
    if(!rt) {
        return 0;
    }
    while (rt->next()) {
        UserFileRelationsInfo::ptr v(new UserFileRelationsInfo);
        v->m_id = rt->getInt64(0);
        v->m_userId = rt->getInt64(1);
        v->m_fileId = rt->getInt64(2);
        v->m_permission = rt->getInt32(3);
        v->m_createdAt = rt->getTime(4);
        v->m_updatedAt = rt->getTime(5);
        results.push_back(v);
    };
    return 0;
}

UserFileRelationsInfo::ptr UserFileRelationsInfoDao::QueryByUserIdFileId( const int64_t& user_id,  const int64_t& file_id, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, permission, created_at, updated_at from user_file_relations where user_id = ? and file_id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return nullptr;
    }
    stmt->bindInt64(1, user_id);
    stmt->bindInt64(2, file_id);
    auto rt = stmt->query();
    if(!rt) {
        return nullptr;
    }
    if(!rt->next()) {
        return nullptr;
    }
    UserFileRelationsInfo::ptr v(new UserFileRelationsInfo);
    v->m_id = rt->getInt64(0);
    v->m_userId = rt->getInt64(1);
    v->m_fileId = rt->getInt64(2);
    v->m_permission = rt->getInt32(3);
    v->m_createdAt = rt->getTime(4);
    v->m_updatedAt = rt->getTime(5);
    return v;
}

int UserFileRelationsInfoDao::CreateTableSQLite3(sylar::IDB::ptr conn) {
    return conn->execute("CREATE TABLE user_file_relations("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "user_id INTEGER NOT NULL DEFAULT 0,"
            "file_id INTEGER NOT NULL DEFAULT 0,"
            "permission INTEGER NOT NULL DEFAULT 1,"
            "created_at TIMESTAMP NOT NULL DEFAULT current_timestamp,"
            "updated_at TIMESTAMP NOT NULL DEFAULT current_timestamp);"
            "CREATE INDEX user_file_relations_user_id ON user_file_relations(user_id);"
            "CREATE INDEX user_file_relations_file_id ON user_file_relations(file_id);"
            "CREATE UNIQUE INDEX user_file_relations_user_id_file_id ON user_file_relations(user_id,file_id);"
            );
}

int UserFileRelationsInfoDao::CreateTableMySQL(sylar::IDB::ptr conn) {
    return conn->execute("CREATE TABLE user_file_relations("
            "`id` bigint AUTO_INCREMENT COMMENT '关联ID',"
            "`user_id` bigint NOT NULL DEFAULT 0 COMMENT '用户ID',"
            "`file_id` bigint NOT NULL DEFAULT 0 COMMENT '文件ID',"
            "`permission` int NOT NULL DEFAULT 1 COMMENT '权限(1-只读,2-读写,3-完全控制)',"
            "`created_at` timestamp NOT NULL DEFAULT current_timestamp COMMENT '创建时间',"
            "`updated_at` timestamp NOT NULL DEFAULT current_timestamp ON UPDATE current_timestamp  COMMENT '更新时间',"
            "PRIMARY KEY(`id`),"
            "KEY `user_file_relations_user_id` (`user_id`),"
            "KEY `user_file_relations_file_id` (`file_id`),"
            "UNIQUE KEY `user_file_relations_user_id_file_id` (`user_id`,`file_id`))");
}
} //namespace rs
