#include "arduinoFFT.h"

const int xPin = 1;
const int yPin = 2;
const int zPin = 3;
const int ADC_AMPLITUDE = 4096; //amplitude of the 12bit-ADC of Arduino is 4096
const int ADC_REF = 3.3;   //ADC reference is 3.3v
const int samples = 512;             //Must be a power of 2
const int samplingFrequency = 512; //Hz, must be less than 10000 due to ADC

unsigned int samplingPeriod_ms;
unsigned long milliseconds;

unsigned int xAdcValue, yAdcValue, zAdcValue;
long xScaled, yScaled, zScaled;
double x_m_s2, y_m_s2, z_m_s2;
float freq;

double vRealX[samples], vRealY[samples], vRealZ[samples];
double vImagX[samples], vImagY[samples], vImagZ[samples];

ArduinoFFT<double> FFTX = ArduinoFFT<double>(vRealX, vImagX, samples, samplingFrequency); 
ArduinoFFT<double> FFTY = ArduinoFFT<double>(vRealY, vImagY, samples, samplingFrequency); 
ArduinoFFT<double> FFTZ = ArduinoFFT<double>(vRealZ, vImagZ, samples, samplingFrequency); 
 
void setup() {
    Serial.begin(115200);
    samplingPeriod_ms = round(1000*(1.0/samplingFrequency));
    //analogReadResolution(10);
}

void loop() {
   
    /*SAMPLING*/
    for(int i=0; i<samples; i++)
    {
        milliseconds = millis();    //Overflows after around 70 minutes!
      // Leia os valores brutos
        xAdcValue = analogRead(xPin);
        yAdcValue = analogRead(yPin);
        zAdcValue = analogRead(zPin);

        xScaled = map(xAdcValue, 0, ADC_AMPLITUDE , -3000, 3000);
        yScaled = map(yAdcValue, 0, ADC_AMPLITUDE, -3000, 3000);
        zScaled = map(zAdcValue, 0, ADC_AMPLITUDE, -3000, 3000);
        // re-scale to fractional Gs
        float x_m_s2 = (xScaled / 1000.0) * 9.81;
        float y_m_s2 = (yScaled / 1000.0) * 9.81;
        float z_m_s2 = (zScaled / 1000.0) * 9.81;

        vRealX[i] = x_m_s2;
        vRealY[i] = y_m_s2;
        vRealZ[i] = z_m_s2;

      Serial.print(milliseconds); 
      Serial.print(" , "); // Separador
      Serial.print(vRealX[i], 1);    //View only this line in serial plotter to visualize the bins
      Serial.print(" , ");    //View only this line in serial plotter to visualize the bins
      Serial.print(vRealY[i], 1);    //View only this line in serial plotter to visualize the bins
      Serial.print(" , ");    //View only this line in serial plotter to visualize the bins
      Serial.print(vRealZ[i], 1);    //View only this line in serial plotter to visualize the bins
      Serial.print(" , "); // Separador

        
        vImagX[i] = 0;
        vImagY[i] = 0;
        vImagZ[i] = 0;
     
        while(millis() < (milliseconds + samplingPeriod_ms)){
        }
    }
 
    /*FFT X*/
    FFTX.windowing(vRealX, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFTX.compute(vRealX, vImagX, samples, FFT_FORWARD);
    FFTX.complexToMagnitude(vRealX, vImagX, samples);

    /*FFT Y*/
    FFTY.windowing(vRealY, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFTY.compute(vRealY, vImagY, samples, FFT_FORWARD);
    FFTY.complexToMagnitude(vRealY, vImagY, samples);

    /*FFT Z*/
    FFTZ.windowing(vRealZ, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFTZ.compute(vRealZ, vImagZ, samples, FFT_FORWARD);
    FFTZ.complexToMagnitude(vRealZ, vImagZ, samples);
 
    for(int i=0; i<(samples/2); i++)
    {
      freq = (float)i * samplingFrequency / samples;

      // Imprimir os valores de x, y, z para cada frequência
      Serial.print(freq, 1); // Imprimir a frequência
      Serial.print(" , "); // Separador
      Serial.print(vRealX[i], 1);    //View only this line in serial plotter to visualize the bins
      Serial.print(" , ");    //View only this line in serial plotter to visualize the bins
      Serial.print(vRealY[i], 1);    //View only this line in serial plotter to visualize the bins
      Serial.print(" , ");    //View only this line in serial plotter to visualize the bins
      Serial.println(vRealZ[i], 1);    //View only this line in serial plotter to visualize the bins
    }
    //delay(1000);  //Repeat the process every second OR:
    //while(1);       //Run code once
}