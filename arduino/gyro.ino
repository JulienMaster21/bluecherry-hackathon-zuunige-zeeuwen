#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>

/* IC2 bus parameters for the ESP32 Azure IoT kit */
#define I2C_SDA_PIN 25
#define I2C_SCL_PIN 26
#define IC2_SPEED 400000

/* Parameters for the OLED screen */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_RESET -1
#define SCREEN_ADDRESS 0x3C

/* Parameters for the WiFi connection */
#define WIFI_SSID "Schuur De Vier Ambachten"
#define WIFI_PASSWORD "gratiswifi"

/* Parameters for the MQTT connection */
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "arduino"
/*#define MQTT_TOPIC_PREFIX "/public"*/

/* Class instantiations */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
Adafruit_MPU6050 mpu;

struct sensor_data
{
  float acceleration_x;
  float acceleration_y;
  float acceleration_z;

  float gyro_x;
  float gyro_y;
  float gyro_z;

  float temperature;
};

/**
 * Initialize the hardware
 */
void setup()
{
  /* Initialize the serial console port */
  Serial.begin(115200);
  Serial.println("BlueCherry hackathon display example");

  /* Initialize the I2C bus on the board */
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(400000);

  setup_display();
  setup_wifi();
  initMPU();
  setup_mqtt();
}

void initMPU()
{
  if (!mpu.begin())
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
    {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
}

/**
 * Construct the display header text line
 */
void display_set_header()
{
  display.cp437(true);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("De Zuunige Zeeuwen");
  display.drawLine(0, 8, 128, 8, SSD1306_WHITE);
}

/**
 * Initialize the display and print program name
 */
void setup_display()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS, true, false))
  {
    Serial.println("Screen initialization failed");
    while (true)
      ;
  }
  Serial.println("Display initialized");

  /* Display header */
  display.clearDisplay();
  display_set_header();
  display.display();
}

void setup_wifi()
{
  Serial.println("Connecting to WiFi");
  display.clearDisplay();
  display_set_header();
  display.setCursor(0, 16);
  display.println("Connecting WiFi");
  display.display();

  display.clearDisplay();
  display_set_header();
  display.setCursor(0, 16);
  display.display();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    display.clearDisplay();
    display_set_header();
    display.setCursor(0, 16);
    display.println("Connecting WiFi.");
    display.display();
    delay(250);
    display.clearDisplay();
    display_set_header();
    display.setCursor(0, 16);
    display.println("Connecting WiFi..");
    display.display();
    delay(250);
    display.clearDisplay();
    display_set_header();
    display.setCursor(0, 16);
    display.println("Connecting WiFi...");
    display.display();
    delay(250);
    display.clearDisplay();
    display_set_header();
    display.setCursor(0, 16);
    display.println("Connecting WiFi");
    display.display();
  }

  display.clearDisplay();
  display_set_header();
  display.setCursor(0, 16);
  display.print("IP: ");
  display.println(WiFi.localIP());
  display.display();

  Serial.println("Connection success");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

/**
 * Called when an MQTT message was received.
 */
void mqtt_callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  display.clearDisplay();
  display_set_header();
  display.setCursor(0, 16);

  uint16_t xPos = 6;
  for (int i = 0; i < length; i++)
  {
    display.print((char)message[i]);
    display.setCursor(xPos, 16);
    xPos += 6;
  }
  display.display();
}

/**
 * Setup the MQTT client
 */
void setup_mqtt()
{
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqtt_callback);

  while (!mqttClient.connected())
  {
    Serial.print("Connecting to MQTT server");
    if (mqttClient.connect(MQTT_CLIENT_ID))
    {
      Serial.println("MQTT connected");
      mqttClient.publish("zuunige zeeuwen", MQTT_CLIENT_ID " is online\n");
      mqttClient.subscribe("zuunige zeeuwen");
    }
    else
    {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

sensor_data getSensorData()
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  return sensor_data{
      a.acceleration.x,
      a.acceleration.y,
      a.acceleration.z,

      g.gyro.y,
      g.gyro.x,
      g.gyro.z,

      temp.temperature};
};

float xmovement;
void displayXYZRotation()
{
  sensor_data sensorData = getSensorData();
  //Lumos spell
  if (sensorData.gyro_x > 1.5 && !(sensorData.gyro_z > 1))
  {
    xmovement = sensorData.gyro_x;
    display.clearDisplay();
    display_set_header();
            display.setCursor(0, 16);
        display.print("Lumos 1");
        display.display();
    delay(400);
    sensor_data sensorData = getSensorData();
    if (sensorData.gyro_x < xmovement - 3 && !(sensorData.gyro_x < 0.1 && sensorData.gyro_x > -0.1))
    {
      xmovement = sensorData.gyro_x;
      display.clearDisplay();
      display_set_header();
              display.setCursor(0, 16);
        display.print("Lumos 2");
        display.display();
      delay(300);
      sensor_data sensorData = getSensorData();
      if (sensorData.gyro_x > 0.4)
      {
        display.clearDisplay();
        display_set_header();
        display.setCursor(0, 16);
        if (mqttClient.connected()) {
          mqttClient.publish("zuunige zeeuwen", "lumos");
          display.print("Lumos");
        }
        else if (mqttClient.connect(MQTT_CLIENT_ID)) {
            mqttClient.publish("zuunige zeeuwen", "lumos");
            display.print("Lumos");
        }
        else {
          display.print("Lumos Failed");
    }
    display.display();
        delay(1000);
  }
      }
    }
  //nox spell
  else if (sensorData.gyro_x < -1.5)
  {
    xmovement = sensorData.gyro_x;
    display.clearDisplay();
    display_set_header();
    display.setCursor(0, 16);
        display.print("Nox 1");
        display.display();
    delay(400);
    sensor_data sensorData = getSensorData();
    if (sensorData.gyro_x > xmovement + 3 && !(sensorData.gyro_x < 0.1 && sensorData.gyro_x > -0.1))
    {
      xmovement = sensorData.gyro_x;
      display.clearDisplay();
      display_set_header();
      display.setCursor(0, 16);
        display.print("Nox 2");
        display.display();
      delay(300);
      sensor_data sensorData = getSensorData();
      if (sensorData.gyro_x < xmovement - 3 && sensorData.gyro_x < -0.4)
      {
        display.clearDisplay();
        display_set_header();
        display.setCursor(0, 16);
        if (mqttClient.connected()) {
          mqttClient.publish("zuunige zeeuwen", "nox");
          display.print("Nox");
        }
        else if (mqttClient.connect(MQTT_CLIENT_ID)) {
            mqttClient.publish("zuunige zeeuwen", "nox");
            display.print("Nox");
        }
        else {
          display.print("Nox Failed");
    }
    display.display();
        delay(1000);
  }
      }
    }
    //leviosa spell
  else if (sensorData.gyro_z > 1.5) {
    display.clearDisplay();
    display_set_header();
    display.setCursor(0, 16);
    display.print("Leviosa 1");
    mqttClient.publish("zuunige zeeuwen", "leviosa1");
    display.display();
    delay(300);
    sensor_data sensorData = getSensorData();
    if (sensorData.acceleration_x < -1.5 || sensorData.gyro_x < -1.5) {
      display.clearDisplay();
      display_set_header();
      display.setCursor(0, 16);
      display.print("Leviosa 2");
      mqttClient.publish("zuunige zeeuwen", "leviosa2");
      display.display();
      delay(600);
      sensor_data sensorData = getSensorData();
      if ((sensorData.gyro_z + sensorData.gyro_y) < -1) {
        display.clearDisplay();
          display_set_header();
          display.setCursor(0, 16);
          if (mqttClient.connected()) {
            mqttClient.publish("zuunige zeeuwen", "leviosa");
            display.print("Leviosa");
          }
          else if (mqttClient.connect(MQTT_CLIENT_ID)) {
            mqttClient.publish("zuunige zeeuwen", "leviosa");
            display.print("Leviosa");
          }
          else {
            display.print("Nox Failed");
          }
          display.display();
          delay(1000);
        }
    }
  }
  else
  {
    display.clearDisplay();
    display_set_header();
    display.setCursor(0, 16);
    display.print("Rotation X: ");
    display.println(sensorData.gyro_x);
    display.print("Rotation Y: ");
    display.println(sensorData.gyro_y);
    display.print("Rotation Z: ");
    display.println(sensorData.gyro_z);
    display.display();
  }
}

void loop()
{
  displayXYZRotation();
}