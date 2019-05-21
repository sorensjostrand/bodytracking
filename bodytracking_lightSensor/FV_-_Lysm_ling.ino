
int readAnalog = A0;
int analogValue = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(readAnalog, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  analogValue = analogRead(readAnalog);
  Serial.println(analogValue);
  delay(1000);

}
