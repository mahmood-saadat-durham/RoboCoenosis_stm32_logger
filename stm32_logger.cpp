// Do not remove the include below
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <HardwareSerial.h>
#include <IWatchdog.h>

#define		HS1			PC0
#define		HS2			PC1
#define		HS3			PC2
#define		HS4			PC3
#define		HS5			PC4
#define		HB1			PC5
#define		HB2			PB1
#define		HB3			PA1
#define		HB4			PA0
#define		HB5			PB0

#define     LED1		PB12
#define     LED2		PB13

#define     SD_CS_PIN	PA4

#define		LOG_FILE_NAME "log.csv"


HardwareSerial Serial2 (PA3, PA2);

File logFile;
bool isSdInitialized = false;
uint16_t logFileWriteCounter = 0;

const uint8_t     	analogPins[10] = {HS1, HS2, HS3, HS4, HS5, HB1, HB2, HB3, HB4, HB5};
int16_t 			analogReadValues[10] = {0};

void updateAnalogInputValues();

//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	Serial2.begin(115200);
	//
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(SD_CS_PIN, OUTPUT);

	analogReadResolution(12);

	// SD card chip select
	digitalWrite(SD_CS_PIN, HIGH);
	delay(1);

	if (!SD.begin(SD_CS_PIN))
	{
		digitalWrite(LED2, HIGH);
		//Serial2.printf("SD initialization failed!\r\n");
	}
	else
	{
		digitalWrite(LED2, LOW);
		isSdInitialized = true;
	}

	if(isSdInitialized)
	{
		// open the file. note that only one file can be open at a time,
		// so you have to close this one before opening another.
		logFile = SD.open(LOG_FILE_NAME, FILE_WRITE);
	}

	// if the file opened okay, write to it:
	if (isSdInitialized && logFile)
	{
		if(logFile.size() < 10)
		{
			logFile.seek(0);
			logFile.println("Time Stamp (ms), HS1, HS2, HS3, HS4, HS5, HB1, HB2, HB3, HB4, HB5");
		}
		else
		{
			logFile.seek(logFile.size());
		}
		logFile.close();
	}
	else
	{
		// if the file didn't open, print an error:
		//Serial2.println("error opening file!");
		digitalWrite(LED2, HIGH);
	}

	// Initialize the watchdog for 1s
	IWatchdog.begin(1000000);

	if(IWatchdog.isReset(true))
	{
		//Serial2.printf("Reset because of WDT!\r\n");
	}
}

// The loop function is called in an endless loop
void loop()
{
	updateAnalogInputValues();
	//Add your repeated code here
	Serial2.printf("%ld, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n", millis(),
			analogReadValues[0],  analogReadValues[1],  analogReadValues[2],  analogReadValues[3],  analogReadValues[4]
			,  analogReadValues[5],  analogReadValues[6],  analogReadValues[7],  analogReadValues[8],  analogReadValues[9]);
	if(isSdInitialized)
	{
		//logFile = SD.open(LOG_FILE_NAME, FILE_WRITE);
		if(logFile)
		{
			logFile.printf("%ld, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n", millis(),
					analogReadValues[0],  analogReadValues[1],  analogReadValues[2],  analogReadValues[3],  analogReadValues[4]
					,  analogReadValues[5],  analogReadValues[6],  analogReadValues[7],  analogReadValues[8],  analogReadValues[9]);
			// Closing and opening the file again to write the pending data or using flush is very slow. We flush it every 50 samples
			//logFile.flush();
			//logFile.close();
			digitalWrite(LED2, LOW);
			logFileWriteCounter ++;
			if(logFileWriteCounter > 50)
			{
				logFile.flush();
			}
		}
		else
		{
			logFile = SD.open(LOG_FILE_NAME, FILE_WRITE);
			digitalWrite(LED2, HIGH);
			//Serial2.printf("Log file is null!\r\n");
		}
	}
	else
	{
		digitalWrite(LED2, HIGH);
		//Serial2.printf("SD Not initialized!\r\n");
	}
	digitalWrite(LED1, !digitalRead(LED1));
	IWatchdog.reload();
	delay(50);
}

void updateAnalogInputValues()
{
	for(int i = 0; i < 10; i ++)
	{
		analogReadValues[i] = (int16_t)((float)analogRead(analogPins[i]) * (float)(3.3f / 4.096f));
	}
}
