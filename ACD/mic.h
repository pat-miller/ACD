

// Adapted from code Enrique Cond�s and Ragnar Ran�yen Homb
// Licenced under GNU General Public License

#include "arduinoFFT.h"

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

    const int inputPin = A0;
    
    const uint16_t samples = 64; //This value MUST ALWAYS be a power of 2
    const double samplingFrequency = 8000; //Hz, must be less than 10000 due to ADC

    double vReal[samples];
    double vImag[samples];

    unsigned long microseconds;
    unsigned int sampling_period_us;

    int checkForSound();

    void setupMic() {
        sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
    }

    int checkForSound() {
        //CONECTIONS:
        //power mic amp module with 3.3v and connect to A0
        // SAMPLING the sound. it takes samples/frequency = 64/8000 = 8ms
        microseconds = micros();
        for (int i = 0; i < samples; i++)
        {
            vReal[i] = analogRead(inputPin);
            vImag[i] = 0;
            while (micros() - microseconds < sampling_period_us) {
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
        if (vReal[15] > TRIG_LEVEL_1_9KHZ) {
            //the 1.9KHz tone has been detected
            return 1;
        }

        else if (vReal[42] > TRIG_LEVEL_5_2KHZ) {
            //the 5.2KHz tone has been detected
            return 2;
        }

        else {
            return 0;
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
        */
    }

