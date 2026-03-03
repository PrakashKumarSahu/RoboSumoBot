/************* PIN DEFINITIONS *************/
#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 14
#define ENA 33
#define ENB 32

/************* WIFI *************/
#include <WiFi.h>

const char* ap_ssid = "ESP32_ROBOT";
const char* ap_password = "12345678";

/************* MOTOR FUNCTIONS *************/
void left() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void right() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void backward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

/************* SERVER *************/
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
  stopMotors();

  //  ESP32 ACCESS POINT MODE
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);

  Serial.println("ESP32 AP Started");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP()); // always 192.168.4.1

  server.begin();
}

/************* LOOP *************/
void loop() {

  // WEB CONTROL
  WiFiClient client = server.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

    if (request.indexOf("/forward") != -1 || request.indexOf("/move?dir=forward") != -1)
    forward();
  else if (request.indexOf("/backward") != -1 || request.indexOf("/move?dir=backward") != -1)
    backward();
  else if (request.indexOf("/left") != -1 || request.indexOf("/move?dir=left") != -1)
    left();
  else if (request.indexOf("/right") != -1 || request.indexOf("/move?dir=right") != -1)
    right();
  else if (request.indexOf("/stop") != -1 || request.indexOf("/move?dir=stop") != -1)
    stopMotors();

  // HTML RESPONSE
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<html><body style='text-align:center;font-family:Arial;'>");

  client.println("<h2>ESP32 Robot Control</h2>");

  client.println("<a href='/forward'><button>Forward</button></a><br><br>");
  client.println("<a href='/backward'><button>Backward</button></a><br><br>");
  client.println("<a href='/left'><button>Left</button></a>");
  client.println("<a href='/right'><button>Right</button></a><br><br>");
  client.println("<a href='/stop'><button>Stop</button></a><br><br>");

  client.println("</body></html>");
  client.stop();
}
