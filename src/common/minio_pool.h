#pragma once
#include "minio_client.h"
#include "free_queue.h"
#include <queue>
#include <mutex>

namespace rs
{
class MinioPool
{
public:
    MinioPool();
    ~MinioPool();

    std::unique_ptr<MinioClient> getClient();
    int releaseClient(std::unique_ptr<MinioClient> client);

private:
    std::mutex m_mutex;
    std::queue<std::unique_ptr<MinioClient>> minioClients;
};
} // namespace rs