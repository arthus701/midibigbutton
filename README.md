# (USB-)MIDI big button
Code for a MIDI version of the big button from lookmumnocomputer.

The original version can be found here:
https://www.lookmumnocomputer.com/projects#/big-button

I messed up the soldering on my Arduino, so I had to change the pin layout...you can find my layout at the beginning of the code.

The code itself is rather simple, it tracks the clock signal from the daw and then updates the sequencer according to the chosen tempo and pattern length.

I did not test it, but in principle you should be able to grab the serial output via a MIDI jack and thus send the notes and forward the clock (the latter requires some minor code modifications). Also I did not test the original output via 3.5mm jacks, but they should work just fine in principle.

Let me know if you have questions or need assistance...
