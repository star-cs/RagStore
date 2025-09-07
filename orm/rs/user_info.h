#ifndef __RSUSER_INFO_H__
#define __RSUSER_INFO_H__

#include <json/json.h>
#include <vector>
#include "sylar/io/db/db.h"
#include "sylar/core/util/json_util.h"


namespace rs {

class UserInfoDao;
class UserInfo {
friend class UserInfoDao;
public:
    typedef std::shared_ptr<UserInfo> ptr;

    UserInfo();

    const int64_t& getId() { return m_id; }
    void setId(const int64_t& v);

    const std::string& getName() { return m_name; }
    void setName(const std::string& v);

    const std::string& getPasswd() { return m_passwd; }
    void setPasswd(const std::string& v);

    const int64_t& getCreateTime() { return m_createTime; }
    void setCreateTime(const int64_t& v);

    const int64_t& getUpdateTime() { return m_updateTime; }
    void setUpdateTime(const int64_t& v);

    std::string toJsonString() const;

private:
    int64_t m_id;
    std::string m_name;
    std::string m_passwd;
    int64_t m_createTime;
    int64_t m_updateTime;
};


class UserInfoDao {
public:
    typedef std::shared_ptr<UserInfoDao> ptr;
    static int Update(UserInfo::ptr info, sylar::IDB::ptr conn);
    static int Insert(UserInfo::ptr info, sylar::IDB::ptr conn);
    static int InsertOrUpdate(UserInfo::ptr info, sylar::IDB::ptr conn);
    static int Delete(UserInfo::ptr info, sylar::IDB::ptr conn);
    static int Delete(const int64_t& id, sylar::IDB::ptr conn);
    static int DeleteById( const int64_t& id, sylar::IDB::ptr conn);
    static int DeleteByName( const std::string& name, sylar::IDB::ptr conn);
    static int QueryAll(std::vector<UserInfo::ptr>& results, sylar::IDB::ptr conn);
    static UserInfo::ptr Query( const int64_t& id, sylar::IDB::ptr conn);
    static UserInfo::ptr QueryByName( const std::string& name, sylar::IDB::ptr conn);
    static int CreateTableSQLite3(sylar::IDB::ptr info);
    static int CreateTableMySQL(sylar::IDB::ptr info);
};

} //namespace rs
#endif //__RSUSER_INFO_H__
