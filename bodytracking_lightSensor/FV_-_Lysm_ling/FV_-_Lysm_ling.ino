
int readSensor1 = A1;
int readSensor2 = A2;
int sensor1Value = 0;
int sensor2Value = 0;
int lumen1 = 0;
int lumen2 = 0;
String stringSensor1 = "Sensor 1:";
String stringSensor2 = "Sensor 2:";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(readSensor1, INPUT);
  pinMode(readSensor2, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  sensor1Value = analogRead(readSensor1);
  sensor2Value = analogRead(readSensor2);
  
  delay(1000);
  lumen1 = map(sensor1Value, 0, 1023, 0, 110);
  lumen2 = map(sensor2Value, 0, 1023, 0, 110);
 
 

delay(1);
Serial.println(stringSensor1 + lumen1);
delay(1);
//Serial.println(stringSensor2 + lumen2);


}
