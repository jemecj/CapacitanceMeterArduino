#define IN_PIN A0
#define OUT_PIN A2
#define BAUD_RATE 115200
#define IN_CAP_TO_GND 24.48
#define R_PULLUP 34.8
#define MAX_ADC_VALUE 1023
#define CALIBRATION 0.46

void setup()
{
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  Serial.begin(BAUD_RATE);
}

void loop()
{
  pinMode(IN_PIN, INPUT);
  digitalWrite(OUT_PIN, HIGH);
  int val = analogRead(IN_PIN);
  digitalWrite(OUT_PIN, LOW);

  if (val < 1000)
  {
    pinMode(IN_PIN, OUTPUT);
    float capacitance = abs((float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val) - CALIBRATION);
    Serial.print(capacitance, 2);
    Serial.println("pF");
    delay(500);
  }

  else
  {
    pinMode(IN_PIN, OUTPUT);
    delay(1);
    pinMode(OUT_PIN, INPUT_PULLUP);
    unsigned long u1 = micros();
    unsigned long t;
    int digVal;

    do
    {
      digVal = digitalRead(OUT_PIN);
      unsigned long u2 = micros();
      t = u2 > u1 ? u2 - u1 : u1 - u2;
    }

    while ((digVal < 1) && (t < 400000L));

    pinMode(OUT_PIN, INPUT);
    val = analogRead(OUT_PIN);
    digitalWrite(IN_PIN, HIGH);
    int dischargeTime = (int)(t / 1000L) * 5;
    delay(dischargeTime);
    pinMode(OUT_PIN, OUTPUT);
    digitalWrite(OUT_PIN, LOW);
    digitalWrite(IN_PIN, LOW);

    float capacitance = -(float)t / R_PULLUP / log(1.0 - (float)val / (float)MAX_ADC_VALUE) - 0.46;


    if (capacitance > 1000.0)
    {
      Serial.print(capacitance / 1000.0, 2);
      Serial.println("uF ");
    }

    else
    {
      Serial.print(capacitance);
      Serial.print("nF");
      delay(500);
    }
  }
  while (micros() % 1000 != 0);
}
