
#include <memory>

struct A
{
    int i;
    A(int i_)
        : i {i_}
    {
        printf("A(%d) : %p\n", i, this);
    }

    ~A()
    {
        printf("~A(%d) : %p\n", i, this);
    }
};

std::unique_ptr<A>&& make_A()
{
    std::unique_ptr<A> int_ptr = std::make_unique<A>(1);
    return std::move(int_ptr);
}

int unique_ptr_test()
{
    auto ptr = make_A();

    printf("-\n");

    return 0;
}
