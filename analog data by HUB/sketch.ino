#define pot 14
int pot_value;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pot, INPUT);
}
int filterdata()
{
  int sum = 0;
  for(int i=0;i<5;i++)
  {
    sum += analogRead(14);
    
  }
  return sum/5;
}

void loop() {
  pot_value =  filterdata();
  Serial.println(pot_value);

  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
}
