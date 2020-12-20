#include <map>
#include <iostream>
#include <chrono>
#include <utility>

int main() {
    /*
     * Пример 1
     * Допустим, мы точно знаем, что вставляемый ключ самый большой\минимальный среди ключей
     */
    {
        std::map<char, int> a;
        for (char i = 'b'; i <= 'z'; i++) {
            a.insert(std::pair<char, int>(i, 2));
        }

        std::map<char, int> b;
        for (char i = 'b'; i <= 'z'; i++) {
            b.insert(std::pair<char, int>(i, 2));
        }

        auto tmp = std::pair<char, int>('a', 2); 
        size_t N = 200;
        
        size_t time = 0;
        for (size_t i = 0; i < N; i++) {
            auto s = std::chrono::steady_clock::now();
            a.insert(tmp);
            auto d = std::chrono::steady_clock::now();
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(d - s).count();
            a.erase('a');
        }
        std::cout << time / N  << ' ';

        time = 0;
        for (size_t i = 0; i < N; i++) {
            auto s = std::chrono::steady_clock::now();
            b.insert(b.begin(), tmp);
            auto d = std::chrono::steady_clock::now();
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(d - s).count();
            b.erase('a');
        }
        std::cout << time / N << std::endl;
    }

    /*
     * Пример 2
     * Когда нам известна какая-то либо подсказка в виде дополнительного ключа, которая упрощает поиск в дереве.
     * Например, если примерно знаем стурктуру красно-черного дерева и знаем в какое поддерево может попасть ключ. 
     */
    {
        std::map<char, int> a;
        for (char i = 'a'; i <= 'z'; i++) {
            if (i == 'u') {
                continue;
            }
            a.insert(std::pair<char, int>(i, 2));
        }

        std::map<char, int> b;
        for (char i = 'a'; i <= 'z'; i++) {
            if (i == 'u') {
                continue;
            }
            b.insert(std::pair<char, int>(i, 2));
        }

        auto tmp = std::pair<char, int>('u', 2); 
        size_t N = 200;
        
        size_t time = 0;
        for (size_t i = 0; i < N; i++) {
            auto s = std::chrono::steady_clock::now();
            a.insert(tmp);
            auto d = std::chrono::steady_clock::now();
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(d - s).count();
            a.erase('u');
        }
        std::cout << time / N  << ' ';

        time = 0;
        for (size_t i = 0; i < N; i++) {
            auto it = b.find('t');
            auto s = std::chrono::steady_clock::now();
            b.insert(it, tmp);
            auto d = std::chrono::steady_clock::now();
            time += std::chrono::duration_cast<std::chrono::nanoseconds>(d - s).count();
            b.erase('u');
        }
        std::cout << time / N << std::endl;
    }
    return 0;
}

