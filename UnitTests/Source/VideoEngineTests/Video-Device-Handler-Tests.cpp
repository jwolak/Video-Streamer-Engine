/*
 * Video-Device-Handler-Tests.cpp
 *
 *  Created on: 2022
 *      Author: Janusz Wolak
 */

/*-
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Janusz Wolak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include "../../../VideoEngine/source/Video-Device-Handler.cpp"

#include <gtest/gtest.h>

#include <memory>
#include <string>

namespace video_device_handler_tests {

namespace {
const std::string kFakeDevicePath = "/dev/null";
const std::string kInvalidDevicePath = "/dev/video99";
const std::string kEmptyDevicePath = "";
}

class VideoDeviceHandlerTests : public ::testing::Test {
public:
  VideoDeviceHandlerTests() : video_device_handler { new video_streamer::VideoDeviceHandler } {}

std::unique_ptr<video_streamer::VideoDeviceHandler> video_device_handler;

};

TEST_F(VideoDeviceHandlerTests, Try_Open_Device_From_Valid_Device_Path_And_It_Returns_True) {
  ASSERT_TRUE(video_device_handler->OpenDevice(kFakeDevicePath));
}

TEST_F(VideoDeviceHandlerTests, Try_Open_Device_From_Inalid_Device_Path_And_It_Returns_False) {
  ASSERT_FALSE(video_device_handler->OpenDevice(kInvalidDevicePath));
}

TEST_F(VideoDeviceHandlerTests, Try_Open_Device_From_Empty_Device_Path_And_It_Returns_False) {
  ASSERT_FALSE(video_device_handler->OpenDevice(kEmptyDevicePath));
}

TEST_F(VideoDeviceHandlerTests, Try_Close_Device_From_Valid_Device_Path_And_It_Returns_True) {
  video_device_handler->OpenDevice(kFakeDevicePath);
  ASSERT_TRUE(video_device_handler->CloseDevice());
}

TEST_F(VideoDeviceHandlerTests, Try_Close_Device_From_Invalid_Device_Path_And_It_Returns_False) {
  video_device_handler->OpenDevice(kInvalidDevicePath);
  ASSERT_FALSE(video_device_handler->CloseDevice());
}

TEST_F(VideoDeviceHandlerTests, Try_Call_GetDeviceFd_And_Is_No_Throw) {
  ASSERT_NO_THROW(video_device_handler->GetDeviceFd());
}

} /*namespace video_device_handler_tests*/
