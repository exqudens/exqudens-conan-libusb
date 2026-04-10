#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <exception>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <exqudens/log/api/Logging.hpp>

#include "TestUtils.hpp"
#include "stress.h"

class FunctionalSystemTests: public testing::Test {

    public:

        inline static const char* LOGGER_ID = "FunctionalSystemTests";

};

TEST_F(FunctionalSystemTests, test_init_and_exit_1) {
    try {
        std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
        std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "bgn";

        std::vector<std::string> stressTests = {
            "init_and_exit"
        };
        std::vector<char*> argv = {};
        for (size_t i = 0; i < stressTests.size(); i++) {
            argv.emplace_back(const_cast<char*>(stressTests.at(i).c_str()));
        }
        int result = stress_main((int) argv.size(), argv.data());

        ASSERT_EQ(0, result);

        EXQUDENS_LOG_INFO(LOGGER_ID) << "end";
    } catch (const std::exception& e) {
        std::string errorMessage = TestUtils::toString(e);
        std::cout << LOGGER_ID << " ERROR: " << errorMessage << std::endl;
        FAIL() << errorMessage;
    }
}

TEST_F(FunctionalSystemTests, test_get_device_list_1) {
    try {
        std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
        std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "bgn";

        std::vector<std::string> stressTests = {
            "get_device_list"
        };
        std::vector<char*> argv = {};
        for (size_t i = 0; i < stressTests.size(); i++) {
            argv.emplace_back(const_cast<char*>(stressTests.at(i).c_str()));
        }
        int result = stress_main((int) argv.size(), argv.data());

        ASSERT_EQ(0, result);

        EXQUDENS_LOG_INFO(LOGGER_ID) << "end";
    } catch (const std::exception& e) {
        std::string errorMessage = TestUtils::toString(e);
        std::cout << LOGGER_ID << " ERROR: " << errorMessage << std::endl;
        FAIL() << errorMessage;
    }
}

TEST_F(FunctionalSystemTests, test_many_device_lists_1) {
    try {
        std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
        std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "bgn";

        std::vector<std::string> stressTests = {
            "many_device_lists"
        };
        std::vector<char*> argv = {};
        for (size_t i = 0; i < stressTests.size(); i++) {
            argv.emplace_back(const_cast<char*>(stressTests.at(i).c_str()));
        }
        int result = stress_main((int) argv.size(), argv.data());

        ASSERT_EQ(0, result);

        EXQUDENS_LOG_INFO(LOGGER_ID) << "end";
    } catch (const std::exception& e) {
        std::string errorMessage = TestUtils::toString(e);
        std::cout << LOGGER_ID << " ERROR: " << errorMessage << std::endl;
        FAIL() << errorMessage;
    }
}

TEST_F(FunctionalSystemTests, test_default_context_change_1) {
    try {
        std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
        std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "bgn";

        std::vector<std::string> stressTests = {
            "default_context_change"
        };
        std::vector<char*> argv = {};
        for (size_t i = 0; i < stressTests.size(); i++) {
            argv.emplace_back(const_cast<char*>(stressTests.at(i).c_str()));
        }
        int result = stress_main((int) argv.size(), argv.data());

        ASSERT_EQ(0, result);

        EXQUDENS_LOG_INFO(LOGGER_ID) << "end";
    } catch (const std::exception& e) {
        std::string errorMessage = TestUtils::toString(e);
        std::cout << LOGGER_ID << " ERROR: " << errorMessage << std::endl;
        FAIL() << errorMessage;
    }
}
