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

bool runWaypointMission(Vehicle* vehicle, int numWaypoints, int responseTimeout, float64_t latM, float64_t lonM) {
    // Waypoint Mission : Initialization
    WayPointInitSettings fdata;
    setWaypointInitDefaults(&fdata);
    setBroadcastFrequency(vehicle);

    fdata.indexNumber = numWaypoints + 1; // We add 1 to get the aircarft back to the start.

    float32_t start_alt = 1.5;

    ACK::ErrorCode initAck = vehicle->missionManager->init(DJI_MISSION_TYPE::WAYPOINT, responseTimeout, &fdata);
    if (ACK::getError(initAck)) {
        ACK::getErrorCodeMessage(initAck, __func__);
    }

    vehicle->missionManager->printInfo();
    std::cout << "Initializing Waypoint Mission..\n";

    // Waypoint Mission: Create Waypoints
    std::vector<WayPointSettings> generatedWaypts = createWaypoints(vehicle, numWaypoints, latM, lonM, start_alt);
    std::cout << "Creating Waypoints..\n";

    // Waypoint Mission: Upload the waypoints
    uploadWaypoints(vehicle, generatedWaypts, responseTimeout);
    std::cout << "Uploading Waypoints..\n";

    // Waypoint Mission: Start
    ACK::ErrorCode startAck = vehicle->missionManager->wpMission->start(responseTimeout);
    std::cout << "Got response!\n";
    if (ACK::getError(startAck)) {
        std::cout << "ERROR in Starting Waypoint Mission.\n";
        ACK::getErrorCodeMessage(initAck, __func__);
    } else {
        std::cout << "Starting Waypoint Mission.\n";
    }
    // Cleanup before return. The mission isn't done yet, but it doesn't need any
    // more input from our side.
    std::cout << "Bout to return true\n";

    return true;
}

void setWaypointDefaults(WayPointSettings* wp) {
    wp->damping = 0;
    wp->yaw = 0;
    wp->gimbalPitch = 0;
    wp->turnMode = 0;
    wp->hasAction = 0;
    wp->actionTimeLimit = 100;
    wp->actionNumber = 0;
    wp->actionRepeat = 0;
    for (int i = 0; i < 16; ++i) {
        wp->commandList[i] = 0;
        wp->commandParameter[i] = 0;
    }
}

void setWaypointInitDefaults(WayPointInitSettings* fdata) {
    fdata->maxVelocity = 10;
    fdata->idleVelocity = 5;
    fdata->finishAction = 0;
    fdata->executiveTimes = 1;
    fdata->yawMode = 0;
    fdata->traceMode = 0;
    fdata->RCLostAction = 1;
    fdata->gimbalPitch = 0;
    fdata->latitude = 0;
    fdata->longitude = 0;
    fdata->altitude = 0;
}

std::vector<DJI::OSDK::WayPointSettings> createWaypoints(DJI::OSDK::Vehicle* vehicle, int numWaypoints, float64_t latM,
                                                         float64_t lonM, float32_t start_alt) {
    // Create Start Waypoint
    WayPointSettings start_wp;
    setWaypointDefaults(&start_wp);

    // Global position retrieved via subscription  //@TODO: Formodenligt fjern nedenstående linier!
    Telemetry::TypeMap<TOPIC_GPS_FUSED>::type subscribeGPosition;
    // Global position retrieved via broadcast
    Telemetry::GlobalPosition broadcastGPosition;

    broadcastGPosition = vehicle->broadcast->getGlobalPosition();
    start_wp.latitude = broadcastGPosition.latitude;
    start_wp.longitude = broadcastGPosition.longitude;
    start_wp.altitude = start_alt;
    printf("Initial location at (LLA): %f \t%f \t%f\n", broadcastGPosition.latitude, broadcastGPosition.longitude,
           start_alt);

    std::vector<DJI::OSDK::WayPointSettings> wpVector = generateWaypoints(&start_wp, numWaypoints, latM, lonM);
    return wpVector;
}

std::vector<DJI::OSDK::WayPointSettings> generateWaypoints(WayPointSettings* start_data, int num_wp, float64_t latM,
                                                           float64_t lonM) {

    // Let's create a vector to store our waypoints in.
    std::vector<DJI::OSDK::WayPointSettings> wp_list;

    float64_t r_earth = 6378100;

    // First waypoint
    start_data->index = 0;
    wp_list.push_back(*start_data);

    int mult = -1;

    // Iterative algorithm
    for (int i = 1; i < num_wp; i++) {
        WayPointSettings wp;
        WayPointSettings* prevWp = &wp_list[i - 1];
        setWaypointDefaults(&wp);
        wp.index = i;
        // Downwards increment
        if (i % 2 != 0) {
            mult = mult * -1;
            wp.latitude = (prevWp->latitude);
            wp.longitude = (prevWp->longitude + (((latM / r_earth) / cos(wp.latitude)) * mult));

        } else // Side ways increment
        {
            wp.longitude = (prevWp->longitude);
            wp.latitude = (prevWp->latitude + ((lonM / r_earth)));
        }
        wp.altitude = (prevWp->altitude);
        wp_list.push_back(wp);
    }

    // Come back home
    start_data->index = num_wp;
    wp_list.push_back(*start_data);
    std::cout << "lenght of wp_list: " << wp_list.size() << "\n";

    return wp_list;
}

void uploadWaypoints(Vehicle* vehicle, std::vector<DJI::OSDK::WayPointSettings>& wp_list, int responseTimeout) {
    for (std::vector<WayPointSettings>::iterator wp = wp_list.begin(); wp != wp_list.end(); ++wp) {
        printf("Waypoint created at (LLA): %f \t%f \t%f\n ", wp->latitude, wp->longitude, wp->altitude);
        ACK::WayPointIndex wpDataACK = vehicle->missionManager->wpMission->uploadIndexData(&(*wp), responseTimeout);

        ACK::getErrorCodeMessage(wpDataACK.ack, __func__);
    }
}

bool stopMission(DJI::OSDK::Vehicle* vehicle, int responseTimeout, int delayBeforeStop) {
    sleep(delayBeforeStop);
    ACK::ErrorCode stopAck = vehicle->missionManager->wpMission->stop(responseTimeout);
    if (ACK::getError(stopAck)) {
        std::cout << "ERROR in Stopping Waypoint Mission.\n";
        ACK::getErrorCodeMessage(stopAck, __func__);
    }
    std::cout << "Succes: Stopping Waypoint Mission.\n";
}

void startProcess(pid_t pid, char* path, char* param) {
    char* bname = basename(path);
    if (pid == -1) {
        printf("Error while forking %s\n", bname);
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("Child process %s initiated. PID is %d\n", bname, getpid());
        char* const args[] = {param};
        char* const envp[] = {NULL};
        execve(path, args, envp); // Override the child process with the BIN file
        perror("execve");
        exit(EXIT_FAILURE); // Exit the child process if it fails
    } else if (pid > 0) {
        printf("Parent process running. PID is %d\n", getpid());
    }
}

float movingAvg(float* ptrArrNumbers, float* ptrSum, int pos, int len, float nextNum) {
    // Subtract the oldest number from the prev sum, add the new number
    *ptrSum = *ptrSum - ptrArrNumbers[pos] + nextNum;
    // Assign the nextNum to the position in the array
    ptrArrNumbers[pos] = nextNum;

    return *ptrSum / (float)len;
}

/// @brief Reads the Quaternation degrees, and converts them into a yaw degree.
/// @param vehicle Pointer to the DJI vehicle class
/// @return Returns the drones current angle on east
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
    angle = getAngle(-t1, t0) + 90;
    //After adding 90, the angle can be above 360, so this makes sure it is between 0 and 360
    if (angle > 360) {
        angle -= 360;
    }
    return angle;
}

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
