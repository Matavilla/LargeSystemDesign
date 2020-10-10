#include <iostream>
#include <memory>

class A {
public:

    static void* operator new(size_t size) {
        std::cout << "operator new!" << std::endl;
        return ::operator new(size);
    }

    static void operator delete(void* p, size_t size) {
        std::cout << "operator delete!" << std::endl;
        return ::operator delete(p);
    }
};

template<typename T, typename F = T>
struct NewAllocator {
    typedef T value_type;

    NewAllocator() {
    }

    template<class U>
    NewAllocator(const NewAllocator<U>& other) {
    }

    T* allocate(std::size_t n) {
        return reinterpret_cast<T*>(F::operator new(n * sizeof(F)));
    }

    void deallocate(T* p, std::size_t n) {
        F::operator delete(reinterpret_cast<F*>(p), n);
    }
};

int main() {
    NewAllocator<A> alloc;
    auto sp = std::allocate_shared<A>(alloc);
    return 0;
}
