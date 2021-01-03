#include <iostream>
#include <string>
#include <functional>
#include <vector>


class OR_BOOL {};
class AND_BOOL {};
class NONE {};

template<class Op, class Return>
class Operation
{
    Return return_value;
public:
        template<class Context, class Method, class Container>
        void call(const Container &container, Context c, Method m);
        Return getResult() const { return return_value; }
};

template<class Op, class Return>
template<class Context, class Method, class Container>
void Operation<Op, Return>::call(const Container &container, Context c, Method m) {
        (void)(c);
        auto func = std::bind(m, std::placeholders::_1);
        for (auto item : container)
                func(item);
}

template<>
template<class Context, class Method, class Container>
void Operation<OR_BOOL, bool>::call(const Container &container, Context c, Method m) {
        return_value = false;
        auto func = std::bind(m, c, std::placeholders::_1);
        for (auto item : container)
                return_value |= func(item);
}

template<>
template<class Context, class Method, class Container>
void Operation<AND_BOOL, bool>::call(const Container &container, Context c, Method m) {
        return_value = true;
        auto func = std::bind(m, c, std::placeholders::_1);
        for (auto item : container)
                return_value &= func(item);
}


class AND_INT{};

template<>
template<class Context, class Method, class Container>
void Operation<AND_INT, int>::call(const Container &container, Context c, Method m) {
        return_value = 0;
        auto func = std::bind(m, c, std::placeholders::_1);
        for (auto item : container)
                return_value |= func(item);
}

template<class Context, class Method, class Oper, class Container, class Return>
void foreach_(const Container &container, Context c, Method m, Return &res, Oper)
{
        Operation<Oper, Return> op;
        op.call(container, c, m);
        res = op.getResult();
}

struct And {
    int s{0};
    int sum(int i) { s += i; return i;}
};

int main()
{
    std::vector<int> d = {1, 2, 3, 4};
    
    // example 1
    auto show = [](int &v) {
        std::cout << v << std::endl;
    };
    bool ok;
    foreach_(d, NONE(), show, ok, NONE());
    
    // example 2
    And suma;
    int and_;
    foreach_(d, &suma, &And::sum, and_, AND_INT());
    std::cout << suma.s << "|" << and_ << std::endl;
    return 0;
}

