#include "mbed.h"
#include "MPU6050.h"
#include "TextLCD.h"
#include "Servo.h"
#include "Keypad.h"
#define OFFSET_SAMPLES 50

DigitalOut myled(LED1);
TextLCD lcd(PTE20, PTE21, PTE22, PTE23, PTE29, PTE30, TextLCD::LCD16x2); // rs, e, d4-d7
Timer timer;                            //need a timer to tell how much time passed from the last calculation
Keypad kpad( PTA12, PTD4, PTA2, PTA1 ,PTC9, PTC8, PTA5, PTA4);
Servo servo1(PTD5);
Servo servo2(PTD0);
Servo servo3(PTD2);

DigitalIn   topleft(PTD3);
DigitalIn   topright(PTD1);
DigitalIn   bottomleft(PTE0);
DigitalIn   bottomright(PTE1);




//I2C    i2c(I2C_SDA, I2C_SCL);
//I2C    i2c(I2C_SDA, I2C_SCL);
MPU6050 mpu(PTC11, PTC10);


char key;
int16_t ax, ay, az;
int16_t gx, gy, gz;
char data[3];
float accelangle;
unsigned int countofturn=0;
int pos1=45;
int pos2=45;

int main()
{
    int released = 1;
    lcd.cls();
 
    while(1){
        lcd.cls();
        lcd.locate(0,0);
        lcd.printf("A gimbal mode");
        lcd.locate(0,1);
        lcd.printf("B light tracking");
        wait(0.2);
        
        key=kpad.ReadKey();
        if(key == '\0'){
            released = 1; }
         
        else if((key != '\0') && (released == 1)) {  
            if ( key == 'A'){
                goto gimbal;     }
                
            if ( key == 'B'){
                goto Lighttrack;     }
                                
            if ( key == '0'){
               servo1=0.5;
               servo2=0.5;
               servo3=0.5;
             }      
                
            if ( key == '1'){
               for(int i=0; i<5; i++) {
               servo1 = servo1+ i/100.0;
               wait(0.01); }
             }

            if ( key == '2'){
               for(int i=0; i<5; i++) {
               servo1 = servo1- i/100.0;
               wait(0.01); }
             }

            if ( key == '4'){
               for(int i=0; i<5; i++) {
               servo2 = servo2+i/100.0;
               wait(0.01); }
             }
            if ( key == '5'){
               for(int i=0; i<5; i++) {
               servo2 = servo2- i/100.0;
               wait(0.01); }
             }             
            if ( key == '7'){
               for(int i=0; i<5; i++) {
               servo3 = servo3+ i/100.0;
               wait(0.01); }
             }
            if ( key == '8'){
               for(int i=0; i<5; i++) {
               servo3 = servo3- i/100.0;
               wait(0.01); }
             }                  
                     
        released=0; }        
        }
    
    ///////////////////////////LİGHT TRACK MODE
    
    Lighttrack:
    lcd.cls();
    lcd.printf("we are here");
    float waittime=0.01;
    servo1=0.5;
    while(1){
        
    if ((topleft)&&(!topright))        {    pos2 = pos2 + 1;    wait(waittime); }
    if ((bottomleft) && (!bottomright))  {    pos2 = pos2 + 1;    wait(waittime); }
    if ((!topleft) && (topright))        {    pos2 = pos2 - 1;    wait(waittime); }
    if ((!bottomleft) && (bottomright))  {    pos2 = pos2 - 1;    wait(waittime); }
    
   
    if ((topleft)&&(!bottomleft)) {    pos1 = pos1 - 1;    wait(waittime); }
    if ((topright) && (!bottomright)) {    pos1 = pos1 - 1;    wait(waittime); }
    if ((!topleft) && (bottomleft)) {    pos1 = pos1 + 1;    wait(waittime); }
    if ((!topright) && (bottomright)) {    pos1 = pos1 + 1;    wait(waittime); }  
    
   /* if (pos1 > 90) {
        pos1 = 90;
            }
    if (pos1 < 0) {
        pos1 = 0;
            }
    if (pos2 > 90) {
        pos2 = 90;
            }
    if (pos2 < 0) {
        pos1 = 0;
            }  */
            
    servo2=pos1/90.0;
    servo3=pos2/90.0;
    
    
    }//end of while
    
    
    
     
    ///////////////////////////////GİMBAL 
     

    gimbal:
    float accOffset[3];
    float gyroOffset[3];
    float angle[3]; 
    float prevangle[3];
    float prevTime;   
        
        
        
    lcd.printf("MPU6050 test");
    wait(1);
    lcd.cls();
    
    lcd.printf("MPU6050 testConnection");
 
    bool mpu6050TestResult = mpu.testConnection();
    if(mpu6050TestResult) {
        lcd.cls();
        lcd.locate(0,0);
        lcd.printf("MPU6050 test passed");
        wait(1);
        lcd.cls();
    } else {
        lcd.cls();
        lcd.locate(0,0);
        lcd.printf("MPU6050 test failed");
        wait(1);
        lcd.cls();
    }
    
    
    
recalibrate:
    
    mpu.getOffset(accOffset, gyroOffset, OFFSET_SAMPLES);
    wait(0.2);                                            //wait to settle down
    timer.start();                  //we need the timer for to knwo the time interval
    prevTime = timer.read(); 
   
   
   int released1 = 1;
   
    while(1) {
       wait(0.01);
       
       
       
        if (countofturn!=100){
         myled=1;
         countofturn++; }
        else{
         countofturn=0;
         myled=0;
         //goto recalibrate;
         } 
       
         mpu.computeAngle (angle, accOffset, gyroOffset, timer.read()-prevTime); 
         
         prevTime = timer.read();
        //mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        //writing current accelerometer and gyro position 
        lcd.cls();
        angle[0]=(angle[0]-0);
        angle[1]=(angle[1]-0);
        angle[2]=(angle[2]-0);

        //angle[0]=round(angle[0]);
        //angle[1]=round(angle[1]);
        //angle[2]=round(angle[2]);  
        

        
        
        if (angle[0]>prevangle[0]+0.5){servo1=servo1+0.08;}
    else if (angle[0]<prevangle[0]-0.5){servo1=servo1-0.08;}
    
        if (angle[1]>prevangle[1]+0.5){servo2=servo2-0.03;}
        else if (angle[1]<prevangle[1]-0.5){servo2=servo2+0.03;}
        
     if (angle[2]>prevangle[2]+0.5){servo3=servo3-0.02;}
        else if (angle[2]<prevangle[2]-0.5){servo3=servo3+0.04;}
        
        lcd.cls();
        lcd.locate(0,0);
        lcd.printf("%.2f;  %.2f;     %.2f",angle[0],angle[1],angle[2]);
        prevangle[0]=(angle[0]-0);
        prevangle[1]=(angle[1]-0);
        prevangle[2]=(angle[2]-0); 
        //wait (0.01);
        
      //  RestartCheck:
        key=kpad.ReadKey();
        if(key == '\0'){released1 = 1; }
        else if((key != '\0') && (released1 == 1)) {  
            if ( key == '#'){                
                lcd.cls();
                lcd.locate(0,0);
                lcd.printf("   Restarting   ");  
                wait(1); 
                servo1=0.5; 
                servo2=0.5; 
                servo3=0.5; 
                released1 = 0;   }}
        
    }
 }
  
