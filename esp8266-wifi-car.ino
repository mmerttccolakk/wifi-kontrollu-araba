#include <ESP8266WiFi.h>
 #include <HCSR04.h>

WiFiClient client;
WiFiServer server(80);
 
const char* ssid = "mert";
const char* password = "12345678";
 
String data ="";
 
int sol = 2; /* GPIO2(D4) -> IN3 */
int sag = 15; /* GPIO15(D8) -> IN1 */
int ileri = 0; /* GPIO0(D3) -> IN4 */
int geri = 13; /* GPIO13(D7) -> IN2 */
int korna = 12; /* GPIO12(D6) -> IN2 */
int sol_led = 5; /* GPIO0(D3) -> IN4 */
int sag_led = 16; /* GPIO13(D7) -> IN2 */
int far = 4; /* GPIO12(D6) -> IN2 */


 
void setup() {
Serial.begin(115200);
delay(10);
 
UltraSonicDistanceSensor distanceSensor(10, 11);
pinMode(sol, OUTPUT);
pinMode(sag, OUTPUT);
pinMode(ileri, OUTPUT);
pinMode(geri, OUTPUT);
pinMode(korna, OUTPUT);
pinMode(sol_led, OUTPUT);
pinMode(sag_led, OUTPUT);
pinMode(far, OUTPUT);

digitalWrite(far,HIGH);
digitalWrite(sol_led,HIGH);
digitalWrite(sag_led,HIGH);
 
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
 
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi baglandi");
 
server.begin();
Serial.println("Server baslatildi");
 
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");
 
}
 
void loop() {
  //mesafe algıla
  //if(distanceSensor.measureDistanceCm()<=30 ){
  //MotorStop();
  //}
client = server.available();
if (!client) return;
data = checkClient ();
 
 
if (data == "forward") MotorForward();
else if (data == "backward") MotorBackward();
else if (data == "left") TurnLeft();
else if (data == "right") TurnRight();
else if (data == "stop") MotorStop();
else if (data == "forwardright") forwardright();
else if (data == "forwardleft") forwardleft();
else if (data == "backwardright") backwardright();
else if (data == "backwardleft") backwardleft();
else if (data == "korna_acik") korna_ac();


client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("");
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<head>");
client.println("<title> WIFI ARABA KONTROLU</title>");
client.println("</head>");
client.println("<body>");
client.println("<center>");
client.println("<h2> MERT COLAK </h2>");
client.println("<br><br>");
client.println("<hr>");
 
/* Buttons */
client.println("<br><br>");
client.println("<a href=\"/forwardleft\"\"><button>ileri sol </button></a>");
client.println("<a href=\"/forwardright\"\"><button>ileri sag </button></a><br />");
client.println("<br><br>");
client.println("<a href=\"/forward\"\"><button>ileri </button></a>");
client.println("<a href=\"/backward\"\"><button>geri </button></a><br />");
client.println("<br><br>");
client.println("<a href=\"/left\"\"><button>sol </button></a>");
client.println("<a href=\"/right\"\"><button>sag </button></a><br />");
client.println("<br><br>");
client.println("<a href=\"/stop\"\"><button>dur </button></a><br />");
client.println("<br><br>");
client.println("<a href=\"/backwardleft\"\"><button>geri sol </button></a>");
client.println("<a href=\"/backwardright\"\"><button>geri sag </button></a><br />");
client.println("<br><br>");
client.println("<a href=\"/korna_acik\"\"><button>korna ac</button></a>");
client.println("<br><br>");
 
client.println("<br><br>");
client.println("<hr>");
client.println("<br><br>");
client.println("<pre><h3>MERT COLAK</h3></pre>");
client.println("</center>");
client.println("</body>");
client.println("</html>");
}

 
/** FORWARD ****/
void MotorForward(void)
{
digitalWrite(ileri,HIGH);
digitalWrite(geri,LOW);
}
 
/** BACKWARD ****/
void MotorBackward(void)
{
digitalWrite(ileri,LOW);
digitalWrite(geri,HIGH);
}
 
/** TURN LEFT ****/
void TurnLeft(void)
{
digitalWrite(sol,HIGH);
digitalWrite(sag,LOW);
}
 
/** TURN RIGHT ****/
void TurnRight(void)
{
digitalWrite(sol,LOW);
digitalWrite(sag,HIGH);
}
 
/** STOP ****/
void MotorStop(void)
{
digitalWrite(sol,LOW);
digitalWrite(ileri,LOW);
digitalWrite(sag,LOW);
digitalWrite(geri,LOW);
}

 /** forwardright ****/
void forwardright(void)
{
digitalWrite(sol,LOW);
digitalWrite(ileri,HIGH);
digitalWrite(sag,HIGH);
digitalWrite(geri,LOW);
}
 /** forwardleft ****/
void forwardleft(void)
{
digitalWrite(sol,HIGH);
digitalWrite(ileri,HIGH);
digitalWrite(sag,LOW);
digitalWrite(geri,LOW);
}
 /** backwardright ****/
void backwardright(void)
{
digitalWrite(sol,LOW);
digitalWrite(ileri,LOW);
digitalWrite(sag,HIGH);
digitalWrite(geri,HIGH);
}
 /** backwardright ****/

void backwardleft(void)
{
digitalWrite(sol,HIGH);
digitalWrite(ileri,LOW);
digitalWrite(sag,LOW);
digitalWrite(geri,HIGH);
}
void korna_ac(void){
  digitalWrite(korna,HIGH);
delay(500);
digitalWrite(korna,LOW);
}
void sol_sinyal(void){
digitalWrite(sol_led,LOW);
delay(500);
digitalWrite(sol_led,HIGH);

}
void sag_sinyal(void){
digitalWrite(sag_led,LOW);
delay(500);
digitalWrite(sag_led,HIGH);
}
void far_ac(void){
digitalWrite(far,LOW);
}
void far_kapa(void){
digitalWrite(far,HIGH);
}
 
String checkClient (void){
while(!client.available()) delay(1);
String request = client.readStringUntil('\r');
request.remove(0, 5);
request.remove(request.length()-9,9);
return request;
}
