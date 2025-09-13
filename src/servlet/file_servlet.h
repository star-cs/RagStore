#pragma once
#include "sylar/net/http/servlet.h"
#include <functional>
#include <string>
#include <unordered_map>
#include "common/minio_pool.h"

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
    /**
    获取所有文件信息，mysql和minio都存储完毕的文件。
    request: user_id
    */
    void getAllFileInfo(sylar::http::HttpRequest::ptr request,
                        sylar::http::HttpResponse::ptr response,
                        sylar::http::HttpSession::ptr session);
    /**
     获取所有的上传中的文件
     request: user_id 
     */
    void getUploadingFileInfo(sylar::http::HttpRequest::ptr request,
                              sylar::http::HttpResponse::ptr response,
                              sylar::http::HttpSession::ptr session);
    /**
    md5 秒传
    1. 如果没有当前文件，需要生成分片上传id ， 写入 file 得到 file_id， 写入 user_file_ 关联表
    */
    void md5(sylar::http::HttpRequest::ptr request, sylar::http::HttpResponse::ptr response,
             sylar::http::HttpSession::ptr session);

    /**
    分片上传
    POST application/octet-stream /api/file/upload?bucketName=rag_store&fileName=test.txt&partSize=1024&uploadID=123456&partNumber=1
    */
    void upload(sylar::http::HttpRequest::ptr request, sylar::http::HttpResponse::ptr response,
                sylar::http::HttpSession::ptr session);
    void completeUpload(sylar::http::HttpRequest::ptr request,
                        sylar::http::HttpResponse::ptr response,
                        sylar::http::HttpSession::ptr session);

    /**
    获取所有下载中的文件信息
    request: user_id
    */
    void getDownloadingFileInfo(sylar::http::HttpRequest::ptr request,
                                sylar::http::HttpResponse::ptr response,
                                sylar::http::HttpSession::ptr session);

    /**
    返回 minio 下载url
    */
    void download(sylar::http::HttpRequest::ptr request, sylar::http::HttpResponse::ptr response,
                  sylar::http::HttpSession::ptr session);

    /**
    删除文件
    */
    void deleteFile(sylar::http::HttpRequest::ptr request, sylar::http::HttpResponse::ptr response,
                    sylar::http::HttpSession::ptr session);

private:
    std::unordered_map<std::string, FunCallBack> m_fun;
    std::unique_ptr<MinioPool> minioPool;
    std::unordered_map<std::string, std::list<minio::s3::Part> > m_parts;
};

} // namespace rs