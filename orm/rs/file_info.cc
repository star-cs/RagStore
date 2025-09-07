#include "file_info.h"
#include "sylar/core/log/log.h"

namespace rs {

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("orm");

FileInfo::FileInfo()
    :m_id()
    ,m_fileSize()
    ,m_user()
    ,m_md5()
    ,m_fileName()
    ,m_fileUrl()
    ,m_fileType()
    ,m_createTime(time(0))
    ,m_updateTime(time(0)) {
}

std::string FileInfo::toJsonString() const {
    Json::Value jvalue;
    jvalue["id"] = std::to_string(m_id);
    jvalue["user"] = m_user;
    jvalue["md5"] = m_md5;
    jvalue["file_name"] = m_fileName;
    jvalue["file_size"] = std::to_string(m_fileSize);
    jvalue["file_url"] = m_fileUrl;
    jvalue["file_type"] = m_fileType;
    jvalue["create_time"] = sylar::Time2Str(m_createTime);
    jvalue["update_time"] = sylar::Time2Str(m_updateTime);
    return sylar::JsonUtil::ToString(jvalue);
}

void FileInfo::setId(const int64_t& v) {
    m_id = v;
}

void FileInfo::setUser(const std::string& v) {
    m_user = v;
}

void FileInfo::setMd5(const std::string& v) {
    m_md5 = v;
}

void FileInfo::setFileName(const std::string& v) {
    m_fileName = v;
}

void FileInfo::setFileSize(const int64_t& v) {
    m_fileSize = v;
}

void FileInfo::setFileUrl(const std::string& v) {
    m_fileUrl = v;
}

void FileInfo::setFileType(const std::string& v) {
    m_fileType = v;
}

void FileInfo::setCreateTime(const int64_t& v) {
    m_createTime = v;
}

void FileInfo::setUpdateTime(const int64_t& v) {
    m_updateTime = v;
}


int FileInfoDao::Update(FileInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "update file set user = ?, md5 = ?, file_name = ?, file_size = ?, file_url = ?, file_type = ?, create_time = ?, update_time = ? where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, info->m_user);
    stmt->bindString(2, info->m_md5);
    stmt->bindString(3, info->m_fileName);
    stmt->bindInt64(4, info->m_fileSize);
    stmt->bindString(5, info->m_fileUrl);
    stmt->bindString(6, info->m_fileType);
    stmt->bindTime(7, info->m_createTime);
    stmt->bindTime(8, info->m_updateTime);
    stmt->bindInt64(9, info->m_id);
    return stmt->execute();
}

int FileInfoDao::Insert(FileInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "insert into file (user, md5, file_name, file_size, file_url, file_type, create_time, update_time) values (?, ?, ?, ?, ?, ?, ?, ?)";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, info->m_user);
    stmt->bindString(2, info->m_md5);
    stmt->bindString(3, info->m_fileName);
    stmt->bindInt64(4, info->m_fileSize);
    stmt->bindString(5, info->m_fileUrl);
    stmt->bindString(6, info->m_fileType);
    stmt->bindTime(7, info->m_createTime);
    stmt->bindTime(8, info->m_updateTime);
    int rt = stmt->execute();
    if(rt == 0) {
        info->m_id = conn->getLastInsertId();
    }
    return rt;
}

int FileInfoDao::InsertOrUpdate(FileInfo::ptr info, sylar::IDB::ptr conn) {
    if(info->m_id == 0) {
        return Insert(info, conn);
    }
    std::string sql = "replace into file (id, user, md5, file_name, file_size, file_url, file_type, create_time, update_time) values (?, ?, ?, ?, ?, ?, ?, ?, ?)";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_id);
    stmt->bindString(2, info->m_user);
    stmt->bindString(3, info->m_md5);
    stmt->bindString(4, info->m_fileName);
    stmt->bindInt64(5, info->m_fileSize);
    stmt->bindString(6, info->m_fileUrl);
    stmt->bindString(7, info->m_fileType);
    stmt->bindTime(8, info->m_createTime);
    stmt->bindTime(9, info->m_updateTime);
    return stmt->execute();
}

int FileInfoDao::Delete(FileInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "delete from file where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_id);
    return stmt->execute();
}

int FileInfoDao::DeleteById( const int64_t& id, sylar::IDB::ptr conn) {
    std::string sql = "delete from file where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, id);
    return stmt->execute();
}

int FileInfoDao::DeleteByUser( const std::string& user, sylar::IDB::ptr conn) {
    std::string sql = "delete from file where user = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, user);
    return stmt->execute();
}

int FileInfoDao::DeleteByFileName( const std::string& file_name, sylar::IDB::ptr conn) {
    std::string sql = "delete from file where file_name = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, file_name);
    return stmt->execute();
}

int FileInfoDao::DeleteByFileType( const std::string& file_type, sylar::IDB::ptr conn) {
    std::string sql = "delete from file where file_type = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, file_type);
    return stmt->execute();
}

int FileInfoDao::DeleteByMd5( const std::string& md5, sylar::IDB::ptr conn) {
    std::string sql = "delete from file where md5 = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, md5);
    return stmt->execute();
}

int FileInfoDao::DeleteByFileUrl( const std::string& file_url, sylar::IDB::ptr conn) {
    std::string sql = "delete from file where file_url = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, file_url);
    return stmt->execute();
}

int FileInfoDao::QueryAll(std::vector<FileInfo::ptr>& results, sylar::IDB::ptr conn) {
    std::string sql = "select id, user, md5, file_name, file_size, file_url, file_type, create_time, update_time from file";
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
        FileInfo::ptr v(new FileInfo);
        v->m_id = rt->getInt64(0);
        v->m_user = rt->getString(1);
        v->m_md5 = rt->getString(2);
        v->m_fileName = rt->getString(3);
        v->m_fileSize = rt->getInt64(4);
        v->m_fileUrl = rt->getString(5);
        v->m_fileType = rt->getString(6);
        v->m_createTime = rt->getTime(7);
        v->m_updateTime = rt->getTime(8);
        results.push_back(v);
    }
    return 0;
}

FileInfo::ptr FileInfoDao::Query( const int64_t& id, sylar::IDB::ptr conn) {
    std::string sql = "select id, user, md5, file_name, file_size, file_url, file_type, create_time, update_time from file where id = ?";
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
    FileInfo::ptr v(new FileInfo);
    v->m_id = rt->getInt64(0);
    v->m_user = rt->getString(1);
    v->m_md5 = rt->getString(2);
    v->m_fileName = rt->getString(3);
    v->m_fileSize = rt->getInt64(4);
    v->m_fileUrl = rt->getString(5);
    v->m_fileType = rt->getString(6);
    v->m_createTime = rt->getTime(7);
    v->m_updateTime = rt->getTime(8);
    return v;
}

int FileInfoDao::QueryByUser(std::vector<FileInfo::ptr>& results,  const std::string& user, sylar::IDB::ptr conn) {
    std::string sql = "select id, user, md5, file_name, file_size, file_url, file_type, create_time, update_time from file where user = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, user);
    auto rt = stmt->query();
    if(!rt) {
        return 0;
    }
    while (rt->next()) {
        FileInfo::ptr v(new FileInfo);
        v->m_id = rt->getInt64(0);
        v->m_user = rt->getString(1);
        v->m_md5 = rt->getString(2);
        v->m_fileName = rt->getString(3);
        v->m_fileSize = rt->getInt64(4);
        v->m_fileUrl = rt->getString(5);
        v->m_fileType = rt->getString(6);
        v->m_createTime = rt->getTime(7);
        v->m_updateTime = rt->getTime(8);
        results.push_back(v);
    };
    return 0;
}

int FileInfoDao::QueryByFileName(std::vector<FileInfo::ptr>& results,  const std::string& file_name, sylar::IDB::ptr conn) {
    std::string sql = "select id, user, md5, file_name, file_size, file_url, file_type, create_time, update_time from file where file_name = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, file_name);
    auto rt = stmt->query();
    if(!rt) {
        return 0;
    }
    while (rt->next()) {
        FileInfo::ptr v(new FileInfo);
        v->m_id = rt->getInt64(0);
        v->m_user = rt->getString(1);
        v->m_md5 = rt->getString(2);
        v->m_fileName = rt->getString(3);
        v->m_fileSize = rt->getInt64(4);
        v->m_fileUrl = rt->getString(5);
        v->m_fileType = rt->getString(6);
        v->m_createTime = rt->getTime(7);
        v->m_updateTime = rt->getTime(8);
        results.push_back(v);
    };
    return 0;
}

int FileInfoDao::QueryByFileType(std::vector<FileInfo::ptr>& results,  const std::string& file_type, sylar::IDB::ptr conn) {
    std::string sql = "select id, user, md5, file_name, file_size, file_url, file_type, create_time, update_time from file where file_type = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, file_type);
    auto rt = stmt->query();
    if(!rt) {
        return 0;
    }
    while (rt->next()) {
        FileInfo::ptr v(new FileInfo);
        v->m_id = rt->getInt64(0);
        v->m_user = rt->getString(1);
        v->m_md5 = rt->getString(2);
        v->m_fileName = rt->getString(3);
        v->m_fileSize = rt->getInt64(4);
        v->m_fileUrl = rt->getString(5);
        v->m_fileType = rt->getString(6);
        v->m_createTime = rt->getTime(7);
        v->m_updateTime = rt->getTime(8);
        results.push_back(v);
    };
    return 0;
}

FileInfo::ptr FileInfoDao::QueryByMd5( const std::string& md5, sylar::IDB::ptr conn) {
    std::string sql = "select id, user, md5, file_name, file_size, file_url, file_type, create_time, update_time from file where md5 = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return nullptr;
    }
    stmt->bindString(1, md5);
    auto rt = stmt->query();
    if(!rt) {
        return nullptr;
    }
    if(!rt->next()) {
        return nullptr;
    }
    FileInfo::ptr v(new FileInfo);
    v->m_id = rt->getInt64(0);
    v->m_user = rt->getString(1);
    v->m_md5 = rt->getString(2);
    v->m_fileName = rt->getString(3);
    v->m_fileSize = rt->getInt64(4);
    v->m_fileUrl = rt->getString(5);
    v->m_fileType = rt->getString(6);
    v->m_createTime = rt->getTime(7);
    v->m_updateTime = rt->getTime(8);
    return v;
}

FileInfo::ptr FileInfoDao::QueryByFileUrl( const std::string& file_url, sylar::IDB::ptr conn) {
    std::string sql = "select id, user, md5, file_name, file_size, file_url, file_type, create_time, update_time from file where file_url = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return nullptr;
    }
    stmt->bindString(1, file_url);
    auto rt = stmt->query();
    if(!rt) {
        return nullptr;
    }
    if(!rt->next()) {
        return nullptr;
    }
    FileInfo::ptr v(new FileInfo);
    v->m_id = rt->getInt64(0);
    v->m_user = rt->getString(1);
    v->m_md5 = rt->getString(2);
    v->m_fileName = rt->getString(3);
    v->m_fileSize = rt->getInt64(4);
    v->m_fileUrl = rt->getString(5);
    v->m_fileType = rt->getString(6);
    v->m_createTime = rt->getTime(7);
    v->m_updateTime = rt->getTime(8);
    return v;
}

int FileInfoDao::CreateTableSQLite3(sylar::IDB::ptr conn) {
    return conn->execute("CREATE TABLE file("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "user TEXT NOT NULL DEFAULT '',"
            "md5 TEXT NOT NULL DEFAULT '',"
            "file_name TEXT NOT NULL DEFAULT '',"
            "file_size INTEGER NOT NULL DEFAULT 0,"
            "file_url TEXT NOT NULL DEFAULT '',"
            "file_type TEXT NOT NULL DEFAULT '',"
            "create_time TIMESTAMP NOT NULL DEFAULT current_timestamp,"
            "update_time TIMESTAMP NOT NULL DEFAULT current_timestamp);"
            "CREATE INDEX file_user ON file(user);"
            "CREATE INDEX file_file_name ON file(file_name);"
            "CREATE INDEX file_file_type ON file(file_type);"
            "CREATE UNIQUE INDEX file_md5 ON file(md5);"
            "CREATE UNIQUE INDEX file_file_url ON file(file_url);"
            );
}

int FileInfoDao::CreateTableMySQL(sylar::IDB::ptr conn) {
    return conn->execute("CREATE TABLE file("
            "`id` bigint AUTO_INCREMENT COMMENT '唯一主键',"
            "`user` varchar(128) NOT NULL DEFAULT '' COMMENT '文件所属用户',"
            "`md5` varchar(128) NOT NULL DEFAULT '' COMMENT 'md5',"
            "`file_name` varchar(128) NOT NULL DEFAULT '' COMMENT '文件名',"
            "`file_size` bigint NOT NULL DEFAULT 0 COMMENT '文件大小',"
            "`file_url` varchar(128) NOT NULL DEFAULT '' COMMENT '文件url',"
            "`file_type` varchar(128) NOT NULL DEFAULT '' COMMENT '文件类型',"
            "`create_time` timestamp NOT NULL DEFAULT current_timestamp,"
            "`update_time` timestamp NOT NULL DEFAULT current_timestamp ON UPDATE current_timestamp ,"
            "PRIMARY KEY(`id`),"
            "KEY `file_user` (`user`),"
            "KEY `file_file_name` (`file_name`),"
            "KEY `file_file_type` (`file_type`),"
            "UNIQUE KEY `file_md5` (`md5`),"
            "UNIQUE KEY `file_file_url` (`file_url`))");
}
} //namespace rs
