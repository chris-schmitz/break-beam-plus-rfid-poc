# Pegasus

## PEG meaning
Plinko Electronic Game

## Physical description
A plinko board with distinct tokens. Each token corresponds to a specific noun, e.g. cupcake, jean jacket, magic wand, diamond, etc. The slot that the token passes by cooresponds to a verb, e.g. eat, sit on, hug, jump over.


Each slot has a clear acrylic tile in front of it with an image of that verb (or maybe the word??) laser etched into it. LED strips are glued onto the edge of each tile so that the tiles can be lit up individually.

The acrylic cover over the pegs has our mascott, Peg, laser etched into it. An LED strip is glued along the edge of the cover similar to the slot tiles.

The bottom of the board has a thermal receipt printer built into it that will print out the results.

A music wing will play sound effects during gameplay.

## Slack integration
The will build and store an array of fortunes. These fortunes will be printed out along with the image of Peg in the game result.

Fortunes can be added via a slack integration. The user Peg will be added to our slack team and if you direct message peg with the trigger `/add-fortune` you can type out a fortune to be added to the array of possible outcomes.

A `pegasus` channel will also be added to slack. Whenever a player gets a result, the game board will message to slack with the result. The ascii of Peg and the fortune chosen will be written to the slack channel so everyone can see that someone has played and what the result was.

## Gameplay

When the player drops the token in, it bounces through the pegs and at the bottom passes through one of the 5 (or so) slots. When it passes by a slot a photo sensor that tells the microcontroller which slot was activated.

When a slot is activated, an LED strip on the side of the acrylic slot label lights up and makes the laser etched acrylic in front of the slot highlight. The slot's verb is logged by the microcontroller.

After passing by the slot, the token is funneled down past an RFID sensor. The tag is read from the token and logged.

With both the token and slot information captured, the LED strip on the edge of the acrylic cover fires a light pattern as the results are generated. The thermal printer will print out an ascii image of Peg that represents the combination of noun and verb (e.g. peg sits on the cupcake). A fortune will be selected at random (? should it be random or should we somehow derive it from the game results? prob random) and printed out under the ascii of Peg.

## Sound effects
- Idle music
- Slot activation
- Result generation
- Result presentation


## Electronic parts needed

- Photo sensors x5: https://www.adafruit.com/product/161
- Thermal printer: https://www.adafruit.com/product/2753
- LED strips: https://www.adafruit.com/?q=neopixel%20strip
- RFID reader:
- RFID tags:
- Music maker wing:
- SD Card:
- Server:
- Esp8266 wifi chip:
