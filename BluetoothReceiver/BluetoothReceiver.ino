#define CONTROL_PIN
#define RELAY1_PIN 22
#define RELAY2_PIN 23
#define RELAY3_PIN 24
#define RELAY4_PIN 25
#define RELAY5_PIN 26
#define RELAY6_PIN 27
#define RELAY7_PIN 28
#define RELAY8_PIN 29
#define RELAY9_PIN 30
#define RELAY10_PIN 31
#define RELAY11_PIN 32
#define RELAY12_PIN 33

#define DELAY 100

bool TRIGGERED = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(13,OUTPUT);
    // put your setup code here, to run once:
  pinMode(RELAY1_PIN,OUTPUT);
    pinMode(RELAY2_PIN,OUTPUT);
    pinMode(RELAY3_PIN,OUTPUT);
    pinMode(RELAY4_PIN,OUTPUT);
    pinMode(RELAY5_PIN,OUTPUT);
    pinMode(RELAY6_PIN,OUTPUT);
    pinMode(RELAY7_PIN,OUTPUT);
    pinMode(RELAY8_PIN,OUTPUT);
    pinMode(RELAY9_PIN,OUTPUT);
    pinMode(RELAY10_PIN,OUTPUT);
    pinMode(RELAY11_PIN,OUTPUT);
    pinMode(RELAY12_PIN,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    if(Serial.read() == '9')
    {
      TRIGGERED = true;
    }
  }

  if(TRIGGERED)
  {
      // put your main code here, to run repeatedly:
    digitalWrite(RELAY1_PIN,HIGH);
    delay(DELAY);
    digitalWrite(RELAY1_PIN,LOW);
    delay(DELAY);
    
    digitalWrite(RELAY2_PIN,HIGH);
    delay(DELAY);
    digitalWrite(RELAY2_PIN,LOW);
    delay(DELAY);
    
    digitalWrite(RELAY3_PIN,HIGH);
    delay(DELAY);
    digitalWrite(RELAY3_PIN,LOW);
    delay(DELAY);
    
    digitalWrite(RELAY4_PIN,HIGH);
    delay(DELAY);
    digitalWrite(RELAY4_PIN,LOW);
    delay(DELAY);
    
    digitalWrite(RELAY5_PIN,HIGH);
    delay(DELAY);
    digitalWrite(RELAY5_PIN,LOW);
    delay(DELAY);
    
    digitalWrite(RELAY6_PIN,HIGH);
    delay(DELAY);
    digitalWrite(RELAY6_PIN,LOW);
    delay(DELAY);
    
    digitalWrite(RELAY7_PIN,HIGH);
    delay(DELAY);
    digitalWrite(RELAY7_PIN,LOW);
    delay(DELAY);
    
    digitalWrite(RELAY8_PIN,HIGH);
    delay(DELAY);
    digitalWrite(RELAY8_PIN,LOW);
    delay(DELAY);
    
    digitalWrite(RELAY9_PIN,HIGH);
    delay(DELAY);
    digitalWrite(RELAY9_PIN,LOW);
    delay(DELAY);
    
    digitalWrite(RELAY10_PIN,HIGH);
    delay(DELAY);
    digitalWrite(RELAY10_PIN,LOW);
    delay(DELAY);
    
    digitalWrite(RELAY11_PIN,HIGH);
    delay(DELAY);
    digitalWrite(RELAY11_PIN,LOW);
    delay(DELAY);
    
    digitalWrite(RELAY12_PIN,HIGH);
    delay(DELAY);
    digitalWrite(RELAY12_PIN,LOW);
    delay(DELAY);
  
    TRIGGERED = false;
  }
}
