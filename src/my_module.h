#include "sylar/core/module.h"

namespace rs
{
class MyModule : public sylar::Module
{
public:
    typedef std::shared_ptr<MyModule> ptr;
    MyModule();
    bool onLoad() override;
    bool onUnload() override;
    bool onServerReady() override;
    bool onServerUp() override;
};

} // namespace rs