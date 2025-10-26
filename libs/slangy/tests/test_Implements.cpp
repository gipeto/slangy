#include <gmock/gmock.h>

#include <slangy/com/Implements.hpp>

using namespace ::testing;
using namespace slangy;

struct ITestImplementsBase : public ISlangUnknown
{
    SLANG_COM_INTERFACE(0x2d1036eb, 0x3715, 0x4ff7, {0xa1, 0x1d, 0x5b, 0xcc, 0x12, 0x54, 0x93, 0x6f});
    virtual int baseValue() = 0;
};

struct ITestImplementsDerived : public ITestImplementsBase
{
    SLANG_COM_INTERFACE(0xfde2c355, 0xe472, 0x4a8a, {0xbe, 0x1d, 0xcb, 0xbc, 0xa6, 0x8b, 0x34, 0x29});
    virtual int derivedValue() = 0;
};

struct IAnotherTestImplements : public ISlangUnknown
{
    SLANG_COM_INTERFACE(0xad38b2ed, 0x68a7, 0x4000, {0xbf, 0x77, 0x31, 0xd3, 0xb8, 0xa2, 0xb8, 0x4});
    virtual int anotherValue() = 0;
};

struct ATestImplements : public Implements<ITestImplementsDerived, Base<ITestImplementsBase>>
{
    int baseValue() override
    {
        return 42;
    }
    int derivedValue() override
    {
        return 24;
    }
};

struct AnotherTestImplements
      : public Implements<IAnotherTestImplements, Base<ITestImplementsBase>, ITestImplementsDerived>
{
    int baseValue() override
    {
        return -1;
    }
    int derivedValue() override
    {
        return 1;
    }
    int anotherValue() override
    {
        return 0;
    }
};

TEST(TestImplements, AddRefRelease)
{
    ITestImplementsBase* testImplements = new ATestImplements();

    ASSERT_THAT(testImplements->addRef(), Eq(2));
    ASSERT_THAT(testImplements->release(), Eq(1));
    ASSERT_THAT(testImplements->release(), Eq(0));
}

TEST(ATestImplements, QueryInterfaceNotImplemented)
{
    ITestImplementsDerived* testImplements = new ATestImplements();
    IAnotherTestImplements* pUnrelated;
    ASSERT_THAT(
        testImplements->queryInterface(IAnotherTestImplements::getTypeGuid(), reinterpret_cast<void**>(&pUnrelated)),
        Eq(SLANG_E_NO_INTERFACE));
    ASSERT_THAT(pUnrelated, Eq(nullptr));
    ASSERT_THAT(testImplements->release(), Eq(0));
}

TEST(ATestImplements, QueryInterfaceInheritance)
{
    ISlangUnknown* testImplements = new ATestImplements();

    ISlangUnknown* pUnk;
    ASSERT_THAT(testImplements->queryInterface(ISlangUnknown::getTypeGuid(), reinterpret_cast<void**>(&pUnk)), Ge(0));
    ASSERT_THAT(pUnk, NotNull());
    ASSERT_THAT(pUnk->release(), Eq(1));

    ITestImplementsBase* pTestImplementsBase;
    ASSERT_THAT(testImplements->queryInterface(ITestImplementsBase::getTypeGuid(),
                                               reinterpret_cast<void**>(&pTestImplementsBase)),
                Ge(0));
    ASSERT_THAT(pTestImplementsBase, NotNull());
    ASSERT_THAT(pTestImplementsBase->baseValue(), Eq(42));
    ASSERT_THAT(pTestImplementsBase->release(), Eq(1));

    ITestImplementsDerived* pTestImplementsDerived;
    ASSERT_THAT(testImplements->queryInterface(ITestImplementsDerived::getTypeGuid(),
                                               reinterpret_cast<void**>(&pTestImplementsDerived)),
                Ge(0));
    ASSERT_THAT(pTestImplementsDerived, NotNull());
    ASSERT_THAT(pTestImplementsDerived->derivedValue(), Eq(24));
    ASSERT_THAT(pTestImplementsDerived->release(), Eq(1));

    ASSERT_THAT(testImplements->release(), Eq(0));
}

TEST(ATestImplements, QueryInterfaceMulipleInterfaces)
{
    IAnotherTestImplements* anotherTestImplements = new AnotherTestImplements();
    ISlangUnknown* testImplements = anotherTestImplements;  // shallow copy without AddRef

    ISlangUnknown* pUnk;
    ASSERT_THAT(testImplements->queryInterface(ISlangUnknown::getTypeGuid(), reinterpret_cast<void**>(&pUnk)), Ge(0));
    ASSERT_THAT(pUnk, NotNull());
    ASSERT_THAT(pUnk->release(), Eq(1));

    ITestImplementsBase* pTestImplementsBase;
    ASSERT_THAT(testImplements->queryInterface(ITestImplementsBase::getTypeGuid(),
                                               reinterpret_cast<void**>(&pTestImplementsBase)),
                Ge(0));
    ASSERT_THAT(pTestImplementsBase, NotNull());
    ASSERT_THAT(pTestImplementsBase->baseValue(), Eq(-1));
    ASSERT_THAT(pTestImplementsBase->release(), Eq(1));

    ITestImplementsDerived* pTestImplementsDerived;
    ASSERT_THAT(testImplements->queryInterface(ITestImplementsDerived::getTypeGuid(),
                                               reinterpret_cast<void**>(&pTestImplementsDerived)),
                Ge(0));
    ASSERT_THAT(pTestImplementsDerived, NotNull());
    ASSERT_THAT(pTestImplementsDerived->derivedValue(), Eq(1));
    ASSERT_THAT(pTestImplementsDerived->release(), Eq(1));

    IAnotherTestImplements* pAnotherTestImplements;
    ASSERT_THAT(testImplements->queryInterface(IAnotherTestImplements::getTypeGuid(),
                                               reinterpret_cast<void**>(&pAnotherTestImplements)),
                Ge(0));
    ASSERT_THAT(pAnotherTestImplements, NotNull());
    ASSERT_THAT(pAnotherTestImplements->anotherValue(), Eq(0));
    ASSERT_THAT(pAnotherTestImplements->release(), Eq(1));

    ASSERT_THAT(testImplements->release(), Eq(0));
}
