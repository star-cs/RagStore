#ifndef __RSUSER_FILE_RELATIONS_INFO_H__
#define __RSUSER_FILE_RELATIONS_INFO_H__

#include <json/json.h>
#include <vector>
#include "sylar/core/util/json_util.h"
#include "sylar/io/db/db.h"


namespace rs {

class UserFileRelationsInfoDao;
class UserFileRelationsInfo {
friend class UserFileRelationsInfoDao;
public:
    typedef std::shared_ptr<UserFileRelationsInfo> ptr;

    UserFileRelationsInfo();

    const int64_t& getId() { return m_id; }
    void setId(const int64_t& v);

    const int64_t& getUserId() { return m_userId; }
    void setUserId(const int64_t& v);

    const int64_t& getFileId() { return m_fileId; }
    void setFileId(const int64_t& v);

    const int32_t& getPermission() { return m_permission; }
    void setPermission(const int32_t& v);

    const int64_t& getCreatedAt() { return m_createdAt; }
    void setCreatedAt(const int64_t& v);

    const int64_t& getUpdatedAt() { return m_updatedAt; }
    void setUpdatedAt(const int64_t& v);

    std::string toJsonString() const;

private:
    int32_t m_permission;
    int64_t m_id;
    int64_t m_userId;
    int64_t m_fileId;
    int64_t m_createdAt;
    int64_t m_updatedAt;
};


class UserFileRelationsInfoDao {
public:
    typedef std::shared_ptr<UserFileRelationsInfoDao> ptr;
    static int Update(UserFileRelationsInfo::ptr info, sylar::IDB::ptr conn);
    static int Insert(UserFileRelationsInfo::ptr info, sylar::IDB::ptr conn);
    static int InsertOrUpdate(UserFileRelationsInfo::ptr info, sylar::IDB::ptr conn);
    static int Delete(UserFileRelationsInfo::ptr info, sylar::IDB::ptr conn);
    static int Delete(const int64_t& id, sylar::IDB::ptr conn);
    static int DeleteById( const int64_t& id, sylar::IDB::ptr conn);
    static int DeleteByUserId( const int64_t& user_id, sylar::IDB::ptr conn);
    static int DeleteByFileId( const int64_t& file_id, sylar::IDB::ptr conn);
    static int DeleteByUserIdFileId( const int64_t& user_id,  const int64_t& file_id, sylar::IDB::ptr conn);
    static int QueryAll(std::vector<UserFileRelationsInfo::ptr>& results, sylar::IDB::ptr conn);
    static UserFileRelationsInfo::ptr Query( const int64_t& id, sylar::IDB::ptr conn);
    static int QueryByUserId(std::vector<UserFileRelationsInfo::ptr>& results,  const int64_t& user_id, sylar::IDB::ptr conn);
    static int QueryByFileId(std::vector<UserFileRelationsInfo::ptr>& results,  const int64_t& file_id, sylar::IDB::ptr conn);
    static UserFileRelationsInfo::ptr QueryByUserIdFileId( const int64_t& user_id,  const int64_t& file_id, sylar::IDB::ptr conn);
    static int CreateTableSQLite3(sylar::IDB::ptr info);
    static int CreateTableMySQL(sylar::IDB::ptr info);
};

} //namespace rs
#endif //__RSUSER_FILE_RELATIONS_INFO_H__
