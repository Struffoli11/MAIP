// Default settings
unsigned long delayMs = 400; // Default delay in milliseconds (2.5 Hz)
float samplingFrequency = 2.5; // Default frequency in Hz

void setup() {
    Serial.begin(9600); // Initialize serial communication
    //Serial.println("Arduino Ready");
}

void loop() {
    // Check for incoming serial commands
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n'); // Read the command until newline
        processCommand(command); // Process the received command
    }

    // Perform the sensor reading
    float flow = measureFlow(); // Placeholder for sensor measurement function
    Serial.println(flow); // Send flow reading
    //Serial.print(";");
    // Delay based on the current sampling frequency
    delay(delayMs);
}

// Function to process serial commands
void processCommand(String command) {
    command.trim(); // Remove any trailing or leading spaces or newline characters

    // Command to set sampling frequency: "SF:<frequency>"
    if (command.startsWith("SF:")) {
        String freqStr = command.substring(3); // Extract the frequency part
        float newFreq = freqStr.toFloat(); // Convert to float

        if (newFreq > 0) { // Ensure frequency is positive
            samplingFrequency = newFreq; // Update sampling frequency
            delayMs = (unsigned long)(1000.0 / samplingFrequency); // Calculate delay in ms
            //Serial.print("Updated Sampling Frequency to: ");
            //Serial.print(samplingFrequency);
            //Serial.println(" Hz");
        } else {
            //Serial.println("Error: Invalid frequency. Must be > 0.");
        }
    } else {
        //Serial.println("Error: Unrecognized command.");
    }
}

// Placeholder for the flow measurement function
float measureFlow() {
    // Simulate a sensor measurement (replace with actual sensor reading code)
    return random(0, 1000) / 100.0; // Example: random value between 0.00 and 10.00
}
