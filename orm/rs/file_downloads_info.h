#ifndef __RSFILE_DOWNLOADS_INFO_H__
#define __RSFILE_DOWNLOADS_INFO_H__

#include <json/json.h>
#include <vector>
#include "sylar/core/util/json_util.h"
#include "sylar/io/db/db.h"


namespace rs {

class FileDownloadsInfoDao;
class FileDownloadsInfo {
friend class FileDownloadsInfoDao;
public:
    typedef std::shared_ptr<FileDownloadsInfo> ptr;

    FileDownloadsInfo();

    const int64_t& getId() { return m_id; }
    void setId(const int64_t& v);

    const int64_t& getUserId() { return m_userId; }
    void setUserId(const int64_t& v);

    const int64_t& getFileId() { return m_fileId; }
    void setFileId(const int64_t& v);

    const int32_t& getChunkNumber() { return m_chunkNumber; }
    void setChunkNumber(const int32_t& v);

    const int64_t& getDownloadStartTime() { return m_downloadStartTime; }
    void setDownloadStartTime(const int64_t& v);

    const int64_t& getDownloadEndTime() { return m_downloadEndTime; }
    void setDownloadEndTime(const int64_t& v);

    const int32_t& getDownloadStatus() { return m_downloadStatus; }
    void setDownloadStatus(const int32_t& v);

    const int64_t& getDownloadSize() { return m_downloadSize; }
    void setDownloadSize(const int64_t& v);

    const std::string& getIpAddress() { return m_ipAddress; }
    void setIpAddress(const std::string& v);

    const std::string& getUserAgent() { return m_userAgent; }
    void setUserAgent(const std::string& v);

    std::string toJsonString() const;

private:
    int32_t m_chunkNumber;
    int32_t m_downloadStatus;
    int64_t m_id;
    int64_t m_userId;
    int64_t m_fileId;
    int64_t m_downloadSize;
    std::string m_ipAddress;
    std::string m_userAgent;
    int64_t m_downloadStartTime;
    int64_t m_downloadEndTime;
};


class FileDownloadsInfoDao {
public:
    typedef std::shared_ptr<FileDownloadsInfoDao> ptr;
    static int Update(FileDownloadsInfo::ptr info, sylar::IDB::ptr conn);
    static int Insert(FileDownloadsInfo::ptr info, sylar::IDB::ptr conn);
    static int InsertOrUpdate(FileDownloadsInfo::ptr info, sylar::IDB::ptr conn);
    static int Delete(FileDownloadsInfo::ptr info, sylar::IDB::ptr conn);
    static int Delete(const int64_t& id, sylar::IDB::ptr conn);
    static int DeleteById( const int64_t& id, sylar::IDB::ptr conn);
    static int DeleteByUserId( const int64_t& user_id, sylar::IDB::ptr conn);
    static int DeleteByFileId( const int64_t& file_id, sylar::IDB::ptr conn);
    static int DeleteByFileIdChunkNumber( const int64_t& file_id,  const int32_t& chunk_number, sylar::IDB::ptr conn);
    static int DeleteByDownloadStatus( const int32_t& download_status, sylar::IDB::ptr conn);
    static int DeleteByDownloadStartTime( const int64_t& download_start_time, sylar::IDB::ptr conn);
    static int QueryAll(std::vector<FileDownloadsInfo::ptr>& results, sylar::IDB::ptr conn);
    static FileDownloadsInfo::ptr Query( const int64_t& id, sylar::IDB::ptr conn);
    static int QueryByUserId(std::vector<FileDownloadsInfo::ptr>& results,  const int64_t& user_id, sylar::IDB::ptr conn);
    static int QueryByFileId(std::vector<FileDownloadsInfo::ptr>& results,  const int64_t& file_id, sylar::IDB::ptr conn);
    static int QueryByFileIdChunkNumber(std::vector<FileDownloadsInfo::ptr>& results,  const int64_t& file_id,  const int32_t& chunk_number, sylar::IDB::ptr conn);
    static int QueryByDownloadStatus(std::vector<FileDownloadsInfo::ptr>& results,  const int32_t& download_status, sylar::IDB::ptr conn);
    static int QueryByDownloadStartTime(std::vector<FileDownloadsInfo::ptr>& results,  const int64_t& download_start_time, sylar::IDB::ptr conn);
    static int CreateTableSQLite3(sylar::IDB::ptr info);
    static int CreateTableMySQL(sylar::IDB::ptr info);
};

} //namespace rs
#endif //__RSFILE_DOWNLOADS_INFO_H__
