#include "PIcontroller.h"

//Constructor
PIcontroller::PIcontroller(float Kp_in, float Ki_in, float sampleFrequency){
    Kp = Kp_in;
    Ki = Ki_in;
    sampleTime = 1/sampleFrequency;
    PIvalue = 0;
    std::cout << "Setup op PIcontroller done \n \t values: Kp: " << Kp << ", Ki: " << Ki << ", sampleTime: " <<sampleTime << ", init pi: " << PIvalue << std::endl;
}

/// @brief This function calculates the PI value
/// @param error The error value that the PI controller should calculate from
void PIcontroller::updatePIController(float error){
    PIvalue = Kp;
    //Stops PI from becomming infinite
    if(Ki != 0){
        PIvalue += (sampleTime/Ki)*error;
    }
    if(abs(PIvalue) > 10){
	PIvalue = (PIvalue/abs(PIvalue))*10;
    }
    sMinus2 = sMinus1;
    sMinus1 = error;
}
