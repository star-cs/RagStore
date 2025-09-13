#include "protocol.h"
#include "sylar/core/log/log.h"
#include "sylar/core/util/json_util.h"
#include "json/value.h"
namespace rs
{
static sylar::Logger::ptr s_logger = sylar::Logger::GetLogger("system");

Message::ptr Message::Create(const std::string &v)
{
    SYLAR_LOG_INFO(s_logger) << v;
    Json::Value val;
    if (sylar::JsonUtil::FromString(val, v)) {
        return nullptr;
    }
    Message::ptr ret(new Message);
    auto names = val.getMemberNames();
    for (auto &i : names) {
        ret->m_datas[i] = val[i].asString();
    }
    return ret;
}

Message::Message()
{
}

std::string Message::get(const std::string &name)
{
    auto it = m_datas.find(name);
    return it == m_datas.end() ? "" : it->second;
}

void Message::set(const std::string &name, const std::string &val)
{
    m_datas[name] = val;
}

std::string Message::toString() const
{
    Json::Value json;
    for (auto &i : m_datas) {
        json[i.first] = i.second;
    }
    return sylar::JsonUtil::ToString(json);
}

} // namespace rs