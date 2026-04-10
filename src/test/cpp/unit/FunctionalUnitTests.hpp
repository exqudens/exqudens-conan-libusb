#pragma once

#include <string>
#include <exception>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <exqudens/log/api/Logging.hpp>

#include "TestUtils.hpp"
#include "libusb.h"

class FunctionalUnitTests: public testing::Test {

    public:

        inline static const char* LOGGER_ID = "FunctionalUnitTests";

};

TEST_F(FunctionalUnitTests, test_free_device_list_1) {
    try {
        std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
        std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
        EXQUDENS_LOG_INFO(LOGGER_ID) << "bgn";

        libusb_context* ctx = nullptr;
        int libusb_init_result = libusb_init(&ctx);
        EXQUDENS_LOG_INFO(LOGGER_ID) << "libusb_init_result: " << libusb_init_result;

        ASSERT_EQ(LIBUSB_SUCCESS, libusb_init_result);

        libusb_device** device_list = nullptr;
        ssize_t libusb_get_device_list_result = libusb_get_device_list(ctx, &device_list);
        EXQUDENS_LOG_INFO(LOGGER_ID) << "libusb_get_device_list_result: " << libusb_get_device_list_result;
        libusb_free_device_list(device_list, 1);

        libusb_exit(ctx);

        EXQUDENS_LOG_INFO(LOGGER_ID) << "end";
    } catch (const std::exception& e) {
        std::string errorMessage = TestUtils::toString(e);
        std::cout << LOGGER_ID << " ERROR: " << errorMessage << std::endl;
        FAIL() << errorMessage;
    }
}
