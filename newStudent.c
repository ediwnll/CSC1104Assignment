/*
=== HOW TO RUN ===
Step 1: cd into C file location
Step 2: gcc -o newStudent newStudent.c -lwiringPi
Step 3: ./student

=== PRE-REQUISITES ===
Install wiringPi: https://learn.sparkfun.com/tutorials/raspberry-gpio/c-wiringpi-setup
softPwm is installed with wiringPi

=== USEFUL COMMANDS ===
Check wiringPi version: gpio -v
Check GPIO status: gpio readall

=== GPIO PIN CONNECTION ===
27 GREEN LED
13 RED LED
GROUND

GPIO14 to Monitor GPIO15
GPIO15 to Monitor GPIO14
GROUND

=== RASPBERRY PI VERSION ===
Check Version Command: cat /etc/os-release

PRETTY_NAME="Raspbian GNU/Linux 10 (buster)"
NAME="Raspbian GNU/Linux"
VERSION_ID="10"
VERSION="10 (buster)"
VERSION_CODENAME=buster
*/

#include <wiringPi.h>
#include <softPwm.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringSerial.h>
#include <unistd.h>
#include <time.h> /* to set up a timer for 60 seconds*/

/* DEFINITIONS */
#define RED 27   // GPIO Pin 27
#define GREEN 13 // GPIO Pin 13

// Program States
#define TURN_OFF 0
#define TURN_ON 1
#define BLINK 2
#define EXIT 3

// LED Blink Selection
#define BLINK_GREEN 1
#define BLINK_RED 2
#define CONFIRM 1

// MONITORING
// #define STUDENTID "2101234" // the student ID is not needed in the group project of 2023

/* FUNCTION PROTOTYPES */
void setupProgram();
void startProgram();
int getUserSelection();
void turnOffLeds();
void turnOnLeds();
void blink();
int getBlinkLed();
int getBlinkFrequency();
int getBlinkBrightness();
int confirmBlinkSelection(int,int,int,float);
float getBlinkDutyCycle();
int connectToMonitorDevice();
void blinkLedWithConfig();
void writeDataIntoCSV();
void createArrayInData(int,int,int,float);
int checkFileExist(const char *fileName);
void endProgram();

/* This creates a structure(object) to store in value with frequency and state*/
struct CSV{
    int frequency;
    float dutyCycle;
    int state;
};

/* MAIN PROGRAM */
int main(void)
{

    setupProgram();
    startProgram();
    endProgram();
    return 0;
}

/*
Sets up the LED GPIO pins as output and PWM
*/
void setupProgram()
{
    wiringPiSetupGpio();
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    softPwmCreate(GREEN, 0, 100);
    softPwmCreate(RED, 0, 100);
    system("clear");
}

/*
Takes the input of the user selection and direct it to different states of the program
*/
void startProgram()
{

    int selection;

    do
    {
        selection = getUserSelection();

        switch (selection)
        {

        case TURN_OFF:
            turnOffLeds();
            break;
        case TURN_ON:
            turnOnLeds();
            break;
        case BLINK:
            blink();
            break;
        case EXIT:
            break;
        default:
            printf("\nInvalid Input. Try Again...\n");
            break;
        }

    } while (selection != EXIT);

    return;
}

/*
The main menu that determines what the user wants to do
*/
int getUserSelection()
{

    int selection;

    printf("\n===== LAD STUDENT DEVICE =====\n");
    printf("\n[0] Turn off both LEDs\n");
    printf("[1] Turn on both LEDs\n");
    printf("[2] Blink LED\n");
    printf("[3] Exit\n");
    printf("\nYour Selection: ");

    scanf("%d", &selection);

    if (selection < 0 || selection > 3 ){
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        getUserSelection();
    }
    else {
        system("clear");
        return selection;
    }
}

/*
For troubleshooting, turning off LEDs and PWM
*/
void turnOffLeds()
{
    system("clear");
    printf("\nTurning off both LEDs...\n");
    digitalWrite(GREEN, LOW);
    softPwmWrite(GREEN, 0);
    digitalWrite(RED, LOW);
    softPwmWrite(RED, 0);
}

/*
For troubleshooting, turning on LEDs and PWM at 100
*/
void turnOnLeds()
{
    system("clear");
    printf("\nTurning on both LEDs...\n");
    digitalWrite(GREEN, HIGH);
    softPwmWrite(GREEN, 100);
    digitalWrite(RED, HIGH);
    softPwmWrite(RED, 100);
}

/*
When user wants to blink LED, this function will get all the blinking configuration
It gets from the user the LED to blink, frequency and brightness.
Then, it will call a function to attempt handshake with Monitor before executing the blink
*/
void blink()
{
    system("clear");
    printf("\nBlink...\n");

    int blinkLed = getBlinkLed();
    int frequency = getBlinkFrequency();
    int brightness = getBlinkBrightness();
    float dutyCycle = getBlinkDutyCycle();

    if (confirmBlinkSelection(blinkLed, frequency, brightness,dutyCycle) == CONFIRM)
    {
        //     if (connectToMonitorDevice(blinkLed, frequency, brightness) < 0)
        //     {
        //         printf("Connection failed, please make sure monitor device is ready.\n");
        //     }
        //     else
        //     {
        createArrayInData(blinkLed,frequency,brightness,dutyCycle);
        //blinkLedWithConfig(blinkLed, frequency, brightness);
        system("clear");
        //     }
    }
    else
        return;
}

/*
Menu to get user selction on LED to blink
*/
int getBlinkLed()
{

    int selection;

    printf("\nSelect LED to blink.\n\n");
    printf("[1] Green LED\n");
    printf("[2] Red LED\n");
    printf("\nYour Selection: ");

    scanf("%d", &selection);

    if (selection != BLINK_GREEN && selection != BLINK_RED)
    {
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        getBlinkLed();
    }
    else
    {
        system("clear");
        return selection;
    }
}


/*
Menu to get user selction on Frequency to blink
*/
int getBlinkFrequency()
{

    int selection;

    printf("Enter frequency to blink.\n\n");
    printf("Enter whole numbers between 0 to 10\n");
    printf("\nFrequency (Hz): ");

    scanf("%d", &selection);

    if (selection < 0 || selection > 10)
    {
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        getBlinkFrequency();
    }
    else
    {
        system("clear");
        return selection;
    }
}

/*
Menu to get user selection on LED brightness
*/
int getBlinkBrightness()
{

    int selection;

    printf("Select LED brightness during blink.\n\n");
    printf("Enter whole numbers between 0 to 100\n");
    printf("Brightness (%%): ");

    scanf("%d", &selection);

    if (selection < 0 || selection > 100)
    {
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        getBlinkBrightness();
    }
    else
    {
        system("clear");
        return selection;
    }
}


float getBlinkDutyCycle(){

    float selection;

    printf("Select LED brightness during blink.\n\n");
    printf("Enter any numbers between 0 to 100\n");
    printf("Duty Cycle (%%): ");

    scanf("%f", &selection);

    if (selection < 0 || selection > 100)
    {
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        getBlinkDutyCycle();
    }
    else
    {
        system("clear");
        return selection;
    }

}


/*
Menu for user to acknowledge the blink configurations input
*/
int confirmBlinkSelection(int blinkLed, int blinkFrequency, int blinkBrightness,float dutyCycle)
{

    int selection;
    char blinkLedString[] = "Green";

    if (blinkLed == BLINK_RED)
    {
        strcpy(blinkLedString, "Red");
    }

    printf("Confirm your blink configurations.\n\n");
    printf("LED to blink: %s\n", blinkLedString);
    printf("Blink Frequency: %dHz\n", blinkFrequency);
    printf("Blink Brightness: %d%%\n", blinkBrightness);
    printf("Blink Brightness: %.2f%%\n\n", dutyCycle);
    printf("[1] Confirm Configuration\n");
    printf("[0] Return to Home\n");
    printf("\nYour Selection: ");

    scanf("%d", &selection);

    if (selection < 0 || selection > 1)
    {
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        confirmBlinkSelection(blinkLed, blinkFrequency, blinkBrightness,dutyCycle);
    }
    else
    {
        return selection;
    }
}

/*
Handshake algorithm to connect and send blink configurations
*/
/*
int connectToMonitorDevice(int blinkLed, int blinkFrequency, int blinkBrightness)
{
    system("clear");
    printf("Connecting to Monitor Device...\n");

    int serial_port;
    char studentid[] = STUDENTID;

    char blinkLedString[1];
    char blinkFrequencyString[2];
    char blinkBrightnessString[3];

    sprintf(blinkLedString, "%d", blinkLed);
    sprintf(blinkFrequencyString, "%d", blinkFrequency);
    sprintf(blinkBrightnessString, "%d", blinkBrightness);

    // Open the serial port
    serial_port = serialOpen("/dev/ttyAMA0", 9600);
    if (serial_port < 0)
    {
        fprintf(stderr, "Error opening serial port\n");
        return -1;
    }

    // Write data to the serial port
    serialPuts(serial_port, studentid);

    // Read data from the serial port
    char buffer[256];
    int n = read(serial_port, buffer, sizeof(buffer));
    buffer[n] = '\0';

    if (strlen(buffer) == 7)
    {
        printf("Acknowledge Student ID: %s\n", buffer);
    }
    else
        return -1;

    // Send Blink Configuration
    serialPuts(serial_port, blinkLedString);
    n = read(serial_port, buffer, sizeof(buffer));
    buffer[n] = '\0';
    printf("%s\n", buffer);

    serialPuts(serial_port, blinkFrequencyString);
    n = read(serial_port, buffer, sizeof(buffer));
    buffer[n] = '\0';
    printf("%s\n", buffer);

    serialPuts(serial_port, blinkBrightnessString);
    n = read(serial_port, buffer, sizeof(buffer));
    buffer[n] = '\0';
    printf("%s\n", buffer);

    serialClose(serial_port);
    printf("Connection Successful!\n");
    delay(5000);
    return 1;
}
*/
/*
Blinks the LED according to the user configuration
*/
/*void blinkLedWithConfig(int blinkLed, int blinkFrequency, int blinkBrightness)
{

    printf("\nBlinking...\n");

    // Setting Frequency
 //   float onOffTime = 1.0f / blinkFrequency * 1000;

    // Setting Blink LED
   /* if (blinkLed == BLINK_GREEN)
    {
        blinkLed = GREEN;
    }
    else
        blinkLed = RED;*/

    
    /*Creates new Array for blinking LED*/
    

    // Blinking
   /* unsigned long previousMillis = 0;
    int ledState = LOW;*/

    /*Edit this part to change to 1 minute instead of blinking values */
    /*for (int blink = 0; blink < 20;)
    {
        unsigned long currentMillis = millis();

        if (currentMillis - previousMillis >= onOffTime)
        {
            previousMillis = currentMillis;
            if (ledState == LOW)
            {
                ledState = HIGH;
                softPwmWrite(blinkLed, blinkBrightness);
            }
            else
            {
                ledState = LOW;
                softPwmWrite(blinkLed, 0);
            }
            blink++;
            digitalWrite(blinkLed, ledState);
        }
    }
}*/

/*
This helps to create an function for the user to store data into the csv 
*/
void createArrayInData(int blinkLed,int blinkFrequency,int blinkBrightness,float dutyCycle){
    printf("\nBlinking...\n");
    /* Formulas and state to be set into the data*/
    int period = (1.0f / blinkFrequency * 1000000) * (dutyCycle/100);
    int ledState = LOW;
    int color = blinkLed == BLINK_GREEN ? GREEN : RED;
    /*Intialized object and allocate the size accordingly*/
    struct CSV* data;
    data = malloc(600000 * sizeof(struct CSV));

    if (data == NULL){
        fprintf(stderr, "Memory allocation failed");
        return;
    }
    /* Intializes the Microsecond counter to process data accordingly*/
    unsigned long currentMicros = micros();
    unsigned long previousMicros = 0;
    unsigned testData = currentMicros + (5000000);
    unsigned long minuteMicros = currentMicros + (60000000);
    unsigned long nextRecord = currentMicros;
    int iterations = 0;

    do  {   
        currentMicros = micros();
        /* If the current microsecond minus previous microsecond more than equal to period then trigger*/
        if (currentMicros - previousMicros >= period ){
            previousMicros = currentMicros;
            ledState = ledState == LOW ?  HIGH : LOW;
            int brightness = ledState == LOW ? blinkBrightness : 0;
            
            softPwmWrite(color, brightness);
            digitalWrite(color, ledState);
        }

        /* This makes the record stores in every 100microseconds (test it out)*/
        if (currentMicros >= nextRecord ){
            
            data[iterations].frequency = blinkFrequency;
            data[iterations].dutyCycle = dutyCycle;
            data[iterations].state = digitalRead(color); 
            iterations ++;
            nextRecord = currentMicros + (100);
        }
    }
    while ( currentMicros < testData );

    /*ensures that the current color will be off after looping and write data into csv and make sure the memory allocation is freed after use*/
    softPwmWrite(color,0);
    writeDataIntoCSV(data,iterations,blinkLed);
    free(data);
}
/*
This function creates the CSV file and writes into it.
*/
 void writeDataIntoCSV(struct CSV *data,int sizeArr,int blinkLed){  
    /* Init array*/
    static struct CSV redLedArray[600000];
    static struct CSV greenLedArray[600000];

    /* Looping through to store the data into array*/
    if (blinkLed == BLINK_GREEN){
         for (int i = 0; i < sizeArr; i ++){
            greenLedArray[i] = data[i];
        }
    }
    else{
        for (int i = 0; i < sizeArr; i ++){
            redLedArray[i] = data[i];
        }
    }
    /* This will trigger the user to key in the value accordingly*/
    if (greenLedArray[0].frequency == 0  || redLedArray[0].frequency == 0 ){
        blink();
    }
    
    /*Creates a csv if the csv doesnt exists*/
    if (checkFileExist("displayPlot.csv") == 0) {   
        FILE *CSV = fopen("displayPlot.csv","wb"); 
        fprintf(CSV,"Green Frequency,Green Duty Cycle,Green State,Red Frequency, Red Duty Cycle,Red State");  //Creating Header for the file
        
        /* Loop through array for this part to store inside CSV*/
        for (int i = 0; i < sizeArr; i++){
            fprintf(CSV,
                "\n%d,%.2lf,%d,%d,%.2lf,%d",
                greenLedArray[i].frequency,greenLedArray[i].dutyCycle,greenLedArray[i].state,
                redLedArray[i].frequency,redLedArray[i].dutyCycle,redLedArray[i].state);
        }

        printf("CSV displayPlot has been created");
        fclose(CSV);
    }
}

/*
This helps to check whether the file exists
*/
int checkFileExist(const char *fileName){
    FILE *file;

    if ( file = fopen(fileName,"rb")  ) { 
        fclose(file);
        return 1;
    }
    return 0;
}

/*
Resetting and cleaning up before safely exiting the program.
*/
void endProgram()
{
    system("clear");
    printf("\nCleaning Up...\n");
    // Turn Off LEDs
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, LOW);

    // Turn Off LED Software PWM
    softPwmWrite(GREEN, 0);
    softPwmWrite(RED, 0);

    // Reset Pins to Original INPUT State
    pinMode(GREEN, INPUT);
    pinMode(RED, INPUT);

    printf("Bye!\n\n");
}
