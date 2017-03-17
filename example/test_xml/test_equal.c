/**
 * @file
 * @brief Equal assertions xml test file
 * @author Radosław Koppel <r.koppel\@k-el.com>
 * @date 2017
 *
 * File that contains exual part of the xml output test.
 * @sa emunit_test_xml
 */
#include "test.h"
#include <emunit.h>
#include <inttypes.h>

static void suite_init(void)
{
	test_expect_sinit_default("test_equal_suite");
}

static void suite_cleanup(void)
{
	test_expect_scleanup_default();
}

static void test_all_passed(void)
{
	test_expect_success_test();
	int a_i = -64;
	unsigned a_u = 78;

	UT_ASSERT_EQUAL(-64, a_i);
	UT_ASSERT_EQUAL_INT( -127, -127);
	UT_ASSERT_EQUAL_UINT(78, a_u);
	UT_ASSERT_EQUAL_HEX( 578, a_u + 500);

	UT_ASSERT_EQUAL_BOOL(  true, 1 == 1);
	int8_t val_i8 = 60;
	UT_ASSERT_EQUAL_INT8(  60, val_i8);
	int16_t val_i16 = 12000;
	UT_ASSERT_EQUAL_INT16( 12000, val_i16);
	int32_t val_i32 = 1000000;
	UT_ASSERT_EQUAL_INT32( 1000000, val_i32);
	int64_t val_i64 = INT64_MAX;
	UT_ASSERT_EQUAL_INT64( INT64_MAX, val_i64);

	uint8_t val_u8 = UINT8_MAX;
	UT_ASSERT_EQUAL_UINT8( 0, val_u8 + 1);

	UT_ASSERT_EQUAL_UINT16(UINT16_MAX, (uint16_t)(-1));
	UT_ASSERT_EQUAL_UINT32(UINT32_MAX, UINT32_MAX);
	UT_ASSERT_EQUAL_UINT64(UINT64_MAX, UINT64_MAX);
	UT_ASSERT_EQUAL_HEX8  (UINT8_MAX , UINT8_MAX);
	UT_ASSERT_EQUAL_HEX16 (UINT16_MAX, UINT16_MAX);
	UT_ASSERT_EQUAL_HEX32 (UINT32_MAX, UINT32_MAX);
	UT_ASSERT_EQUAL_HEX64 (UINT64_MAX, UINT64_MAX);
}

/**
 * @brief Macro that creates simple failed test
 *
 * Auxiliary macro that builds testing function.
 * The test would be expected to fail.
 *
 * @param name     The name of created function
 * @param opt      The macro option (the postfix after UT_ASSERT_EQUAL_)
 * @param e        Expected value
 * @param a        Actual value
 * @param pri_fmt  Print format
 * @param pri_cast The casting operator for the value used as a printf argument
 */
#define TEST_EQUAL_FAIL_CREATE(name, opt, e, a, pri_fmt, pri_cast) \
	static void name(void)                                         \
	{                                                              \
		test_expect_fail_assert(                                   \
			TEST_STR_ID_ANY,                                       \
			"EQUAL",                                               \
			NULL,                                                  \
			"[[:space:]]*<expected>"pri_fmt"</expected>"          \
			"[[:space:]]*<actual>"pri_fmt"</actual>",             \
			pri_cast e,                                            \
			pri_cast a                                             \
		);                                                         \
		UT_ASSERT_EQUAL_ ## opt(e, a);                             \
	}

static void test_fail_equal(void)
{
	test_expect_fail_assert(
		TEST_STR_ID_ANY,
		"EQUAL",
		NULL,
		"%s",
		"[[:space:]]*<expected>9223372036854775807</expected>"
		"[[:space:]]*<actual>-9223372036854775808</actual>"
	);
	UT_ASSERT_EQUAL(9223372036854775807LL, (int64_t)(1ULL<<63));
}

TEST_EQUAL_FAIL_CREATE(test_fail_equal_int,  INT, -200, -300, "%"PRId64, (int64_t))
TEST_EQUAL_FAIL_CREATE(test_fail_equal_uint, UINT, 455455455, 238238238, "%"PRIu64, (uint64_t))
TEST_EQUAL_FAIL_CREATE(test_fail_equal_hex,  HEX, 0x12345678, 0x87654321, "0x%"PRIx64, (uint64_t))

TEST_EQUAL_FAIL_CREATE(test_fail_equal_int8 , INT8 , -100, -127, "%"PRId8, )
TEST_EQUAL_FAIL_CREATE(test_fail_equal_int16, INT16, -10000, -12700, "%"PRId16, )
TEST_EQUAL_FAIL_CREATE(test_fail_equal_int32, INT32, -100000, -127000, "%"PRId32, )
TEST_EQUAL_FAIL_CREATE(test_fail_equal_int64, INT64, -100000000, -127000000, "%"PRId64, (int64_t))

TEST_EQUAL_FAIL_CREATE(test_fail_equal_uint8 , UINT8 , 100, 128, "%"PRIu8, )
TEST_EQUAL_FAIL_CREATE(test_fail_equal_uint16, UINT16, 10000, 12700, "%"PRIu16, )
TEST_EQUAL_FAIL_CREATE(test_fail_equal_uint32, UINT32, 100000, 127000, "%"PRIu32, )
TEST_EQUAL_FAIL_CREATE(test_fail_equal_uint64, UINT64, 100000000, 1ULL<<63, "%"PRIu64, (uint64_t))

TEST_EQUAL_FAIL_CREATE(test_fail_equal_hex8 , HEX8 , 0xff, 128, "0x%"PRIx8, )
TEST_EQUAL_FAIL_CREATE(test_fail_equal_hex16, HEX16, 10000, 12700, "0x%"PRIx16, )
TEST_EQUAL_FAIL_CREATE(test_fail_equal_hex32, HEX32, 100000, 127000, "0x%"PRIx32, )
TEST_EQUAL_FAIL_CREATE(test_fail_equal_hex64, HEX64, 0x12345678, 1ULL<<63, "0x%"PRIx64, (uint64_t))

UT_DESC_TS_BEGIN(test_equal_suite, suite_init, suite_cleanup, NULL, NULL)
	UT_DESC_TC(test_all_passed)
	UT_DESC_TC(test_fail_equal)
	UT_DESC_TC(test_fail_equal_int)
	UT_DESC_TC(test_fail_equal_uint)
	UT_DESC_TC(test_fail_equal_hex)
	UT_DESC_TC(test_fail_equal_int8)
	UT_DESC_TC(test_fail_equal_int16)
	UT_DESC_TC(test_fail_equal_int32)
	UT_DESC_TC(test_fail_equal_int64)
	UT_DESC_TC(test_fail_equal_uint8)
	UT_DESC_TC(test_fail_equal_uint16)
	UT_DESC_TC(test_fail_equal_uint32)
	UT_DESC_TC(test_fail_equal_uint64)
	UT_DESC_TC(test_fail_equal_hex8)
	UT_DESC_TC(test_fail_equal_hex16)
	UT_DESC_TC(test_fail_equal_hex32)
	UT_DESC_TC(test_fail_equal_hex64)
UT_DESC_TS_END();
