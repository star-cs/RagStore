#include "my_module.h"
#include "sylar/core/config/config.h"
#include "sylar/core/log/log.h"
#include "sylar/core/application.h"
#include "sylar/core/env.h"
#include "sylar/net/http/http_server.h"
#include "sylar/net/tcp_server.h"
#include "sylar/io/db/mysql.h"
#include "servlet/user_servlet.h"
#include "servlet/file_servlet.h"

#include <memory>
#include <vector>

namespace rs
{
static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

MyModule::MyModule() : sylar::Module("rag_store", "1.0", "")
{
}

bool MyModule::onLoad()
{
    SYLAR_LOG_INFO(g_logger) << "onLoad";
    return true;
}

bool MyModule::onUnload()
{
    SYLAR_LOG_INFO(g_logger) << "onUnload";
    return true;
}

bool MyModule::onServerReady()
{
    SYLAR_LOG_INFO(g_logger) << "onServerReady";
    std::vector<sylar::TcpServer::ptr> svrs;
    if (!sylar::Application::GetInstance()->getServer("http", svrs)) {
        SYLAR_LOG_INFO(g_logger) << "no httpserver alive";
        return false;
    }

    for (auto &i : svrs) {
        auto http_server = std::dynamic_pointer_cast<sylar::http::HttpServer>(i);
        if (!http_server) {
            continue;
        }

        auto slt_dispatch = http_server->getServletDispatch();

        UserServlet::ptr user_servlet(new UserServlet);
        FileServlet::ptr file_servlet(new FileServlet);

        slt_dispatch->addGlobServlet("/api/user/*", user_servlet);
        slt_dispatch->addGlobServlet("/api/file/*", file_servlet);
    }

    return true;
}

bool MyModule::onServerUp()
{
    SYLAR_LOG_INFO(g_logger) << "onServerUp";
    return true;
}

extern "C"
{

    sylar::Module *CreateModule()
    {
        sylar::Module *module = new rs::MyModule;
        SYLAR_LOG_INFO(g_logger) << "CreateModule " << module;
        return module;
    }

    void DestoryModule(sylar::Module *module)
    {
        SYLAR_LOG_INFO(g_logger) << "CreateModule " << module;
        delete module;
    }
}
} // namespace rs