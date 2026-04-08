#include "HomeSpan.h"
#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::HALF4WIRE, 18, 19, 21, 22);

struct DEV_WindowShade : Service::WindowCovering {
  Characteristic::CurrentPosition currentPos{0,true};
  Characteristic::TargetPosition targetPos{0,true};

  DEV_WindowShade() : Service::WindowCovering() {
    stepper.setMaxSpeed(800.0);    
    stepper.setAcceleration(200.0);   
  }

  boolean update() {
    if(targetPos.updated()) {
      Serial.printf("\n>>> Received command from IPHONE! Target: %d <<<\n", targetPos.getNewVal());
      stepper.moveTo(targetPos.getNewVal() * 150); 
    }
    return(true);
  }

  void loop() {
    stepper.run();
    if(stepper.distanceToGo() == 0 && currentPos.getVal() != targetPos.getVal()) {
      currentPos.setVal(targetPos.getVal());
      Serial.println(">>> Motor stopped - Target Aquired <<<");
    }
  }
};

void setup() {
  Serial.begin(115200);
  homeSpan.begin(Category::WindowCoverings, "Roleta ESP32");

  new SpanAccessory();                                                          
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
    
    new DEV_WindowShade();
}

void loop() {
  homeSpan.poll();  
}