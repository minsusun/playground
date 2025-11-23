# Arduino Distributed Floating Point Calculator

Arudino Distributed Floating Point Calculator using I2C protocol

- Each slave nodes has 1~N slave id on the i2c bus

- Master node does not participate on the calculation

- The number of calculation put into the master node meens that each slave nodes calculate the same number of floating point calculation

- 32bit Floating point calculation(fp32)

- Since arduino does not support 4byte I2C communication directly, I used union type to represent the 32bit data
