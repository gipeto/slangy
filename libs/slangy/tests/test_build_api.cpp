#include <gmock/gmock.h>
#include <slang.h>
#include <slangy/slangy.hpp>

#include <test_shader.h>
#include <test_lib.h>

using namespace ::testing;
using namespace slangy;

TEST(ABuildShader, buildShaderProgram)
{
    slang::SessionDesc sd{};
    slang::TargetDesc td{};

#ifdef _WIN32
    td.format = SLANG_DXIL;
#elif __APPLE__
    td.format = SLANG_METAL;
#else
    td.format = SLANG_SPIRV;
#endif

    sd.targetCount = 1;
    sd.targets = &td;

    auto maybeSession = makeGlobalSession({});
    ASSERT_TRUE(maybeSession);

    auto maybeProgram =
        buildShaderProgram<test_shader::TEST_SHADER_SLANG_MODULE,test_shader::TEST_LIB_SLANG_MODULE>(sd, *maybeSession, "computeMain");
    ASSERT_TRUE(maybeProgram);
    ASSERT_THAT(maybeProgram.value().size(), Gt(0));
}
