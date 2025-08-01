// Transmitter Side (IR LED + DHT11)
#include <DHT.h>

#define DHTPIN 2          // Pin where the DHT11 is connected
#define DHTTYPE DHT11
#define IR_LED_PIN 9      // IR LED connected to digital pin 9

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(IR_LED_PIN, OUTPUT);
}

void loop() {
  float temperature = dht.readTemperature();

  if (!isnan(temperature)) {
    Serial.print("Sending temperature: ");
    Serial.println(temperature);

    // Convert to integer
    int tempInt = (int)temperature;

    // Send temperature over IR as binary pulses
    for (int i = 7; i >= 0; i--) {
      int bitVal = (tempInt >> i) & 1;
      digitalWrite(IR_LED_PIN, bitVal);
      delay(500); // Adjust timing for receiver
    }

    digitalWrite(IR_LED_PIN, LOW);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  delay(5000); // Wait 5 seconds before next reading
}


// Receiver Side (Photodiode connected to digital pin 8)
#define IR_RECEIVER_PIN 8

void setup() {
  Serial.begin(9600);
  pinMode(IR_RECEIVER_PIN, INPUT);
}

void loop() {
  int receivedByte = 0;

  // Read 8 bits
  for (int i = 7; i >= 0; i--) {
    int bitVal = digitalRead(IR_RECEIVER_PIN);
    receivedByte |= (bitVal << i);
    delay(500); // Same timing as transmitter
  }

  Serial.print("Received Temperature: ");
  Serial.println(receivedByte);

  delay(5000);
}
