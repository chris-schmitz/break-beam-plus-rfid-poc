enum states
{
    idle = 0,
    start,
    complete
};

// char state[10] = "hello";
uint8_t currentState = idle;

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
}

void loop()
{
    if (currentState == idle)
    {
        currentState = start;
    }
    else if (currentState == start)
    {
        currentState = complete;
    }
    else if (currentState == complete)
    {
        currentState = idle;
    }
    Serial.print("Seinding state: ");
    Serial.println(currentState);
    Serial1.write(currentState);
    delay(1000);
}