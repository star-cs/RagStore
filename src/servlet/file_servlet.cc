#include "file_servlet.h"
#include "protocol.h"
#include "sylar/io/db/mysql.h"
#include "sylar/io/db/redis.h"
#include "file_info.h"
#include "sylar/net/http/servlet.h"
#include "sylar/core/util/hash_util.h"

namespace rs
{
static sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

FileServlet::FileServlet() : sylar::http::Servlet("user_servlet")
{
    m_fun["/api/file/upload"] = std::bind(&FileServlet::upload, this, std::placeholders::_1,
                                          std::placeholders::_2, std::placeholders::_3);
    m_fun["/api/file/download"] = std::bind(&FileServlet::download, this, std::placeholders::_1,
                                            std::placeholders::_2, std::placeholders::_3);
    m_fun["/api/file/delete"] = std::bind(&FileServlet::deleteFile, this, std::placeholders::_1,
                                          std::placeholders::_2, std::placeholders::_3);
    m_fun["/api/file/query"] = std::bind(&FileServlet::query, this, std::placeholders::_1,
                                         std::placeholders::_2, std::placeholders::_3);
    m_fun["/api/file/update"] = std::bind(&FileServlet::update, this, std::placeholders::_1,
                                          std::placeholders::_2, std::placeholders::_3);
}

int32_t FileServlet::handle(sylar::http::HttpRequest::ptr request,
                            sylar::http::HttpResponse::ptr response,
                            sylar::http::HttpSession::ptr session)
{
    SYLAR_LOG_INFO(g_logger) << "FileServlet::handle"
                             << " request: " << HttpMethodToString(request->getMethod())
                             << " url: " << request->getPath();

    auto it = m_fun.find(request->getPath());
    if (it != m_fun.end()) {
        it->second(request, response, session);
    } else {
        response->setStatus(sylar::http::HttpStatus::NOT_FOUND);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::NOT_FOUND));
    }
    return session->sendResponse(response);
}

void FileServlet::upload(sylar::http::HttpRequest::ptr request,
                         sylar::http::HttpResponse::ptr response,
                         sylar::http::HttpSession::ptr session)
{
}
void FileServlet::download(sylar::http::HttpRequest::ptr request,
                           sylar::http::HttpResponse::ptr response,
                           sylar::http::HttpSession::ptr session)
{
}
void FileServlet::deleteFile(sylar::http::HttpRequest::ptr request,
                             sylar::http::HttpResponse::ptr response,
                             sylar::http::HttpSession::ptr session)
{
}
void FileServlet::query(sylar::http::HttpRequest::ptr request,
                        sylar::http::HttpResponse::ptr response,
                        sylar::http::HttpSession::ptr session)
{
}
void FileServlet::update(sylar::http::HttpRequest::ptr request,
                         sylar::http::HttpResponse::ptr response,
                         sylar::http::HttpSession::ptr session)
{
}

} // namespace rs