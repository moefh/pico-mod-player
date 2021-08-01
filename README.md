# pico-mod-player

A simple MOD player for the Raspberry Pi Pico.

The audio output code is based on the PWM/DMA code described in [this
blog post](https://gregchadwick.co.uk/blog/playing-with-the-pico-pt3/)
by Greg Chadwick.

The MOD player is currently very basic, outputing mono with 8 bits per
sample and doesn't support many MOD effects (like tremolo and
vibrato).  Still, it does a nice enough job of playing most MODs I've
tested so far.

To convert a MOD file for playing in the Pico you'll need the `mod2h`
tool from [this project](https://github.com/moefh/mod-tools) to
convert the MOD file to a C header file for inclusion in the code.


## Using an External Amplifier Module

The sound is output to the selected pin as PWM, so you'll need a
capacitor to smooth the waveform, some resistors to lower the voltage
level and a coupling capacitor to remove the DC offset.  Here's what I
used to send the sound over to a [cheap LM386
module](https://pt.aliexpress.com/item/32664418371.html):

![Schematic of the sound output for an external LM386 module](images/pico-output-schematic.png)

And this is the setup on a breadboard connected to the LM386 module:

![Breadboard with the Pico and LM386 module](images/pico-with-amp-module.jpg)

This setup will likely work well with other amplifier modules. If you
have a stereo module, just connect the single output to both left and
right inputs of the amplifier module.

The output of the module is connected to a small 8Ω speaker.

## Using an LM386 Chip as the Amplifier

An alternative is to use just the LM386 chip, here's the schematic I
used (it mixes the previous schematic and a schematic taken from
the LM386 datasheet):

![Schematic of the sound output using an LM386 chip](images/pico-amp-schematic.png)

And this is the setup on a breadboard with the LM386 chip:

![Breadboard with the Pico and LM386 chip](images/pico-with-homemade-amp.jpg)

As before, the output of the amplifier is connected to a small 8Ω speaker.


## License

The source code is distributed under the MIT License.

This project includes the MOD song "[The
Soft-liner](https://modarchive.org/index.php?request=view_by_moduleid&query=61156)"
by Zilly Mike, licensed under Creative Commons [CC BY
3.0](https://creativecommons.org/licenses/by/3.0/). No changes were
made other than the conversion from the MOD to a header file for
inclusion in the code.
