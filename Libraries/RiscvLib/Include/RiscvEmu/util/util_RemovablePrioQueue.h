#pragma once
#include <queue>

namespace riscv {
namespace util {

template<typename T, typename... BaseArgs>
class RemovablePrioQueue : public std::priority_queue<T, BaseArgs...> {
public:
    using std::priority_queue<T, BaseArgs...>::priority_queue;

    bool remove(const T& val) {
        auto iter = std::find(this->c.begin(), this->c.end(), val);

        if(iter == this->c.end()) {
            return false;
        }
        else if(iter == this->c.begin()) {
            this->pop();
        }
        else {
            this->c.erase(iter);
            std::make_heap(this->c.begin(), this->c.end(), this->comp);
        }
        return true;
    }
}; // class RemovablePrioQueue

} // namespace util
} // namespace riscv
