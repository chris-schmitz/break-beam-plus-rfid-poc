# Pegasus pinouts needed

## FRID reader

-   SPI pins

## Breakbeam sensors

-   6 gpio pins

## Neopixel bars

-   i2c pins, bars daisy chained

## Neopixel strip

-   i2c pins, daisy chained to bars

# Power requirements

## RFID Reader

-   https://www.hotmcu.com/mifare-1356mhz-rc522-rfid-card-reader-module-p-84.html
-   Peak current: **30ma**

## Break Beams

-   https://www.adafruit.com/product/2167
-   3volt
-   Current draw: 10mA at 3v, 20ma at 5v
-   With 6 receivers that's: **60ma**

## neopixel bars

-   https://www.adafruit.com/product/1426
-   https://learn.adafruit.com/adafruit-neopixel-uberguide/powering-neopixels#estimating-power-requirements-5-11

## Strip

-   https://www.adafruit.com/product/1376?length=1
-   60ma

## Amplifier

-   https://www.adafruit.com/product/2130
-   4ma

# Total neopixels

## Low estimate

-   40 from the bars
-   9 from the strip?
-   0.98 amps minimum

## High estimate

-   40 from the bars
-   30 from the strip
-   1.4 amps minimum

# Total current draw rough

## High

-   1.4a + .06a + .03a + .004a = 2.744 amps

## Low
