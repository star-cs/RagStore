#pragma once

#include <map>
#include <memory>

namespace rs
{
class Message
{
public:
    typedef std::shared_ptr<Message> ptr;

    static Message::ptr Create(const std::string &v);
    Message();
    std::string get(const std::string &name);
    void set(const std::string &name, const std::string &val);
    std::string toString() const;

private:
    std::map<std::string, std::string> m_datas;
};

} // namespace rs