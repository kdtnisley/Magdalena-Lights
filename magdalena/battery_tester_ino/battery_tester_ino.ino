for(i=1; i <= 10; i++){
  battV = calcVoltage(analogRead(A0));
  vattVInt = (int)battV;
  Vrange = battVInt - 10;
  if((battV < 10.0) || (battV > 17.0)){
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
    //make chinese food
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
