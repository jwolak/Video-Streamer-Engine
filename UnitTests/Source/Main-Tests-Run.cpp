#include <gtest/gtest.h>
#include "gmock/gmock.h"

#include "Logger.h"

#include "VideoEngineTests/Video-Device-Handler-Tests.cpp"
#include "VideoEngineTests/Device-Buffers-Control-Tests.cpp"
#include "VideoEngineTests/Video-Buffer-Tests.cpp"

int main(int argc, char **argv) {
    SET_LOG_LEVEL(equinox_logger::LogLevelType::LOG_LEVEL_DEBUG);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
