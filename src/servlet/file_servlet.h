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
class FileServlet : public sylar::http::Servlet
{
public:
    typedef std::shared_ptr<FileServlet> ptr;
    FileServlet();
    int32_t handle(sylar::http::HttpRequest::ptr request, sylar::http::HttpResponse::ptr response,
                   sylar::http::HttpSession::ptr session);

    void upload(sylar::http::HttpRequest::ptr request, sylar::http::HttpResponse::ptr response,
                sylar::http::HttpSession::ptr session);
    void download(sylar::http::HttpRequest::ptr request, sylar::http::HttpResponse::ptr response,
                  sylar::http::HttpSession::ptr session);
    void deleteFile(sylar::http::HttpRequest::ptr request, sylar::http::HttpResponse::ptr response,
                    sylar::http::HttpSession::ptr session);
    void query(sylar::http::HttpRequest::ptr request, sylar::http::HttpResponse::ptr response,
               sylar::http::HttpSession::ptr session);
    void update(sylar::http::HttpRequest::ptr request, sylar::http::HttpResponse::ptr response,
                sylar::http::HttpSession::ptr session);

private:
    std::unordered_map<std::string, FunCallBack> m_fun;
};

} // namespace rs