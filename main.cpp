#include "mbed.h" 
#include "HCSR04.h"
#include "TextLCD.h"
#include "Keypad.h"

//Program takes a input distance and measures real the distance with HCSR04 ultrasonic distance sensor.
//Gives different outputs with LEDs when the real distance is in certain intervals of the input distance. 
//2x16 LCD, 4x4 keypad, HCSR04 distance sensor and some LEDs are used.

HCSR04  distsensor(PTC1,PTC2);
TextLCD lcd(PTE20, PTE21, PTE22, PTE23, PTE29, PTE30, TextLCD::LCD16x2); // rs, e, d4-d7
Keypad kpad( PTA12, PTD4, PTA2, PTA1 ,PTC9, PTC8, PTA5, PTA4);

PwmOut R(LED_RED);
PwmOut G(LED_GREEN);
PwmOut B(LED_BLUE);
DigitalOut led(PTA13); 

int distdiv3;
int distdiv3mul2;
unsigned int sensordist;
unsigned int sensordist2;
unsigned int inputdist;
char key;
char inputarray[8]; // can be 4
int d2;
int d1;
int d0;
bool mod3flag=false;
int released1 = 1;


int main(){
restart: 
    R=1.0f; G=1.0f; B=1.0f;
    lcd.cls();
    lcd.printf("Input:");
    int released = 1;
    int ind = 0;
 
    while(ind < 3){
        key=kpad.ReadKey();
        if(key == '\0'){
            released = 1; }
         
        else if((key != '\0') && (released == 1)) {  
            if ( key == '*'){
                break;     }
        
        inputarray[ind]=key;
        lcd.printf("%c", key);
        ind++;
        released=0; }
        }
    
    d2 = (int)(inputarray[2]- '0' );
    d1 = (int)(inputarray[1]- '0' );
    d0 = (int)(inputarray[0]- '0' );


    if (ind != 3) {
        if (ind==2){inputdist=(d0*10+ d1); }
        else { inputdist=d0;   }    }
    else {
        inputdist=d0*100+ d1*10+d2;   }     

// got the input at this point
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Input is: %icm", inputdist);  
    lcd.locate(0,1);
    lcd.printf("Press * in order");
    wait(1.5);
    lcd.locate(0,1);
    lcd.printf("   to confirm   ");  // sliding effect would be nice 
    wait(1.5);
    lcd.locate(0,1);
    lcd.printf("  # to restart  ");
    wait(1.5);
    key='a';
    while(key != '*'){
        key=kpad.ReadKey();
        if (key=='#'){lcd.cls();
                lcd.locate(0,0);
                lcd.printf("   Restarting   ");  
                wait(1); goto restart;}
        wait(0.1);
    }
    lcd.cls();
    distdiv3=inputdist/3;
    distdiv3mul2=(2*inputdist)/3;
  //  lcd.printf("Next");

    while(true){ 
    
    long sensordist = distsensor.distance(CM);
    lcd.cls();
    lcd.printf("Distance: %icm", sensordist);
    lcd.locate(0,1);
    lcd.printf("  # to restart  ");
    wait(0.1);
    sensordist2=distsensor.distance(CM);
    if (mod3flag==false){
        B=1;
        R=1;
        G=1;}
    mod3flag=false;
    if (sensordist > inputdist ) { led=0;
    goto RestartCheck;
        }
    else {  
        led=1;       
        if (distdiv3 < sensordist && sensordist < distdiv3mul2 ) {
            led=0; 
            for (float i = 0.0f; i < 1.0f ; i += 0.1f) {
            float p = 3 * i;
            if (sensordist2 <= sensordist){
                R = 1.0f - ((p < 1.0f) ? 1.0f - p : (p > 2.0f) ? p - 2.0f : 0.0f);
                G = 1.0f - ((p < 1.0f) ? 1.0f - p : (p > 2.0f) ? p - 2.0f : 0.0f);
                wait (0.1f);
            }
            else if (sensordist2 > sensordist){
                B = 1.0f - ((p < 1.0f) ? 1.0f - p : (p > 2.0f) ? p - 2.0f : 0.0f);
                wait (0.1f);}
            
           }} 
        else if (distdiv3 > sensordist)  { led=0; mod3flag=true;
            if (sensordist2 <= sensordist){
                R = 0; G = 0; B=1; 
            }
            else if (sensordist2 > sensordist){
                R = 1; G = 1;B = 0;}
        }
    }
  
    RestartCheck:
    key=kpad.ReadKey();
    if(key == '\0'){released1 = 1; }
    else if((key != '\0') && (released1 == 1)) {  
            if ( key == '#'){                
                lcd.cls();
                lcd.locate(0,0);
                lcd.printf("   Restarting   ");  
                wait(1); 
                goto restart;     }
  
        } 
    }

}
