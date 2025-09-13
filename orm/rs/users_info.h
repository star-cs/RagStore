#ifndef __RSUSERS_INFO_H__
#define __RSUSERS_INFO_H__

#include <json/json.h>
#include <vector>
#include "sylar/core/util/json_util.h"
#include "sylar/io/db/db.h"


namespace rs {

class UsersInfoDao;
class UsersInfo {
friend class UsersInfoDao;
public:
    typedef std::shared_ptr<UsersInfo> ptr;

    UsersInfo();

    const int64_t& getId() { return m_id; }
    void setId(const int64_t& v);

    const std::string& getUsername() { return m_username; }
    void setUsername(const std::string& v);

    const std::string& getPasswordHash() { return m_passwordHash; }
    void setPasswordHash(const std::string& v);

    const std::string& getEmail() { return m_email; }
    void setEmail(const std::string& v);

    const int64_t& getTotalStorage() { return m_totalStorage; }
    void setTotalStorage(const int64_t& v);

    const int64_t& getUsedStorage() { return m_usedStorage; }
    void setUsedStorage(const int64_t& v);

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
    int64_t m_totalStorage;
    int64_t m_usedStorage;
    std::string m_username;
    std::string m_passwordHash;
    std::string m_email;
    int64_t m_createdAt;
    int64_t m_updatedAt;
};


class UsersInfoDao {
public:
    typedef std::shared_ptr<UsersInfoDao> ptr;
    static int Update(UsersInfo::ptr info, sylar::IDB::ptr conn);
    static int Insert(UsersInfo::ptr info, sylar::IDB::ptr conn);
    static int InsertOrUpdate(UsersInfo::ptr info, sylar::IDB::ptr conn);
    static int Delete(UsersInfo::ptr info, sylar::IDB::ptr conn);
    static int Delete(const int64_t& id, sylar::IDB::ptr conn);
    static int DeleteById( const int64_t& id, sylar::IDB::ptr conn);
    static int DeleteByUsername( const std::string& username, sylar::IDB::ptr conn);
    static int DeleteByEmail( const std::string& email, sylar::IDB::ptr conn);
    static int QueryAll(std::vector<UsersInfo::ptr>& results, sylar::IDB::ptr conn);
    static UsersInfo::ptr Query( const int64_t& id, sylar::IDB::ptr conn);
    static UsersInfo::ptr QueryByUsername( const std::string& username, sylar::IDB::ptr conn);
    static UsersInfo::ptr QueryByEmail( const std::string& email, sylar::IDB::ptr conn);
    static int CreateTableSQLite3(sylar::IDB::ptr info);
    static int CreateTableMySQL(sylar::IDB::ptr info);
};

} //namespace rs
#endif //__RSUSERS_INFO_H__
