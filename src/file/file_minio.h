#pragma once

#include "file/file_operator.h"

namespace rs
{
class FileMinIO : public FileOperator
{
public:
    struct MinIO_Config {
        std::string access_key;
        std::string bucket_name;
        std::string endpoint;
        std::string secret_key;
    };

    FileMinIO(const MinIO_Config &config);
    virtual void upload(const File &file);
    virtual void download(const File &file);
    virtual void deleteFile(const File &file);
    virtual void query(const File &file);
    virtual void update(const File &file);
}

} // namespace rs