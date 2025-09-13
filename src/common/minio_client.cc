#include "minio_client.h"
#include <exception>
#include <memory>
#include <fstream>
#include <sstream> // 包含必要的头文件以解决模板未定义问题

namespace rs
{

MinioClient::MinioClient(const std::string &minioEndpoint, const std::string &accessKey,
                         const std::string &secretKey, bool is_SSL)
    : minioEndpoint(minioEndpoint), accessKey(accessKey), secretKey(secretKey), useSSL(is_SSL)
{
    minio::s3::BaseUrl baseUrl = minio::s3::BaseUrl(minioEndpoint, useSSL);
    minio::creds::StaticProvider provider = minio::creds::StaticProvider(accessKey, secretKey);
    minioClient = std::make_unique<minio::s3::Client>(baseUrl, &provider);
}
MinioClient::~MinioClient()
{
    minioClient.reset();
}

// 获取上传ID
int MinioClient::getUploadID(const std::string &bucketName, const std::string &objectName,
                             std::string &uploadID)
{
    if (!minioClient) {
        return 1;
    }
    minio::s3::CreateMultipartUploadArgs createArgs;
    createArgs.bucket = bucketName;
    createArgs.object = objectName;
    minio::s3::CreateMultipartUploadResponse createResp =
        minioClient->CreateMultipartUpload(createArgs);
    if (!createResp) {
        std::cerr << "创建Multipart Upload失败: " << createResp.Error().String() << std::endl;
        return 1;
    }
    uploadID = createResp.upload_id;
    return 0;
}

// 分块上传
int MinioClient::uploadPart(const std::string &bucketName, const std::string &objectName,
                            const std::string &uploadID, int partNumber,
                            const std::string &partData, minio::s3::Part &part)
{
    if (!minioClient) {
        return 1;
    }
    minio::s3::UploadPartArgs uploadPartArgs;
    uploadPartArgs.bucket = bucketName;
    uploadPartArgs.object = objectName;
    uploadPartArgs.upload_id = uploadID;
    uploadPartArgs.part_number = partNumber;
    uploadPartArgs.data = partData;
    minio::s3::UploadPartResponse uploadPartResp = minioClient->UploadPart(uploadPartArgs);
    if (!uploadPartResp) {
        std::cerr << "分块 " << partNumber << " 上传失败: " << uploadPartResp.Error().String()
                  << std::endl;
        return 1;
    }
    part.number = partNumber;
    part.etag = uploadPartResp.etag;
    return 0;
}

// 完成分块上传
int MinioClient::completeUpload(const std::string &bucketName, const std::string &objectName,
                                const std::string &uploadID,
                                const std::list<minio::s3::Part> &partNumbers)
{
    if (!minioClient) {
        return 1;
    }
    minio::s3::CompleteMultipartUploadArgs completeArgs;
    completeArgs.bucket = bucketName;
    completeArgs.object = objectName;
    completeArgs.upload_id = uploadID;
    completeArgs.parts = partNumbers;
    minio::s3::CompleteMultipartUploadResponse completeResp =
        minioClient->CompleteMultipartUpload(completeArgs);
    if (!completeResp) {
        std::cerr << "完成Multipart Upload失败: " << completeResp.Error().String() << std::endl;
        return 1;
    }
    return 0;
}

int MinioClient::downloadPart(const std::string &bucketName, const std::string &objectName,
                              int partNumber, std::string &partData)
{
    if (!minioClient) {
        return 1;
    }
    minio::s3::GetObjectArgs args;
    args.bucket = bucketName;
    args.object = objectName;
    try {
        // 使用回调函数来处理响应数据
        args.datafunc = [&partData](minio::http::DataFunctionArgs dataArgs) -> bool {
            partData.append(dataArgs.datachunk.c_str(), dataArgs.datachunk.length());
            return true; // 返回true继续接收数据
        };

        // 添加HTTP Range支持，使用partNumber计算分块范围
        // 注意：这里假设每个分块大小为MIN_PART_SIZE
        size_t offset = static_cast<size_t>((partNumber - 1)) * MIN_PART_SIZE;
        size_t length = MIN_PART_SIZE;

        args.offset = &offset;
        args.length = &length;

        minio::s3::GetObjectResponse resp = minioClient->GetObject(args);
        if (!resp) {
            std::cerr << "分块 " << partNumber << " 下载失败: " << resp.Error().String()
                      << std::endl;
            return 1;
        }

    } catch (const std::exception &e) {
        std::cerr << "分块 " << partNumber << " 下载失败: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

int MinioClient::deleteObject(const std::string &bucketName, const std::string &objectName)
{
    if (!minioClient) {
        return 1;
    }
    minio::s3::RemoveObjectArgs args;
    args.bucket = bucketName;
    args.object = objectName;
    minio::s3::RemoveObjectResponse resp = minioClient->RemoveObject(args);
    if (!resp) {
        std::cerr << "删除失败: " << resp.Error().String() << std::endl;
        return 1;
    }
    return 0;
}
} // namespace rs