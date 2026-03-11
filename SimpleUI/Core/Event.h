#pragma once
#include <functional>
#include <vector>

template<typename... Args>
class Event {
public:
    using Callback = std::function<void(Args...)>;

    void connect(Callback cb) {
        callbacks.push_back(cb);
    }

    void emit(Args... args) {
        for (auto& cb : callbacks)
            cb(args...);
    }

private:
    std::vector<Callback> callbacks;
};
