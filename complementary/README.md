The Complementary contains code that can is used by other boards which fake communication.

- uno/ -- An UNO disguised as an identical device. A version which strips off any user interface.
- stm32/ -- An STM32 disguised as non-user IO. Includes GPS, radio, Accelerometer, and Magnetometer. Controlled through USB, and acts as bridge between UNO and project board.
