
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "your router ssid";
const char *password = "your router password";
const char *mqtt_server = "your device static IP";

WiFiClient espClient12 ;
PubSubClient client(espClient12) ;

void setup_wifi() {
  delay(10) ;
  // we start by connecting to a wifi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messagein;
  for ( int i = 0 ; i < length ; i ++) {
    Serial.print((char)message[i]);
    messagein += (char)messagein[i];
  }

  if(topic == "room/lamp") {
    if (messagein == "ON")
      digitalWrite(5 , HIGH);
      client.publish("room/r" , "roshan");
  }else{
    digitalWrite(5 , LOW );
    client.publish("room/r" , "khamush");
  }

}

void reconnect() {
  while(!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if(client.connect("ESP_8266")) {
      Serial.println("connected");
      client.subscribe("room/lamp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);


}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.loop())
    client.connect("ESP_8266");
  delay(100);
  
}
