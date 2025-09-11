#include <atomic>
#include <memory>

template <typename T>
class LockFreeQueue
{
private:
    struct Node {
        std::shared_ptr<T> data;
        std::atomic<Node *> next;

        Node() : next(nullptr) {}
        explicit Node(T value) : data(std::make_shared<T>(std::move(value))), next(nullptr) {}
    };

    std::atomic<Node *> head;
    std::atomic<Node *> tail;

public:
    LockFreeQueue()
    {
        Node *dummy = new Node();
        head.store(dummy);
        tail.store(dummy);
    }

    ~LockFreeQueue()
    {
        while (Node *const old_head = head.load()) {
            head.store(old_head->next);
            delete old_head;
        }
    }

    // 禁止拷贝和赋值
    LockFreeQueue(const LockFreeQueue &) = delete;
    LockFreeQueue &operator=(const LockFreeQueue &) = delete;

    void enqueue(T value)
    {
        Node *new_node = new Node(std::move(value));

        while (true) {
            Node *old_tail = tail.load();
            Node *next = old_tail->next.load();

            // 检查tail是否仍然指向我们看到的节点
            if (old_tail == tail.load()) {
                // 如果tail指向最后一个节点
                if (next == nullptr) {
                    // 尝试将新节点链接到列表末尾
                    if (old_tail->next.compare_exchange_weak(next, new_node)) {
                        // 成功添加节点，尝试移动tail
                        tail.compare_exchange_weak(old_tail, new_node);
                        return;
                    }
                } else {
                    // tail没有指向最后一个节点，尝试帮助其他线程完成操作
                    tail.compare_exchange_weak(old_tail, next);
                }
            }
        }
    }

    bool dequeue(std::shared_ptr<T> &result)
    {
        while (true) {
            Node *old_head = head.load();
            Node *old_tail = tail.load();
            Node *next = old_head->next.load();

            // 检查head是否仍然指向我们看到的节点
            if (old_head == head.load()) {
                // 如果head和tail指向同一个节点，队列可能为空或处于中间状态
                if (old_head == old_tail) {
                    // 如果next为空，说明队列为空
                    if (next == nullptr) {
                        return false;
                    }
                    // tail落后了，帮助推进它
                    tail.compare_exchange_weak(old_tail, next);
                } else {
                    // 确保在读取数据前获取next指针
                    if (next == nullptr) {
                        continue;
                    }

                    // 尝试移动head指针
                    result = next->data;
                    if (head.compare_exchange_weak(old_head, next)) {
                        delete old_head;
                        return true;
                    }
                }
            }
        }
    }

    bool empty() const { return head.load()->next.load() == nullptr; }
};