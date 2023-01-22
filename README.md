# Homebaked Pi: Arithmetic
###### by [N. Escobar](https://nickesc.github.io)/[nickesc](https://github.com/nickesc)

Homebaked Pi: Arthmetic was the final project for my Operating Systems class. It turns a Raspberry Pi into a calculator, and outputs the GUI over the HDMI connection. The project is written in C as a bare metal application for a Raspberry Pi 3, and utilizes code and information from [this tutorial](https://github.com/bztsrc/raspi3-tutorial) by [Zoltan Baldaszti](https://github.com/bztsrc).

The calculator takes input on the left side of the screen and show the output on the right side. It shows an evalutaion of the full expression, as well as showing off other operations on the first two terms. The user can input an expression containing positive integers separated by operators. The calculator accepts `+`, `+`, `-`, `*`, `x`, `/`, `^`, and `%` as operators. To input a new expression, restart the calculator. All calculator functionality can be seen in the demonstration below:

> ## Demo
> https://user-images.githubusercontent.com/12503203/144731501-1463c962-3b72-480b-b5b6-22b9652b47d9.mp4

Most of the project's code can be found in the `main.c` file, which sets up the GUI and handles the input loop. The GUI shape and size can be set in the `setupScreen()` function, examples of which can be seen in the images below:

> ### Default Sizing
> ![default](/img/default.jpg)
> the default layout for the GUI, designed for this size

> ### Wide Sizing
> ![wide](/img/wide.jpg)
> default layout, but with wider input and output fields

> ### Big Sizing
> ![big](/img/big.jpg)
> spaced out layout, with more space between characters

## Components:
- Raspberry Pi 3 Model B
- MicroSD card
- [Serial cable](https://www.adafruit.com/product/954)

## Limitations:
- The calculator breaks with very large numbers
- Input is taken over serial connection and not natively over the USB port on the Raspberry Pi
