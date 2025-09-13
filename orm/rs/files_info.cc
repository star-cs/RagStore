#include "files_info.h"
#include "sylar/core/log/log.h"

namespace rs {

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("orm");

FilesInfo::FilesInfo()
    :m_status(0)
    ,m_id()
    ,m_fileSize()
    ,m_fileName()
    ,m_filePath()
    ,m_minioObjectName()
    ,m_minioBucket()
    ,m_minioUploadId()
    ,m_fileType()
    ,m_md5Hash()
    ,m_createdAt(time(0))
    ,m_updatedAt(time(0)) {
}

std::string FilesInfo::toJsonString() const {
    Json::Value jvalue;
    jvalue["id"] = std::to_string(m_id);
    jvalue["file_name"] = m_fileName;
    jvalue["file_path"] = m_filePath;
    jvalue["minio_object_name"] = m_minioObjectName;
    jvalue["minio_bucket"] = m_minioBucket;
    jvalue["minio_upload_id"] = m_minioUploadId;
    jvalue["file_size"] = std::to_string(m_fileSize);
    jvalue["file_type"] = m_fileType;
    jvalue["md5_hash"] = m_md5Hash;
    jvalue["status"] = m_status;
    jvalue["created_at"] = sylar::Time2Str(m_createdAt);
    jvalue["updated_at"] = sylar::Time2Str(m_updatedAt);
    return sylar::JsonUtil::ToString(jvalue);
}

void FilesInfo::setId(const int64_t& v) {
    m_id = v;
}

void FilesInfo::setFileName(const std::string& v) {
    m_fileName = v;
}

void FilesInfo::setFilePath(const std::string& v) {
    m_filePath = v;
}

void FilesInfo::setMinioObjectName(const std::string& v) {
    m_minioObjectName = v;
}

void FilesInfo::setMinioBucket(const std::string& v) {
    m_minioBucket = v;
}

void FilesInfo::setMinioUploadId(const std::string& v) {
    m_minioUploadId = v;
}

void FilesInfo::setFileSize(const int64_t& v) {
    m_fileSize = v;
}

void FilesInfo::setFileType(const std::string& v) {
    m_fileType = v;
}

void FilesInfo::setMd5Hash(const std::string& v) {
    m_md5Hash = v;
}

void FilesInfo::setStatus(const int32_t& v) {
    m_status = v;
}

void FilesInfo::setCreatedAt(const int64_t& v) {
    m_createdAt = v;
}

void FilesInfo::setUpdatedAt(const int64_t& v) {
    m_updatedAt = v;
}


int FilesInfoDao::Update(FilesInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "update files set file_name = ?, file_path = ?, minio_object_name = ?, minio_bucket = ?, minio_upload_id = ?, file_size = ?, file_type = ?, md5_hash = ?, status = ?, created_at = ?, updated_at = ? where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, info->m_fileName);
    stmt->bindString(2, info->m_filePath);
    stmt->bindString(3, info->m_minioObjectName);
    stmt->bindString(4, info->m_minioBucket);
    stmt->bindString(5, info->m_minioUploadId);
    stmt->bindInt64(6, info->m_fileSize);
    stmt->bindString(7, info->m_fileType);
    stmt->bindString(8, info->m_md5Hash);
    stmt->bindInt32(9, info->m_status);
    stmt->bindTime(10, info->m_createdAt);
    stmt->bindTime(11, info->m_updatedAt);
    stmt->bindInt64(12, info->m_id);
    return stmt->execute();
}

int FilesInfoDao::Insert(FilesInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "insert into files (file_name, file_path, minio_object_name, minio_bucket, minio_upload_id, file_size, file_type, md5_hash, status, created_at, updated_at) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, info->m_fileName);
    stmt->bindString(2, info->m_filePath);
    stmt->bindString(3, info->m_minioObjectName);
    stmt->bindString(4, info->m_minioBucket);
    stmt->bindString(5, info->m_minioUploadId);
    stmt->bindInt64(6, info->m_fileSize);
    stmt->bindString(7, info->m_fileType);
    stmt->bindString(8, info->m_md5Hash);
    stmt->bindInt32(9, info->m_status);
    stmt->bindTime(10, info->m_createdAt);
    stmt->bindTime(11, info->m_updatedAt);
    int rt = stmt->execute();
    if(rt == 0) {
        info->m_id = conn->getLastInsertId();
    }
    return rt;
}

int FilesInfoDao::InsertOrUpdate(FilesInfo::ptr info, sylar::IDB::ptr conn) {
    if(info->m_id == 0) {
        return Insert(info, conn);
    }
    std::string sql = "replace into files (id, file_name, file_path, minio_object_name, minio_bucket, minio_upload_id, file_size, file_type, md5_hash, status, created_at, updated_at) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_id);
    stmt->bindString(2, info->m_fileName);
    stmt->bindString(3, info->m_filePath);
    stmt->bindString(4, info->m_minioObjectName);
    stmt->bindString(5, info->m_minioBucket);
    stmt->bindString(6, info->m_minioUploadId);
    stmt->bindInt64(7, info->m_fileSize);
    stmt->bindString(8, info->m_fileType);
    stmt->bindString(9, info->m_md5Hash);
    stmt->bindInt32(10, info->m_status);
    stmt->bindTime(11, info->m_createdAt);
    stmt->bindTime(12, info->m_updatedAt);
    return stmt->execute();
}

int FilesInfoDao::Delete(FilesInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "delete from files where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_id);
    return stmt->execute();
}

int FilesInfoDao::DeleteById( const int64_t& id, sylar::IDB::ptr conn) {
    std::string sql = "delete from files where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, id);
    return stmt->execute();
}

int FilesInfoDao::DeleteByStatus( const int32_t& status, sylar::IDB::ptr conn) {
    std::string sql = "delete from files where status = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt32(1, status);
    return stmt->execute();
}

int FilesInfoDao::DeleteByMd5Hash( const std::string& md5_hash, sylar::IDB::ptr conn) {
    std::string sql = "delete from files where md5_hash = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, md5_hash);
    return stmt->execute();
}

int FilesInfoDao::DeleteByMinioUploadId( const std::string& minio_upload_id, sylar::IDB::ptr conn) {
    std::string sql = "delete from files where minio_upload_id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, minio_upload_id);
    return stmt->execute();
}

int FilesInfoDao::QueryAll(std::vector<FilesInfo::ptr>& results, sylar::IDB::ptr conn) {
    std::string sql = "select id, file_name, file_path, minio_object_name, minio_bucket, minio_upload_id, file_size, file_type, md5_hash, status, created_at, updated_at from files";
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
        FilesInfo::ptr v(new FilesInfo);
        v->m_id = rt->getInt64(0);
        v->m_fileName = rt->getString(1);
        v->m_filePath = rt->getString(2);
        v->m_minioObjectName = rt->getString(3);
        v->m_minioBucket = rt->getString(4);
        v->m_minioUploadId = rt->getString(5);
        v->m_fileSize = rt->getInt64(6);
        v->m_fileType = rt->getString(7);
        v->m_md5Hash = rt->getString(8);
        v->m_status = rt->getInt32(9);
        v->m_createdAt = rt->getTime(10);
        v->m_updatedAt = rt->getTime(11);
        results.push_back(v);
    }
    return 0;
}

FilesInfo::ptr FilesInfoDao::Query( const int64_t& id, sylar::IDB::ptr conn) {
    std::string sql = "select id, file_name, file_path, minio_object_name, minio_bucket, minio_upload_id, file_size, file_type, md5_hash, status, created_at, updated_at from files where id = ?";
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
    FilesInfo::ptr v(new FilesInfo);
    v->m_id = rt->getInt64(0);
    v->m_fileName = rt->getString(1);
    v->m_filePath = rt->getString(2);
    v->m_minioObjectName = rt->getString(3);
    v->m_minioBucket = rt->getString(4);
    v->m_minioUploadId = rt->getString(5);
    v->m_fileSize = rt->getInt64(6);
    v->m_fileType = rt->getString(7);
    v->m_md5Hash = rt->getString(8);
    v->m_status = rt->getInt32(9);
    v->m_createdAt = rt->getTime(10);
    v->m_updatedAt = rt->getTime(11);
    return v;
}

int FilesInfoDao::QueryByStatus(std::vector<FilesInfo::ptr>& results,  const int32_t& status, sylar::IDB::ptr conn) {
    std::string sql = "select id, file_name, file_path, minio_object_name, minio_bucket, minio_upload_id, file_size, file_type, md5_hash, status, created_at, updated_at from files where status = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt32(1, status);
    auto rt = stmt->query();
    if(!rt) {
        return 0;
    }
    while (rt->next()) {
        FilesInfo::ptr v(new FilesInfo);
        v->m_id = rt->getInt64(0);
        v->m_fileName = rt->getString(1);
        v->m_filePath = rt->getString(2);
        v->m_minioObjectName = rt->getString(3);
        v->m_minioBucket = rt->getString(4);
        v->m_minioUploadId = rt->getString(5);
        v->m_fileSize = rt->getInt64(6);
        v->m_fileType = rt->getString(7);
        v->m_md5Hash = rt->getString(8);
        v->m_status = rt->getInt32(9);
        v->m_createdAt = rt->getTime(10);
        v->m_updatedAt = rt->getTime(11);
        results.push_back(v);
    };
    return 0;
}

FilesInfo::ptr FilesInfoDao::QueryByMd5Hash( const std::string& md5_hash, sylar::IDB::ptr conn) {
    std::string sql = "select id, file_name, file_path, minio_object_name, minio_bucket, minio_upload_id, file_size, file_type, md5_hash, status, created_at, updated_at from files where md5_hash = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return nullptr;
    }
    stmt->bindString(1, md5_hash);
    auto rt = stmt->query();
    if(!rt) {
        return nullptr;
    }
    if(!rt->next()) {
        return nullptr;
    }
    FilesInfo::ptr v(new FilesInfo);
    v->m_id = rt->getInt64(0);
    v->m_fileName = rt->getString(1);
    v->m_filePath = rt->getString(2);
    v->m_minioObjectName = rt->getString(3);
    v->m_minioBucket = rt->getString(4);
    v->m_minioUploadId = rt->getString(5);
    v->m_fileSize = rt->getInt64(6);
    v->m_fileType = rt->getString(7);
    v->m_md5Hash = rt->getString(8);
    v->m_status = rt->getInt32(9);
    v->m_createdAt = rt->getTime(10);
    v->m_updatedAt = rt->getTime(11);
    return v;
}

int FilesInfoDao::QueryByMinioUploadId(std::vector<FilesInfo::ptr>& results,  const std::string& minio_upload_id, sylar::IDB::ptr conn) {
    std::string sql = "select id, file_name, file_path, minio_object_name, minio_bucket, minio_upload_id, file_size, file_type, md5_hash, status, created_at, updated_at from files where minio_upload_id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindString(1, minio_upload_id);
    auto rt = stmt->query();
    if(!rt) {
        return 0;
    }
    while (rt->next()) {
        FilesInfo::ptr v(new FilesInfo);
        v->m_id = rt->getInt64(0);
        v->m_fileName = rt->getString(1);
        v->m_filePath = rt->getString(2);
        v->m_minioObjectName = rt->getString(3);
        v->m_minioBucket = rt->getString(4);
        v->m_minioUploadId = rt->getString(5);
        v->m_fileSize = rt->getInt64(6);
        v->m_fileType = rt->getString(7);
        v->m_md5Hash = rt->getString(8);
        v->m_status = rt->getInt32(9);
        v->m_createdAt = rt->getTime(10);
        v->m_updatedAt = rt->getTime(11);
        results.push_back(v);
    };
    return 0;
}

int FilesInfoDao::CreateTableSQLite3(sylar::IDB::ptr conn) {
    return conn->execute("CREATE TABLE files("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "file_name TEXT NOT NULL DEFAULT '',"
            "file_path TEXT NOT NULL DEFAULT '',"
            "minio_object_name TEXT NOT NULL DEFAULT '',"
            "minio_bucket TEXT NOT NULL DEFAULT '',"
            "minio_upload_id TEXT NOT NULL DEFAULT '',"
            "file_size INTEGER NOT NULL DEFAULT 0,"
            "file_type TEXT NOT NULL DEFAULT '',"
            "md5_hash TEXT NOT NULL DEFAULT '',"
            "status INTEGER NOT NULL DEFAULT 0,"
            "created_at TIMESTAMP NOT NULL DEFAULT current_timestamp,"
            "updated_at TIMESTAMP NOT NULL DEFAULT current_timestamp);"
            "CREATE INDEX files_status ON files(status);"
            "CREATE UNIQUE INDEX files_md5_hash ON files(md5_hash);"
            "CREATE INDEX files_minio_upload_id ON files(minio_upload_id);"
            );
}

int FilesInfoDao::CreateTableMySQL(sylar::IDB::ptr conn) {
    return conn->execute("CREATE TABLE files("
            "`id` bigint AUTO_INCREMENT COMMENT '文件ID',"
            "`file_name` varchar(128) NOT NULL DEFAULT '' COMMENT '文件名',"
            "`file_path` varchar(128) NOT NULL DEFAULT '' COMMENT '文件在MinIO中的存储路径',"
            "`minio_object_name` varchar(128) NOT NULL DEFAULT '' COMMENT 'MinIO对象名称',"
            "`minio_bucket` varchar(128) NOT NULL DEFAULT '' COMMENT 'MinIO存储桶名称',"
            "`minio_upload_id` varchar(128) NOT NULL DEFAULT '' COMMENT 'MinIO分片上传ID',"
            "`file_size` bigint NOT NULL DEFAULT 0 COMMENT '文件总大小(字节)',"
            "`file_type` varchar(128) NOT NULL DEFAULT '' COMMENT '文件类型(MIME类型)',"
            "`md5_hash` varchar(128) NOT NULL DEFAULT '' COMMENT '文件MD5哈希值',"
            "`status` int NOT NULL DEFAULT 0 COMMENT '状态(0-上传中,1-已完成,2-已删除)',"
            "`created_at` timestamp NOT NULL DEFAULT current_timestamp COMMENT '创建时间',"
            "`updated_at` timestamp NOT NULL DEFAULT current_timestamp ON UPDATE current_timestamp  COMMENT '更新时间',"
            "PRIMARY KEY(`id`),"
            "KEY `files_status` (`status`),"
            "UNIQUE KEY `files_md5_hash` (`md5_hash`),"
            "KEY `files_minio_upload_id` (`minio_upload_id`))");
}
} //namespace rs
