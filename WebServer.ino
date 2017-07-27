/* Made by Vagelis Simitsis */

#include <SPI.h>
#include <Ethernet.h>
#define REQ_BUF_SZ   60

int spots = 6;
int strips = 9;

int spotsDimmer = 250;
int stripsDimmer = 250;

char url[REQ_BUF_SZ] = {0};
char url_index = 0;

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,5);

EthernetServer server(80);

void setup() {

  //Serial.begin(9600); //Debugg Mode

  Ethernet.begin(mac, ip);
  server.begin();
  //Serial.print("Server is at "); //Debugg Mode
  //Serial.println(Ethernet.localIP()); //Debugg Mode
}


void loop() {
  EthernetClient client = server.available();

  if (client) {  
    boolean currentLineIsBlank = true;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (url_index < (REQ_BUF_SZ - 1)) {
          url[url_index] = c;         
          url_index++;
        }

        

        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title>Controller</title>");
          client.println("<style>button{width:50px;height:50px;border-radius:50%;}button:focus {outline:0;}body{background:black;color:white;font-size:14pt;font-style:italic;text-align:center;}</style>");
          client.println("</head>");
          client.println("<body>");

          
            //Spots
            client.println("<p>Spot</p>");
            client.println("<a href = \"/spotsOn\"><button style = \"background:green;\">ON</button></a>");
            client.println("<a href = \"/spotsDim\"><button style = \"background:orange;\">Dimm</button></a>");
            client.println("<a href = \"/spotsOff\"><button style = \"background:red;\">OFF</button></a><br></div>");
            //Strips
            client.println("<div style = \"font-style:italic;text-align:center;\"><p>LedStrips</p>");
            client.println("<a href = \"/stripsOn\"><button style = \"background:green;\">ON</button></a>");
            client.println("<a href = \"/stripsDim\"><button style = \"background:orange;\">Dimm</button></a>");
            client.println("<a href = \"/stripsOff\"><button style = \"background:red;\">OFF</button></a><br></div>");
            
            //client.println("<div style = \"font-style:italic;text-align:center;\"><p>Side Strips</p>");
            //client.println("<a href = \"/stripsOn\"><button style = \"background:green;\">ON</button></a>");
            //client.println("<a href = \"/stripsDim\"><button style = \"background:orange;\">Dimm</button></a>");
            //client.println("<a href = \"/stripsOff\"><button style = \"background:red;\">OFF</button></a><br></div>");
            
            client.println("</body>");
            client.println("</html>");
          
  
          //CHANGE STATUS
          if (StrContains(url, "spotsOn")) {
            analogWrite(spots, 255);
            spotsDimmer = 250;
          }
          else if (StrContains(url, "spotsDim")) {
            if(spotsDimmer > 50) {
            spotsDimmer -= 50;
            analogWrite(spots, spotsDimmer);
            }
            else if(spotsDimmer == 50) {
             spotsDimmer -= 30;
             analogWrite(spots, spotsDimmer);
            }
            else if(spotsDimmer == 20) {
             analogWrite(spots, spotsDimmer);
            }
          }
          else if (StrContains(url, "spotsOff")) { 
            analogWrite(spots, 0);
          }
          else if (StrContains(url, "stripsOn")) {
            analogWrite(strips, 255);
            stripsDimmer = 250;
          }
          else if (StrContains(url, "stripsDim")) {
            if(stripsDimmer > 50) {
            stripsDimmer -= 50;
            analogWrite(strips, stripsDimmer);
            }
            else if(stripsDimmer == 50) {
             stripsDimmer -= 30;
             analogWrite(strips, stripsDimmer);
            }
            else if(stripsDimmer == 20) {
             analogWrite(strips, stripsDimmer);
            }
          }
          else if (StrContains(url, "stripsOff")) { 
            analogWrite(strips, 0);
          }
          
          //Serial.print(url); //Debugg Mode
                    
          url_index = 0;
          StrClear(url, REQ_BUF_SZ);
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    //Serial.println("client disonnected"); //Debugg Mode
  }
}


void StrClear(char *str, char length)
{
    for (int i = 0; i <= length; i++) {
        str[i] = 0;
    }
}


char StrContains(char *str, char *sfind)
{
  char found = 0;
  char index = 0;
  char len;

  len = strlen(str);

  if (strlen(sfind) > len) {
    return 0;
  }
  while (index < len) {
    if (str[index] == sfind[found]) {
      found++;
      if (strlen(sfind) == found) {
        return 1;
      }
    }
    else {
      found = 0;
    }
    index++;
  }

  return 0;
}



