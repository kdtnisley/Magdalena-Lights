const int hall = 4;
const int led=13;
const int blue=3;
const int red=10;
const int green=9;
const int OFF = 0;
const int ON = 255;
float MAX = 128.0;
const int DIM = MAX/2;

int currentValue;
int previousValue;

unsigned long currentEdge;
unsigned long previousEdge;
float currentSpeed = 0.0;
float maxSpeed = 5.0;
unsigned long previousMillis = 0;
unsigned long wait = 500;
unsigned long currentMillis;

int sensorValue;
int voltage;
float battV;
int battVInt;
int Vrange;
int MaskRed= 0x04;
int MaskGreen= 0x02;
int MaskBlue= 0x01;

float calcVoltage(word sensorValue){
  return (sensorValue * (5.0 / 1023.0))*4.8;
}
void forevererror(){
  Serial.println("Battery voltage is too low.");
  analogWrite(green, OFF);
  analogWrite(red, OFF);
  analogWrite(blue, OFF);
  do{
    digitalWrite(led, HIGH);
    delay(wait/2);
    digitalWrite(led, LOW);
    delay(wait/2);
  }
  while(1);
}

void setup() {
  Serial.begin(9600);
  pinMode(hall, INPUT);
  pinMode(led, OUTPUT);

  digitalWrite(led,LOW);

  pinMode(blue, OUTPUT);
  analogWrite(blue,0);
  pinMode(red, OUTPUT);
  analogWrite(red,0);
  pinMode(green, OUTPUT);
  analogWrite(green,0);

  previousValue = digitalRead(hall);
  previousEdge= millis();
  
  for(int i=1; i <= 10; i++){
    battV = calcVoltage(analogRead(A0));
    battVInt = (int)battV;
    Vrange = battVInt - 10;
    Serial.print("battV is ");
    Serial.print(battV);
    Serial.print(" Vrange is ");
    Serial.println(Vrange);
    
    if(battV < 12.0){
      forevererror();
    }
    if(battV > 17.0){
      analogWrite(red, DIM);
      delay(wait);
      analogWrite(red, OFF);
      delay(wait);
    }
    if(Vrange){
      if(Vrange & MaskRed){
        analogWrite(red, MAX);
      }
      if(Vrange & MaskGreen){
        analogWrite(green, MAX);
      }
      if(Vrange & MaskBlue){
        analogWrite(blue, MAX);
      }
    }
    else{
      analogWrite(red, DIM);
      analogWrite(green, DIM);
      analogWrite(blue, DIM);
    }
    delay(wait/2);
    analogWrite(red, OFF);
    analogWrite(green, OFF);
    analogWrite(blue, OFF);
    delay(wait/2);
  }
}

void loop() {
  currentValue = digitalRead(hall);
  currentMillis = millis();
  //calculate the speed on a leading edge
  if((currentValue == 0) && (previousValue == 1)){
    currentEdge = millis();
    currentSpeed = 1000.0/(currentEdge - previousEdge);
    if(currentSpeed >= 60.0){
      currentSpeed = 60.0;
    }
  //maxspeed shuffle
  if(currentSpeed > maxSpeed){
    maxSpeed = currentSpeed;
  }
    previousEdge = currentEdge;
  }
  //do every half second
  if((currentMillis - previousMillis) > wait){
    battV = calcVoltage(analogRead(A0));
    if(battV < 13.0){
      forevererror();
    }
    //print a bunch of diagnostics
    Serial.print(currentMillis);
    Serial.print(" ms ");
    Serial.print(currentSpeed);
    Serial.print(" 1/s and maxspeed is ");
    Serial.print(maxSpeed);
    Serial.print(" ");
    Serial.print(battV);
    Serial.println(" V");
    previousMillis = currentMillis;
    //if the speed is less than 1 rev/second, turn on green
    if(currentSpeed < 1.0){
      analogWrite(green, MAX);
      analogWrite(red, OFF);
      analogWrite(blue, OFF);
    }
    //if speed is greater than 1 rev/second, do colors
    if(currentSpeed >= 1.0){
      analogWrite(red, (((maxSpeed + 1.0)-currentSpeed)*MAX)/maxSpeed);
      analogWrite(blue, (currentSpeed*MAX)/maxSpeed);
      analogWrite(green, OFF);
    }
  }
  previousValue = currentValue;
}
