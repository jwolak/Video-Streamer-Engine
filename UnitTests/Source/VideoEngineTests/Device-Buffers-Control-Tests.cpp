/*
 * Device-Buffers-Control-Tests.cpp
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

#include "../../../VideoEngine/source/Device-Buffers-Control.cpp"

#include "../Mocks/VideoDeviceHandlerMock.h"

#include <linux/videodev2.h>

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <cstring>

using ::testing::Return;
using ::testing::_;

namespace device_buffers_control_tests {

class DeviceBuffersControlTests : public ::testing::Test {
 public:
  DeviceBuffersControlTests() : video_device_handler_mock { new video_engine_tests_mocks::VideoDeviceHandlerMock },
  device_buffers_control { new video_streamer::DeviceBuffersControl(video_device_handler_mock.get()) } {}

  std::shared_ptr<video_engine_tests_mocks::VideoDeviceHandlerMock> video_device_handler_mock;
  std::unique_ptr<video_streamer::DeviceBuffersControl> device_buffers_control;
};

TEST_F(DeviceBuffersControlTests, Set) {

  v4l2_format tmp_frmt_buffer;

  std::memset(&tmp_frmt_buffer, 0, sizeof(tmp_frmt_buffer));
  tmp_frmt_buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  EXPECT_CALL(*video_device_handler_mock, GetDeviceFd()).WillOnce(Return(1));
  device_buffers_control->SetBuffreForDevice(VIDIOC_G_FMT, &tmp_frmt_buffer);
}

} /*namespace device_buffers_control_tests*/
