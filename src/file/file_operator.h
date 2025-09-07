#pragma once

#include <string>
namespace rs
{
enum class FileType {
    txt = 1,
    doc = 2,
    xls = 3,
    ppt = 4,
    pdf = 5,
    zip = 6,
    rar = 7,
    jpg = 8,
    png = 9,
    gif = 10,
    mp3 = 11,
    mp4 = 12,
    other = 13
};

struct File {
    std::string name;
    std::string url;
    std::string md5;
    FileType type;
    uint64_t size;
    std::string time;
    std::string user;
};

class FileOperator
{
    virtual void upload(const File &file) = 0;
    virtual void download(const File &file) = 0;
    virtual void deleteFile(const File &file) = 0;
    virtual void query(const File &file) = 0;
    virtual void update(const File &file) = 0;
};
} // namespace rs