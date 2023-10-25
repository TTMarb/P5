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

bool runWaypointMission(Vehicle* vehicle, uint8_t numWaypoints, int responseTimeout, float64_t latM, float64_t lonM) {
    // Waypoint Mission : Initialization
    WayPointInitSettings fdata;
    setWaypointInitDefaults(&fdata);

    fdata.indexNumber = numWaypoints + 1; // We add 1 to get the aircarft back to the start.

    float32_t start_alt = 4;

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

    // Global position retrieved via subscription
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
        if (i % 2 == 0) {

            wp.latitude = (prevWp->latitude);
            wp.longitude = (prevWp->longitude + ((latM / r_earth) / cos(wp.latitude)));

        } else // Side ways increment
        {
            mult = mult * -1;
            wp.longitude = (prevWp->longitude);
            wp.latitude = (prevWp->latitude + ((lonM / r_earth) * mult));
        }
        wp.altitude = (prevWp->altitude);
        wp_list.push_back(wp);
    }

    // Come back home
    start_data->index = num_wp;
    wp_list.push_back(*start_data);

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