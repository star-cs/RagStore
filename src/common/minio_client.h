#pragma once
#include <string>
#include <miniocpp/client.h>
#include <miniocpp/providers.h>

namespace rs
{
class MinioClient
{
public:
    MinioClient(const std::string &minioEndpoint, const std::string &accessKey,
                const std::string &secretKey, bool is_SSL);
    ~MinioClient();

    // 获取上传ID
    // bucket object 重复，每次返回都是唯一的。
    int getUploadID(const std::string &bucketName, const std::string &objectName,
                    std::string &uploadID);

    // 分块上传
    int uploadPart(const std::string &bucketName, const std::string &objectName,
                   const std::string &uploadID, int partNumber, const std::string &partData,
                   minio::s3::Part &part);

    // 完成分块上传
    int completeUpload(const std::string &bucketName, const std::string &objectName,
                       const std::string &uploadID, const std::list<minio::s3::Part> &partNumbers);

    // 分块下载
    int downloadPart(const std::string &bucketName, const std::string &objectName, int partNumber,
                     std::string &partData);

    // 删除文件
    int deleteObject(const std::string &bucketName, const std::string &objectName);

private:
    std::string minioEndpoint;
    std::string accessKey;
    std::string secretKey;
    bool useSSL = false;
    std::unique_ptr<minio::s3::Client> minioClient;

    const size_t CHUNK_SIZE = 32 * 1024;          // 32KB - 模拟Web客户端分块大小
    const size_t MIN_PART_SIZE = 5 * 1024 * 1024; // 5MB - MinIO Multipart Upload最小分块要求
};

} // namespace rs