/*
 * Project: Angry Bear
 * 
 * Description: Flash an LED mounted inbetween a teddy bear's eyes when sound is detected
 */

//#define DEBUG
#define LED 12
#define MICROPHONE A0
#define BAUD_RATE 115200
#define AVG_SAMPLES 20          // Average samples to filter out high frequency noise
#define SOUND_THRESHOLD 8       // Values below 8 cause oscillation

#define ledPinSetup() pinMode(LED, OUTPUT);
#define ledOn() digitalWrite(LED, HIGH);
#define ledOff() digitalWrite(LED, LOW);

/************************************************/
const uint32_t led_on_time_ms = 25;
int prev_average = 0;
bool led_on = false;
uint32_t led_off_time = 0;

/************************************************/
void ledFlash();

/************************************************/
void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Startup...");
  #ifdef DEBUG
  Serial.println("Debugging enabled");
  #endif

  ledPinSetup();

  // Flash on startup to check hardware
  ledFlash();
}

void loop() {
  int average = 0;
  int sum = 0;
  
  for (int i = 0; i < AVG_SAMPLES; i++) {
    sum += analogRead(MICROPHONE);
    delay(1);
  }
  
  if (sum > AVG_SAMPLES) {
    average = sum / AVG_SAMPLES;
  }
  
  #ifdef DEBUG
  if (sum != 0) {
    Serial.println(average);
  }
  #endif

  if ((average - prev_average) > SOUND_THRESHOLD) {
    if (!led_on) {
      led_on = true;
      ledOn();
      led_off_time = millis() + led_on_time_ms;
    } else { led_on = false; }
  }

  if (millis() >= led_off_time) { ledOff(); led_on = false; }
  
  delay(1);

  prev_average = average;
}

void ledFlash() {
  ledOn();
  delay(500);
  ledOff();
  delay(500);
}
