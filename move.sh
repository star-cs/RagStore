#!/bin/bash
pro_name="rag_store"

cd "$(dirname "$0")"

# 创建目录（如果不存在）
if [ ! -d "bin/module" ]; then
    mkdir -p bin/module
else
    # 仅当链接存在时才删除
    if [ -L "bin/${pro_name}" ]; then
        unlink "bin/${pro_name}"
    fi
    if [ -L "bin/module/lib${pro_name}.so" ]; then
        unlink "bin/module/lib${pro_name}.so"
    fi
fi

# 复制文件
cp "third_party/webserver/bin/sylar" "bin/${pro_name}"
cp "lib/lib${pro_name}.so" "bin/module/"
