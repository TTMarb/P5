#include "DataFaker.h"

/// @brief Initiated the DataFaker class, which "Fakes" the Antenna data.
/// @param vehicle Vehicle pointer to the DJI vechicle class
/// @param sT SampleTime - time between samples
/// @param sR SearchRadius - The actual distance the antenna can reach
/// @note Will be removed once actual data can be generated
DataFaker::DataFaker() {
    std::cout << "Created DF " << std::endl;
}

/// @brief Initiated the DataFaker class, which "Fakes" the Antenna data.
/// @param vehicle Vehicle pointer to the DJI vechicle class
/// @param sT SampleTime - time between samples
/// @param sR SearchRadius - The actual distance the antenna can reach
/// @note Will be removed once actual data can be generated

void DataFaker::init(Vehicle* vehicle, int sT, int xLoc, int yLoc) {
    Telemetry::GlobalPosition pos;
    pos = vehicle->broadcast->getGlobalPosition(); 
    sampleTime = sT;

    iY = calcMfromLat(pos);
    iX = calcMfromLon(pos);
    tX = xLoc;
    tY = yLoc;

    std::cout << "target position calculated: tX = " << tX << ", tY = " << tY << "\n";
    std::cout << "about to enter while loop: \n";
}

/// @brief Generates the "fake" antenna data from GPS location and the UAV's current angle
/// @param vehicle 
void DataFaker::Fake(Vehicle* vehicle, FIO fileIO,bool update){
        Telemetry::GlobalPosition pos;
        pos = vehicle->broadcast->getGlobalPosition(); 
        float UAVAngle = QtoDEG(vehicle);
        float dY = calcMfromLat(pos)-iY;
        float dX = calcMfromLon(pos)-iX;
        float tA1;
        float tA2;
        std::cout << "dX: " << dX << ", dY: " << dY << "\n";
        float distanceTo = getSize(dY-tY, dX-tX);
        std::cout << "distanceTo: " << distanceTo << "\n";
        int maxADCvalue = 4096;
        float signalStrength = maxADCvalue * (1 / pow(distanceTo, 3));
        std::cout << "signalStrength: " << signalStrength << "\n";
        //Finds the difference between the UAVs angle and the targets angle
        float targetAngle = 180-2*getAngle(dY-tY, dX-tX);
        if (targetAngle < 0) {
            targetAngle += 360;
        }
        float diffAngle = targetAngle-UAVAngle;
        tA1 = fabs(signalStrength*cos((diffAngle*M_PI/180)+M_PI_4));
        tA2 = fabs(signalStrength*cos((diffAngle*M_PI/180)-M_PI_4));
        if(update){
            A1 = tA1;
            A2 = tA2;
            std::cout << "\t Distance from sender: " << distanceTo << "dX: " << dX << ", dY: " << dY << "\n";
        }else{ 
            std::string data = std::to_string(timecounterMilliseconds) + "," + std::to_string(dX) + "," + std::to_string(dY)+ "," + std::to_string(tA1)+ "," + std::to_string(tA2)+ "," + std::to_string(signalStrength);
            fileIO.write2file(data);
        }
}


//This function converts the latitude to meters
double calcMfromLat(Telemetry::GlobalPosition pos) { return pos.latitude * EARTH_RADIUS; }

//This function converts the longitude to meters
double calcMfromLon(Telemetry::GlobalPosition pos) { return pos.longitude * cos(pos.latitude) * EARTH_RADIUS; }
