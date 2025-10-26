
#include <iostream>
#include <slangy/error_handling.hpp>
#include <slangy/slangy.hpp>

#include <test_shader.h>

int main()
{
    using namespace slangy;

    if (auto globalSession = slangy::valueOrThrow(makeGlobalSession()))
    {
        std::cout << "[Success] Test package could create a global session and the compiled shader module is not empty"
                  << std::endl;
    }
    else
    {
        std::cout << "[Failure] Test package could not create a global session" << std::endl;
        return -1;
    }

    if (!shaders::TEST_SHADER_SLANG_MODULE.empty())
    {
        std::cout << "[Success] The pre-compiled test shader module is not empty" << std::endl;
        return 0;
    }

    std::cout << "[Failure] The pre-compiled test shader module is empty" << std::endl;
    return -1;
}
