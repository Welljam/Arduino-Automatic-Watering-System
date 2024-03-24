const int fuktighetsPin = A0;  // Sensor Pin
const int relayPin = 7;          // Relay Pin
const int bevattningsTid = 3000;
const long milliSekunderPerDag = 86400000;
const long antalKontrollerPerDag = 2;
const long intervall = milliSekunderPerDag / antalKontrollerPerDag;
const long ledPin = 13;
int troskelgrans = 88;

unsigned long startMillis;
unsigned long nuvarandeMillis;
unsigned long bevattningStartMillis;

void setup() {
  Serial.begin(9600);
  pinMode(fuktighetsPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(relayPin, HIGH);
  startMillis = millis();
}

void loop() {
  int fuktighet = analogRead(fuktighetsPin);
  int sensorData = map(fuktighet, 230, 1023, 0, 100);

  Serial.println("Sensor_data: ");
  Serial.print(sensorData);
  Serial.print("\t | ");

  nuvarandeMillis = millis();

  if(sensorData >= troskelgrans){
    Serial.println("Låg fuktighetsvärde");
    digitalWrite(relayPin, LOW);
    Serial.println("Pumpen är igång");
    Serial.println(" ");

    bevattningStartMillis = nuvarandeMillis;
    while(millis() - bevattningStartMillis <= bevattningsTid){}
    
    digitalWrite(relayPin, HIGH);
    Serial.println("Pumpen är avstängd");

    while(millis() - startMillis <= intervall){}
    startMillis = nuvarandeMillis;
  }
  else if(sensorData <= troskelgrans){
    digitalWrite(relayPin, HIGH);
    Serial.println("Pumpen är avstängd");
    while(millis() - startMillis <= intervall){}
    startMillis = nuvarandeMillis;
  }
  
  if (digitalRead(relayPin) == LOW && nuvarandeMillis - startMillis >= 5000) {
    BevattningSafety();
    startMillis = nuvarandeMillis;
  }
}

void BevattningSafety(){
  digitalWrite(relayPin, HIGH);
  digitalWrite(ledPin, HIGH);  
  Serial.println("Säkerhetsåtgärd: Pumpen har varit på för länge. Programmet avstängt.");
  while (1);
}