/********************
- www.geekstips.com
- Arduino Time Sync from NTP Server using ESP8266 WiFi module 
- Arduino code example
 ********************/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "HOME-2F25-2.4";  //  your network SSID (name)
char pass[] = "dairy1336camera";       // your network password


unsigned int localPort = 2390;      // local port to listen for UDP packets

/* Don't hardwire the IP address or we won't get the benefits of the pool.
 *  Lookup the IP address for the host name instead */
//IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

void setup()
{
  Serial.begin(9600);
  //println();
  //println();

  // We start by connecting to a WiFi network
  //print("Connecting to ");
  //println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //print(".");
  }
  //println("");
  
  //println("WiFi connected");
  //println("IP address: ");
  //println(WiFi.localIP());

  //println("Starting UDP");
  udp.begin(localPort);
  //print("Local port: ");
  //println(udp.localPort());
}

void loop()
{
  //get a random server from the pool
  delay(1000);

  WiFi.hostByName(ntpServerName, timeServerIP); 
  String TimeSend;
  int nHour;
  int i;
  char myTime[9];

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  
  int cb = udp.parsePacket();
  if (!cb) {
    //println("no packet yet");
  }
  else {
    //print("packet received, length=");
    //println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    //print("Seconds since Jan 1 1900 = " );
    //println(secsSince1900);

    // now convert NTP time into everyday time:
    //print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    //println(epoch);


    // print the hour, minute and second:
    //print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    nHour = (((epoch  % 86400L) / 3600)); // print the hour (86400 equals secs per day)
    if (nHour > 12) {
      nHour = (nHour - 12);
    }
    nHour  = (nHour - 4);  // timezone adjust
    if (nHour < 1) {
      nHour = (nHour + 12);
    }
    
    TimeSend = String(nHour) + (',');
//    if (nHour > 9) {
//      TimeSend = '0' + TimeSend;
//    }
  //  TimeSend = TimeSend  + (',');
    if ( ((epoch % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      //TimeSend = TimeSend  + '0';
    }
    TimeSend = TimeSend  +  (((epoch  % 3600) / 60)); // print the minute (3600 equals secs per minute)
    TimeSend = TimeSend  + (',');
    if ( (epoch % 60) < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      //TimeSend = TimeSend  + '0';
    }
    TimeSend = TimeSend  + ((epoch % 60)); // print the second
    TimeSend = TimeSend  + "\n";
    
    TimeSend.toCharArray(myTime, 10) ;

    Serial.write(myTime);
    //Serial.println(TimeSend);
  }

      // wait X times 10 seconds before asking for the time again
    for(i=1; i<2; i++) {
      delay(10000);
    }
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address)
{
  //println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
