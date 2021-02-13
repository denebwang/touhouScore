
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;
int main()
{
    for (auto& p : fs::directory_iterator())
        std::cout << p.path() << '\n';
}
