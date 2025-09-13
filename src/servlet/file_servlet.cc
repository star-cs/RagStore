#include "file_servlet.h"
#include "common/minio_client.h"
#include "protocol.h"
#include "sylar/io/db/mysql.h"
#include "sylar/io/db/redis.h"
#include "files_info.h"
#include "file_chunks_info.h"
#include "file_downloads_info.h"
#include "user_file_relations_info.h"
#include "sylar/net/http/servlet.h"
#include "sylar/core/util/hash_util.h"
#include "util.h"
#include <bits/types/struct_tm.h>
#include <memory>
namespace rs
{
enum class FilePermission : int32_t { READ = 1, WRITE = 2, FULL = 3 };

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

FileServlet::FileServlet() : sylar::http::Servlet("user_servlet")
{

    minioPool = std::make_unique<MinioPool>();
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

void FileServlet::getAllFileInfo(sylar::http::HttpRequest::ptr request,
                                 sylar::http::HttpResponse::ptr response,
                                 sylar::http::HttpSession::ptr session)
{
    if (!validateSession(request, response, session)) {
        response->setStatus(sylar::http::HttpStatus::UNAUTHORIZED);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::UNAUTHORIZED));
        return;
    }
    auto user_id = request->getParam("user_id");
    if (user_id.empty()) {
        response->setStatus(sylar::http::HttpStatus::BAD_REQUEST);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::BAD_REQUEST));
        return;
    }
    // 获取 user_id 关联的所有文件id
    std::vector<UserFileRelationsInfo::ptr> fileIdList;
    if (UserFileRelationsInfoDao::QueryByUserId(fileIdList, std::stoll(user_id),
                                                sylar::MySQLMgr::GetInstance()->get("rag_store"))) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    // 根据文件id获取所有文件信息
    Message::ptr msg(new Message);
    for (auto &fileId : fileIdList) {
        FilesInfo::ptr info = FilesInfoDao::Query(fileId->getFileId(),
                                                  sylar::MySQLMgr::GetInstance()->get("rag_store"));
        msg->set("file" + std::to_string(fileId->getFileId()), info->toJsonString());
    }
    response->setStatus(sylar::http::HttpStatus::OK);
    response->setBody(msg->toString());
}

void FileServlet::getUploadingFileInfo(sylar::http::HttpRequest::ptr request,
                                       sylar::http::HttpResponse::ptr response,
                                       sylar::http::HttpSession::ptr session)
{
    if (!validateSession(request, response, session)) {
        response->setStatus(sylar::http::HttpStatus::UNAUTHORIZED);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::UNAUTHORIZED));
        return;
    }
    auto user_id = request->getParam("user_id");
    if (user_id.empty()) {
        response->setStatus(sylar::http::HttpStatus::BAD_REQUEST);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::BAD_REQUEST));
        return;
    }
    std::vector<FileChunksInfo::ptr> fileChunksInfoList;
    // 通过user_id，查找所有分片上传的记录
    if (FileChunksInfoDao::QueryByUserId(fileChunksInfoList, std::stoll(user_id),
                                         sylar::MySQLMgr::GetInstance()->get("rag_store"))) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }
    // 遍历所有分片上传记录，返回每个文件最新的一条上传记录
    std::unordered_map<int64_t, FileChunksInfo::ptr> fileChunksInfoMap;
    for (auto &fileChunksInfo : fileChunksInfoList) {
        if (fileChunksInfoMap.find(fileChunksInfo->getFileId()) == fileChunksInfoMap.end()) {
            fileChunksInfoMap[fileChunksInfo->getFileId()] = fileChunksInfo;
        } else {
            if (fileChunksInfo->getChunkNumber()
                > fileChunksInfoMap[fileChunksInfo->getFileId()]->getChunkNumber()) {
                fileChunksInfoMap[fileChunksInfo->getFileId()] = fileChunksInfo;
            }
        }
    }
    Message::ptr msg(new Message);
    for (auto &fileChunksInfo : fileChunksInfoMap) {
        msg->set("fileChunk" + std::to_string(fileChunksInfo.second->getFileId()),
                 fileChunksInfo.second->toJsonString());
    }
    response->setStatus(sylar::http::HttpStatus::OK);
    response->setBody(msg->toString());
}

void FileServlet::md5(sylar::http::HttpRequest::ptr request,
                      sylar::http::HttpResponse::ptr response,
                      sylar::http::HttpSession::ptr session)
{
    if (!validateSession(request, response, session)) {
        response->setStatus(sylar::http::HttpStatus::UNAUTHORIZED);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::UNAUTHORIZED));
        return;
    }
    auto md5 = request->getParam("md5");
    // 文件不存在，生成 minio 分片上传 id
    auto user_id = request->getParam("user_id");
    auto file_name = request->getParam("file_name");
    auto file_size = request->getParam("file_size");
    auto file_type = request->getParam("file_type");

    if (user_id.empty() || file_name.empty() || file_type.empty() || file_size.empty()) {
        response->setStatus(sylar::http::HttpStatus::BAD_REQUEST);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::BAD_REQUEST));
        return;
    }

    FilesInfo::ptr fileInfo =
        FilesInfoDao::QueryByMd5Hash(md5, sylar::MySQLMgr::GetInstance()->get("rag_store"));
    if (!fileInfo) {
        // 1 文件已经存在，查找 当前用户是否拥有这个文件
        UserFileRelationsInfo::ptr userFileRelationsInfo =
            UserFileRelationsInfoDao::QueryByUserIdFileId(
                std::stoll(user_id), fileInfo->getId(),
                sylar::MySQLMgr::GetInstance()->get("rag_store"));
        if (!userFileRelationsInfo) {
            // 2 不存在，创建一个关联记录
            userFileRelationsInfo = std::make_shared<UserFileRelationsInfo>();
            userFileRelationsInfo->setUserId(std::stoll(user_id));
            userFileRelationsInfo->setFileId(fileInfo->getId());
            userFileRelationsInfo->setPermission(static_cast<int32_t>(FilePermission::FULL));
            if (UserFileRelationsInfoDao::Insert(
                    userFileRelationsInfo, sylar::MySQLMgr::GetInstance()->get("rag_store"))) {
                response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
                response->setBody(
                    HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
                return;
            }
        }

        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    // 3 文件不存在，生成上传 分片 id
    std::unique_ptr<MinioClient> minioClient = minioPool->getClient();
    if (!minioClient) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }
    std::string uploadID;
    auto ret = minioClient->getUploadID(file_type, file_name, uploadID);
    if (ret != 0 || uploadID.empty()) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    //事务
    auto db = sylar::MySQLMgr::GetInstance()->get("rag_store");
    if (!db) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }
    auto tx = db->openTransaction(false);
    if (!tx) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }
    Message::ptr msg(new Message);
    try {
        tx->begin();
        // 4 保存到 file 表
        FilesInfo::ptr fileInfo2 = std::make_shared<FilesInfo>();
        fileInfo2->setFileName(file_name);
        fileInfo2->setFilePath("");
        fileInfo2->setMinioBucket(file_type);
        fileInfo2->setMinioUploadId(uploadID);
        fileInfo2->setFileSize(std::stoll(file_size));
        fileInfo2->setFileType(file_type);
        fileInfo2->setMd5Hash(md5);
        fileInfo2->setStatus(0);

        if (FilesInfoDao::Insert(
                fileInfo2, std::dynamic_pointer_cast<sylar::MySQLTransaction>(tx)->getMySQL())) {
            tx->rollback();
            response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
            response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
            return;
        }

        // 5 保存到 user_file_relations 表
        UserFileRelationsInfo::ptr userFileRelationsInfo =
            std::make_shared<UserFileRelationsInfo>();
        userFileRelationsInfo->setUserId(std::stoll(user_id));
        userFileRelationsInfo->setFileId(fileInfo2->getId());
        userFileRelationsInfo->setPermission(static_cast<int32_t>(FilePermission::FULL));
        if (UserFileRelationsInfoDao::Insert(
                userFileRelationsInfo,
                std::dynamic_pointer_cast<sylar::MySQLTransaction>(tx)->getMySQL())) {
            tx->rollback();
            response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
            response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
            return;
        }
        tx->commit();

        msg->set("user_id", user_id);
        msg->set("file_id", std::to_string(fileInfo2->getId()));
        msg->set("uploadID", uploadID);
    } catch (const std::exception &e) {
        tx->rollback();
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    response->setStatus(sylar::http::HttpStatus::OK);
    response->setBody(msg->toString());
}

void FileServlet::upload(sylar::http::HttpRequest::ptr request,
                         sylar::http::HttpResponse::ptr response,
                         sylar::http::HttpSession::ptr session)
{
    if (!validateSession(request, response, session)) {
        response->setStatus(sylar::http::HttpStatus::UNAUTHORIZED);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::UNAUTHORIZED));
        return;
    }
    auto chunckHash = request->getParam("chunckHash");
    // 验证 request.body()  hash 一致
    if (sylar::md5(request->getBody()) != chunckHash) {
        response->setStatus(sylar::http::HttpStatus::BAD_REQUEST);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::BAD_REQUEST));
        return;
    }

    auto user_id = request->getParam("user_id");
    auto file_id = request->getParam("file_id");
    auto minio_bucket = request->getParam("minio_bucket");
    auto minio_object_name = request->getParam("minio_object_name");
    auto chunckSize = request->getParam("chunckSize");
    auto uploadID = request->getParam("uploadID");
    auto chunckNumber = request->getParam("chunckNumber");
    if (chunckSize.empty() || uploadID.empty() || chunckNumber.empty() || chunckHash.empty()) {
        response->setStatus(sylar::http::HttpStatus::BAD_REQUEST);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::BAD_REQUEST));
        return;
    }

    // 上传到 minio
    std::unique_ptr<MinioClient> minioClient = minioPool->getClient();
    if (!minioClient) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }
    minio::s3::Part part;
    auto ret = minioClient->uploadPart(minio_bucket, minio_object_name, uploadID,
                                       std::stoi(chunckNumber), request->getBody(), part);
    if (ret != 0) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    // 分片上传记录写入mysql file_chunks表
    FileChunksInfo::ptr fileChunkInfo = std::make_shared<FileChunksInfo>();
    fileChunkInfo->setFileId(std::stoll(file_id));
    fileChunkInfo->setUserId(std::stoll(user_id));
    fileChunkInfo->setChunkNumber(std::stoi(chunckNumber));
    fileChunkInfo->setChunkSize(std::stoll(chunckSize));
    fileChunkInfo->setChunkHash("xxx");
    fileChunkInfo->setMinioPartNumber(part.number);
    fileChunkInfo->setMinioEtag(part.etag);
    fileChunkInfo->setStatus(1);
    if (FileChunksInfoDao::Insert(fileChunkInfo,
                                  sylar::MySQLMgr::GetInstance()->get("rag_store"))) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    Message::ptr msg(new Message);
    msg->set("user_id", user_id);
    msg->set("file_id", file_id);
    msg->set("chunkNumber", chunckNumber);
    response->setStatus(sylar::http::HttpStatus::OK);
    response->setBody(msg->toString());
}

void FileServlet::completeUpload(sylar::http::HttpRequest::ptr request,
                                 sylar::http::HttpResponse::ptr response,
                                 sylar::http::HttpSession::ptr session)
{
    if (!validateSession(request, response, session)) {
        response->setStatus(sylar::http::HttpStatus::UNAUTHORIZED);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::UNAUTHORIZED));
        return;
    }

    auto user_id = request->getParam("user_id");
    auto file_id = request->getParam("file_id");
    auto file_size = request->getParam("file_size");
    auto minio_bucket = request->getParam("minio_bucket");
    auto minio_object_name = request->getParam("minio_object_name");
    auto uploadID = request->getParam("uploadID");
    if (user_id.empty() || file_id.empty() || file_size.empty() || minio_bucket.empty()
        || minio_object_name.empty() || uploadID.empty()) {
        response->setStatus(sylar::http::HttpStatus::BAD_REQUEST);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::BAD_REQUEST));
        return;
    }
    // 验证当前 file_id 是否上传完毕
    // 查询 file_chunk表 得到所有分片 记录
    std::vector<FileChunksInfo::ptr> fileChunks;
    if (FileChunksInfoDao::QueryByFileId(fileChunks, std::stoll(file_id),
                                         sylar::MySQLMgr::GetInstance()->get("rag_store"))) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }
    // 验证 fileChunks 所有的分片大小等于file_size，且 chunk_number 不能存在重复
    std::list<minio::s3::Part> parts;
    std::set<int> chunkNumbers;
    int totalSize = 0;
    for (const auto &fileChunk : fileChunks) {
        totalSize += fileChunk->getChunkSize();
        chunkNumbers.insert(fileChunk->getChunkNumber());

        minio::s3::Part part;
        part.number = fileChunk->getMinioPartNumber();
        part.etag = fileChunk->getMinioEtag();
        parts.push_back(part);
    }
    if (totalSize != std::stoll(file_size) || chunkNumbers.size() != fileChunks.size()) {
        response->setStatus(sylar::http::HttpStatus::BAD_REQUEST);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::BAD_REQUEST));
        return;
    }

    // 完成上传 调用 minio completeMultipartUpload 方法
    std::unique_ptr<MinioClient> minioClient = minioPool->getClient();
    if (!minioClient) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    auto ret = minioClient->completeUpload(minio_bucket, minio_object_name, uploadID, parts);
    if (ret != 0) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    // 上传完成后删除 file_chunks 表中记录
    if (FileChunksInfoDao::DeleteByFileId(std::stoll(file_id),
                                          sylar::MySQLMgr::GetInstance()->get("rag_store"))) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    // 更新 file 表中状态 todo 使用 redis 优化
    FilesInfo::ptr fileInfo =
        FilesInfoDao::Query(std::stoll(file_id), sylar::MySQLMgr::GetInstance()->get("rag_store"));
    if (!fileInfo) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }
    fileInfo->setStatus(1);
    if (FilesInfoDao::Update(fileInfo, sylar::MySQLMgr::GetInstance()->get("rag_store"))) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    response->setStatus(sylar::http::HttpStatus::OK);
    response->setBody("complete");
}

void FileServlet::getDownloadingFileInfo(sylar::http::HttpRequest::ptr request,
                                         sylar::http::HttpResponse::ptr response,
                                         sylar::http::HttpSession::ptr session)
{
    if (!validateSession(request, response, session)) {
        response->setStatus(sylar::http::HttpStatus::UNAUTHORIZED);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::UNAUTHORIZED));
        return;
    }

    auto user_id = request->getParam("user_id");
    if (user_id.empty()) {
        response->setStatus(sylar::http::HttpStatus::BAD_REQUEST);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::BAD_REQUEST));
        return;
    }

    std::vector<FileDownloadsInfo::ptr> fileDownloadsInfoList;
    // 通过user_id，查找所有分片下载的记录
    if (FileDownloadsInfoDao::QueryByUserId(fileDownloadsInfoList, std::stoll(user_id),
                                            sylar::MySQLMgr::GetInstance()->get("rag_store"))) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    // 遍历所有分片下载记录，返回每个文件最新的一条下载记录
    std::unordered_map<int64_t, FileDownloadsInfo::ptr> fileDownloadsInfoMap;
    for (auto &fileDownloadsInfo : fileDownloadsInfoList) {
        // 只返回下载状态为进行中的记录
        if (fileDownloadsInfo->getDownloadStatus() != 0) {
            continue;
        }

        if (fileDownloadsInfoMap.find(fileDownloadsInfo->getFileId())
            == fileDownloadsInfoMap.end()) {
            fileDownloadsInfoMap[fileDownloadsInfo->getFileId()] = fileDownloadsInfo;
        } else {
            if (fileDownloadsInfo->getDownloadStartTime()
                > fileDownloadsInfoMap[fileDownloadsInfo->getFileId()]->getDownloadStartTime()) {
                fileDownloadsInfoMap[fileDownloadsInfo->getFileId()] = fileDownloadsInfo;
            }
        }
    }

    Message::ptr msg(new Message);
    for (auto &fileDownloadsInfo : fileDownloadsInfoMap) {
        msg->set("fileDownload" + std::to_string(fileDownloadsInfo.second->getFileId()),
                 fileDownloadsInfo.second->toJsonString());
    }

    response->setStatus(sylar::http::HttpStatus::OK);
    response->setBody(msg->toString());
}

void FileServlet::download(sylar::http::HttpRequest::ptr request,
                           sylar::http::HttpResponse::ptr response,
                           sylar::http::HttpSession::ptr session)
{
    if (!validateSession(request, response, session)) {
        response->setStatus(sylar::http::HttpStatus::UNAUTHORIZED);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::UNAUTHORIZED));
        return;
    }

    auto user_id = request->getParam("user_id");
    auto file_id = request->getParam("file_id");
    auto chunk_number = request->getParam("chunk_number");

    if (user_id.empty() || file_id.empty() || chunk_number.empty()) {
        response->setStatus(sylar::http::HttpStatus::BAD_REQUEST);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::BAD_REQUEST));
        return;
    }

    // 查找文件信息
    FilesInfo::ptr fileInfo =
        FilesInfoDao::Query(std::stoll(file_id), sylar::MySQLMgr::GetInstance()->get("rag_store"));
    if (!fileInfo) {
        response->setStatus(sylar::http::HttpStatus::NOT_FOUND);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::NOT_FOUND));
        return;
    }

    // 验证文件状态是否为已上传完成
    if (fileInfo->getStatus() != 1) {
        response->setStatus(sylar::http::HttpStatus::BAD_REQUEST);
        response->setBody("File is not ready for download");
        return;
    }

    // 从Minio下载文件分片
    std::unique_ptr<MinioClient> minioClient = minioPool->getClient();
    if (!minioClient) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    std::string partData;
    auto ret = minioClient->downloadPart(fileInfo->getMinioBucket(), fileInfo->getMinioObjectName(),
                                         std::stoi(chunk_number), partData);
    if (ret != 0) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    // 记录下载信息到file_downloads表
    FileDownloadsInfo::ptr fileDownloadInfo = std::make_shared<FileDownloadsInfo>();
    fileDownloadInfo->setUserId(std::stoll(user_id));
    fileDownloadInfo->setFileId(std::stoll(file_id));
    fileDownloadInfo->setChunkNumber(std::stoi(chunk_number));
    fileDownloadInfo->setDownloadStartTime(time(nullptr));
    fileDownloadInfo->setDownloadSize(partData.size());
    fileDownloadInfo->setDownloadStatus(1); // 下载完成
    fileDownloadInfo->setDownloadEndTime(time(nullptr));
    fileDownloadInfo->setIpAddress(session->getSocket()->getRemoteAddress().toString());
    fileDownloadInfo->setUserAgent(request->getHeader("User-Agent"));

    if (FileDownloadsInfoDao::Insert(fileDownloadInfo,
                                     sylar::MySQLMgr::GetInstance()->get("rag_store"))) {
        // 插入失败不影响下载，继续返回数据
        SYLAR_LOG_WARN(g_logger) << "Failed to record download info for file_id=" << file_id;
    }

    // 设置响应头
    response->setHeader("Content-Type", fileInfo->getFileType());
    response->setHeader("Content-Disposition",
                        "attachment; filename=\"" + fileInfo->getFileName() + "\"");
    response->setHeader("Content-Length", std::to_string(partData.size()));
    response->setStatus(sylar::http::HttpStatus::OK);
    response->setBody(partData);
}

void FileServlet::deleteFile(sylar::http::HttpRequest::ptr request,
                             sylar::http::HttpResponse::ptr response,
                             sylar::http::HttpSession::ptr session)
{
    if (!validateSession(request, response, session)) {
        response->setStatus(sylar::http::HttpStatus::UNAUTHORIZED);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::UNAUTHORIZED));
        return;
    }
    auto user_id = request->getParam("user_id");
    auto file_id = request->getParam("file_id");
    if (user_id.empty() || file_id.empty()) {
        response->setStatus(sylar::http::HttpStatus::BAD_REQUEST);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::BAD_REQUEST));
        return;
    }
    // file查找 fileInfo
    FilesInfo::ptr fileInfo =
        FilesInfoDao::Query(std::stoll(file_id), sylar::MySQLMgr::GetInstance()->get("rag_store"));
    if (!fileInfo) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    // minio删除
    std::unique_ptr<MinioClient> minioClient = minioPool->getClient();
    if (!minioClient) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    auto ret =
        minioClient->deleteObject(fileInfo->getMinioBucket(), fileInfo->getMinioObjectName());
    if (ret != 0) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    // 事务
    auto db = sylar::MySQLMgr::GetInstance()->get("rag_store");
    if (!db) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }
    auto tx = db->openTransaction(false);
    if (!tx) {
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }
    try {
        // 从 user_file 关联表删除记录
        if (UserFileRelationsInfoDao::DeleteByUserIdFileId(
                std::stoll(file_id), std::stoll(user_id),
                std::dynamic_pointer_cast<sylar::MySQLTransaction>(tx)->getMySQL())) {
            response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
            response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
            return;
        }

        // 从 file 表中删除记录
        if (FilesInfoDao::Delete(
                std::stoll(file_id),
                std::dynamic_pointer_cast<sylar::MySQLTransaction>(tx)->getMySQL())) {
            response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
            response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
            return;
        }
        tx->commit();
    } catch (const std::exception &e) {
        tx->rollback();
        response->setStatus(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR);
        response->setBody(HttpStatusToString(sylar::http::HttpStatus::INTERNAL_SERVER_ERROR));
        return;
    }

    response->setStatus(sylar::http::HttpStatus::OK);
    response->setBody("delete success");
}

} // namespace rs