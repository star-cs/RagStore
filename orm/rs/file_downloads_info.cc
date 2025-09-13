#include "file_downloads_info.h"
#include "sylar/core/log/log.h"

namespace rs {

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("orm");

FileDownloadsInfo::FileDownloadsInfo()
    :m_chunkNumber()
    ,m_downloadStatus(0)
    ,m_id()
    ,m_userId()
    ,m_fileId()
    ,m_downloadSize()
    ,m_ipAddress()
    ,m_userAgent()
    ,m_downloadStartTime(time(0))
    ,m_downloadEndTime() {
}

std::string FileDownloadsInfo::toJsonString() const {
    Json::Value jvalue;
    jvalue["id"] = std::to_string(m_id);
    jvalue["user_id"] = std::to_string(m_userId);
    jvalue["file_id"] = std::to_string(m_fileId);
    jvalue["chunk_number"] = m_chunkNumber;
    jvalue["download_start_time"] = sylar::Time2Str(m_downloadStartTime);
    jvalue["download_end_time"] = sylar::Time2Str(m_downloadEndTime);
    jvalue["download_status"] = m_downloadStatus;
    jvalue["download_size"] = std::to_string(m_downloadSize);
    jvalue["ip_address"] = m_ipAddress;
    jvalue["user_agent"] = m_userAgent;
    return sylar::JsonUtil::ToString(jvalue);
}

void FileDownloadsInfo::setId(const int64_t& v) {
    m_id = v;
}

void FileDownloadsInfo::setUserId(const int64_t& v) {
    m_userId = v;
}

void FileDownloadsInfo::setFileId(const int64_t& v) {
    m_fileId = v;
}

void FileDownloadsInfo::setChunkNumber(const int32_t& v) {
    m_chunkNumber = v;
}

void FileDownloadsInfo::setDownloadStartTime(const int64_t& v) {
    m_downloadStartTime = v;
}

void FileDownloadsInfo::setDownloadEndTime(const int64_t& v) {
    m_downloadEndTime = v;
}

void FileDownloadsInfo::setDownloadStatus(const int32_t& v) {
    m_downloadStatus = v;
}

void FileDownloadsInfo::setDownloadSize(const int64_t& v) {
    m_downloadSize = v;
}

void FileDownloadsInfo::setIpAddress(const std::string& v) {
    m_ipAddress = v;
}

void FileDownloadsInfo::setUserAgent(const std::string& v) {
    m_userAgent = v;
}


int FileDownloadsInfoDao::Update(FileDownloadsInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "update file_downloads set user_id = ?, file_id = ?, chunk_number = ?, download_start_time = ?, download_end_time = ?, download_status = ?, download_size = ?, ip_address = ?, user_agent = ? where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_userId);
    stmt->bindInt64(2, info->m_fileId);
    stmt->bindInt32(3, info->m_chunkNumber);
    stmt->bindTime(4, info->m_downloadStartTime);
    stmt->bindTime(5, info->m_downloadEndTime);
    stmt->bindInt32(6, info->m_downloadStatus);
    stmt->bindInt64(7, info->m_downloadSize);
    stmt->bindString(8, info->m_ipAddress);
    stmt->bindString(9, info->m_userAgent);
    stmt->bindInt64(10, info->m_id);
    return stmt->execute();
}

int FileDownloadsInfoDao::Insert(FileDownloadsInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "insert into file_downloads (user_id, file_id, chunk_number, download_start_time, download_end_time, download_status, download_size, ip_address, user_agent) values (?, ?, ?, ?, ?, ?, ?, ?, ?)";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_userId);
    stmt->bindInt64(2, info->m_fileId);
    stmt->bindInt32(3, info->m_chunkNumber);
    stmt->bindTime(4, info->m_downloadStartTime);
    stmt->bindTime(5, info->m_downloadEndTime);
    stmt->bindInt32(6, info->m_downloadStatus);
    stmt->bindInt64(7, info->m_downloadSize);
    stmt->bindString(8, info->m_ipAddress);
    stmt->bindString(9, info->m_userAgent);
    int rt = stmt->execute();
    if(rt == 0) {
        info->m_id = conn->getLastInsertId();
    }
    return rt;
}

int FileDownloadsInfoDao::InsertOrUpdate(FileDownloadsInfo::ptr info, sylar::IDB::ptr conn) {
    if(info->m_id == 0) {
        return Insert(info, conn);
    }
    std::string sql = "replace into file_downloads (id, user_id, file_id, chunk_number, download_start_time, download_end_time, download_status, download_size, ip_address, user_agent) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
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
    stmt->bindTime(5, info->m_downloadStartTime);
    stmt->bindTime(6, info->m_downloadEndTime);
    stmt->bindInt32(7, info->m_downloadStatus);
    stmt->bindInt64(8, info->m_downloadSize);
    stmt->bindString(9, info->m_ipAddress);
    stmt->bindString(10, info->m_userAgent);
    return stmt->execute();
}

int FileDownloadsInfoDao::Delete(FileDownloadsInfo::ptr info, sylar::IDB::ptr conn) {
    std::string sql = "delete from file_downloads where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, info->m_id);
    return stmt->execute();
}

int FileDownloadsInfoDao::DeleteById( const int64_t& id, sylar::IDB::ptr conn) {
    std::string sql = "delete from file_downloads where id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, id);
    return stmt->execute();
}

int FileDownloadsInfoDao::DeleteByUserId( const int64_t& user_id, sylar::IDB::ptr conn) {
    std::string sql = "delete from file_downloads where user_id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, user_id);
    return stmt->execute();
}

int FileDownloadsInfoDao::DeleteByFileId( const int64_t& file_id, sylar::IDB::ptr conn) {
    std::string sql = "delete from file_downloads where file_id = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, file_id);
    return stmt->execute();
}

int FileDownloadsInfoDao::DeleteByFileIdChunkNumber( const int64_t& file_id,  const int32_t& chunk_number, sylar::IDB::ptr conn) {
    std::string sql = "delete from file_downloads where file_id = ? and chunk_number = ?";
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

int FileDownloadsInfoDao::DeleteByDownloadStatus( const int32_t& download_status, sylar::IDB::ptr conn) {
    std::string sql = "delete from file_downloads where download_status = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt32(1, download_status);
    return stmt->execute();
}

int FileDownloadsInfoDao::DeleteByDownloadStartTime( const int64_t& download_start_time, sylar::IDB::ptr conn) {
    std::string sql = "delete from file_downloads where download_start_time = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindTime(1, download_start_time);
    return stmt->execute();
}

int FileDownloadsInfoDao::QueryAll(std::vector<FileDownloadsInfo::ptr>& results, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, chunk_number, download_start_time, download_end_time, download_status, download_size, ip_address, user_agent from file_downloads";
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
        FileDownloadsInfo::ptr v(new FileDownloadsInfo);
        v->m_id = rt->getInt64(0);
        v->m_userId = rt->getInt64(1);
        v->m_fileId = rt->getInt64(2);
        v->m_chunkNumber = rt->getInt32(3);
        v->m_downloadStartTime = rt->getTime(4);
        v->m_downloadEndTime = rt->getTime(5);
        v->m_downloadStatus = rt->getInt32(6);
        v->m_downloadSize = rt->getInt64(7);
        v->m_ipAddress = rt->getString(8);
        v->m_userAgent = rt->getString(9);
        results.push_back(v);
    }
    return 0;
}

FileDownloadsInfo::ptr FileDownloadsInfoDao::Query( const int64_t& id, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, chunk_number, download_start_time, download_end_time, download_status, download_size, ip_address, user_agent from file_downloads where id = ?";
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
    FileDownloadsInfo::ptr v(new FileDownloadsInfo);
    v->m_id = rt->getInt64(0);
    v->m_userId = rt->getInt64(1);
    v->m_fileId = rt->getInt64(2);
    v->m_chunkNumber = rt->getInt32(3);
    v->m_downloadStartTime = rt->getTime(4);
    v->m_downloadEndTime = rt->getTime(5);
    v->m_downloadStatus = rt->getInt32(6);
    v->m_downloadSize = rt->getInt64(7);
    v->m_ipAddress = rt->getString(8);
    v->m_userAgent = rt->getString(9);
    return v;
}

int FileDownloadsInfoDao::QueryByUserId(std::vector<FileDownloadsInfo::ptr>& results,  const int64_t& user_id, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, chunk_number, download_start_time, download_end_time, download_status, download_size, ip_address, user_agent from file_downloads where user_id = ?";
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
        FileDownloadsInfo::ptr v(new FileDownloadsInfo);
        v->m_id = rt->getInt64(0);
        v->m_userId = rt->getInt64(1);
        v->m_fileId = rt->getInt64(2);
        v->m_chunkNumber = rt->getInt32(3);
        v->m_downloadStartTime = rt->getTime(4);
        v->m_downloadEndTime = rt->getTime(5);
        v->m_downloadStatus = rt->getInt32(6);
        v->m_downloadSize = rt->getInt64(7);
        v->m_ipAddress = rt->getString(8);
        v->m_userAgent = rt->getString(9);
        results.push_back(v);
    };
    return 0;
}

int FileDownloadsInfoDao::QueryByFileId(std::vector<FileDownloadsInfo::ptr>& results,  const int64_t& file_id, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, chunk_number, download_start_time, download_end_time, download_status, download_size, ip_address, user_agent from file_downloads where file_id = ?";
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
        FileDownloadsInfo::ptr v(new FileDownloadsInfo);
        v->m_id = rt->getInt64(0);
        v->m_userId = rt->getInt64(1);
        v->m_fileId = rt->getInt64(2);
        v->m_chunkNumber = rt->getInt32(3);
        v->m_downloadStartTime = rt->getTime(4);
        v->m_downloadEndTime = rt->getTime(5);
        v->m_downloadStatus = rt->getInt32(6);
        v->m_downloadSize = rt->getInt64(7);
        v->m_ipAddress = rt->getString(8);
        v->m_userAgent = rt->getString(9);
        results.push_back(v);
    };
    return 0;
}

int FileDownloadsInfoDao::QueryByFileIdChunkNumber(std::vector<FileDownloadsInfo::ptr>& results,  const int64_t& file_id,  const int32_t& chunk_number, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, chunk_number, download_start_time, download_end_time, download_status, download_size, ip_address, user_agent from file_downloads where file_id = ? and chunk_number = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt64(1, file_id);
    stmt->bindInt32(2, chunk_number);
    auto rt = stmt->query();
    if(!rt) {
        return 0;
    }
    while (rt->next()) {
        FileDownloadsInfo::ptr v(new FileDownloadsInfo);
        v->m_id = rt->getInt64(0);
        v->m_userId = rt->getInt64(1);
        v->m_fileId = rt->getInt64(2);
        v->m_chunkNumber = rt->getInt32(3);
        v->m_downloadStartTime = rt->getTime(4);
        v->m_downloadEndTime = rt->getTime(5);
        v->m_downloadStatus = rt->getInt32(6);
        v->m_downloadSize = rt->getInt64(7);
        v->m_ipAddress = rt->getString(8);
        v->m_userAgent = rt->getString(9);
        results.push_back(v);
    };
    return 0;
}

int FileDownloadsInfoDao::QueryByDownloadStatus(std::vector<FileDownloadsInfo::ptr>& results,  const int32_t& download_status, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, chunk_number, download_start_time, download_end_time, download_status, download_size, ip_address, user_agent from file_downloads where download_status = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindInt32(1, download_status);
    auto rt = stmt->query();
    if(!rt) {
        return 0;
    }
    while (rt->next()) {
        FileDownloadsInfo::ptr v(new FileDownloadsInfo);
        v->m_id = rt->getInt64(0);
        v->m_userId = rt->getInt64(1);
        v->m_fileId = rt->getInt64(2);
        v->m_chunkNumber = rt->getInt32(3);
        v->m_downloadStartTime = rt->getTime(4);
        v->m_downloadEndTime = rt->getTime(5);
        v->m_downloadStatus = rt->getInt32(6);
        v->m_downloadSize = rt->getInt64(7);
        v->m_ipAddress = rt->getString(8);
        v->m_userAgent = rt->getString(9);
        results.push_back(v);
    };
    return 0;
}

int FileDownloadsInfoDao::QueryByDownloadStartTime(std::vector<FileDownloadsInfo::ptr>& results,  const int64_t& download_start_time, sylar::IDB::ptr conn) {
    std::string sql = "select id, user_id, file_id, chunk_number, download_start_time, download_end_time, download_status, download_size, ip_address, user_agent from file_downloads where download_start_time = ?";
    auto stmt = conn->prepare(sql);
    if(!stmt) {
        SYLAR_LOG_ERROR(g_logger) << "stmt=" << sql
                 << " errno=" << conn->getErrno() << " errstr=" << conn->getErrStr();
        return conn->getErrno();
    }
    stmt->bindTime(1, download_start_time);
    auto rt = stmt->query();
    if(!rt) {
        return 0;
    }
    while (rt->next()) {
        FileDownloadsInfo::ptr v(new FileDownloadsInfo);
        v->m_id = rt->getInt64(0);
        v->m_userId = rt->getInt64(1);
        v->m_fileId = rt->getInt64(2);
        v->m_chunkNumber = rt->getInt32(3);
        v->m_downloadStartTime = rt->getTime(4);
        v->m_downloadEndTime = rt->getTime(5);
        v->m_downloadStatus = rt->getInt32(6);
        v->m_downloadSize = rt->getInt64(7);
        v->m_ipAddress = rt->getString(8);
        v->m_userAgent = rt->getString(9);
        results.push_back(v);
    };
    return 0;
}

int FileDownloadsInfoDao::CreateTableSQLite3(sylar::IDB::ptr conn) {
    return conn->execute("CREATE TABLE file_downloads("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "user_id INTEGER NOT NULL DEFAULT 0,"
            "file_id INTEGER NOT NULL DEFAULT 0,"
            "chunk_number INTEGER NOT NULL DEFAULT 0,"
            "download_start_time TIMESTAMP NOT NULL DEFAULT current_timestamp,"
            "download_end_time TIMESTAMP NOT NULL DEFAULT '1980-01-01 00:00:00',"
            "download_status INTEGER NOT NULL DEFAULT 0,"
            "download_size INTEGER NOT NULL DEFAULT 0,"
            "ip_address TEXT NOT NULL DEFAULT '',"
            "user_agent TEXT NOT NULL DEFAULT '');"
            "CREATE INDEX file_downloads_user_id ON file_downloads(user_id);"
            "CREATE INDEX file_downloads_file_id ON file_downloads(file_id);"
            "CREATE INDEX file_downloads_file_id_chunk_number ON file_downloads(file_id,chunk_number);"
            "CREATE INDEX file_downloads_download_status ON file_downloads(download_status);"
            "CREATE INDEX file_downloads_download_start_time ON file_downloads(download_start_time);"
            );
}

int FileDownloadsInfoDao::CreateTableMySQL(sylar::IDB::ptr conn) {
    return conn->execute("CREATE TABLE file_downloads("
            "`id` bigint AUTO_INCREMENT COMMENT '下载记录ID',"
            "`user_id` bigint NOT NULL DEFAULT 0 COMMENT '用户ID',"
            "`file_id` bigint NOT NULL DEFAULT 0 COMMENT '文件ID',"
            "`chunk_number` int NOT NULL DEFAULT 0 COMMENT '分片序号',"
            "`download_start_time` timestamp NOT NULL DEFAULT current_timestamp COMMENT '下载开始时间',"
            "`download_end_time` timestamp NOT NULL DEFAULT '1980-01-01 00:00:00' COMMENT '下载结束时间',"
            "`download_status` int NOT NULL DEFAULT 0 COMMENT '下载状态(0-进行中,1-完成,2-失败)',"
            "`download_size` bigint NOT NULL DEFAULT 0 COMMENT '实际下载的分片大小(字节)',"
            "`ip_address` varchar(128) NOT NULL DEFAULT '' COMMENT '下载时的IP地址',"
            "`user_agent` varchar(128) NOT NULL DEFAULT '' COMMENT '下载时的用户代理信息',"
            "PRIMARY KEY(`id`),"
            "KEY `file_downloads_user_id` (`user_id`),"
            "KEY `file_downloads_file_id` (`file_id`),"
            "KEY `file_downloads_file_id_chunk_number` (`file_id`,`chunk_number`),"
            "KEY `file_downloads_download_status` (`download_status`),"
            "KEY `file_downloads_download_start_time` (`download_start_time`))");
}
} //namespace rs
