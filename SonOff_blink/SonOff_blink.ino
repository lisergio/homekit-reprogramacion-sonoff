
int led = 13; // el pin 13 del led funciona invertido, es decir LOW = encendido y HIGH apagado


void setup() {
  // put your setup code here, to run once:
pinMode(led,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite (led, HIGH);
delay(1000);
digitalWrite (led, LOW);
delay(1000);
}
