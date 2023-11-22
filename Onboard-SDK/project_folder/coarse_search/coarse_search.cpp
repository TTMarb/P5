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

#include "coarse_search.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

void tellMeAboutTheData(DJI::OSDK::Vehicle* vehicle){
    std::cout << "Entered tellMeAboutTheData: \n";
    setBroadcastFrequency(vehicle);
    Telemetry::GlobalPosition pos;
    Telemetry::Quaternion quaternion;
    int searchRadius = 20;
    float32_t droneAngle;

    std::cout << "Bout to calculate init position: \n";  
    pos = vehicle->broadcast->getGlobalPosition();
    int randomnumber = 1;
    
    float64_t iY = calcMfromLat(pos);
    float64_t iX = calcMfromLon(pos);
    randomnumber = getRandomNumber(10,randomnumber);
    float64_t tY = randomnumber;
    randomnumber = getRandomNumber(10,randomnumber);
    float64_t tX = randomnumber;
    std::cout << "target position calculated: tX = " << tY << ", tY = " << tX << "\n";
    std::cout << "about to enter while loop: \n";
    while(true){
        pos = vehicle->broadcast->getGlobalPosition();
        float64_t dY = calcMfromLat(pos)-iY;
        float64_t dX = calcMfromLon(pos)-iX;
        droneAngle = QtoDEG(vehicle);
        float64_t distanceTo = getSize(dY-tY, dX-tX);
        float64_t signalStrength = searchRadius-distanceTo;
        float64_t senderAngle = getAngle(dY-tY, dX-tX);
        float64_t targetAngle = senderAngle-90;
        if (targetAngle < 0) {
            targetAngle += 360;
        }
        float64_t diffAngle = targetAngle-droneAngle;
        if (diffAngle < 0) {
            diffAngle += 360;
        }
        float64_t A1 = pow(signalStrength,3)*cos((diffAngle*M_PI/180)-M_PI_4);
        float64_t A2 = pow(signalStrength,3)*cos((diffAngle*M_PI/180)+M_PI_4);
        std::cout << "dX: " << dX << ", dY: " << dY << "\n";
        //std::cout << "\t Position angle on sender: " << senderAngle << "\n";
        //std::cout << "\t Drones angle: " << droneAngle<< "\n";
        std::cout << "\t Distance from sender: " << distanceTo << "\n";
        //std::cout << "\t Target angle : " << targetAngle << "\n";
        std::cout << "\t Diff angle : " << diffAngle << "\n";
        std::cout << "\t Signal strength: " << signalStrength << "\n";
        std::cout << "\t A1: " << A1 << "\n";
        std::cout << "\t A2: " << A2 << "\n";
        float64_t alg = M_PI_4-acos((A1-A2)/sqrt(pow(A1,2)+pow(A2,2)));
        std::cout << "\t Alg: " << alg << "\n";
        sleep(2);
    }
}

float64_t getAngle(float64_t y, float64_t x) {
    //std::cout << "y: " << y << ", x: " << x << "\n";
    float64_t angle = atan2(y, x);
    if (angle < 0) {
        angle += 2 * M_PI;
    }
    angle *= 180.0 / M_PI;
    return angle;
}

float32_t QtoDEG(Vehicle* vehicle) {
    //This function converts the quaternion to degrees
    float32_t angle;
    Telemetry::Quaternion quaternion;
    quaternion = vehicle->broadcast->getQuaternion();
    //Largely based on a mix of https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Quaternion_to_Euler_angles_(in_3-2-1_sequence)_conversion
    //and the flight-control sample
    double t1 = +2.0 * (quaternion.q1 * quaternion.q2 + quaternion.q0 * quaternion.q3);
    double t0 = -2.0 * (quaternion.q2 * quaternion.q2 + quaternion.q3 * quaternion.q3) + 1.0;

    //The +90 is to make the drone face the same way as the sender
    //The -t1 is to make the drone rotate in the same direction as the sender
    angle = getAngle(-t1, t0)+90;
    //After adding 90, the angle can be above 360, so this makes sure it is between 0 and 360
    if (angle > 360) {
        angle -= 360;
    }
    return angle;
}

float64_t getSize(float64_t y, float64_t x) {
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

float64_t calcMfromLat(Telemetry::GlobalPosition pos){
    float64_t iY;
    //float64_t r_earth = 6378100;
    float64_t r_earth = 6356752;
    iY = pos.latitude*r_earth;
    return iY;
}

float64_t calcMfromLon(Telemetry::GlobalPosition pos){
    float64_t iX;
    //float64_t r_earth = 6378100;
    float64_t r_earth = 6356752; 
    iX = pos.longitude*cos(pos.latitude)*r_earth;
    return iX;
}
int getRandomNumber(int randomsize, int randomnumber){
    srand((unsigned) time(NULL) + randomnumber);
	int random = 10 + (rand() % randomsize);
    std::cout << "Random number: " << random << "\n";
    return random;
}
