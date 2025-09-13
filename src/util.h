#pragma once
#include "sylar/net/http/http.h"
#include "sylar/net/http/http_session.h"
#include "sylar/io/db/redis.h"
#include "sylar/core/util/json_util.h"

namespace rs
{

// 验证 request 里的 session是否有效
inline bool validateSession(sylar::http::HttpRequest::ptr request,
                            sylar::http::HttpResponse::ptr response,
                            sylar::http::HttpSession::ptr session)
{
    auto session_id = request->getCookie("SESSIONID");
    if (session_id.empty()) {
        response->setStatus(sylar::http::HttpStatus::UNAUTHORIZED);
        response->setBody("SessionID 为空");
        return false;
    }
    auto session = sylar::RedisUtil::Get(session_id);
    if (session.empty()) {
        response->setStatus(sylar::http::HttpStatus::UNAUTHORIZED);
        response->setBody("SessionID 不存在");
        return false;
    }
    Json::Value session_json;
    sylar::JsonUtil::FromString(session_json, session);
    if (session_json.isMember("user")
        && session_json["user"].asString() == request->getParam("user")) {
        return true;
    }
    return false;
}
} // namespace rs