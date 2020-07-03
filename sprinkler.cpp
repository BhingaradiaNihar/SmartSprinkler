#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// (FAST) SOFTWARE SPI
// pin A3 - Serial clock out (SCLK) - User defined
// pin A5 - Serial data out (DIN/MOSI) - User defined
// pin D2 - Data/Command select (D/C) - User defined
// pin A2 - LCD chip select (CS) - User defined
// pin D3 - LCD reset (RST) - User defined
// Adafruit_PCD8544(SCLK, DIN, DC, CS, RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(A3, A5, D2, A2, D3);

int zone1 = D0;
int zone2 = D1;
int zone3 = A0;
int zone4 = A1;
int zone5 = D4;
int zone6 = D5;
int zone7 = D6;
int zone8 = D7;
int button1= A6; // red
int button2= A4; // green
int button3 =A7; // yellow

struct DateTime{
    String _date, _time, _day;
    time_t raw;
    DateTime() : _date(""), _time(""), _day(""), raw(0){}
};

/* global variables */
DateTime curr, prev;
bool lock = false;
// mobile_data used for sending notifications to mobile 
// data code  [1-8] for perticular sprinklars 
// 9 turn on all one by one
// 10 turn off all sprinklers Forced Stop
bool forced_stop = false;
int zone_time[9]= {-1,1,1,1,1,1,1,1,1};  // ignore index 0 // in minutes // zones are from 1-8 
int day_to_water = 0;
int Mode = 0; 
int stopper1 = 0; 
int stopper2 = 0;
int stopper3 = 0; 
int mode_1_zone = 0;
const int TOTAL_MODE = 6;
int time_to_water = 4; // in 24 hours specify hour


/* function declaratin */
 void set_day_to_water();
 void turn_on_zone(int zone);
 String get_day(int num);
 void increment_day_to_water();
 void display_skip_day();
 void display_and_water_zone(int number);
 void mode_0();
 void mode_1();
 void mode_2();
 void mode_3();
 void mode_4();
 void display_forced_stop();
 int minute_to_milis(int minute);
 void reset_mode_1_zones() {mode_1_zone = 0;}
 DateTime get_DateTime(time_t mytime);


void setup() {
    pinMode (zone1, OUTPUT);
    pinMode (zone2, OUTPUT);
    pinMode (zone3, OUTPUT);
    pinMode (zone4, OUTPUT);
    pinMode (zone5, OUTPUT);
    pinMode (zone6, OUTPUT);
    pinMode (zone7, OUTPUT);
    pinMode (zone8, OUTPUT);
    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    digitalWrite(zone1, HIGH);
    digitalWrite(zone2, HIGH);
    digitalWrite(zone3, HIGH);
    digitalWrite(zone4, HIGH);
    digitalWrite(zone5, HIGH);
    digitalWrite(zone6, HIGH);
    digitalWrite(zone7, HIGH);
    digitalWrite(zone8, HIGH);
    
    // set Day-Light saving time if applicable and Sync time
    Particle.syncTime();
    if(Time.isDST()) 
        Time.zone(-5.00);
    else
        Time.zone(-4.00);
        
    set_day_to_water(); // sets next schedule
        
    display.begin();
    display.setContrast(55);
    display.display(); // show splashscre
    delay(1000);
    display.clearDisplay();   // clears the screen and buffer
    Particle.subscribe("Mobile", mobile_handle, ALL_DEVICES); //
}

void turn_on_zone(int zone){
    if (forced_stop == false ) {           // force stop is false
        switch(zone){
            case 1:
                digitalWrite(zone1, LOW);
                display_and_water_zone(1, minute_to_milis(zone_time[1]));                // FIX THIS
                //delay(minute_to_milis(zone_time[1]));
                digitalWrite(zone1, HIGH);
                break;
            case 2:
                digitalWrite(zone2, LOW);
                display_and_water_zone(2, minute_to_milis(zone_time[2]));
                //delay(minute_to_milis(zone_time[2]));
                digitalWrite(zone2, HIGH);
                break;
            case 3:
                digitalWrite(zone3, LOW);
                display_and_water_zone(3, minute_to_milis(zone_time[3]));
                //delay(minute_to_milis(zone_time[3]));
                digitalWrite(zone3, HIGH);
                break;
            case 4:
                digitalWrite(zone4, LOW);
                display_and_water_zone(4, minute_to_milis(zone_time[4]));
                //delay(minute_to_milis(zone_time[4]));
                digitalWrite(zone4, HIGH);
                break;
            case 5:
                digitalWrite(zone5, LOW);
                display_and_water_zone(5, minute_to_milis(zone_time[5]));
                //delay(minute_to_milis(zone_time[5]));
                digitalWrite(zone5, HIGH);
                break;
            case 6:
                digitalWrite(zone6, LOW);
                display_and_water_zone(6, minute_to_milis(zone_time[6]));
                //delay(minute_to_milis(zone_time[6]));
                digitalWrite(zone6, HIGH);
                break;
            case 7:
                digitalWrite(zone7, LOW);
                display_and_water_zone(7, minute_to_milis(zone_time[7]));
                //delay(minute_to_milis(zone_time[7]));
                digitalWrite(zone7, HIGH);
                break;
            case 8:
                digitalWrite(zone8, LOW);
                display_and_water_zone(8, minute_to_milis(zone_time[8]));
                //delay(minute_to_milis(zone_time[8]));
                digitalWrite(zone8, HIGH);
                break; 
            default:
                digitalWrite(zone1, HIGH);
                digitalWrite(zone2, HIGH);
                digitalWrite(zone3, HIGH);
                digitalWrite(zone4, HIGH);
                digitalWrite(zone5, HIGH);
                digitalWrite(zone6, HIGH);
                digitalWrite(zone7, HIGH);
                digitalWrite(zone8, HIGH);
                break;
        }
    }
    
    prev = get_DateTime(Time.now()); // sets previously water date/time
    
}


void mobile_handle(const char* event, const char* data) {
    int recieve_data = atoi(data);         // based on data run functions
   if (lock == false) {
     lock = true;                  // serve only one request at a time 
    switch (recieve_data){
        case 1: 
            turn_on_zone(1);
            break;
        case 2: 
            turn_on_zone(2);
            break;
        case 3: 
            turn_on_zone(3);
            break;
        case 4: 
            turn_on_zone(4);
            break;
        case 5: 
            turn_on_zone(5);
            break;
        case 6:
            turn_on_zone(6);
            break;
        case 7: 
            turn_on_zone(7);
            break;
        case 8: 
            turn_on_zone(8);
            break;
        case 9:                           // turn on all sprinkler one by one
            for (int i = 1 ; i <= 8 ; i++){
              if(digitalRead(button1) == LOW || forced_stop){ // Forced Stop
                forced_stop = true;  // Forced stop variable
                display_forced_stop();
                // reset happens in loop after sending notification
                forced_stop = false; // reset
                break;
                 }
                turn_on_zone(i);
            }
            break;
        case 10:                           //  Forced to stop
            forced_stop = true;
            break;
        case 11:                           // If rain then skipped or user can skip it as well
            increment_day_to_water();
            display_skip_day();
            break;
        default:
        break;
    }
   }
   lock = false;
}

void loop() {

    int btnPress1 = digitalRead(button1);
    if(btnPress1 == LOW && stopper1 == 0){
        Mode = (Mode + 1 ) % TOTAL_MODE;
        stopper1 = 1;
    }
    int btnPress2 = digitalRead(button2);
    if(btnPress2 == LOW && stopper2 == 0){

        if(Mode == 2){
            for (int i = 1 ; i <= 8 ; i++){
                if(digitalRead(button1) == LOW || forced_stop){ // Forced Stop
                    forced_stop = true;  // Forced stop variable
                    display_forced_stop();
                    forced_stop = false; // reset
                    break;
                }
                turn_on_zone(i); // turns all zone one by one
            }
        }
        else if(Mode == 3){
            increment_day_to_water(); // skip a day
            display_skip_day();
        }
        else if(Mode == 4){
             for (int i = 1 ; i <= 8 ; i++)
                if(++zone_time[i] > 15)
                    zone_time[i] = 1;
        }
        else if(Mode == 5)
            time_to_water++;
            
        stopper2 = 1;
    }
    
    if (Mode == 0) 
        mode_0();
    else if(Mode == 1)
        mode_1();
    else if(Mode == 2)
        mode_2();
    else if (Mode == 3)
        mode_3();
    else if(Mode  == 4)
        mode_4();
    else
        mode_5();
    

    if (btnPress1 == HIGH) stopper1 = 0;
    if (btnPress2 == HIGH) stopper2 = 0;
    delay(200);
}

DateTime get_DateTime(time_t mytime){
    
    DateTime data;
    data.raw = mytime;
    String weekday, month, date, time_str;
    
    switch (Time.weekday(mytime)) {
        case 1 : 
        weekday = "Sunday"; break;
        case 2 : 
        weekday = "Monday"; break;
        case 3 : 
        weekday = "Tuesday"; break;
        case 4 : 
        weekday = "Wednesday"; break;
        case 5 : 
        weekday = "Thursday"; break;
        case 6 : 
        weekday = "Friday"; break;
        case 7 : 
        weekday = "Saturday"; break;
    }
     switch (Time.month(mytime)){
         case 1: 
         month = "Jan";break;
         case 2: 
         month = "Feb";break;
         case 3: 
         month = "Mar";break;
         case 4: 
         month = "Apr";break;
         case 5: 
         month = "May";break;
         case 6: 
         month = "Jun";break;
         case 7: 
         month = "Jul";break;
         case 8: 
         month = "Aug";break;
         case 9: 
         month = "Sep";break;
         case 10: 
         month = "Oct";break;
         case 11: 
         month = "Nov";break;
         case 12: 
         month = "Dec";break;
     }
    
    date = month + " " + Time.day(mytime) + " " + Time.year(mytime);
    time_str =(String(Time.hour(mytime))); 
    time_str.concat(":");
    time_str.concat(String(Time.minute(mytime)));
    time_str.concat(":");
    time_str.concat(String(Time.second(mytime)));
    
    data._date = date;
    data._time = time_str;
    data._day = weekday;
    return data;
}       

 void display_and_water_zone(int number, int time){  // FIX THIS
    int sec = time / 1000;
    while (sec){
    display.clearDisplay(); 
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println(" Watering Zone ");
    display.setTextSize(2);
    display.println("   " + String(number));
    display.setTextSize(1);
    display.println("Timer: " + String(sec / 60)+ ":" + String(sec % 60));
    display.display();
    delay(1000);
        if(digitalRead(button1) == LOW || forced_stop){ // Forced Stop
            forced_stop = true;  // Forced stop variable
            display_forced_stop();
            ///forced_stop = false; // make  skip
            break;
        }
    sec--;
    }
}
  
  
  
void set_day_to_water(){
    curr = get_DateTime(Time.now());
    Serial.println( Time.hour(curr.raw));
    if(Time.hour(curr.raw) < 4)
        day_to_water = Time.weekday(curr.raw);
    else{
        day_to_water = Time.weekday(curr.raw);
        increment_day_to_water();
    }
}

String get_day_string(int num){

    num = (num == 8) ? 1 : num ;
    String temp;
    switch (num) {
        case 1 : 
        temp = "Sunday"; break;
        case 2 : 
        temp = "Monday"; break;
        case 3 : 
        temp = "Tuesday"; break;
        case 4 : 
        temp = "Wednesday"; break;
        case 5 : 
        temp = "Thursday"; break;
        case 6 : 
        temp = "Friday"; break;
        case 7 : 
        temp = "Saturday"; break;
    }
    return temp;
}

 void increment_day_to_water(){
    if(day_to_water == 7)
        day_to_water = 1;
    else
        day_to_water++;
 }
  
 void display_skip_day(){
    display.clearDisplay(); 
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println("successfully  Skipped");
    display.println("Next Schedule");
    display.println("is on");
    display.println(get_day_string(day_to_water));
    display.display();
    delay(2000);
 }
 
 void mode_0(){
    DateTime curr = get_DateTime(Time.now());
    display.clearDisplay(); 
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.println(curr._date);
    display.println(curr._day);
    display.println("");
    display.println("   " +curr._time);
    display.println();
    display.display();
}
 void mode_2(){
    if(prev.raw){
        display.clearDisplay(); 
        display.setTextColor(BLACK);
        display.setCursor(0,0);
        display.setTextSize(1);
        display.println("Waterd Previously");
        display.println("");
        display.println(prev._day);
        display.println(prev._time);
        display.println(prev._date);
        display.display();
    }
    else{
        display.clearDisplay(); 
        display.setTextColor(BLACK);
        display.setCursor(0,0);
        display.setTextSize(1);
        display.println("");
        display.println("You haven't   Waterd \nPreviously");
        display.display();
    }
}
  
void mode_3(){
    display.clearDisplay(); 
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println("Next Schedule");
    display.println("is on");
    display.println(get_day_string(day_to_water));
    
    if(time_to_water < 12)
        display.println("at " + String(time_to_water) + " am");
    else if (time_to_water == 12)
        display.println("at " + String(time_to_water) + " pm");
    else
        display.println("at " + String(time_to_water - 12) + " pm");
    
    display.display();
}  

void mode_1(){

    auto btnPress3 = digitalRead(button3);
   
   if(btnPress3 == LOW && stopper3 == 0){
        mode_1_zone = ++mode_1_zone % 9;
        stopper3 = 1;
    }

    display.clearDisplay(); 
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println("     Zone");
    display.println("");
    display.setTextSize(2);
    if (mode_1_zone != 0 )
        display.println("   " + String(mode_1_zone));
    else
        display.println("  ALL");

    display.setTextSize(1);
    display.display();




    if(digitalRead(button2) == LOW){


        if(mode_1_zone == 0){
            for (int i = 1 ; i <= 8 ; i++){
                if(forced_stop){ // Forced Stop
                    forced_stop = true;  // Forced stop variable
                    display_forced_stop();
                    forced_stop = false; // reset
                    break;
                }
                turn_on_zone(i); // turns all zone one by one
            }
        }
        else
            turn_on_zone(mode_1_zone);

    }

    if(btnPress3 == HIGH) stopper3 = 0;

}
  
void mode_4(){
    display.clearDisplay(); 
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.setTextSize(1);
    String message = "Each zones are schedule to  water for " + String(zone_time[1]);
    display.println(message);
    display.println("minutes");
    display.display();
}

void display_forced_stop(){
    display.clearDisplay(); 
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.setTextSize(2);
    display.println("FORCED\nSTOP");
    display.display();
    delay(2000);
}

void mode_5(){
    display.clearDisplay(); 
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println("Time to water");
    if(time_to_water < 12)
        display.println("set to " + String(time_to_water) + " am");
    else if (time_to_water == 12)
        display.println("set to " + String(time_to_water) + " pm");
    else
        display.println("set to " + String(time_to_water - 12) + " pm");
    
    display.display();
}
  
int minute_to_milis(int minute){             // minute to second conversion
    return (minute * 60 *1000);
}