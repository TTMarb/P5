/*! @file test_project.cpp
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

#include "test_project.hpp"
#include <dji_telemetry.hpp>
#include <signal.h>
#include <stdlib.h>

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

bool
general(DJI::OSDK::Vehicle* vehicle)
{
  return true;
}

bool
takeoff(DJI::OSDK::Vehicle* vehicle)
{
  const int TIMEOUT = 1000;
  char      func[50];
  int       pkgIndex;

  int motorsNotStarted = 0;
  int timeoutCycles    = 20;

  int stillOnGround = 0;
  timeoutCycles     = 110;

  // Re-set Broadcast frequencies to their default values
  ACK::ErrorCode ack = vehicle->broadcast->setBroadcastFreqDefaults(TIMEOUT);
  // Start takeoff
  ACK::ErrorCode takeoffStatus = vehicle->control->takeoff(TIMEOUT);
  if (ACK::getError(takeoffStatus) != ACK::SUCCESS)
  {
    std::cout << "Takeoff failed:";
    ACK::getErrorCodeMessage(takeoffStatus, func);
    return true;
  }

  {
    while ((vehicle->broadcast->getStatus().flight <
            DJI::OSDK::VehicleStatus::M100FlightStatus::TAKEOFF) &&
           motorsNotStarted < timeoutCycles)
    {
      motorsNotStarted++;
      usleep(100000);
    }

    if (motorsNotStarted < timeoutCycles)
    {
      std::cout << "Successful TakeOff!" << std::endl;
    }
  }

  {
    while ((vehicle->broadcast->getStatus().flight !=
            DJI::OSDK::VehicleStatus::M100FlightStatus::IN_AIR_STANDBY) &&
           stillOnGround < timeoutCycles)
    {
      stillOnGround++;
      usleep(100000);
    }

    if (stillOnGround < timeoutCycles)
    {
      std::cout << "Aircraft in air!" << std::endl;
    }
  }

  {
    float32_t                 delta;
    Telemetry::GlobalPosition currentHeight;
    Telemetry::GlobalPosition deltaHeight =
      vehicle->broadcast->getGlobalPosition();

    do
    {
      sleep(4);
      currentHeight = vehicle->broadcast->getGlobalPosition();
      delta         = fabs(currentHeight.altitude - deltaHeight.altitude);
      deltaHeight.altitude = currentHeight.altitude;
    } while (delta >= 0.009);

    std::cout << "Aircraft hovering at " << currentHeight.altitude << "m!\n";
  }

  std::cout << "Done printing!\n";
}
bool
flytohight(DJI::OSDK::Vehicle* vehicle, uint8_t hight)
{
}
bool
flyforward(DJI::OSDK::Vehicle* vehicle, uint8_t distance)
{
}
bool
rotate(DJI::OSDK::Vehicle* vehicle, uint8_t angle)
{
}