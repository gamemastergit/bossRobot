

/*
  PROJECT     :  ROBOT ROOM
  NAME        :  BOSS ROBOT
  BOARD       :  ARDUINO MEGA
  PROCESSOR   :  AT mega 2560
  DATE        :  6 / 12 / 2018
  PROGRAMMERS :  SERGEI DINES / JACK / JACOB
*/

// LED CALL :
#include <FastLED.h>
#define DATA_PIN 7 // Digital Pin # 7 = Analog Pin -- Link all LEDs together !
#define NUM_LED 48 // 4 Rings Of 12 LEDs
CRGB leds[NUM_LED];
CRGB colors[] = {CRGB::HotPink, CRGB::Green, CRGB::Blue, CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Purple, CRGB::Cyan, CRGB::Magenta, CRGB::Aqua, CRGB::Lavender};
// END LED CALl

#define motor_speed              2
#define motor_dir                14

//#define BOSS_LEDring1            7

#define boss_robot_start_sound   4
#define boss_robot_hitsound      5
#define boss_robot_defeatsound   6

// ROBOT SOUNDS //

// ROBOT #1 & #2 SOUNDS :

#define s1_fx1 20    // Background Sound
#define s2_fx1 21    // Hit Sound #1
#define s3_fx1 22    // Hit Sound #2
#define s4_fx1 23    // Hit Sound #3
#define s5_fx1 24    // Death Sound
#define s6_fx1 25


//// ROBOT #3 & #4 SOUNDS :
//#define s1_fx2            //
//#define s2_fx2            // Boss
//#define s3_fx2
//#define s4_fx2            //

// BOSS ROBOT SOUNDS :
#define s1_BOSSfx 26 // BOSS Initial Sound - " We cannot be defeated puny humans" 
#define s2_BOSSfx 27 // BOSS LOOP - 
#define s3_BOSSfx 28 // BOSS TAUNT - "You may have defeated my small robots but you'll never kill me"
#define s4_BOSSfx 29 // BOSS HIT
#define s5_BOSSfx 30 // BOSS DEFEAT

#define FINAL_PIN 39  // To master escape room board 

// POWER RELAYS :

#define r1_relay    31   // Main Power ( 7V ) to Vin of Arduino for robot
#define led1_relay  32   // Main Power ( 5V ) to Robot and LEDs

#define r2_relay    33   // Main Power ( 7V ) to Vin of Arduino for robot
#define led2_relay  34  // Main Power ( 5V ) to Robot and LEDs

#define r3_relay    35   // Main Power ( 7V ) to Vin of Arduino for robot
#define led3_relay  36   // Main Power ( 5V ) to Robot and LEDs

#define r4_relay    37   // Main Power ( 7V ) to Vin of Arduino for robot
#define led4_relay  38   // Main Power ( 5V ) to Robot and LEDs

#define r1_read     4   // Read Value if robot is hit three times
#define r2_read     5   // Read Value if robot is hit three times
#define r3_read     6   // Read Value if robot is hit three times
#define r4_read     3   // Read Value if robot is hit three times


// INDICATOR LEDs :
#define r1_led      8
#define r2_led      9
#define r3_led      10
#define r4_led      11
#define r5_led      12

int r1_count = 0 ;
int r2_count = 0 ;
int r3_count = 0 ;
int r4_count = 0 ;

#define BOSS_IR_sensor_pin1  50   // BOSS IR # 1
#define BOSS_IR_sensor_pin2  51   // BOSS IR # 1
#define BOSS_IR_sensor_pin3  52   // BOSS IR # 1
#define BOSS_IR_sensor_pin4  53   // BOSS IR # 1

int small_ir_1_read    = 1;       // IR 1 of Small Robot # 1
int small_ir_2_read    = 1;       // IR 2 of Small Robot # 2
int small_ir_3_read    = 1;       // IR 3 of Small Robot # 3
int small_ir_4_read    = 1;       // IR 4 of Small Robot # 4
int last_small_ir_1    = 1;       // Previous State of the IR Sensor ( flips ) - should be opposite of active sensor value
int last_small_ir_2    = 1;       // Previous State of the IR Sensor ( flips )
int last_small_ir_3    = 1;       // Previous State of the IR Sensor ( flips )
int last_small_ir_4    = 1;       // Previous State of the IR Sensor ( flips )
int small_ir_1_count   = 1;       // Tells how many times IR sensor was hit on main side
int small_ir_2_count   = 1;       // Tells how many times IR sensor was hit on main side
int small_ir_3_count   = 1;       // Tells how many times IR sensor was hit on main side
int small_ir_4_count   = 1;       // Tells how many times IR sensor was hit on main side


#define ir1_boss  46  // IR BOSS SENSOR # 1
#define ir2_boss  47  // IR BOSS SENSOR # 2
#define ir3_boss  48  // IR BOSS SENSOR # 3
#define ir4_boss  49  // IR BOSS SENSOR # 4

int ir1_boss_state = 0; // Variable to hold / read state of the IR Sensor
int ir2_boss_state = 0; // Variable to hold / read state of the IR Sensor
int ir3_boss_state = 0; // Variable to hold / read state of the IR Sensor
int ir4_boss_state = 0; // Variable to hold / read state of the IR Sensor


//int ir_5_read = 0;

#define TOY_TO_ROBOT_SWITCH_READ   44   // TOGGLE SWITCH TO MAKE ROBOTS COME ALIVE
#define DOOR_OPEN_SWITCH           45   // MAIN TOGGLE SWITCH TO TURN EVERYTHING ON

int toy_bit_state;
int door_bit_state;

int main_state             = 0;
int robot_sound1_state     = 0;
int robot_hitsound_state   = 0;

int x                      = 0; // Jump from loop to loop
float y                    = 0; // For LEDs and their for loops
int count                  = 0;
int volume_state           = 0; // To increase / decrease volume

// RGB STRIP :
// Analog Pins Only!
//const int redPin    = 9;
//const int greenPin  = 10;
//const int bluePin   = 11;

boolean boss_state_1 = 0;
boolean boss_state_2 = 0;
boolean boss_state_3 = 0;
boolean boss_state_4 = 0;


void setup() {
  Serial.begin(9600);



  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LED);
  //  pinMode(LEDring,                  OUTPUT);
  //  pinMode(robot_start_sound,        OUTPUT);
  pinMode(BOSS_IR_sensor_pin1,           INPUT);     // Digital Pin 50
  pinMode(BOSS_IR_sensor_pin2,           INPUT);     // Digital Pin 51
  pinMode(BOSS_IR_sensor_pin3,           INPUT);     // Digital Pin 52
  pinMode(BOSS_IR_sensor_pin4,           INPUT);     // Digital Pin 53
  pinMode(r1_read,                       INPUT);     // DATA FROM ROBOT 1 - Digital Pin #4 ( PWM Pin )
  pinMode(r2_read,                       INPUT);     // DATA FROM ROBOT 2 - Digital Pin #5 ( PWM Pin )
  pinMode(r3_read,                       INPUT);     // DATA FROM ROBOT 3 - Digital Pin #6 ( PWM Pin )
  pinMode(r4_read,                       INPUT);     // DATA FROM ROBOT 4 - Digital Pin #7 ( PWM Pin )


  pinMode(BOSS_IR_sensor_pin1, INPUT);
  pinMode(BOSS_IR_sensor_pin2, INPUT);
  pinMode(BOSS_IR_sensor_pin3, INPUT);
  pinMode(BOSS_IR_sensor_pin4, INPUT);

  pinMode(ir1_boss, INPUT);
  pinMode(ir2_boss, INPUT);
  pinMode(ir3_boss, INPUT);
  pinMode(ir4_boss, INPUT);

  pinMode(TOY_TO_ROBOT_SWITCH_READ, INPUT);     // Robot power on sound
  pinMode(DOOR_OPEN_SWITCH, INPUT);             // When door is open, start actual main sequence of robots attacking, ect

  // SET LINEAR ACTUATOR INITIAL POSITION:
  pinMode(motor_speed, OUTPUT);
  pinMode(motor_dir , OUTPUT);
  digitalWrite(motor_dir , 1);
  digitalWrite(motor_speed, 127);

  pinMode(r1_relay, OUTPUT);
  pinMode(r2_relay, OUTPUT);
  pinMode(r3_relay, OUTPUT);
  pinMode(r4_relay, OUTPUT);
  //  pinMode(r5_relay, OUTPUT);

  pinMode(led1_relay, OUTPUT);
  pinMode(led2_relay, OUTPUT);
  pinMode(led3_relay, OUTPUT);
  pinMode(led4_relay, OUTPUT);
  //  pinMode(led5_relay, OUTPUT);

  digitalWrite(r1_relay, HIGH);
  digitalWrite(r2_relay, HIGH);
  digitalWrite(r3_relay, HIGH);
  digitalWrite(r4_relay, HIGH);
  digitalWrite(led1_relay, HIGH);
  digitalWrite(led2_relay, HIGH);
  digitalWrite(led3_relay, HIGH);
  digitalWrite(led4_relay, HIGH);

  pinMode(s1_BOSSfx, OUTPUT); // Boss Intro
  pinMode(s2_BOSSfx, OUTPUT); // Boos Loop
  pinMode(s3_BOSSfx, OUTPUT); // Boss smalls are defeated - You may have defeated them but you'll never defeat me
  pinMode(s4_BOSSfx, OUTPUT); // Boss Hit
  pinMode(s5_BOSSfx, OUTPUT); // Boss Death

  pinMode(s1_fx1, OUTPUT);    // Boot Up
  pinMode(s2_fx1, OUTPUT);    // Loop during small battle
  pinMode(s3_fx1, OUTPUT);
  pinMode(s4_fx1, OUTPUT);
  pinMode(s5_fx1, OUTPUT);
  pinMode(s6_fx1, OUTPUT);    // END LOOP SOUND


  digitalWrite(s1_BOSSfx, HIGH); // Boss Intro
  pinMode(s2_BOSSfx, HIGH); // Boos Loop
  pinMode(s3_BOSSfx, OUTPUT); // Boss smalls are defeated - You may have defeated them but you'll never defeat me
  pinMode(s4_BOSSfx, OUTPUT); // Boss Hit
  pinMode(s5_BOSSfx, OUTPUT); // Boss Death

  digitalWrite(s1_fx1, HIGH);    // Boot Up
  digitalWrite(s2_fx1, HIGH);    // Loop during small battle
  pinMode(s3_fx1, OUTPUT);
  pinMode(s4_fx1, OUTPUT);
  pinMode(s5_fx1, OUTPUT);
  pinMode(s6_fx1, OUTPUT);    // END LOOP SOUND




  //  pinMode(s1_fx2, OUTPUT);
  //  pinMode(s2_fx2, OUTPUT);
  //  pinMode(s3_fx2, OUTPUT);
  //  pinMode(s4_fx2, OUTPUT);
  //  pinMode(s5_fx2, OUTPUT);
  //  pinMode(s6_fx2, OUTPUT);
  //  pinMode(s7_fx2, OUTPUT);

  pinMode(r1_led, OUTPUT);
  pinMode(r2_led, OUTPUT);
  pinMode(r3_led, OUTPUT);
  pinMode(r4_led, OUTPUT);
  pinMode(r5_led, OUTPUT);
  digitalWrite(r1_led, HIGH);
  digitalWrite(r2_led, HIGH);
  digitalWrite(r3_led, HIGH);
  digitalWrite(r4_led, HIGH);
  digitalWrite(r5_led, HIGH);
  delay(200);
  digitalWrite(r1_led, LOW);
  digitalWrite(r2_led, LOW);
  digitalWrite(r3_led, LOW);
  digitalWrite(r4_led, LOW);
  digitalWrite(r5_led, LOW);
  pinMode(FINAL_PIN, OUTPUT);   // TO MAIN ESCAPE ROOM MASTER CONTROL BOARD



}

void loop() {

  /*
      LOGIC FLOW :
         READ TOGGLE
         Linear Actuator Up when Toggle True
         Flip Relays for Small Robots & LEDs on

  */

  toy_bit_state    = digitalRead(TOY_TO_ROBOT_SWITCH_READ);   // Looking for 5 V ( 1 == HIGH )
  door_bit_state   = digitalRead(DOOR_OPEN_SWITCH);         // Looking for 5 V     ( 1 == HIGH )

  Serial.print("X : ");
  Serial.println(x);
  Serial.print(" DOOR STATE : ");
  Serial.println(toy_bit_state);

  // PRE KNIFE SWITCH //
  while (x == 0) {

    toy_bit_state    = digitalRead(TOY_TO_ROBOT_SWITCH_READ);   // Looking for 5 V ( 1 == HIGH )

    Serial.print("X : ");
    Serial.println(x);
    Serial.print(" DOOR STATE : ");
    Serial.println(toy_bit_state);


    if (toy_bit_state == 1) { // Robots Turn On
      digitalWrite(s1_fx1, LOW);  // Start up sound
      delay(50);
      digitalWrite(s1_fx1, HIGH); // Turn pin back off
      delay(1000);
      digitalWrite(s2_fx1, LOW);  // Loop Sound - turn pin on
      delay(50);
      digitalWrite(s2_fx1, HIGH); // Turn pin back off
      x = 1;
      break;
    }
  }

  // POST KNIFE SWITCH - DOOR OPEN ROUTINE :
  while (x == 1) {
    door_bit_state   = digitalRead(DOOR_OPEN_SWITCH);         // Looking for 5 V     ( 1 == HIGH )

    if (door_bit_state == 0) {  // Magnet Lock Circuit Opens ( Cut 5V and be ground )

      digitalWrite(s2_BOSSfx, LOW); // LOOPING SOUND
      delay(3000);
      digitalWrite(motor_dir , 0); // SEND LINEAR ACTUATOR DOWN ( REVEAL SMALL ROBOTS )
      digitalWrite(motor_speed, 250); // Max Speed = 255 , Min = 1

      // POWER ON ALL SMALL ROBOTS AND LEDS RINGS FOR SMALL ROBOTS :
      digitalWrite(r1_relay,   LOW);
      digitalWrite(led1_relay, LOW);
      delay(3000);
      
      digitalWrite(r2_relay,   LOW);
      digitalWrite(led2_relay, LOW);
      delay(3000);
         
      digitalWrite(r3_relay,   LOW);
      digitalWrite(led3_relay, LOW);
      delay(4000);
      
      digitalWrite(r4_relay,   LOW); 
      digitalWrite(led4_relay, LOW);           
      delay(10000); // Was 60000 to avoid false triggering - Give time for relays to give power to smaller boards before reading values to avoid false triggering
      x = 2;
      break;
    }
  }

  // SMALL ROBOT BATTLE ROUTINE :
  while (x == 2) {

    small_ir_1_read =  digitalRead(r1_read);  // IR Sensor State for Robot # 1
    small_ir_2_read =  digitalRead(r2_read);  // IR Sensor State for Robot # 2
    small_ir_3_read =  digitalRead(r3_read);  // IR Sensor State for Robot # 3
    small_ir_4_read =  digitalRead(r4_read);  // IR Sensor State for Robot # 4

    // INDICATION LEDs for Small Robot IR Sensors
    //    if( small_ir_1_read == 1){
    //      digitalWrite(r1_led, HIGH);
    //      delay(25);
    //      digitalWrite(r1_led, LOW);
    //    }
    //
    //     if( small_ir_2_read == 1){
    //      digitalWrite(r2_led, HIGH);
    //      delay(25);
    //      digitalWrite(r2_led, LOW);
    //    }
    //
    //      if( small_ir_3_read == 1){
    //      digitalWrite(r3_led, HIGH);
    //      delay(25);
    //      digitalWrite(r3_led, LOW);
    //    }
    //
    //     if( small_ir_4_read == 1){
    //      digitalWrite(r4_led, HIGH);
    //      delay(25);
    //      digitalWrite(r4_led, LOW);
    //    }
    // CHECK AUTOMATED ROBOT ARM IR COUNT :
  
    // END AUTOMATED ROBOT ARM IR COUNT INDICATION


    // SERIAL PRINT ROBOT SENSOR VALUES :

    Serial.print("R1 COUNT: ");
    Serial.print( small_ir_1_count);
    Serial.print("/t");

    Serial.print("R2 COUNT: ");
    Serial.print( small_ir_2_count);
    Serial.print("/t");

    Serial.print("R3 COUNT: ");
    Serial.print( small_ir_3_count);
    Serial.print("/t");

    Serial.print("R4 COUNT: ");
    Serial.print( small_ir_4_count);
    Serial.println();

    // ROBOT ONE READING :
    if (small_ir_1_read != last_small_ir_1) {
      if (small_ir_1_read == 0) {             // MAIN_DATA_OUT_PIN of the Automated Robot / Small Robots
        
        small_ir_1_count++;
        delay(100); // was 1000 for all
      }
    }
    last_small_ir_1 = small_ir_1_read;        // Save current state as the last state



    // ROBOT TWO READING :
    if (small_ir_2_read != last_small_ir_2) {
      if (small_ir_2_read == 0) {
        
        small_ir_2_count++;
        delay(100);
      }
    }
    last_small_ir_2 = small_ir_2_read;  // Save current state as the last state

    // ROBOT THREE READING :
    if (small_ir_3_read != last_small_ir_3) {
      if (small_ir_3_read == 0) {
        
        small_ir_3_count++;
        delay(100);
      }
    }
    last_small_ir_3 = small_ir_3_read; // Save current state as the last state

    // ROBOT FOUR / AUTOMATED ROBOT READING :
    if (small_ir_4_read != last_small_ir_4) {
      if (small_ir_4_read == 0) {
        
        small_ir_4_count++;
        delay(100);
      }
    }
    last_small_ir_4 = small_ir_4_read; // Save Current state as the last state

      if ( small_ir_3_count == 1) {
      digitalWrite(r1_led, HIGH);
    }
    if ( small_ir_3_count == 2) {
      digitalWrite(r2_led, HIGH);
    }
    if ( small_ir_3_count == 3) {
      digitalWrite(r3_led, HIGH);
    }
    // TURN ROBOTS OFF WHEN HIT 3 TIMES -- Was 4 before 6 12 2018 [ 4 because of boot up adds one automatically for some reason ]
    if (small_ir_1_count >= 5) {  // was 3
      digitalWrite(r1_relay,   HIGH);     // OFF
      digitalWrite(led1_relay, HIGH);     // OFF
    }

    if (small_ir_2_count >= 5) {
      digitalWrite(r2_relay,   HIGH);       // OFF
      digitalWrite(led2_relay, HIGH);     // OFF
    }

    if (small_ir_3_count >= 5) {
      digitalWrite(r3_relay,   HIGH);     // OFF
      digitalWrite(led3_relay, HIGH);     // OFF
    }

    if (small_ir_4_count >= 5) {
      digitalWrite(r4_relay,   HIGH);     // OFF
      digitalWrite(led4_relay, HIGH);     // OFF
    }

    // If Small Robot/s is/are Hit :
    if (small_ir_1_read == 1 || small_ir_2_read == 1 || small_ir_3_read == 1 ) { // Hit Robot #1 or Hit Robot #2 or Hit Robot # 3
      digitalWrite(s1_fx1, LOW);      // Small Robot Hit Sound
      delay(50);
      digitalWrite(s1_fx1, HIGH);     // Turn sound pin off after triggered
      delay(300);
    }

    // ALL SMALL ROBOTS DEAD :
    if ( small_ir_1_count >=  5 && small_ir_2_count >= 5 && small_ir_3_count >= 5 && small_ir_4_count >= 5) {

      digitalWrite(s3_BOSSfx, LOW);   // Boss makes " You may be defeated me .... " sound
      delay(50);
      digitalWrite(s3_BOSSfx, HIGH);  // Turn sound pin off after triggered
      delay(3000);
      x = 3;
      break;
    }
  }
  // ALL LED Rings Red FOR BOSS
  while (x == 3) {
    for (int  y = 0; y < 48; y++) {
      leds[y] = CRGB::Red;
      FastLED.show();
      delay(50);
    }
    delay(500);
    x = 4;
    break;
  }

  // LARGE / BOSS ROBOT BATTLE ROUTINE :
  while (x == 4) {

    // BOSS BATTLE LOOP

    if (boss_state_1 == 0) {
      int ir1_boss_state = digitalRead(ir1_boss);

      // IR HIT = 0 ( Zero )
      if (ir1_boss_state == 0) { // IF BOSS ROBOT IS HIT IN SENSOR # 1 :
        digitalWrite(s4_BOSSfx, LOW); // HIT SOUND
        delay(50);
        digitalWrite(s4_BOSSfx, HIGH);   // Turn sound pin off after triggered

        // LED ROUTINE HIT ONCE  :
        // Turn Off LEDs in ring
        for (float y = 0; y < 12; y++) { // First Boss LED Ring off
          leds[(int) y] = CRGB:: Black ; // Set Boss LEDs off
        }
        boss_state_1 = 1; // SET CHECK BIT TO FALSE TO AVOID CHECKING IN FUTURE
      }
    }



    if (boss_state_2 == 0) {
      int ir2_boss_state = digitalRead(ir2_boss);

      if (ir2_boss_state == 0) { // IF BOSS ROBOT IS HIT IN SENSOR # 1 :
        digitalWrite(s4_BOSSfx, LOW);    // BOSS HIT SOUND
        delay(50);
        digitalWrite(s4_BOSSfx, HIGH);   // Turn sound pin off after triggered
        for (float y = 12; y < 24; y++) {  // First Boss LED Ring off
          leds[(int) y] = CRGB:: Black ;  // Set Boss LEDs off
        }
        boss_state_2 = 1; // SET CHECK BIT TO FALSE TO AVOID CHECKING IN FUTURE
      }
    }


    // SENSOR # 3 && LED RING # 3
    if (boss_state_3 == 0) {
      int ir3_boss_state = digitalRead(ir3_boss);

      if (ir3_boss_state == 1) { // IF BOSS ROBOT IS HIT IN SENSOR # 1 :

        for (float y = 24; y < 36; y++) {  // First Boss LED Ring off
          leds[(int) y] = CRGB:: Black ;  // Set Boss LEDs off
        }
        delay(200);
        digitalWrite(s4_BOSSfx, LOW);    // BOSS HIT SOUND
        delay(50);
        digitalWrite(s4_BOSSfx, HIGH);   // Turn sound pin off after triggered



        boss_state_3 = 1; // SET CHECK BIT TO FALSE TO AVOID CHECKING IN FUTURE
      }
    }


    // SENSOR # 4 && LED RING # 4
    if (boss_state_4 == 0) {
      int ir4_boss_state = digitalRead(ir4_boss);

      for (float y = 36; y < 48; y++) {  // First Boss LED Ring off
        leds[(int) y] = CRGB:: Black ;  // Set Boss LEDs off
      }
      delay(200);


      if (ir3_boss_state == 1) { // IF BOSS ROBOT IS HIT IN SENSOR # 4 :
        digitalWrite(s4_BOSSfx, LOW); // HIT SOUND


        boss_state_4 = 1; // SET CHECK BIT TO FALSE TO AVOID CHECKING IN FUTURE
      }
    }

    if (boss_state_1 == 1 && boss_state_2 == 1 && boss_state_3 == 1 && boss_state_4 == 1) {
      // digitalWrite( // Boss death
      digitalWrite(s4_BOSSfx, LOW);   // BOSS DEATH SOUND
      delay(50);
      digitalWrite(s4_BOSSfx, HIGH);  // Set sound pin off after triggered
      delay(1000);                    // GIVE TIME TO HEAR SOUND BEFORE NEXT ACTION
      digitalWrite(FINAL_PIN, HIGH);  // TO MAIN ESCAPE ROOM CONTROLLER FOR ROOM
      x = 5;
      break;
    }
  }


  while (x == 5) { // ENDING / NULL LOOP

    digitalWrite(s6_fx1, LOW);  // END LOOP MUSIC ROUTINE -- Looping trigger?
    //    delay(50);
    //    digitalWrite(s6_fx1, HIGH); // Turn off
  }
}
