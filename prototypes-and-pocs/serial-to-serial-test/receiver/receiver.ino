enum states
{
    idle = 0,
    start,
    complete
};

uint8_t currentState;

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
}

void loop()
{
    if (Serial1.available())
    {
        currentState = Serial1.read();

        Serial.print("Current state number: ");
        Serial.println(currentState);

        Serial.print("Current state text: ");
        if (currentState == idle)
        {
            Serial.println("Idle");
        }
        else if (currentState == start)
        {
            Serial.println("Start");
        }
        else if (currentState == complete)
        {
            Serial.println("Complete!!");
        }
    }
    delay(1000);
}