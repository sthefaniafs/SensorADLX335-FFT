import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import csv

# Constants
SERIAL_PORT = 'COM10'
BAUD_RATE = 115200

# Initialize serial connection
arduinoData = serial.Serial(SERIAL_PORT, BAUD_RATE)

# Initialize empty lists to store data
tempo = []
dataVibX = []
dataVibY = []
dataVibZ = []
freq = []
FFTx = []
FFTy = []
FFTz = []

# Create a function to read and process data from Arduino
def read_and_process_data():
    arduinoString = arduinoData.readline().decode('utf-8').strip()
    dataArray = arduinoString.split(' , ')

    tempo.append(float(dataArray[0]))
    dataVibX.append(float(dataArray[1]))
    dataVibY.append(float(dataArray[2]))
    dataVibZ.append(float(dataArray[3]))
    freq.append(float(dataArray[4]))
    FFTx.append(float(dataArray[5]))
    FFTy.append(float(dataArray[6]))
    FFTz.append(float(dataArray[7]))

    # Print the received values
    #print(f'Time: {dataArray[0]}, X: {dataArray[1]}, Y: {dataArray[2]}, Z: {dataArray[2]}')

# Create a function to update the plot
def update_plot(frame):
    read_and_process_data()
    plt.cla()
    plt.plot(tempo, dataVibX, label='X')
    plt.plot(tempo, dataVibY, label='Y')
    plt.plot(tempo, dataVibZ, label='Z')


    ax[0].plot(freq, dataVibX, color='blue')
    ax[0].set_title('FFT da vibração em x')

    ax[1].plot(freq, dataVibY, color='red')
    ax[1].set_title('FFT da vibração em y')
    #plt.ylabel('Amplitude (m/s^2)')


    ax[2].plot(freq, dataVibZ, color='green')
    ax[2].set_title('FFT da vibração em z')

    plt.xlabel('Frequência (Hz)')
    # Adicionando nome para todos os eixos
    #fig.text(0.5, 0.02, 'Frequência (Hz)', ha='center')
    #fig.text(0.02, 0.5, 'Amplitude (m/s^2)', va='center', rotation='vertical')

    plt.tight_layout()
    plt.legend()

# Create a function to save data to a CSV file when the plot window is closed
def on_close(event):
    with open('FFT_vib_teste.csv', 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['Tempo', 'X', 'Y', 'Z'])
        for t, x, y, z, f, fftx, ffty, fftz in zip(freq, dataVibX, dataVibY, dataVibZ, freq, FFTx, FFTy, FFTz):
            writer.writerow([t, x, y, z, f, fftx, ffty, fftz])

# Register the callback function for when the plot window is closed
fig, ax = plt.subplots(4,1)
#fig, ax = plt.subplots(3,1) # consigo msotrar no tempo e na frequencia

fig.canvas.mpl_connect('close_event', on_close)

ani = FuncAnimation(fig, update_plot, interval=2, cache_frame_data=True)
plt.show()