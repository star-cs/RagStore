#include "user_servlet.h"
#include "protocol.h"
#include "sylar/io/db/mysql.h"
#include "sylar/io/db/redis.h"
#include "user_info.h"
#include "sylar/net/http/servlet.h"
#include "sylar/core/util/hash_util.h"

namespace rs
{
static sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

UserServlet::UserServlet() : sylar::http::Servlet("user_servlet")
{
    m_fun["/api/user/login"] = std::bind(&UserServlet::login, this, std::placeholders::_1,
                                         std::placeholders::_2, std::placeholders::_3);
    m_fun["/api/user/logout"] = std::bind(&UserServlet::logout, this, std::placeholders::_1,
                                          std::placeholders::_2, std::placeholders::_3);
    m_fun["/api/user/register"] = std::bind(&UserServlet::registerUser, this, std::placeholders::_1,
                                            std::placeholders::_2, std::placeholders::_3);
}

int32_t UserServlet::handle(sylar::http::HttpRequest::ptr request,
                            sylar::http::HttpResponse::ptr response,
                            sylar::http::HttpSession::ptr session)
{
    SYLAR_LOG_INFO(g_logger) << "UserServlet::handle"
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

void UserServlet::login(sylar::http::HttpRequest::ptr request,
                        sylar::http::HttpResponse::ptr response,
                        sylar::http::HttpSession::ptr session)
{
    Message::ptr msg = Message::Create(request->getBody());
    UserInfo::ptr info(new UserInfo);
    info->setName(msg->get("name"));
    info->setPasswd(msg->get("passwd"));

    auto m_conn = sylar::MySQLMgr::GetInstance()->get("rag_store");
    auto user = UserInfoDao::QueryByName(info->getName(), m_conn);
    if (!user) {
        response->setStatus(sylar::http::HttpStatus::NOT_FOUND);
        response->setBody("User not found");
        return;
    }
    if (user->getPasswd() != info->getPasswd()) {
        response->setStatus(sylar::http::HttpStatus::UNAUTHORIZED);
        response->setBody("Password incorrect");
        return;
    }

    // 生成 Session
    std::string session_id =
        sylar::random_string(32, "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");

    // Session 添加到 Redis
    auto reply = sylar::RedisUtil::Cmd("rag_store",
                                       ("SET " + session_id + " " + user->toJsonString()).c_str());
    if (!reply) {
        response->setBody("Redis Internal Server Error");
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        return;
    }

    // 设置 Cookie
    response->setCookie("SESSIONID", session_id, 0, "/", "", false);

    response->setBody(user->toJsonString());
    response->setStatus(sylar::http::HttpStatus::OK);
}

void UserServlet::logout(sylar::http::HttpRequest::ptr request,
                         sylar::http::HttpResponse::ptr response,
                         sylar::http::HttpSession::ptr session)
{
    // 清除 Session Cookie
    response->setCookie("SESSIONID", "", -1, "/", "", false);
    response->setStatus(sylar::http::HttpStatus::OK);
    response->setBody("Logout successful");
}
void UserServlet::registerUser(sylar::http::HttpRequest::ptr request,
                               sylar::http::HttpResponse::ptr response,
                               sylar::http::HttpSession::ptr session)
{
    Message::ptr msg = Message::Create(request->getBody());
    UserInfo::ptr info(new UserInfo);
    info->setName(msg->get("name"));
    info->setPasswd(msg->get("passwd"));

    int ret = UserInfoDao::Insert(info, sylar::MySQLMgr::GetInstance()->get("rag_store"));
    if (ret == -1) {
        response->setStatus(sylar::http::HttpStatus::SERVICE_UNAVAILABLE);
        response->setBody("Registration failed");
        return;
    }
    response->setStatus(sylar::http::HttpStatus::OK);
    response->setBody(info->toJsonString());
}

} // namespace rs