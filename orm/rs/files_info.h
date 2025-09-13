#ifndef __RSFILES_INFO_H__
#define __RSFILES_INFO_H__

#include <json/json.h>
#include <vector>
#include "sylar/core/util/json_util.h"
#include "sylar/io/db/db.h"


namespace rs {

class FilesInfoDao;
class FilesInfo {
friend class FilesInfoDao;
public:
    typedef std::shared_ptr<FilesInfo> ptr;

    FilesInfo();

    const int64_t& getId() { return m_id; }
    void setId(const int64_t& v);

    const std::string& getFileName() { return m_fileName; }
    void setFileName(const std::string& v);

    const std::string& getFilePath() { return m_filePath; }
    void setFilePath(const std::string& v);

    const std::string& getMinioObjectName() { return m_minioObjectName; }
    void setMinioObjectName(const std::string& v);

    const std::string& getMinioBucket() { return m_minioBucket; }
    void setMinioBucket(const std::string& v);

    const std::string& getMinioUploadId() { return m_minioUploadId; }
    void setMinioUploadId(const std::string& v);

    const int64_t& getFileSize() { return m_fileSize; }
    void setFileSize(const int64_t& v);

    const std::string& getFileType() { return m_fileType; }
    void setFileType(const std::string& v);

    const std::string& getMd5Hash() { return m_md5Hash; }
    void setMd5Hash(const std::string& v);

    const int32_t& getStatus() { return m_status; }
    void setStatus(const int32_t& v);

    const int64_t& getCreatedAt() { return m_createdAt; }
    void setCreatedAt(const int64_t& v);

    const int64_t& getUpdatedAt() { return m_updatedAt; }
    void setUpdatedAt(const int64_t& v);

    std::string toJsonString() const;

private:
    int32_t m_status;
    int64_t m_id;
    int64_t m_fileSize;
    std::string m_fileName;
    std::string m_filePath;
    std::string m_minioObjectName;
    std::string m_minioBucket;
    std::string m_minioUploadId;
    std::string m_fileType;
    std::string m_md5Hash;
    int64_t m_createdAt;
    int64_t m_updatedAt;
};


class FilesInfoDao {
public:
    typedef std::shared_ptr<FilesInfoDao> ptr;
    static int Update(FilesInfo::ptr info, sylar::IDB::ptr conn);
    static int Insert(FilesInfo::ptr info, sylar::IDB::ptr conn);
    static int InsertOrUpdate(FilesInfo::ptr info, sylar::IDB::ptr conn);
    static int Delete(FilesInfo::ptr info, sylar::IDB::ptr conn);
    static int Delete(const int64_t& id, sylar::IDB::ptr conn);
    static int DeleteById( const int64_t& id, sylar::IDB::ptr conn);
    static int DeleteByStatus( const int32_t& status, sylar::IDB::ptr conn);
    static int DeleteByMd5Hash( const std::string& md5_hash, sylar::IDB::ptr conn);
    static int DeleteByMinioUploadId( const std::string& minio_upload_id, sylar::IDB::ptr conn);
    static int QueryAll(std::vector<FilesInfo::ptr>& results, sylar::IDB::ptr conn);
    static FilesInfo::ptr Query( const int64_t& id, sylar::IDB::ptr conn);
    static int QueryByStatus(std::vector<FilesInfo::ptr>& results,  const int32_t& status, sylar::IDB::ptr conn);
    static FilesInfo::ptr QueryByMd5Hash( const std::string& md5_hash, sylar::IDB::ptr conn);
    static int QueryByMinioUploadId(std::vector<FilesInfo::ptr>& results,  const std::string& minio_upload_id, sylar::IDB::ptr conn);
    static int CreateTableSQLite3(sylar::IDB::ptr info);
    static int CreateTableMySQL(sylar::IDB::ptr info);
};

} //namespace rs
#endif //__RSFILES_INFO_H__
