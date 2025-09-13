#include "minio_pool.h"
#include "sylar/core/config/config.h"
#include <utility>

namespace rs
{
static sylar::ConfigVar<std::map<std::string, std::map<std::string, std::string> > >::ptr
    g_minio_config =
        sylar::Config::Lookup("minio.config",
                              std::map<std::string, std::map<std::string, std::string> >(),
                              "minio client config");

MinioPool::MinioPool()
{
    auto minioConfig = g_minio_config->getValue();
    if (minioConfig.empty()) {
        return;
    }
    auto it = minioConfig.find("rag_store");
    if (it == minioConfig.end()) {
        return;
    }
    auto minioEndpoint = it->second["endpoint"];
    auto accessKey = it->second["access_key"];
    auto secretKey = it->second["secret_key"];
    auto useSSL = it->second["use_ssl"] == "true";
    auto pool = std::atoi(it->second["pool"].c_str());
    for (int i = 0; i < pool; i++) {
        minioClients.push(
            std::make_unique<MinioClient>(minioEndpoint, accessKey, secretKey, useSSL));
    }
}
MinioPool::~MinioPool()
{
}

std::unique_ptr<MinioClient> MinioPool::getClient()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (minioClients.empty()) {
        return nullptr;
    }
    auto client = std::move(minioClients.front());
    minioClients.pop();
    return client;
}

int MinioPool::releaseClient(std::unique_ptr<MinioClient> client)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    minioClients.push(std::move(client));
    return 0;
}
} // namespace rs
