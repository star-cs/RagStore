#include "file_chunks_info.h"
#include "sylar/core/log/log.h"

namespace rs {

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("orm");

FileChunksInfo::FileChunksInfo()
    :m_chunkNumber()
    ,m_chunkSize()
    ,m_minioPartNumber()
    ,m_status(0)
    ,m_id()
    ,m_userId()
    ,m_fileId()
    ,m_chunkHash()
    ,m_minioEtag()
    ,m_uploadedAt() {
}

std::string FileChunksInfo::toJsonString() const {
    Json::Value jvalue;
    jvalue["id"] = std::to_string(m_id);
    jvalue["user_id"] = std::to_string(m_userId);
    jvalue["file_id"] = std::to_string(m_fileId);
    jvalue["chunk_number"] = m_chunkNumber;
    jvalue["chunk_size"] = m_chunkSize;
    jvalue["chunk_hash"] = m_chunkHash;
    jvalue["minio_part_number"] = m_minioPartNumber;
    jvalue["minio_etag"] = m_minioEtag;
    jvalue["status"] = m_status;
    jvalue["uploaded_at"] = sylar::Time2Str(m_uploadedAt);
    return sylar::JsonUtil::ToString(jvalue);
}

void FileChunksInfo::setId(const int64_t& v) {
    m_id = v;
}

void FileChunksInfo::setUserId(const int64_t& v) {
    m_userId = v;
}

void FileChunksInfo::setFileId(const int64_t& v) {
    m_fileId = v;
}

void FileChunksInfo::setChunkNumber(const int32_t& v) {
    m_chunkNumber = v;
}

void FileChunksInfo::setChunkSize(const int32_t& v) {
    m_chunkSize = v;
}

void FileChunksInfo::setChunkHash(const std::string& v) {
    m_chunkHash = v;
}

void FileChunksInfo::setMinioPartNumber(const int32_t& v) {
    m_minioPartNumber = v;
}

void FileChunksInfo::setMinioEtag(const std::string& v) {
    m_minioEtag = v;
}

void FileChunksInfo::setStatus(const int32_t& v) {
    m_status = v;
}

void FileChunksInfo::setUploadedAt(const int64_t& v) {
    m_uploadedAt = v;
}


int FileChunksInfoDao::Update(FileChunksInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "update file_chunks set user_id = ?, file_id = ?, chunk_number = ?, chunk_size = ?, chunk_hash = ?, minio_part_number = ?, minio_etag = ?, status = ?, uploaded_at = ? where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_userId);
    stmt->bindInt64(2, info->m_fileId);
    stmt->bindInt32(3, info->m_chunkNumber);
    stmt->bindInt32(4, info->m_chunkSize);
    stmt->bindString(5, info->m_chunkHash);
    stmt->bindInt32(6, info->m_minioPartNumber);
    stmt->bindString(7, info->m_minioEtag);
    stmt->bindInt32(8, info->m_status);
    stmt->bindTime(9, info->m_uploadedAt);
    stmt->bindInt64(10, info->m_id);
    return stmt->execute();
}

int FileChunksInfoDao::Insert(FileChunksInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "insert into file_chunks (user_id, file_id, chunk_number, chunk_size, chunk_hash, minio_part_number, minio_etag, status, uploaded_at) values (?, ?, ?, ?, ?, ?, ?, ?, ?)";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_userId);
    stmt->bindInt64(2, info->m_fileId);
    stmt->bindInt32(3, info->m_chunkNumber);
    stmt->bindInt32(4, info->m_chunkSize);
    stmt->bindString(5, info->m_chunkHash);
    stmt->bindInt32(6, info->m_minioPartNumber);
    stmt->bindString(7, info->m_minioEtag);
    stmt->bindInt32(8, info->m_status);
    stmt->bindTime(9, info->m_uploadedAt);
    int rt = stmt->execute();
    if(rt == 0) {
        info->m_id = conn->getLastInsertId();
    }
    return rt;
}

int FileChunksInfoDao::InsertOrUpdate(FileChunksInfo::ptr info, sylar::IDB::ptr conn) {
    if(info->m_id == 0) {
        return Insert(info, conn);
    }
    std::string sql = "replace into file_chunks (id, user_id, file_id, chunk_number, chunk_size, chunk_hash, minio_part_number, minio_etag, status, uploaded_at) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_id);
    stmt->bindInt64(2, info->m_userId);
    stmt->bindInt64(3, info->m_fileId);
    stmt->bindInt32(4, info->m_chunkNumber);
    stmt->bindInt32(5, info->m_chunkSize);
    stmt->bindString(6, info->m_chunkHash);
    stmt->bindInt32(7, info->m_minioPartNumber);
    stmt->bindString(8, info->m_minioEtag);
    stmt->bindInt32(9, info->m_status);
    stmt->bindTime(10, info->m_uploadedAt);
    return stmt->execute();
}

int FileChunksInfoDao::Delete(FileChunksInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "delete from file_chunks where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_id);
    return stmt->execute();
}

int FileChunksInfoDao::DeleteById( const int64_t& id, sylar::IDB::ptr conn) {
    std::string sql = "delete from file_chunks where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, id);
    return stmt->execute();
}

int FileChunksInfoDao::DeleteByUserId( const int64_t& user_id, sylar::IDB::ptr conn) {
    std::string sql = "delete from file_chunks where user_id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, user_id);
    return stmt->execute();
}

int FileChunksInfoDao::DeleteByFileId( const int64_t& file_id, sylar::IDB::ptr conn) {
    std::string sql = "delete from file_chunks where file_id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, file_id);
    return stmt->execute();
}

int FileChunksInfoDao::DeleteByFileIdChunkNumber( const int64_t& file_id,  const int32_t& chunk_number, sylar::IDB::ptr conn) {
    std::string sql = "delete from file_chunks where file_id = ? and chunk_number = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, file_id);
    stmt->bindInt32(1, chunk_number);
    return stmt->execute();
}

int FileChunksInfoDao::DeleteByStatus( const int32_t& status, sylar::IDB::ptr conn) {
    std::string sql = "delete from file_chunks where status = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt32(1, status);
    return stmt->execute();
}

int FileChunksInfoDao::QueryAll(std::vector<FileChunksInfo::ptr>& results, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, chunk_number, chunk_size, chunk_hash, minio_part_number, minio_etag, status, uploaded_at from file_chunks";
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
        FileChunksInfo::ptr v(new FileChunksInfo);
        v->m_id = rt->getInt64(0);
        v->m_userId = rt->getInt64(1);
        v->m_fileId = rt->getInt64(2);
        v->m_chunkNumber = rt->getInt32(3);
        v->m_chunkSize = rt->getInt32(4);
        v->m_chunkHash = rt->getString(5);
        v->m_minioPartNumber = rt->getInt32(6);
        v->m_minioEtag = rt->getString(7);
        v->m_status = rt->getInt32(8);
        v->m_uploadedAt = rt->getTime(9);
        results.push_back(v);
    }
    return 0;
}

FileChunksInfo::ptr FileChunksInfoDao::Query( const int64_t& id, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, chunk_number, chunk_size, chunk_hash, minio_part_number, minio_etag, status, uploaded_at from file_chunks where id = ?";
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
    FileChunksInfo::ptr v(new FileChunksInfo);
    v->m_id = rt->getInt64(0);
    v->m_userId = rt->getInt64(1);
    v->m_fileId = rt->getInt64(2);
    v->m_chunkNumber = rt->getInt32(3);
    v->m_chunkSize = rt->getInt32(4);
    v->m_chunkHash = rt->getString(5);
    v->m_minioPartNumber = rt->getInt32(6);
    v->m_minioEtag = rt->getString(7);
    v->m_status = rt->getInt32(8);
    v->m_uploadedAt = rt->getTime(9);
    return v;
}

int FileChunksInfoDao::QueryByUserId(std::vector<FileChunksInfo::ptr>& results,  const int64_t& user_id, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, chunk_number, chunk_size, chunk_hash, minio_part_number, minio_etag, status, uploaded_at from file_chunks where user_id = ?";
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
        FileChunksInfo::ptr v(new FileChunksInfo);
        v->m_id = rt->getInt64(0);
        v->m_userId = rt->getInt64(1);
        v->m_fileId = rt->getInt64(2);
        v->m_chunkNumber = rt->getInt32(3);
        v->m_chunkSize = rt->getInt32(4);
        v->m_chunkHash = rt->getString(5);
        v->m_minioPartNumber = rt->getInt32(6);
        v->m_minioEtag = rt->getString(7);
        v->m_status = rt->getInt32(8);
        v->m_uploadedAt = rt->getTime(9);
        results.push_back(v);
    };
    return 0;
}

int FileChunksInfoDao::QueryByFileId(std::vector<FileChunksInfo::ptr>& results,  const int64_t& file_id, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, chunk_number, chunk_size, chunk_hash, minio_part_number, minio_etag, status, uploaded_at from file_chunks where file_id = ?";
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
        FileChunksInfo::ptr v(new FileChunksInfo);
        v->m_id = rt->getInt64(0);
        v->m_userId = rt->getInt64(1);
        v->m_fileId = rt->getInt64(2);
        v->m_chunkNumber = rt->getInt32(3);
        v->m_chunkSize = rt->getInt32(4);
        v->m_chunkHash = rt->getString(5);
        v->m_minioPartNumber = rt->getInt32(6);
        v->m_minioEtag = rt->getString(7);
        v->m_status = rt->getInt32(8);
        v->m_uploadedAt = rt->getTime(9);
        results.push_back(v);
    };
    return 0;
}

FileChunksInfo::ptr FileChunksInfoDao::QueryByFileIdChunkNumber( const int64_t& file_id,  const int32_t& chunk_number, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, chunk_number, chunk_size, chunk_hash, minio_part_number, minio_etag, status, uploaded_at from file_chunks where file_id = ? and chunk_number = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return nullptr;
    }
    stmt->bindInt64(1, file_id);
    stmt->bindInt32(2, chunk_number);
    auto rt = stmt->query();
    if(!rt) {
        return nullptr;
    }
    if(!rt->next()) {
        return nullptr;
    }
    FileChunksInfo::ptr v(new FileChunksInfo);
    v->m_id = rt->getInt64(0);
    v->m_userId = rt->getInt64(1);
    v->m_fileId = rt->getInt64(2);
    v->m_chunkNumber = rt->getInt32(3);
    v->m_chunkSize = rt->getInt32(4);
    v->m_chunkHash = rt->getString(5);
    v->m_minioPartNumber = rt->getInt32(6);
    v->m_minioEtag = rt->getString(7);
    v->m_status = rt->getInt32(8);
    v->m_uploadedAt = rt->getTime(9);
    return v;
}

int FileChunksInfoDao::QueryByStatus(std::vector<FileChunksInfo::ptr>& results,  const int32_t& status, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, chunk_number, chunk_size, chunk_hash, minio_part_number, minio_etag, status, uploaded_at from file_chunks where status = ?";
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
        FileChunksInfo::ptr v(new FileChunksInfo);
        v->m_id = rt->getInt64(0);
        v->m_userId = rt->getInt64(1);
        v->m_fileId = rt->getInt64(2);
        v->m_chunkNumber = rt->getInt32(3);
        v->m_chunkSize = rt->getInt32(4);
        v->m_chunkHash = rt->getString(5);
        v->m_minioPartNumber = rt->getInt32(6);
        v->m_minioEtag = rt->getString(7);
        v->m_status = rt->getInt32(8);
        v->m_uploadedAt = rt->getTime(9);
        results.push_back(v);
    };
    return 0;
}

int FileChunksInfoDao::CreateTableSQLite3(sylar::IDB::ptr conn) {
    return conn->execute("CREATE TABLE file_chunks("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "user_id INTEGER NOT NULL DEFAULT 0,"
            "file_id INTEGER NOT NULL DEFAULT 0,"
            "chunk_number INTEGER NOT NULL DEFAULT 0,"
            "chunk_size INTEGER NOT NULL DEFAULT 0,"
            "chunk_hash TEXT NOT NULL DEFAULT '',"
            "minio_part_number INTEGER NOT NULL DEFAULT 0,"
            "minio_etag TEXT NOT NULL DEFAULT '',"
            "status INTEGER NOT NULL DEFAULT 0,"
            "uploaded_at TIMESTAMP NOT NULL DEFAULT '1980-01-01 00:00:00');"
            "CREATE INDEX file_chunks_user_id ON file_chunks(user_id);"
            "CREATE INDEX file_chunks_file_id ON file_chunks(file_id);"
            "CREATE UNIQUE INDEX file_chunks_file_id_chunk_number ON file_chunks(file_id,chunk_number);"
            "CREATE INDEX file_chunks_status ON file_chunks(status);"
            );
}

int FileChunksInfoDao::CreateTableMySQL(sylar::IDB::ptr conn) {
    return conn->execute("CREATE TABLE file_chunks("
            "`id` bigint AUTO_INCREMENT COMMENT '分片ID',"
            "`user_id` bigint NOT NULL DEFAULT 0 COMMENT '用户ID',"
            "`file_id` bigint NOT NULL DEFAULT 0 COMMENT '所属文件ID',"
            "`chunk_number` int NOT NULL DEFAULT 0 COMMENT '分片序号(从1开始)',"
            "`chunk_size` int NOT NULL DEFAULT 0 COMMENT '当前分片大小(字节)',"
            "`chunk_hash` varchar(128) NOT NULL DEFAULT '' COMMENT '分片MD5哈希值',"
            "`minio_part_number` int NOT NULL DEFAULT 0 COMMENT 'MinIO分片编号',"
            "`minio_etag` varchar(128) NOT NULL DEFAULT '' COMMENT 'MinIO返回的ETag',"
            "`status` int NOT NULL DEFAULT 0 COMMENT '状态(0-未上传,1-已上传,2-校验失败,3-已删除)',"
            "`uploaded_at` timestamp NOT NULL DEFAULT '1980-01-01 00:00:00' COMMENT '上传时间',"
            "PRIMARY KEY(`id`),"
            "KEY `file_chunks_user_id` (`user_id`),"
            "KEY `file_chunks_file_id` (`file_id`),"
            "UNIQUE KEY `file_chunks_file_id_chunk_number` (`file_id`,`chunk_number`),"
            "KEY `file_chunks_status` (`status`))");
}
} //namespace rs
