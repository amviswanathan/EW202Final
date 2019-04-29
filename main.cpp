#include "mbed.h"
#include "Tach.h"
#include "TCS3472_I2C.h"
#include "ContinuousServo.h"
//LEDS
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

// PC Input
Serial pc(USBTX,USBRX);

//Color Sensor Input
TCS3472_I2C rgb_sensor(p9,p10);

// Hall Effect Sensor
DigitalIn hall(p21);
DigitalOut hallpwr(p22);

//Sonar Sensor
AnalogIn sonar(p19);

//Servos
ContinuousServo left(p23);
ContinuousServo right(p26);
//encoders
Tach tLeft(p17,64);
Tach tRight(p13,64);
int main(){
    hall.mode(PullUp);
    hallpwr=1;
    rgb_sensor.enablePowerAndRGBC();
    rgb_sensor.setIntegrationTime(100);
    pc.baud(9600);
    float setleft;
    float setright;
    float lerror;
    float rerror;
    int rgb_data[4];
    float PWMbrightness = 1.0;
    int Unfilt;
    int Red;
    int Green;
    int Blue;
    float leftspeed;
    float rightspeed;
    float leftspeedin;
    float rightspeedin;
    float sonarraw;
    float distance;
    while(1) {
        
        //MBED Indicating Lights
        
        rgb_sensor.getAllColors(rgb_data);
        Unfilt = rgb_data[0];
        Red = rgb_data[1];
        Green  = rgb_data[2];
        Blue  = rgb_data[3];
        hallpwr=1;
        //pc.printf("%f,%f,%f,%f\r\n",rgb_data[0],rgb_data[1],rgb_data[2],rgb_data[3]);
        if ((Unfilt>4500) && (Red>Green) && (Red>Blue) && (Green>Blue)){
            pc.printf("White\r\n");
            }
        else if ((Green>Blue) && (Blue>Red)){
           pc.printf("Black\r\n");
            }
        else if ((Green>Red) && (Red>Blue) && (Unfilt<3000)) {
            pc.printf("Green\r\n");
            //LED2
            led1=0;
            led2=1;
            led3=0;
            led4=0;
            }
        else if (Red>(Blue+Green)){
            pc.printf("Red\r\n");
            //LED1
            led1=1;
            led2=0;
            led3=0;
            led4=0;
            }
        else if (Blue>Red && Blue>Green){
            pc.printf("Blue\r\n");
            //LED3
            led1=0;
            led2=0;
            led3=1;
            led4=0;
            }
        if (hall.read() == 1){
            led4=1;
            hallpwr=0;
            }
        else if (hall.read() == 0){
            led4=0; 
            }
        
        // Wheel Control
        
        leftspeed=tLeft.getSpeed(); //Read Tachometer
        rightspeed=tRight.getSpeed();
        
        setleft = .1;
        setright = -.1;
        
        //pc.printf("%f\r\n", rgb_data[0]);
        
        lerror=setleft-leftspeed; //compute error and define speed
        leftspeedin=setleft+(.05*lerror); 
        rerror=setright-rightspeed;
        rightspeedin=setright+(.05*rerror);
        
        left.speed(leftspeedin); //input speeds
        right.speed(rightspeedin);
        
        
        
        pc.printf("%f,%f\r\n",lerror, rerror);
        pc.printf("%f,%f\r\n",leftspeedin, rightspeedin);
        
        //Sonar Sensor'
        sonarraw = sonar.read();
        distance = ((sonarraw-.003)/.0034);
        pc.printf("%f,%f\r\n", sonarraw, distance);
        wait(.1);
        }
    } 
    
