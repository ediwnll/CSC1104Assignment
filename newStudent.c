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

// Defining Millisecond
#define TO_MILLIS 1000

// Define Confirmation Status for how many led to blink
#define oneLed 45
#define twoLed 30

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
float getBlinkBrightness();
int confirmLedToShine();
int confirmBlinkSelection(int, int, float, int);
void recordWaveDataIntoMemory(int, int, float);
void recordBothWaveFormIntoData(int, int, float, int);
void updateLED();
void storeDataIntoMemory();
void writeDataIntoCSV();
int checkFileExist(const char *fileName);
void endProgram();

/* This creates a structure(object)*/
struct CSV
{
    int timeIterations;
    int frequency;
    float dutyCycle;
    int state;
};

struct ledData
{
    int blinkLed;
    int blinkFrequency;
    float blinkBrightness;
    int ledState;
    unsigned long previousMillis;
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

    if (selection < 0 || selection > 3)
    {
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        getUserSelection();
    }
    else
    {
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
void blink(int color, int blinkData)
{
    system("clear");
    printf("\nBlink...\n");

    int blinkLed = getBlinkLed(color);
    int frequency = getBlinkFrequency();
    float brightness = getBlinkBrightness();
    int ledToShine = confirmLedToShine(blinkData);

    if (confirmBlinkSelection(blinkLed, frequency, brightness, ledToShine) == CONFIRM)
    {
        if (ledToShine == oneLed)
        {
            recordWaveDataIntoMemory(blinkLed, frequency, brightness);
        }
        else
        {
            recordBothWaveFormIntoData(blinkLed, frequency, brightness, ledToShine);
        }
    }
    else
        system("clear");
    return;
}

/*
Menu to get user selction on LED to blink
*/
int getBlinkLed(int color)
{
    if (color == RED || color == GREEN)
    {
        return color;
    }

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
        getBlinkLed(0);
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

    if (scanf("%d", &selection) == 1)
    {
        if (selection >= 0 && selection <= 10)
        {
            system("clear");
            return selection;
        }
        else
        {
            system("clear");
            printf("Invalid Input. Try again...\n\n");
            getBlinkFrequency();
        }
    }
    else
    {
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        getBlinkFrequency();
    }
}

/*
Menu to get user selection on LED brightness
*/
float getBlinkBrightness()
{

    float selection;

    printf("Select LED brightness during blink.\n\n");
    printf("Enter any numbers between 0 to 100\n");
    printf("Brightness (%%): ");

    if (scanf("%f", &selection) == 1 && selection >= 0 && selection <= 100)
    {
        system("clear");
        return selection;
    }
    else
    {
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        return getBlinkBrightness();
    }
}

/*
Menu for user to acknowledge the blink configurations input
*/
int confirmBlinkSelection(int blinkLed, int blinkFrequency, float blinkBrightness, int ledToShine)
{

    int selection;
    char blinkLedString[] = "Green";

    if (blinkLed == BLINK_RED || blinkLed == RED)
    {
        strcpy(blinkLedString, "Red");
    }

    printf("Confirm your blink configurations.\n\n");
    printf("LED to blink: %s\n", blinkLedString);
    printf("Blink Frequency: %dHz\n", blinkFrequency);
    printf("Blink Brightness: %.2lf%%\n", blinkBrightness);
    printf("How many LED to blink: %d\n", ledToShine == oneLed ? 1 : 2);
    printf("[1] Confirm Configuration\n");
    printf("[0] Return to Home\n");
    printf("\nYour Selection: ");

    scanf("%d", &selection);

    if (selection < 0 || selection > 1)
    {
        system("clear");
        printf("Invalid Input. Try Again...\n\n");
        confirmBlinkSelection(blinkLed, blinkFrequency, blinkBrightness, ledToShine);
    }

    return selection;
}

int confirmLedToShine(int ledToShine)
{

    if (ledToShine == oneLed || ledToShine == twoLed)
    {
        return ledToShine == oneLed ? oneLed : twoLed;
    }

    int selection;

    printf("Choose your blink choice.\n\n");
    printf("Enter blink choice:\n");
    printf("[1] Blink one LED\n");
    printf("[2] Blink both LEDs\n");
    printf("\nYour Selection: ");

    if (scanf("%d", &selection) == 1)
    {
        if (selection == 1 || selection == 2)
        {
            system("clear");
            return (selection == 1) ? oneLed : twoLed;
        }
    }

    system("clear");
    printf("Invalid Input. Try Again...\n\n");

    // Clear the input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    return confirmLedToShine(0);
}

void recordBothWaveFormIntoData(int blinkLed, int blinkFrequency, float blinkBrightness, int ledBlink)
{
    /*Initi object to store values*/
    static struct ledData redData, greenData;
    int oppositeColor;
    /* To check whether both values are stored inside to do while loop*/
    if (blinkLed == BLINK_GREEN || blinkLed == GREEN)
    {
        greenData = (struct ledData){GREEN, blinkFrequency, blinkBrightness, LOW, 0};
        oppositeColor = RED;
    }
    else
    {
        redData = (struct ledData){RED, blinkFrequency, blinkBrightness, LOW, 0};
        oppositeColor = GREEN;
    }

    if (redData.blinkLed != RED || greenData.blinkLed != GREEN)
    {
        blink(oppositeColor, ledBlink);
    }

    /*Initializing Variables to do while loop for 1 minute*/
    int currentMillis = millis();
    int minuteMillis = millis() + (60 * TO_MILLIS);
    int iterations = 0;
    int nextRecord = currentMillis;
    struct CSV *redDataArr = malloc(6000 * sizeof(struct CSV));
    struct CSV *greenDataArr = malloc(6000 * sizeof(struct CSV));
    int timeLapse = 0;
    /*Make LED blink based on duty cycle and record wave form data into CSV*/
    do
    {
        currentMillis = millis();
        updateLED(&redData, currentMillis);
        updateLED(&greenData, currentMillis);
        /* Write data into array based on 10ms period*/
        if (currentMillis >= nextRecord)
        {
            storeDataIntoMemory(redDataArr, &redData, timeLapse, iterations);
            storeDataIntoMemory(greenDataArr, &greenData, timeLapse, iterations);
            iterations++;
            nextRecord = currentMillis + (10);
            timeLapse += 10;
            /* Once it reaches 6000 break the while stateemnt*/
            if (iterations == 6000)
            {
                break;
            }
        }
    } while (currentMillis < minuteMillis);
    /*To free up memory spaces for the structure arrays and write data information into LED*/
    softPwmWrite(GREEN, 0);
    softPwmWrite(RED, 0);
    writeDataIntoCSV(redDataArr, greenDataArr, iterations, blinkLed);
    free(greenDataArr);
    free(redDataArr);
    /* Reset the static array to store empty values if the users want to try other LED duty cycle for both LED*/
    redData.blinkLed = 0;
    greenData.blinkLed = 0;
}
/*
This helps to create an function for the user to store data into the csv
*/
void recordWaveDataIntoMemory(int blinkLed, int blinkFrequency, float blinkBrightness)
{
    printf("\nBlinking...\n");
    /* Formulas and initializer*/
    int period = (1.0f / blinkFrequency) * TO_MILLIS;
    int color = blinkLed == BLINK_GREEN ? GREEN : RED;
    struct ledData dataStruct = (struct ledData){color, blinkFrequency, blinkBrightness, LOW, 0};
    struct CSV *data;
    data = malloc(6000 * sizeof(struct CSV));

    if (data == NULL)
    {
        fprintf(stderr, "Memory allocation failed");
        return;
    }

    /* Intializes the Millisecond counter to compare insert data into memory*/
    unsigned long currentMillis = millis();
    unsigned long previousMillis = 0;
    unsigned long nextRecord = currentMillis;
    unsigned long testData = currentMillis + (60 * TO_MILLIS);
    int onOffTime = period * blinkBrightness / 100;
    int iterations = 0;
    int timeLapse = 0;
    int brightness = 0;

    do
    {
        currentMillis = millis();

        updateLED(&dataStruct, currentMillis);

        /* Stores record every 10millisecond */
        if (currentMillis >= nextRecord)
        {
            storeDataIntoMemory(data, &dataStruct, timeLapse, iterations);
            iterations++;
            nextRecord = currentMillis + (10);
            timeLapse += 10;
        }

        /*If the iteration reaches 6k, will break the function and continue on*/
        if (iterations == 6000)
        {
            break;
        }

    } while (currentMillis < testData);

    /*ensures that the current color will be off after looping and write data into csv and make sure the memory allocation is freed after use*/
    softPwmWrite(color, 0);
    writeDataIntoCSV(data, NULL, iterations, blinkLed);
    free(data);
}

void storeDataIntoMemory(struct CSV *data, struct ledData *LEDdata, int timeLapse, int iterations)
{
    /*This will store data based on each iterations of the array*/
    data[iterations].timeIterations = timeLapse;
    data[iterations].frequency = LEDdata->blinkFrequency;
    data[iterations].dutyCycle = LEDdata->blinkBrightness;
    data[iterations].state = digitalRead(LEDdata->blinkLed);
}

void updateLED(struct ledData *ledData, unsigned long currentMillis)
{
    /*Based on the duty Cycle the LED will blink accordingly and set the values in*/
    int period = (1.0f / ledData->blinkFrequency) * TO_MILLIS;
    int onOffTime = ledData->ledState == HIGH || ledData->blinkBrightness == 100 ? period * (ledData->blinkBrightness / 100) : period * (1 - (ledData->blinkBrightness / 100));

    if (currentMillis - ledData->previousMillis >= onOffTime)
    {
        ledData->ledState = ledData->ledState == LOW ? HIGH : LOW;
        ledData->previousMillis = currentMillis;
    }

    int brightness = ledData->ledState == HIGH ? ledData->blinkBrightness : 0;
    softPwmWrite(ledData->blinkLed, brightness);
    digitalWrite(ledData->blinkLed, ledData->ledState);
}

/*
This function creates the CSV file and writes into it.
*/
void writeDataIntoCSV(struct CSV *data, struct CSV *secondData, int sizeArr, int blinkLed)
{
    /* Init array to store data inside*/
    static struct CSV redLedArray[6000];
    static struct CSV greenLedArray[6000];
    int oppositeColor;
    /*Checks which function did it prompt from and set accordingly to the data set*/
    if (data == NULL || secondData == NULL)
    {

        if (blinkLed == BLINK_GREEN)
        {
            for (int i = 0; i < sizeArr; i++)
            {
                greenLedArray[i] = data[i];
                oppositeColor = RED;
            }
        }
        else
        {
            for (int i = 0; i < sizeArr; i++)
            {
                redLedArray[i] = data[i];
                oppositeColor = GREEN;
            }
        }

        /* This will trigger the user to key in the value if one of the array doesnt contain value accordingly*/
        if (greenLedArray[0].frequency == 0 || redLedArray[0].frequency == 0)
        {
            blink(oppositeColor, oneLed);
        }
    }
    else
    {
        for (int i = 0; i < sizeArr; i++)
        {
            redLedArray[i] = data[i];
            greenLedArray[i] = secondData[i];
        }
    }
    /*Checks whether file is empty based on data integerity*/
    if (checkFileExist("displayPlot.csv") == 0)
    {
        /*Creating a new csv to store the data in and header*/
        FILE *CSV = fopen("displayPlot.csv", "wb+");
        fprintf(CSV, "Green Iterations,Green Frequency,Green Duty Cycle,Green State,Red Iterations,Red Frequency, Red Duty Cycle,Red State"); // Creating Header for the file

        for (int i = 0; i < sizeArr; i++)
        {
            fprintf(CSV,
                    "\n%d,%d,%.2f,%d,%d,%d,%.2f,%d",
                    greenLedArray[i].timeIterations, greenLedArray[i].frequency, greenLedArray[i].dutyCycle, greenLedArray[i].state,
                    redLedArray[i].timeIterations, redLedArray[i].frequency, redLedArray[i].dutyCycle, redLedArray[i].state);
        }

        /* Informs user CSV has been created and close the file editor*/
        printf("New CSV file displayPlot has been created");
        fclose(CSV);
    }

    greenLedArray[0].frequency = 0;
    redLedArray[0].frequency = 0;
}

/*
This helps to check whether the file exists
*/
int checkFileExist(const char *fileName)
{
    FILE *file;

    if (file = fopen(fileName, "rb"))
    {
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
