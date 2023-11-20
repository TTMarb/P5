/*! @file mission_sample.cpp
 *  @version 3.3
 *  @date Jun 05 2017
 *
 *  @brief
 *  GPS Missions API usage in a Linux environment.
 *  Shows example usage of the Waypoint Missions and Hotpoint Missions through
 * the
 *  Mission Manager API.
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

#include "transceiver_search.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

void tellMeAboutTheData(DJI::OSDK::Vehicle* vehicle){
    setBroadcastFrequency(vehicle);
    Telemetry::GlobalPosition pos;
    float64_t dLatPos;
    float64_t dLonPos;
    float64_t senderLatPos;
    float64_t senderLonPos;
    pos = vehicle->broadcast->getGlobalPosition();

    //Note: Everything is NOT tested!
    // Create random number
    // Providing a seed value
	srand((unsigned) time(NULL));
	// Get a random number
	int random = rand() % 100;
    std::cout << "Random number 1: " << random << "\n";
    senderLatPos = pos.latitude+random*0.00001;
    std::cout << "Sender latitude: " << senderLatPos << "\n";
	int random = rand() % 100;
    std::cout << "Random number 2: " << random << "\n";
    senderLonPos = pos.longitude+random*0.00001;
    std::cout << "Sender longitude: " << senderLonPos << "\n";

    //Create random location based on init GPS location + a bit.
    while(true){
        pos = vehicle->broadcast->getGlobalPosition();
        dLatPos = pos.latitude-senderLatPos;
        dLonPos = pos.longitude-senderLonPos;
        std::cout << "Current position: " << pos.latitude << ", " << pos.longitude << "\n";
        std::cout << "Distance from start: " << dLatPos << ", " << dLonPos << "\n";
        std::cout << "angle on sender: " << getAngle(dLatPos, dLonPos) << "\n";
        std::cout << "Distance off sender: " << getSize(dLatPos, dLonPos) << "\n";
    }
}

float64_t getAngle(float64_t x, float64_t y) {
    float64_t angle = atan2(y, x) * 180.0 / M_PI;
    /*if (angle < 0) {
        angle += 360.0;
    }*/ //Aner ikke om det her kode er nødvendigt (○｀ 3′○)
    return angle;
}

float64_t getSize(float64_t x, float64_t y) {
    return sqrt(pow(x, 2) + pow(y, 2));
}

//Ikke nødvendigt i den endelige version, men her kan det bruges til test
void setBroadcastFrequency(Vehicle* vehicle) {
    //To ensure a faster response, the broadcast frequency is set to 100Hz for Quaternion
    enum FREQ {
        FREQ_0HZ = 0,
        FREQ_1HZ = 1,
        FREQ_10HZ = 2,
        FREQ_50HZ = 3,
        FREQ_100HZ = 4,
        FREQ_200HZ = 6,
        FREQ_400HZ = 7,
        FREQ_HOLD = 5,
    };

    //Timeout is for acknowledgement
    const int TIMEOUT = 20;
    //Initialises the frequency array - 16 is the given lenght by documentation
    uint8_t freq[16];
    /* Channels definition for M100
   * 0 - Timestamp
   * 1 - Attitude Quaterniouns
   * 2 - Acceleration
   * 3 - Velocity (Ground Frame)
   * 4 - Angular Velocity (Body Frame)
   * 5 - Position
   * 6 - Magnetometer
   * 7 - RC Channels Data
   * 8 - Gimbal Data
   * 9 - Flight Status
   * 10 - Battery Level
   * 11 - Control Information
   */
    freq[0] = FREQ_100HZ;
    freq[1] = FREQ_100HZ;
    freq[2] = FREQ_100HZ;
    freq[3] = FREQ_100HZ;
    freq[4] = FREQ_100HZ;
    freq[5] = FREQ_100HZ;
    freq[6] = FREQ_100HZ;
    freq[7] = FREQ_100HZ;
    freq[8] = FREQ_100HZ;
    freq[9] = FREQ_100HZ;
    freq[10] = FREQ_100HZ;
    freq[11] = FREQ_100HZ;

    ACK::ErrorCode ack = vehicle->broadcast->setBroadcastFreq(freq, TIMEOUT);
}
