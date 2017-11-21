#pragma once

#ifndef BOOST_TEST_NO_MAIN
#define BOOST_TEST_NO_MAIN
#endif

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#define TEST_SUITE			BOOST_AUTO_TEST_SUITE
#define TEST_SUITE_END		BOOST_AUTO_TEST_SUITE_END
#define TEST_CASE			BOOST_AUTO_TEST_CASE
#define TEST_CHECK			BOOST_CHECK
#define FLOAT_CHECK(x, y)	BOOST_CHECK_CLOSE(x, y, 1e-5f);
#define TEST_REQUIRE		BOOST_REQUIRE
#define TEST_ERROR			BOOST_ERROR
#define TEST_FAIL			BOOST_FAIL
