-- 用户表：存储系统用户信息
CREATE TABLE `users` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '用户ID',
  `username` varchar(50) NOT NULL COMMENT '用户名',
  `password_hash` varchar(255) NOT NULL COMMENT '密码哈希',
  `email` varchar(100) NOT NULL COMMENT '用户邮箱',
  `total_storage` bigint(20) NOT NULL DEFAULT 0 COMMENT '总存储空间(字节)',
  `used_storage` bigint(20) NOT NULL DEFAULT 0 COMMENT '已使用存储空间(字节)',
  `status` tinyint(4) NOT NULL DEFAULT 1 COMMENT '状态(0-禁用,1-正常)',
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  `updated_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间',
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_username` (`username`),
  UNIQUE KEY `uk_email` (`email`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户表';

-- 文件表：存储文件元数据
CREATE TABLE `files` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '文件ID',
  `user_id` bigint(20) NOT NULL COMMENT '所属用户ID',
  `file_name` varchar(255) NOT NULL COMMENT '文件名',
  `file_path` varchar(1024) NOT NULL COMMENT '文件在MinIO中的存储路径',
  `minio_bucket` varchar(100) NOT NULL COMMENT 'MinIO存储桶名称',
  `minio_upload_id` varchar(255) DEFAULT NULL COMMENT 'MinIO分片上传ID',
  `file_size` bigint(20) NOT NULL COMMENT '文件总大小(字节)',
  `file_type` varchar(100) NOT NULL COMMENT '文件类型(MIME类型)',
  `md5_hash` varchar(32) NOT NULL COMMENT '文件MD5哈希值',
  `status` tinyint(4) NOT NULL DEFAULT 0 COMMENT '状态(0-上传中,1-已完成,2-已删除)',
  `chunk_total` int(11) NOT NULL COMMENT '总分片数量',
  `chunk_size` int(11) NOT NULL COMMENT '分片大小(字节)',
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  `updated_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间',
  `last_accessed_at` datetime DEFAULT NULL COMMENT '最后访问时间',
  PRIMARY KEY (`id`),
  KEY `idx_user_id` (`user_id`),
  KEY `idx_status` (`status`),
  KEY `idx_md5_hash` (`md5_hash`),
  KEY `idx_minio_upload_id` (`minio_upload_id`),
  CONSTRAINT `fk_files_user_id` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='文件表';

-- 文件分片表：用于支持断点续传
CREATE TABLE `file_chunks` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '分片ID',
  `file_id` bigint(20) NOT NULL COMMENT '所属文件ID',
  `chunk_number` int(11) NOT NULL COMMENT '分片序号(从1开始)',
  `chunk_size` int(11) NOT NULL COMMENT '当前分片大小(字节)',
  `chunk_hash` varchar(32) NOT NULL COMMENT '分片MD5哈希值',
  `minio_part_number` int(11) NOT NULL COMMENT 'MinIO分片编号',
  `minio_etag` varchar(100) DEFAULT NULL COMMENT 'MinIO返回的ETag',
  `status` tinyint(4) NOT NULL DEFAULT 0 COMMENT '状态(0-未上传,1-已上传,2-校验失败)',
  `uploaded_at` datetime DEFAULT NULL COMMENT '上传时间',
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_file_chunk` (`file_id`,`chunk_number`),
  KEY `idx_status` (`status`),
  CONSTRAINT `fk_chunks_file_id` FOREIGN KEY (`file_id`) REFERENCES `files` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='文件分片表';
