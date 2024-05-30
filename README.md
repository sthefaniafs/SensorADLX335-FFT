# FFT da vibração do ADXL335 com Arduino e Python

## Em construção

Este algoritmo realiza a leitura dos dados de vibração nos eixos x, y e z do sensor ADLX335 utilizando Arduino, além de calcular a FFT diretamente no dispositivo embarcado. Os dados coletados são processados em Python, onde são armazenados em um arquivo CSV e plotados em um gráfico em tempo real. 

Este projeto fornece uma solução eficiente para monitorar e analisar oscilações em três direções, possibilitando uma compreensão detalhada do comportamento do equipamento analisado em tempo real.

## Hardware
- Franzininho WiFi, baseada no ESP32-S2.

## Software
O software conta com duas partes distintas, cada uma desempenhando funções específicas. 

### Arduino
A primeira parte é responsável por ler os dados do sensor ADLX335 e calcular a FFT.
- 512 amostras são coletadas por segundo;
- Após a normalização, as 512 amostras são reduzidas a 256 pontos;
- Trabalhar com 256 pontos normalizados torna o cálculo da FFT mais rápido e menos intensivo em termos de computação, o que é especialmente importante ao se utilizar microcontroladores.


### Python
A segunda parte recebe os dados lidos pelo Arduino, os salva em um arquivo CSV e os plota em um gráfico na frequência para verificar se a leitura está sendo feita corretamente.
