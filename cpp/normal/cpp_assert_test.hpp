#ifndef SANDBOX_CPP_ASSERT_TEST
#define SANDBOX_CPP_ASSERT_TEST

#include "cpp_assert.hpp"

namespace testing_cpp_assert
{
    class BasicClass
    {
    public:
        void class_function() {}
        static void static_class_function() {}
    };
    struct BasicStruct
    {
        void struct_function() {}
        static void static_struct_function() {}
    };

    void basic_function() {}

    ASSERT_IS_CLASS(BasicClass);
    ASSERT_IS_CLASS(BasicStruct);

    ASSERT_IS_FUNCTION(basic_function);
    // Doesn't work: ASSERT_IS_FUNCTION(BasicClass::class_function);
    ASSERT_IS_FUNCTION(BasicClass::static_class_function);
    // Doesn't work: ASSERT_IS_FUNCTION(BasicStruct::struct_function);
    ASSERT_IS_FUNCTION(BasicStruct::static_struct_function);

    ASSERT_IS_INTEGRAL(int8_t);
    ASSERT_IS_INTEGRAL(int16_t);
    ASSERT_IS_INTEGRAL(int32_t);
    ASSERT_IS_INTEGRAL(int64_t);
    ASSERT_IS_INTEGRAL(uint8_t);
    ASSERT_IS_INTEGRAL(uint16_t);
    ASSERT_IS_INTEGRAL(uint32_t);
    ASSERT_IS_INTEGRAL(uint64_t);

    ASSERT_IS_SIGNED(int8_t);
    ASSERT_IS_SIGNED(int16_t);
    ASSERT_IS_SIGNED(int32_t);
    ASSERT_IS_SIGNED(int64_t);
    ASSERT_IS_UNSIGNED(uint8_t);
    ASSERT_IS_UNSIGNED(uint16_t);
    ASSERT_IS_UNSIGNED(uint32_t);
    ASSERT_IS_UNSIGNED(uint64_t);

    ASSERT_ACTUAL_SIZEOF(int8_t, 1);
    ASSERT_ACTUAL_SIZEOF(int16_t, 2);
    ASSERT_ACTUAL_SIZEOF(int32_t, 4);
    ASSERT_ACTUAL_SIZEOF(int64_t, 8);
    ASSERT_ACTUAL_SIZEOF(uint8_t, 1);
    ASSERT_ACTUAL_SIZEOF(uint16_t, 2);
    ASSERT_ACTUAL_SIZEOF(uint32_t, 4);
    ASSERT_ACTUAL_SIZEOF(uint64_t, 8);

} // namespace testing_cpp_assert

#endif // SANDBOX_CPP_ASSERT_TEST
