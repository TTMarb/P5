/*! @file test_project.hpp
 *  @version 3.3
 *  @date Jun 05 2017
 *
 *  @brief
 *  Telemetry API usage in a Linux environment.
 *  Shows example usage of the new data subscription API.
 *
 *  @Copyright (c) 2017 DJI
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef DJIOSDK_TELEMETRYSAMPLE_HPP
#define DJIOSDK_TELEMETRYSAMPLE_HPP

// System Includes
#include <cmath>
#include <iostream>

// DJI OSDK includes
#include "dji_status.hpp"
#include <dji_vehicle.hpp>

// Helpers
#include <dji_linux_helpers.hpp>

general(DJI::OSDK::Vehicle* vehicle);
takeoff(DJI::OSDK::Vehicle* vehicle);
flytohight(DJI::OSDK::Vehicle* vehicle, uint8_t hight);
flyforward(DJI::OSDK::Vehicle* vehicle, uint8_t distance);
rotate(DJI::OSDK::Vehicle* vehicle, uint8_t angle);
#endif // DJIOSDK_TELEMETRYSAMPLE_HPP
