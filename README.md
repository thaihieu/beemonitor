# beemonitor
Codes for using the Hx711 amplifier with 3 wire load cells and the DHT11 for monitoring through BLE device on a Galileo.

This project is using the DHT11 sensor for Temp & Humility connected to a Genuino 101 board,
the other sensor is four 3wire load cells with a HX711 Amplifier to measure the mass. The communication method is
using BLE device on a Inter Galileo board.
The project is broken into 3 parts the sketch for the Genuino board
The bash script to the communication and retrieve the data
NodeJS to push it to the SparkFun cloud server or and server wish.

