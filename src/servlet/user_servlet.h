#pragma once
#include "sylar/net/http/servlet.h"
#include <functional>
#include <string>
#include <unordered_map>

namespace rs
{

typedef std::function<void(sylar::http::HttpRequest::ptr, sylar::http::HttpResponse::ptr,
                           sylar::http::HttpSession::ptr)>
    FunCallBack;
class UserServlet : public sylar::http::Servlet
{
public:
    typedef std::shared_ptr<UserServlet> ptr;
    UserServlet();
    int32_t handle(sylar::http::HttpRequest::ptr request, sylar::http::HttpResponse::ptr response,
                   sylar::http::HttpSession::ptr session);

    void login(sylar::http::HttpRequest::ptr request, sylar::http::HttpResponse::ptr response,
               sylar::http::HttpSession::ptr session);
    void logout(sylar::http::HttpRequest::ptr request, sylar::http::HttpResponse::ptr response,
                sylar::http::HttpSession::ptr session);
    void registerUser(sylar::http::HttpRequest::ptr request,
                      sylar::http::HttpResponse::ptr response,
                      sylar::http::HttpSession::ptr session);

private:
    std::unordered_map<std::string, FunCallBack> m_fun;
};

} // namespace rs