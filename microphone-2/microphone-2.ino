/*

  Example of use of the FFT libray to compute FFT for a signal sampled through the ADC.
        Copyright (C) 2018 Enrique Condés and Ragnar Ranøyen Homb

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "arduinoFFT.h"

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
/*
These values can be changed in order to evaluate the functions
*/
#define CHANNEL A0
const uint16_t samples = 64; //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 8000; //Hz, must be less than 10000 due to ADC

unsigned int sampling_period_us;
unsigned long microseconds;

/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/
double vReal[samples];
double vImag[samples];

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

void setup()
{
  sampling_period_us = round(1000000*(1.0/samplingFrequency));
  Serial.begin(115200);
  Serial.println("Ready");
}

void loop()
{

  //CONECTIONS:
  //power mic amp module with 3.3v and connect to A0
  // SAMPLING the sound. it takes samples/frequency = 64/8000 = 8ms
  microseconds = micros();
  for(int i=0; i<samples; i++)
  {
      vReal[i] = analogRead(CHANNEL);
      vImag[i] = 0;
      while(micros() - microseconds < sampling_period_us){
        //empty loop
      }
      microseconds += sampling_period_us;
  }

  // Calcs about the audio signal
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  // Weigh data
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); // Compute FFT
  FFT.ComplexToMagnitude(vReal, vImag, samples); // Compute magnitudes

  //check if an audio trigger has been detected
  #define TRIG_LEVEL_1_9KHZ 400
  #define TRIG_LEVEL_5_2KHZ 800
    if(vReal[15] > TRIG_LEVEL_1_9KHZ) {
    //the 1.9KHz tone has been detected
    }
   
    if(vReal[42] > TRIG_LEVEL_5_2KHZ) {
      //the 5.2KHz tone has been detected
    }
  
    /* READINGS FROM HEADPHONE NEXT TO MIC AT COLLEGE. stored in array caled vReal[]
    1900Hz Signal: vReal[14] > 100 && vReal[15] > 400 && vReal[16] > 200
    [14]1750.000000Hz 144.6338
    [15]1875.000000Hz 482.4529
    [16]2000.000000Hz 292.5368
    
    5200Hz Signal
    [21]2625.000000Hz 162.9220
    [22]2750.000000Hz 915.0484
    [23]2875.000000Hz 791.5623
    
    [41]5125.000000Hz 791.5623
    [42]5250.000000Hz 915.0484
    [43]5375.000000Hz 162.9220

    READINGS FROM CONTROL MODULE:
    LOW FREQ:
    M1:
    [14]1750.000000Hz 727.3148
    [15]1875.000000Hz 1648.7812
    [16]2000.000000Hz 755.1448

    M2:
    1750.000000Hz 307.0199
    1875.000000Hz 470.8489
    2000.000000Hz 156.1473

    M3:
    2250.000000Hz 167.6262
    2375.000000Hz 751.1906
    2500.000000Hz 612.3876



    HIGH FREQ:
    M1:
    [44]5500.000000Hz 137.1266
    [45]5625.000000Hz 454.9480
    [46]5750.000000Hz 275.0983
    M2:
    5375.000000Hz 250.5158
    5500.000000Hz 360.4578
    5625.000000Hz 94.2462
    M3:
    5375.000000Hz 101.4472
    5500.000000Hz 800.0160
    5625.000000Hz 1000.3213
    5750.000000Hz 218.7915



    
    */

  //uncomment to print magnitudes of frequency bands to serial monitor
  Serial.println("Computed magnitudes:");
  PrintVector(vReal, (samples /*>> 1*/), SCL_FREQUENCY);
  delay(2000); /* Repeat after delay */
  
  
//  /* Print the results of the sampling according to time */
//  Serial.println("Data:");
//  PrintVector(vReal, samples, SCL_TIME);
//  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
//  Serial.println("Weighed data:");
//  PrintVector(vReal, samples, SCL_TIME);
//  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
//  Serial.println("Computed Real values:");
//  PrintVector(vReal, samples, SCL_INDEX);
//  Serial.println("Computed Imaginary values:");
//  PrintVector(vImag, samples, SCL_INDEX);
//  FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
//  Serial.println("Computed magnitudes:");
//  PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);
//  double x = FFT.MajorPeak(vReal, samples, samplingFrequency);
//  Serial.println(x, 6); //Print out what frequency is the most dominant.
//  //while(1); /* Run Once */
//  delay(2000); /* Repeat after delay */
}

//prints formatted array
void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
  break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
  break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / samples);
  break;
    }
    Serial.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}
