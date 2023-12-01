/*! @file coarse_search.cpp
 *  @date Nov. 11, 2023
 *
 *  @brief
 * @Copyright (c) 2017 DJI
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
//#include "DataFaker.h"
#include "coarse_search.hpp"

using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

/*void initializeFake(DJI::OSDK::Vehicle* vehicle, DataFaker* df, FIO* fileIO) {
    Telemetry::GlobalPosition pos;
    std::cout << "Bout to calculate init position: \n";
    pos = vehicle->broadcast->getGlobalPosition();
    std::cout << "X-location 4 transceiver: " << std::endl;
    int xLoc;
    std::cin >> xLoc;
    std::cout << "Y-location 4 transceiver: " << std::endl;
    int yLoc;
    std::cin >> yLoc;
    std::string filename = "trace" + std::to_string(xLoc) + std::to_string(yLoc) + ".csv";
    fileIO->changeActiveFile(filename);
    fileIO->createFile();
    df->init(vehicle, 1000, xLoc, yLoc);
}*/

float calcH(DJI::OSDK::Vehicle* vehicle, float* A1, float* A2, float* H) { return sqrt(pow(*A1, 2) + pow(*A2, 2)); }

float calcAlg(DJI::OSDK::Vehicle* vehicle, float* A1, float* A2, float* H) {
    return (acos((*A1 - *A2) / (*H + 0.001)) - M_PI_2) * (180 / M_PI);
}

float calcVel(DJI::OSDK::Vehicle* vehicle, float* H, float* prevH, int* cnt, int* mult) {
    if (H < prevH) {
        if (*cnt > (5 + 1)) {
            *mult *= -1;
            std::cout << "\t\t\t changed velocityraptor" << std::endl;
            cnt = 0;
        }
    } else {
        cnt = 0;
    }
    return ((1 - log1p(*H)) + (1 / 0.1)) * (*mult);
}

void controlVehicle(DJI::OSDK::Vehicle* vehicle, float* vel, float* alg, FIO* fileIO, PIcontroller* yawRate,
                    PIcontroller* vX, PIcontroller* vY, int sampleFrequency, int* timecounterMilliseconds) {
    std::cout << "Entered tellMeAboutTheData: \n";
    Telemetry::GlobalPosition pos;
    Telemetry::Quaternion quaternion;
    int searchRadius = 20;
    float UAVAngle;
    float sampleTimeInSeconds = 1 / sampleFrequency;
    int maxADCvalue = 4096;

    yawRate->updatePIController(alg);
    //Calculate velocity in x and y direction
    //Sets velocity and yaw rate
    for (int i = 0; i < sampleFrequency; i++) {
        UAVAngle = QtoDEG(vehicle)*(M_PI / 180);
        float eX = *vel * cosf(UAVAngle);
        float eY = *vel * sinf(UAVAngle);
        vX->updatePIController(&eX);
        vY->updatePIController(&eY);
        vehicle->control->velocityAndYawRateCtrl(vX->PIvalue, vY->PIvalue, 0, yawRate->PIvalue);
        float sampleTimeInMicroSeconds = sampleTimeInSeconds * 1000 * 1000;
        timecounterMilliseconds += 10;
        //df.Fake(vehicle,fileIO,false);
        usleep(sampleTimeInMicroSeconds);
    }
}

/// @brief Calculate the angle (in degrees) between two vectors
/// @param vector1
/// @param vector2
/// @return Returns the angle between the vectors
float getAngle(float vector1, float vector2) {
    float angleBetweenVectors = atan2(vector1, vector2);
    //converts from -pi to pi to 0 to 2pi
    if (angleBetweenVectors < 0) {
        angleBetweenVectors += 2 * M_PI;
    }
    //converts from radians to degrees
    angleBetweenVectors *= 180.0 / M_PI;
    return angleBetweenVectors;
}

/// @brief Reads the Quaternation degrees, and converts them into a yaw degree.
/// @param vehicle Pointer to the DJI vehicle class
/// @return Returns the drones current angle on east
float QtoDEG(Vehicle* vehicle) {
    Telemetry::Quaternion quaternion;
    float angle;
    quaternion = vehicle->broadcast->getQuaternion();
    //This code is largely based on a mix of
    //https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Quaternion_to_Euler_angles_(in_3-2-1_sequence)_conversion
    //and the flight-control sample
    double t1 = +2.0 * (quaternion.q1 * quaternion.q2 + quaternion.q0 * quaternion.q3);
    double t0 = -2.0 * (quaternion.q2 * quaternion.q2 + quaternion.q3 * quaternion.q3) + 1.0;

    //We add +90 to correct the 0 degree angle to be east instead of north
    //We multiply t1 with -1 to rotate anti-clockwise instead of clockwise
    angle = getAngle(-t1, t0) + 90;
    //After adding 90, the angle can be above 360, so this makes sure it is between 0 and 360
    if (angle > 360) {
        angle -= 360;
    }
    return angle;
}

//This functions finds the length of a vector
float getSize(float y, float x) { return sqrt(pow(x, 2) + pow(y, 2)); }

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

void UAVtakoff(Vehicle* vehicle, int functionTimeout) {
    setBroadcastFrequency(vehicle);
    Telemetry::Status status = vehicle->broadcast->getStatus();

    ACK::ErrorCode ctrlAuth = vehicle->obtainCtrlAuthority(functionTimeout);
    if (ACK::getError(ctrlAuth)) {
        ACK::getErrorCodeMessage(ctrlAuth, __func__);
    }
    if (status.flight < 2) {
        sleep(5);
        std::cout << "Preparing UAV" << std::endl;

        std::cout << "Arm motor \n";
        ACK::ErrorCode armAck = vehicle->control->armMotors(functionTimeout);
        if (ACK::getError(armAck)) {
            ACK::getErrorCodeMessage(armAck, __func__);
        }

        std::cout << "About to take off \n";
        sleep(5);

        ACK::ErrorCode takeoffAck = vehicle->control->takeoff(functionTimeout);
        if (ACK::getError(takeoffAck)) {
            ACK::getErrorCodeMessage(takeoffAck, __func__);
        }

        std::cout << "Took off \n";
        sleep(10);
    } else {
        std::cout << "Drone already in air" << std::endl;
    }
    sleep(1);
}


void UAVland(Vehicle* vehicle, int functionTimeout) {
    ACK::ErrorCode landAck = vehicle->control->land(functionTimeout);
    if (ACK::getError(landAck)) {
        ACK::getErrorCodeMessage(landAck, __func__);
    }

    std::cout << "disarm motor \n";
    ACK::ErrorCode disarmAck = vehicle->control->disArmMotors(functionTimeout);
    if (ACK::getError(disarmAck)) {
        ACK::getErrorCodeMessage(disarmAck, __func__);
    }

    std::cout << "Release control authority. \n";
    vehicle->releaseCtrlAuthority(functionTimeout);
}