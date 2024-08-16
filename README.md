# Low Level Tetris
A Tetris game coded in C, running directly on bare metal Raspberry Pi 2 (or 3). No OS, direct hardware control for graphics and input.

## About USPi

USPi is an USB driver for bare metal Raspberry Pi written in C. Here is the original repository: [USPi](https://github.com/rsta2/uspi)

USPi is used with little adaptations in our Low Level Tetris project. The needed files are inside the `uspi` directory.

## Compiling Options

The USPi driver has flexibility to decide the Raspberry Pi model, aswell as the architecture (for RPi3, since it can be a 32-bit architecture or 64-bit archutecture).

Also, the prefix for the compiler is flexible to the USPi driver, however we used the `arm-none-eabi-gcc` (and the other tools necessary - loader, assembler, etc).

## Raspberry Pi Models covered

Our game is supported on Raspberry Pi 2 and 3. Even though the driver is available for other models, we only managed to test on those two.

## Generate .img file

The USPi driver is adapted here to work by default for the Raspberry Pi 2. For that, just simply use the `makeall` script

```bash
./makeall
```

It is also possible to use it for the Raspberry Pi 3 (32-bit architecture):

```bash
./makeall RASPPI=3 
```

To remove all the generated files:
```bash
./makeall clean
```

## Disk Images

Despite the .img file generating methods described above, there are ready-to-use .img files inside the `disk_images` directory.

The kernel8-32.img is used for Raspberry Pi 3 (8 from armv8-a, and 32 for the 32-bit architecture), and kernel7.img is for the Raspberry Pi 2 (7 from armv7).

## Logging

The USPi driver contains logging on the initialization of the driver, showing relevant messages related to the keyboard integration. 

For example, for a successful connection:

![Successful connection: USPi library successfully initialized](./assets/keyboard_found.jpeg)

By default, we left logging disabled. If it is wanted to enable the logging, the makeall command should be the following:

```bash
./makeall LOGGING=1 DEBUG=1 
```

Or, for the Raspberry Pi 3:
```bash
./makeall RASPPI=3 LOGGING=1 DEBUG=1 
```