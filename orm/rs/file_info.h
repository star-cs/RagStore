#ifndef __RSFILE_INFO_H__
#define __RSFILE_INFO_H__

#include <json/json.h>
#include <vector>
#include "sylar/io/db/db.h"
#include "sylar/core/util/json_util.h"


namespace rs {

class FileInfoDao;
class FileInfo {
friend class FileInfoDao;
public:
    typedef std::shared_ptr<FileInfo> ptr;

    FileInfo();

    const int64_t& getId() { return m_id; }
    void setId(const int64_t& v);

    const std::string& getUser() { return m_user; }
    void setUser(const std::string& v);

    const std::string& getMd5() { return m_md5; }
    void setMd5(const std::string& v);

    const std::string& getFileName() { return m_fileName; }
    void setFileName(const std::string& v);

    const int64_t& getFileSize() { return m_fileSize; }
    void setFileSize(const int64_t& v);

    const std::string& getFileUrl() { return m_fileUrl; }
    void setFileUrl(const std::string& v);

    const std::string& getFileType() { return m_fileType; }
    void setFileType(const std::string& v);

    const int64_t& getCreateTime() { return m_createTime; }
    void setCreateTime(const int64_t& v);

    const int64_t& getUpdateTime() { return m_updateTime; }
    void setUpdateTime(const int64_t& v);

    std::string toJsonString() const;

private:
    int64_t m_id;
    int64_t m_fileSize;
    std::string m_user;
    std::string m_md5;
    std::string m_fileName;
    std::string m_fileUrl;
    std::string m_fileType;
    int64_t m_createTime;
    int64_t m_updateTime;
};


class FileInfoDao {
public:
    typedef std::shared_ptr<FileInfoDao> ptr;
    static int Update(FileInfo::ptr info, sylar::IDB::ptr conn);
    static int Insert(FileInfo::ptr info, sylar::IDB::ptr conn);
    static int InsertOrUpdate(FileInfo::ptr info, sylar::IDB::ptr conn);
    static int Delete(FileInfo::ptr info, sylar::IDB::ptr conn);
    static int Delete(const int64_t& id, sylar::IDB::ptr conn);
    static int DeleteById( const int64_t& id, sylar::IDB::ptr conn);
    static int DeleteByUser( const std::string& user, sylar::IDB::ptr conn);
    static int DeleteByFileName( const std::string& file_name, sylar::IDB::ptr conn);
    static int DeleteByFileType( const std::string& file_type, sylar::IDB::ptr conn);
    static int DeleteByMd5( const std::string& md5, sylar::IDB::ptr conn);
    static int DeleteByFileUrl( const std::string& file_url, sylar::IDB::ptr conn);
    static int QueryAll(std::vector<FileInfo::ptr>& results, sylar::IDB::ptr conn);
    static FileInfo::ptr Query( const int64_t& id, sylar::IDB::ptr conn);
    static int QueryByUser(std::vector<FileInfo::ptr>& results,  const std::string& user, sylar::IDB::ptr conn);
    static int QueryByFileName(std::vector<FileInfo::ptr>& results,  const std::string& file_name, sylar::IDB::ptr conn);
    static int QueryByFileType(std::vector<FileInfo::ptr>& results,  const std::string& file_type, sylar::IDB::ptr conn);
    static FileInfo::ptr QueryByMd5( const std::string& md5, sylar::IDB::ptr conn);
    static FileInfo::ptr QueryByFileUrl( const std::string& file_url, sylar::IDB::ptr conn);
    static int CreateTableSQLite3(sylar::IDB::ptr info);
    static int CreateTableMySQL(sylar::IDB::ptr info);
};

} //namespace rs
#endif //__RSFILE_INFO_H__
