# Debugging interrupts

I ran into an issue while building out the prototype where the disk would pass by the break beams but that wouldn't be reflected in the pin state when I queried the pins.

After physical debugging turned up nothing I stepped back and looked at the code. It seemed like I was trying to do so much in the loop that I was probably missing the break beam read due to timing. That seemed like an easy fix; use pin interrupts instead of querying the pins in the loop!

I ran into issues doing that, mostly centering around my still-somewhat-green-ness in cpp and arduino lands.

So, I created this proof of concept isolation of the logic so that I could debug the interrupt attachment, ISR, and loop state checks. Once I got it figured it I fit it back into the prototype.
