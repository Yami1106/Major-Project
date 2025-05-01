#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

// ---------------- WiFi Credentials ----------------
const char* ssid = "S23";
const char* password = "viropo2310";

// ---------------- Motor Pins ----------------
#define ENA 14
#define ENB 12
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33

#define MOTOR_SPEED 255

// ---------------- Sensors ----------------
#define TRIG_FRONT 5
#define ECHO_FRONT 18
#define TRIG_BACK 19
#define ECHO_BACK 21

#define IR_FL 15
#define IR_FR 35
#define IR_BL 16
#define IR_BR 17

bool manualOverride = false;
unsigned long lastManualCommandTime = 0;
const unsigned long overrideTimeout = 60000; // 1min

AsyncWebServer server(8080);  // Running server on port 8080

// ---------------- Motor Functions ----------------
void setMotors(int leftSpeed, int rightSpeed) {
  analogWrite(ENA, constrain(leftSpeed, 0, 255));
  analogWrite(ENB, constrain(rightSpeed, 0, 255));
}

void moveForward() {
  printf("Forward");
  // digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  // digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  // setMotors(MOTOR_SPEED, MOTOR_SPEED);
}

void moveBackward() {
  printf("Backward");
  // digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  // digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  // setMotors(MOTOR_SPEED, MOTOR_SPEED);
}

void turnLeft() {
  printf("Left");
  // digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  // digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  // setMotors(MOTOR_SPEED, 0);
}

void turnRight() {
  printf("Right");
  // digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  // digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  // setMotors(0, MOTOR_SPEED);
}

void stopRobot() {
  printf("Stop");
  // digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  // digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  // setMotors(0, 0);
}

// ---------------- Sensor Functions ----------------
float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

// ---------------- Command Parser ----------------
void handleManualCommand(String cmd) {
  manualOverride = true;
  lastManualCommandTime = millis();

  cmd.trim();
  cmd.toLowerCase();

  if (cmd == "forward") {
    moveForward();
  } else if (cmd == "backward") {
    moveBackward();
  } else if (cmd == "left") {
    turnLeft();
  } else if (cmd == "right") {
    turnRight();
  } else if (cmd == "stop") {
    stopRobot();
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_BACK, OUTPUT);
  pinMode(ECHO_BACK, INPUT);

  pinMode(IR_FL, INPUT);
  pinMode(IR_FR, INPUT);
  pinMode(IR_BL, INPUT);
  pinMode(IR_BR, INPUT);

  stopRobot();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/control", HTTP_POST, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Command Received");

  }, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, data);

    if (!error) {
      String direction = doc["direction"];
      Serial.println("Received Command: " + direction);
      handleManualCommand(direction);
    }
  });

  server.begin();
}

void loop() {
  if (manualOverride && (millis() - lastManualCommandTime > overrideTimeout)) {
    manualOverride = false;
    Serial.println("Manual override timeout. Resuming autonomous mode.");
    stopRobot();
  }

  if (!manualOverride) {
    float frontDist = getDistance(TRIG_FRONT, ECHO_FRONT);
    float backDist = getDistance(TRIG_BACK, ECHO_BACK);

    int irFrontLeft = digitalRead(IR_FL);
    int irFrontRight = digitalRead(IR_FR);
    int irBackLeft = digitalRead(IR_BL);
    int irBackRight = digitalRead(IR_BR);

    Serial.print("Front: "); Serial.print(frontDist);
    Serial.print(" | Back: "); Serial.println(backDist);

    if (frontDist < 15 || irFrontLeft == LOW || irFrontRight == LOW) {
      stopRobot();
      delay(300);

      if (irFrontLeft == LOW) {
        Serial.println("Turning Right...");
        turnRight();
      } else {
        Serial.println("Turning Left...");
        turnLeft();
      }
      delay(700);
      stopRobot();
    } else if (backDist < 15 || irBackLeft == LOW || irBackRight == LOW) {
      Serial.println("Obstacle Behind! Moving Forward...");
      moveForward();
    } else {
      Serial.println("Path Clear. Moving Forward...");
      moveForward();
    }
  }

  delay(100);
}