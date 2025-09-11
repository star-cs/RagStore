# RagStore


```bash
./bin/orm ./bin/orm_conf/ ./orm # 生成 ORM 文件

```
## 升级CMake
[CMake版本升级](https://blog.csdn.net/qq_21570025/article/details/133924930)
```bash
wget https://cmake.org/files/v3.28/cmake-3.28.0.tar.gz
cd cmake-3.28.0

chmod 777 ./configure
./configure
make 
sudo make install

sudo update-alternatives --install /usr/bin/cmake cmake /usr/local/bin/cmake 1 --force
cmake --version
```


## webserver 协程网络库
环境依赖
```bash

```

## Minio 编译
```bash

```

## FAISS 编译
```bash
sudo apt install intel-mkl
sudo apt install swig
swig -version

sudo apt-get install libgflags-dev

cd faiss
mkdir build & cd build
cmake -DBUILD_TESTING=ON -DFAISS_ENABLE_GPU=OFF build ..

make
make demo_imi_flat
./demos/demo_imi_flat
sudo make install
```