#pragma once

#include <functional>

namespace native_invoke {
    typedef std::function<void()> native_invoke_function;
    typedef std::function<bool()> recurring_invoke_function;

    void init();
    void destroy();
    void add(native_invoke_function func);
    //func should return false when recurrance should stop 
    void add_recurring(recurring_invoke_function func);
    void run();
}