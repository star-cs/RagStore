#ifndef __RSFILE_CHUNKS_INFO_H__
#define __RSFILE_CHUNKS_INFO_H__

#include <json/json.h>
#include <vector>
#include "sylar/core/util/json_util.h"
#include "sylar/io/db/db.h"


namespace rs {

class FileChunksInfoDao;
class FileChunksInfo {
friend class FileChunksInfoDao;
public:
    typedef std::shared_ptr<FileChunksInfo> ptr;

    FileChunksInfo();

    const int64_t& getId() { return m_id; }
    void setId(const int64_t& v);

    const int64_t& getUserId() { return m_userId; }
    void setUserId(const int64_t& v);

    const int64_t& getFileId() { return m_fileId; }
    void setFileId(const int64_t& v);

    const int32_t& getChunkNumber() { return m_chunkNumber; }
    void setChunkNumber(const int32_t& v);

    const int32_t& getChunkSize() { return m_chunkSize; }
    void setChunkSize(const int32_t& v);

    const std::string& getChunkHash() { return m_chunkHash; }
    void setChunkHash(const std::string& v);

    const int32_t& getMinioPartNumber() { return m_minioPartNumber; }
    void setMinioPartNumber(const int32_t& v);

    const std::string& getMinioEtag() { return m_minioEtag; }
    void setMinioEtag(const std::string& v);

    const int32_t& getStatus() { return m_status; }
    void setStatus(const int32_t& v);

    const int64_t& getUploadedAt() { return m_uploadedAt; }
    void setUploadedAt(const int64_t& v);

    std::string toJsonString() const;

private:
    int32_t m_chunkNumber;
    int32_t m_chunkSize;
    int32_t m_minioPartNumber;
    int32_t m_status;
    int64_t m_id;
    int64_t m_userId;
    int64_t m_fileId;
    std::string m_chunkHash;
    std::string m_minioEtag;
    int64_t m_uploadedAt;
};


class FileChunksInfoDao {
public:
    typedef std::shared_ptr<FileChunksInfoDao> ptr;
    static int Update(FileChunksInfo::ptr info, sylar::IDB::ptr conn);
    static int Insert(FileChunksInfo::ptr info, sylar::IDB::ptr conn);
    static int InsertOrUpdate(FileChunksInfo::ptr info, sylar::IDB::ptr conn);
    static int Delete(FileChunksInfo::ptr info, sylar::IDB::ptr conn);
    static int Delete(const int64_t& id, sylar::IDB::ptr conn);
    static int DeleteById( const int64_t& id, sylar::IDB::ptr conn);
    static int DeleteByUserId( const int64_t& user_id, sylar::IDB::ptr conn);
    static int DeleteByFileId( const int64_t& file_id, sylar::IDB::ptr conn);
    static int DeleteByFileIdChunkNumber( const int64_t& file_id,  const int32_t& chunk_number, sylar::IDB::ptr conn);
    static int DeleteByStatus( const int32_t& status, sylar::IDB::ptr conn);
    static int QueryAll(std::vector<FileChunksInfo::ptr>& results, sylar::IDB::ptr conn);
    static FileChunksInfo::ptr Query( const int64_t& id, sylar::IDB::ptr conn);
    static int QueryByUserId(std::vector<FileChunksInfo::ptr>& results,  const int64_t& user_id, sylar::IDB::ptr conn);
    static int QueryByFileId(std::vector<FileChunksInfo::ptr>& results,  const int64_t& file_id, sylar::IDB::ptr conn);
    static FileChunksInfo::ptr QueryByFileIdChunkNumber( const int64_t& file_id,  const int32_t& chunk_number, sylar::IDB::ptr conn);
    static int QueryByStatus(std::vector<FileChunksInfo::ptr>& results,  const int32_t& status, sylar::IDB::ptr conn);
    static int CreateTableSQLite3(sylar::IDB::ptr info);
    static int CreateTableMySQL(sylar::IDB::ptr info);
};

} //namespace rs
#endif //__RSFILE_CHUNKS_INFO_H__
