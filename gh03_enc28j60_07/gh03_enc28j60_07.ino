// GH03 test opzet 02 jan 2019
// add state on oled 17-01-2019
// add button 19-01-2019
// add ntp time 21-01-2019
// add LM35 25-01-2019

// -------------------------------------------------------------------------------

#include <Time.h>
#include <TimeLib.h>
#include <SPI.h>
#include <Wire.h>
#include <ssd1306.h>
#include <EtherCard.h>

// Ethernet mac address - must be unique on your network
const byte mymac[] PROGMEM = { 0x70, 0x69, 0x69, 0x2D, 0x32, 0x31 };
const char NTP_REMOTEHOST[] PROGMEM = "ntp.bit.nl";  // NTP server name
const unsigned int NTP_REMOTEPORT = 123;             // NTP requests are to port 123
const unsigned int NTP_LOCALPORT = 8888;             // Local UDP port to use
const unsigned int NTP_PACKET_SIZE = 48;             // NTP time stamp is in the first 48 bytes of the message
byte Ethernet::buffer[350];                          // Buffer must be 350 for DHCP to work

byte ThisByte;
BufferFiller bfill;

// Auxiliary variables to store the current output state
String output01State = "off";         
String output02State = "off";
String output03State = "off";         
String output04State = "off";

String epochDay = "1";
String epochMonth = "1";
String epochYear = "1";

String temperatureC = "0.0";

// Assign output variables GPIO pins
const int output01 = 4;          
const int output02 = 5;
const int output03 = 6;          
const int output04 = 7;

const int buttonPin1 = A0;                // number of the pushbutton pin
const int buttonPin2 = A1;
const int buttonPin3 = A2;
const int buttonPin4 = A3;

int outputpin= A7;

int buttonState1;                         // current reading from the input pin
int buttonState2;
int buttonState3;
int buttonState4;

int lastButtonState1 = LOW;               // previous reading from the input pin
int lastButtonState2 = LOW;
int lastButtonState3 = LOW;
int lastButtonState4 = LOW;

unsigned long lastDebounceTime1 = 0;      // last time output pin was toggled
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
unsigned long lastDebounceTime4 = 0;

unsigned long debounceDelay1 = 100;       // debounce time 
unsigned long debounceDelay2 = 100;       // increase if the output flickers
unsigned long debounceDelay3 = 100;
unsigned long debounceDelay4 = 100;

uint32_t lastMillis;
uint8_t  hours = 12;
uint8_t  minutes = 30;
uint8_t  seconds = 30;

// -------------------------------------------------------------------------------

static word homePage1(){
bfill = ether.tcpOffset();
bfill.emit_p(PSTR(
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "LED 1 On"
  "</title></head>"
  "<body>"
    "<h3>LED 1 : 1</h3>"
  "</body>"
"</html>"
));
return bfill.position();
}

static word homePage1a(){
bfill = ether.tcpOffset();
bfill.emit_p(PSTR(
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "LED 1 Off"
  "</title></head>"
  "<body>"
    "<h3>LED 1 : 0</h3>"
  "</body>"
"</html>"
));
return bfill.position();
}

static word homePage2(){
bfill = ether.tcpOffset();
bfill.emit_p(PSTR(
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "LED 2 On"
  "</title></head>"
  "<body>"
    "<h3>LED 2 : 1</h3>"
  "</body>"
"</html>"
));
return bfill.position();
}

static word homePage2a(){
bfill = ether.tcpOffset();
bfill.emit_p(PSTR(
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "LED 2 Off"
  "</title></head>"
  "<body>"
    "<h3>LED 2 : 0</h3>"
  "</body>"
"</html>"
));
return bfill.position();
}

static word homePage3(){
bfill = ether.tcpOffset();
bfill.emit_p(PSTR(
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "LED 3 On"
  "</title></head>"
  "<body>"
    "<h3>LED 3 : 1</h3>"
  "</body>"
"</html>"
));
return bfill.position();
}

static word homePage3a(){
bfill = ether.tcpOffset();
bfill.emit_p(PSTR(
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "LED 3 Off"
  "</title></head>"
  "<body>"
    "<h3>LED 3 : 0</h3>"
  "</body>"
"</html>"
));
return bfill.position();
}

static word homePage4(){
bfill = ether.tcpOffset();
bfill.emit_p(PSTR(
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "LED 4 On"
  "</title></head>"
  "<body>"
    "<h3>LED 4 : 1</h3>"
  "</body>"
"</html>"
));
return bfill.position();
}

static word homePage4a(){
bfill = ether.tcpOffset();
bfill.emit_p(PSTR(
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "LED 4 Off"
  "</title></head>"
  "<body>"
    "<h3>LED 4 : 0</h3>"
  "</body>"
"</html>"
));
return bfill.position();
}

// -------------------------------------------------------------------------------

void show_splash() {
  ssd1306_clearScreen();
  ssd1306_printFixed(35,  2, "Connecting", STYLE_NORMAL);

  ssd1306_printFixed(50, 26, "GH03", STYLE_NORMAL);

  ssd1306_printFixed(40,  56, "Just wait", STYLE_NORMAL);
  delay(500);
  ssd1306_clearScreen();
}

// -------------------------------------------------------------------------------

void show_info() {
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  ssd1306_printFixed(2,  56, "GH03", STYLE_NORMAL);
  
  ssd1306_printFixed(82,  16, "A :", STYLE_NORMAL);
  const char* Astate = output01State.c_str();
  ssd1306_printFixed(102,  16, Astate, STYLE_NORMAL);
  
  ssd1306_printFixed(82,  24, "B :", STYLE_NORMAL);
  const char* Bstate = output02State.c_str();
  ssd1306_printFixed(102,  24, Bstate, STYLE_NORMAL);

  ssd1306_printFixed(82,  32, "C :", STYLE_NORMAL);
  const char* Cstate = output03State.c_str();
  ssd1306_printFixed(102,  32, Cstate, STYLE_NORMAL);

  ssd1306_printFixed(82,  40, "D :", STYLE_NORMAL);
  const char* Dstate = output04State.c_str();
  ssd1306_printFixed(102,  40, Dstate, STYLE_NORMAL);

  char* ip2CharArray(ether.myip) ;
  static char a[16];
  sprintf(a, "%d.%d.%d.%d", ether.myip[0], ether.myip[1], ether.myip[2], ether.myip[3]);
  ssd1306_printFixed(32,  56, a, STYLE_NORMAL);
}

// -------------------------------------------------------------------------------

void printSeconds()
{  
    ssd1306_setFixedFont(courier_new_font11x16_digits);
    
    if (seconds & 1)
    {
        ssd1306_printFixed(32,  20, ":", STYLE_NORMAL);
    }
    else
    {
        ssd1306_printFixed(32,  20, " ", STYLE_NORMAL);
    }
}

// -------------------------------------------------------------------------------

void printMinutes()
{
    ssd1306_setFixedFont(courier_new_font11x16_digits);

    char minutesStr[3] = "00";
    minutesStr[0] = '0' + minutes / 10;
    minutesStr[1] = '0' + minutes % 10;
    ssd1306_printFixed(46,  20, minutesStr, STYLE_NORMAL);
}

// -------------------------------------------------------------------------------

void printHours()
{
    ssd1306_setFixedFont(courier_new_font11x16_digits);
  
    char hoursStr[3] = "00";
    hoursStr[0] = '0' + hours / 10;
    hoursStr[1] = '0' + hours % 10;
    ssd1306_printFixed(6,  20, hoursStr, STYLE_NORMAL);
}

// -------------------------------------------------------------------------------

void printTemp()
{
    ssd1306_setFixedFont(ssd1306xled_font6x8);
  
    char curtemp[6];
    temperatureC.toCharArray(curtemp, 6);
    ssd1306_printFixed(24,  44, curtemp, STYLE_NORMAL);
    ssd1306_printFixed(56,  44, "c", STYLE_NORMAL);
}

// -------------------------------------------------------------------------------

void printNowDate()
{
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  
  char charDay[3];
  epochDay.toCharArray(charDay, 3);
  ssd1306_printFixed(26,  2, charDay, STYLE_NORMAL);
  ssd1306_printFixed(40,  2, "/", STYLE_NORMAL);

  char charMth[3];
  epochMonth.toCharArray(charMth, 3);
  ssd1306_printFixed(48,  2, charMth, STYLE_NORMAL);
  ssd1306_printFixed(62,  2, "/", STYLE_NORMAL);

  char charYear[5];
  epochYear.toCharArray(charYear, 5);
  ssd1306_printFixed(70,  2, charYear, STYLE_NORMAL);
}

// -------------------------------------------------------------------------------

void readTemperatureSensor(void)
{
int rawvoltage= analogRead(outputpin);
float millivolts= (rawvoltage/1023.0) * 5000;
float celsius= millivolts/10;
// Serial.print(celsius);
// Serial.print(" degrees Celsius, ");
// Serial.print((celsius * 9)/5 + 32);
// Serial.println(" degrees Fahrenheit");

temperatureC = celsius;
}

// -------------------------------------------------------------------------------

void startNtp()
{
  uint8_t ntpIp[IP_LEN];
  ether.copyIp(ntpIp, ether.hisip);
  ether.printIp("NTP: ", ntpIp);

  ether.udpServerListenOnPort(&udpReceiveNtpPacket, NTP_LOCALPORT);
  Serial.println("Started listening for response.");
  sendNTPpacket(ntpIp);
}

// -------------------------------------------------------------------------------

// send an NTP request to the time server at the given address
void sendNTPpacket(const uint8_t* remoteAddress) {
  // buffer to hold outgoing packet
  byte packetBuffer[ NTP_PACKET_SIZE];
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;            // Stratum, or type of clock
  packetBuffer[2] = 6;            // Polling Interval
  packetBuffer[3] = 0xEC;         // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  ether.sendUdp(packetBuffer, NTP_PACKET_SIZE, NTP_LOCALPORT, remoteAddress, NTP_REMOTEPORT );
  Serial.println("NTP request sent.");
}

// -------------------------------------------------------------------------------

void udpReceiveNtpPacket(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *packetBuffer, uint16_t len) {
  Serial.println("NTP response received.");

  // the timestamp starts at byte 40 of the received packet and is four bytes,
  // or two words, long. First, extract the two words:
  unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
  unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
  // combine the four bytes (two words) into a long integer
  // this is NTP time (seconds since Jan 1 1900):
  unsigned long secsSince1900 = highWord << 16 | lowWord;
  // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
  const unsigned long seventyYears = 2208988800UL;
  // subtract seventy years:
  unsigned long epoch = secsSince1900 - seventyYears;

  // print Unix time:
  Serial.print("Unix time = ");
  Serial.println(epoch);

    Serial.print(weekday(epoch));
    Serial.print(" ");
    epochYear = (year(epoch));
    Serial.print(year(epoch));
    Serial.print(" ");
    epochMonth = (month(epoch));
    Serial.print(month(epoch));
    Serial.print(" ");
    epochDay = (day(epoch));
    Serial.print(day(epoch));
    Serial.print(" ");
    hours = (hour(epoch) + 1);
    Serial.print(hour(epoch));
    Serial.print(":");
    minutes = (minute(epoch));
    Serial.print(minute(epoch));
    Serial.print(":");
    Serial.print(second(epoch));
    Serial.println(" ");

    printNowDate();
    printHours();
    printMinutes();
}

// -------------------------------------------------------------------------------

void setup() {
  Wire.begin();
  pinMode(output01, OUTPUT);
  pinMode(output02, OUTPUT);
  pinMode(output03, OUTPUT);
  pinMode(output04, OUTPUT);

  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  
  Serial.begin(57600);
  Serial.println("[Oled config]");
    /* Replace the line below with ssd1306_128x32_i2c_init() if you need to use 128x32 display */
    ssd1306_128x64_i2c_init();
    ssd1306_fillScreen(0x00);
    ssd1306_setFixedFont(ssd1306xled_font6x8);
 
    show_splash();
  Serial.println("[connecting to lan]");

   // Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);

  if (!ether.dnsLookup(NTP_REMOTEHOST))
    Serial.println("DNS failed");

    startNtp();
    show_info();
    printHours();
    printMinutes();
    printTemp();
}

// -------------------------------------------------------------------------------

void loop(){

readTemperatureSensor();

int reading1 = digitalRead(buttonPin1);   // state of button 01
  if (reading1 != lastButtonState1) {
    lastDebounceTime1 = millis();
  }

  if ((millis() - lastDebounceTime1) > debounceDelay1) {
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
      if (buttonState1 == HIGH) {
        Serial.println("[Switch 1]");
        if (output01State == "off") {
        output01State = " on";
        digitalWrite(output01, HIGH);
        }
        else if (output01State == " on") {
        output01State = "off";
        digitalWrite(output01, LOW);
        }
      }
      show_info();
    }
  }
  lastButtonState1 = reading1;

  int reading2 = digitalRead(buttonPin2);   // state of button 02
  if (reading2 != lastButtonState2) {
    lastDebounceTime2 = millis();
  }

  if ((millis() - lastDebounceTime2) > debounceDelay2) {
    if (reading2 != buttonState2) {
      buttonState2 = reading2;
      if (buttonState2 == HIGH) {
        Serial.println("[Switch 2]");
        if (output02State == "off") {
        output02State = " on";
        digitalWrite(output02, HIGH);
        }
        else if (output02State == " on") {
        output02State = "off";
        digitalWrite(output02, LOW);
        }
      }
      show_info();
    }
  }
  lastButtonState2 = reading2;

  int reading3 = digitalRead(buttonPin3);   // state of button 03
  if (reading3 != lastButtonState3) {
    lastDebounceTime2 = millis();
  }

  if ((millis() - lastDebounceTime3) > debounceDelay3) {
    if (reading3 != buttonState3) {
      buttonState3 = reading3;
      if (buttonState3 == HIGH) {
        Serial.println("[Switch 3]");
        if (output03State == "off") {
        output03State = " on";
        digitalWrite(output03, HIGH);
        }
        else if (output03State == " on") {
        output03State = "off";
        digitalWrite(output03, LOW);
        }
      }
      show_info();
    }
  }
  lastButtonState3 = reading3;

  int reading4 = digitalRead(buttonPin4);   // state of button 04
  if (reading4 != lastButtonState4) {
    lastDebounceTime4 = millis();
  }

  if ((millis() - lastDebounceTime4) > debounceDelay4) {
    if (reading4 != buttonState4) {
      buttonState4 = reading4;
      if (buttonState4 == HIGH) {
        Serial.println("[Switch 4]");
        if (output04State == "off") {
        output04State = " on";
        digitalWrite(output04, HIGH);
        }
        else if (output04State == " on") {
        output04State = "off";
        digitalWrite(output04, LOW);
        }
      }
      
      show_info();
    }
  }
  lastButtonState4 = reading4;
  

if ((uint32_t)(millis() - lastMillis) >= 1000)
    {
      
    Serial.print(hours);
    Serial.print(" ");
    Serial.print(minutes);
    Serial.print(" ");
    Serial.print(seconds);
    Serial.println(" ");

   if (hours == 3 && minutes == 1 && seconds == 10) 
                     {
                       startNtp();
                       printNowDate();
                       delay(1000);
                     }
      
    lastMillis += 1000;
		    if (++seconds > 59)
        {
            seconds = 0;
            if (++minutes > 59)
            {
                minutes = 0;
                if (++hours > 23)
                {
                    hours = 0;
                }
                
                printHours();
            }
            printMinutes();
        }
        printSeconds();
        printTemp();
    }

 

  
  word pos = ether.packetLoop(ether.packetReceive());
  if(pos){
   
   if (Ethernet::buffer[59]==97){ //97 = a , GROENE LED
     Serial.println("[Switch a]");
        if (output01State == "off") {
        output01State = " on";
        digitalWrite(output01, HIGH);
        ether.httpServerReply(homePage1());
        }
        else if (output01State == " on") {
        output01State = "off";
        digitalWrite(output01, LOW);
        ether.httpServerReply(homePage1a());
        }
  }
   
   if (Ethernet::buffer[59]==98){ //98 = b , LED GEEL
     Serial.println("[Switch b]");
        if (output02State == "off") {
        output02State = " on";
        digitalWrite(output02, HIGH);
        ether.httpServerReply(homePage2());
        }
        else if (output02State == " on") {
        output02State = "off";
        digitalWrite(output02, LOW);
        ether.httpServerReply(homePage2a());
        }
  }
    
    if (Ethernet::buffer[59]==99){ //99 = c , LED ROOD
     Serial.println("[Switch c]");
        if (output03State == "off") {
        output03State = " on";
        digitalWrite(output03, HIGH);
        ether.httpServerReply(homePage3());
        }
        else if (output03State == " on") {
        output03State = "off";
        digitalWrite(output03, LOW);
        ether.httpServerReply(homePage3a());
        }
  } 
   
   if (Ethernet::buffer[59]==100){ //100 = d , LED BLAUW
     Serial.println("[Switch d]");
        if (output04State == "off") {
        output04State = " on";
        digitalWrite(output04, HIGH);
        ether.httpServerReply(homePage4());
        }
        else if (output04State == " on") {
        output04State = "off";
        digitalWrite(output04, LOW);
        ether.httpServerReply(homePage4a());
        }
  }
  show_info();
  printHours();
  printMinutes();
  }
  ether.packetLoop(ether.packetReceive());
}
