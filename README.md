# (USB-)MIDI big button

**Update regarding issue #1**  
The code here has only been tested on an arduino micro. It appears it doesn't work on a nano, due to different usb capabilities. It should be possible to use the serial connection and a MIDI jack to port the code for a nano. However, nothing of this has been tested and you may have to use a different MIDI library.

Code for a MIDI version of the big button from lookmumnocomputer.

The original version can be found here:
https://www.lookmumnocomputer.com/projects#/big-button

I messed up the soldering on my Arduino, so I had to change the pin layout...you can find my layout at the beginning of the code.

The code itself is rather simple, it tracks the clock signal from the daw and then updates the sequencer according to the chosen tempo and pattern length.

I did not test it, but in principle you should be able to grab the serial output via a MIDI jack and thus send the notes and forward the clock (the latter requires some minor code modifications). Also I did not test the original output via 3.5mm jacks, but they should work just fine in principle.

Let me know if you have questions or need assistance...
