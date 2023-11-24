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
using namespace DJI::OSDK;
using namespace DJI::OSDK::Telemetry;

void tellMeAboutTheData(DJI::OSDK::Vehicle* vehicle){
    std::cout << "Entered tellMeAboutTheData: \n";
    Telemetry::GlobalPosition pos;
    Telemetry::Quaternion quaternion;
    int searchRadius = 50;
    float32_t droneAngle;
    float32_t A1 = 0;
    float32_t A2 = 0;
    float32_t H = 0;
    float32_t alg = 0;
    float32_t vX = 0;
    float32_t vY = 0;
    float32_t sampleFrequency = 1;

    std::cout << "Bout to calculate init position: \n";  
    pos = vehicle->broadcast->getGlobalPosition();
    PIcontroller pic = PIcontroller(10, 20, sampleFrequency);
    DataFaker df = DataFaker(vehicle, 1000, searchRadius);
    
    float vel = 0;
    while(true){
        //Get new data
        df.FakeAs(vehicle);
        A1 = df.A1;
        A2 = df.A2;
        H = sqrt(pow(A1,2)+pow(A2,2));
        alg = acos((A1-A2)/(H+0.001))-M_PI_2;
        vel = (sqrt(2)*searchRadius-H);
        

        std::cout <<"!PIc: " << pic.pi <<", A1: " << A1 << ", A2: " << A2 << ", H: " << H << ", alg: " << alg << ", vel: " << vel << "\n";
        std::cout << "\t Alg: " << alg << ", H: " << H << "\n";
        //Calculate velocity in x and y direction
        //Sets velocity and yaw rate
        for (int i = 0; i <100; i++){
            droneAngle = QtoDEG(vehicle);
            vX = vel*cos(droneAngle*(M_PI/180))*0.1; 
            vY = vel*sin(droneAngle*(M_PI/180))*0.1;
            vehicle->control->velocityAndYawRateCtrl(vX, vY, 0, alg*10);
        usleep(10000);
        }
        //std::cout << "\t A1: " << A1 << "\n";
        //std::cout << "\t A2: " << A2 << "\n";
        //std::cout << "\t vX: " << vX << ", vY: " << vY << "\n";
        //std::cout << "\t yaw rate: " << alg*100 << "\n";
        //Breakstatement
        if (sqrt(2)*searchRadius-H < 2){
            std::cout << "Target found! \n";
            break;
        }
        //sampleFrequency => sampletime in us
    }
}

//Converts x and y vectors to a direction
float32_t getAngle(float32_t y, float32_t x) {
    float32_t angle = atan2(y, x);
    //converts from -pi to pi to 0 to 2pi
    if (angle < 0) {
        angle += 2 * M_PI;
    }
    //converts from radians to degrees
    angle *= 180.0 / M_PI;
    return angle;
}

//This function converts the quaternion to degrees
float32_t QtoDEG(Vehicle* vehicle) {
    Telemetry::Quaternion quaternion;
    float32_t angle;
    quaternion = vehicle->broadcast->getQuaternion();
    //This code is largely based on a mix of 
    //https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Quaternion_to_Euler_angles_(in_3-2-1_sequence)_conversion
    //and the flight-control sample
    double t1 = +2.0 * (quaternion.q1 * quaternion.q2 + quaternion.q0 * quaternion.q3);
    double t0 = -2.0 * (quaternion.q2 * quaternion.q2 + quaternion.q3 * quaternion.q3) + 1.0;

    //We add +90 to correct the 0 degree angle to be east instead of north
    //We multiply t1 with -1 to rotate anti-clockwise instead of clockwise
    angle = getAngle(-t1, t0)+90;
    //After adding 90, the angle can be above 360, so this makes sure it is between 0 and 360
    if (angle > 360) {
        angle -= 360;
    }
    return angle;
}

DataFaker::DataFaker(Vehicle* vehicle, int sT, int sR) {
    Telemetry::GlobalPosition pos;
    pos = vehicle->broadcast->getGlobalPosition(); 
    searchRadius = sR;
    sampleTime = sT;
    int random;

    iY = calcMfromLat(pos);
    iX = calcMfromLon(pos);
    srand((unsigned) time(NULL));
    random = (-searchRadius + (rand() % (2*searchRadius)));
    tX = random;
    random = (-searchRadius + (rand() % (2*searchRadius)));
    tY = random;

    std::cout << "target position calculated: tX = " << tX << ", tY = " << tY << "\n";
    std::cout << "about to enter while loop: \n";
}

void DataFaker::FakeAs(Vehicle* vehicle){
        Telemetry::GlobalPosition pos;
        pos = vehicle->broadcast->getGlobalPosition(); 
        float32_t droneAngle = QtoDEG(vehicle);
        float32_t dY = calcMfromLat(pos)-iY;
        float32_t dX = calcMfromLon(pos)-iX;
        float32_t distanceTo = getSize(dY-tY, dX-tX);
        float32_t signalStrength = sqrt(2)*searchRadius-distanceTo;
        //Finds the difference between the drones angle and the targets angle
        float32_t targetAngle = 180-2*getAngle(dY-tY, dX-tX);
        if (targetAngle < 0) {
            targetAngle += 360;
        }
        
        float32_t diffAngle = targetAngle-droneAngle;
        A1 = fabs(signalStrength*cos((diffAngle*M_PI/180)-M_PI_4));
        A2 = fabs(signalStrength*cos((diffAngle*M_PI/180)+M_PI_4));

        std::cout << "\t Distance from sender: " << distanceTo << "dX: " << dX << ", dY: " << dY << "\n";
        //std::cout << "\t Diff angle : " << diffAngle << "\n";
        //std::cout << "\t Signal strength: " << signalStrength << "\n";
}

//This functions finds the length of a vector
float32_t getSize(float32_t y, float32_t x) {
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

//This function converts the latitude to meters
float64_t calcMfromLat(Telemetry::GlobalPosition pos){
    float64_t iY;
    //float32_t r_earth = 6378100;
    float32_t r_earth = 6356752;
    iY = pos.latitude*r_earth;
    return iY;
}

//This function converts the longitude to meters
float64_t calcMfromLon(Telemetry::GlobalPosition pos){
    float64_t iX;
    //float32_t r_earth = 6378100;
    float32_t r_earth = 6356752; 
    iX = pos.longitude*cos(pos.latitude)*r_earth;
    return iX;
}

//*************************************************************************************//
//Implementation of the PIController class - This is an implementation of the controller used in the system
//*************************************************************************************//
//Constructor
PIcontroller::PIcontroller(float32_t Kp_in, float32_t Ki_in, float32_t sampleTime_in){
    Kp = Kp_in;
    Ki = Ki_in;
    sampleTime = sampleTime_in/1000;
    pi = 0;
}

/// @brief This function calculates the PI value
/// @param error The error value that the PI controller should calculate from
void PIcontroller::calculatePI(float32_t error){
    pi = Kp*error + (sampleTime/Ki)*error;
}