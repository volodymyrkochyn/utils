// Example program
#include <iostream>
#include <string>
#include <functional>
#include <vector>


class OR {};
class AND {};
class NONE {};

template<class Op>
class Operation
{
public:
        Operation() {}
        template<class Context, class Method, class Container, class Return>
        Return call(const Container &container, Context c, Method m);
};

template<class Op>
template<class Context, class Method, class Container, class Return>
Return Operation<Op>::call(const Container &container, Context c, Method m) {
        (void)(c);
        auto func = std::bind(m, std::placeholders::_1);
        for (auto item : container)
                func(item);
        return Return();
}

template<>
template<class Context, class Method, class Container, class Return>
Return Operation<OR>::call(const Container &container, Context c, Method m) {
        Return r{false};
        auto func = std::bind(m, c, std::placeholders::_1);
        for (auto item : container)
                r |= func(item);
        return r;
}

template<>
template<class Context, class Method, class Container, class Return>
Return Operation<AND>::call(const Container &container, Context c, Method m) {
        Return r{true};
        auto func = std::bind(m, c, std::placeholders::_1);
        for (auto item : container)
                r &= func(item);
        return r;
}

template<class Context, class Method, class Oper, class Container, class Return=bool>
Return foreach_(const Container &container, Context c, Method m, Oper)
{
        Operation<Oper> op;
        Return r = op.call(container, c, m);
        return r;
}

int main()
{
    std::vector<int> d = {1, 2, 3, 4};
    auto show = [](int v) {
        std::cout << v << std::endl;
    };
    int dummy = 1;
    foreach_(d, dummy, &show, NONE());
}

