// Big Button MIDI Version, inspired and mainly copied from SAM BATTLE aka. Look Mum No Computer
// Original comment {

//KOSMO BIG BUTTON 2016 SAM BATTLE
//email address is computer@lookmumnocomputer.com
//look mum no computer
//lookmumnocomputer@gmail.com
//www.facebook.com/lookmumnocomputer
//ITS AN ABSOLUTE MESS BUT IT WORKS.... SORT OF.... 
//YOU NEED QUITE A CLEAN CLOCK TRIGGER SIGNAL. QUITE A SHORT PULSE!
//To make it work better add this circuit to the clock input :-
//https://www.cgs.synth.net/modules/cgs24_gatetotrigger.html

//the premise of this is a simple performance sequencer. 
//it is used in synth bike mk2 to sequencer the drums.
//i figured whats the point in not sharing it!!!
//dont be a pleb and steal it for a product or some shit. Build it and
//enjoy it as a musical instrument :)
// }

// Uses the MIDIUSB library (https://github.com/arduino-libraries/MIDIUSB) to read MIDI via USB and sync a step sequencer to the MIDI clock.

// Sends the first three channels via MIDI channel 1 and the second three via MIDI channel 2 so you can put different effects on them
// Use the ChannelChannels array below to change this, if you want.

// Pin layout: I had to change this, due to my lack of soldering skills...
// -----------
// Output LEDs from pin 4-9
// Speed potentiometer at A2
// Pattern Length potentiometer at A0
// Channel Selector at A3 (I used a poti since my rotary switch came broken)
// The big button at pin 2
// Clear button at pin 16
// Delte button at 14
// Fill button at 15
// Reset button at pin 10
// Bank button at pin 0
// Bank LED at pin 3

#include "MIDIUSB.h"

int NullPattern[12][32] = {
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
};

// For testing:
int Pattern[12][32] = {
{HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW,},
};

// LedOut pins and pitches
int Out1 = 4;
int Out2 = 5;
int Out3 = 6;
int Out4 = 7;
int Out5 = 8;
int Out6 = 9;

int Outs[6] = {Out1, Out2, Out3, Out4, Out5, Out6,};
int Pitch = 56;

// Speed potentiometer pin
int SpeedSelect = A2;
int SpeedState = 0;   // State of the speed potentiometer
int Steps = 24;       // Speed
int HalfStep = 12;
int StepFind = 0;

// Pattern length potentiometer
int PatternSelect = A0;
int PatternState = 0;     // State of the pattern length potentiometer
int Length = 4;          // Pattern length

// Channel potentiometer
int ChannelSelect = A3;
int ChannelState = 0;     // State of the channel potentiometer
int Channel = 0;          // Channel
int ChannelChannels[6] = {0, 0, 0, 1, 1, 1};

// The Big Button
int ButtonPin = 2;
int LastButtonState = LOW;
int ButtonState = LOW;
int ButtonPressed = LOW;

// Clear Button
int ClearPin = 16;
int ClearState = LOW;

// Delete Button
int DeletePin = 14;
int DeleteState = LOW;


// Fill Button
int FillPin = 15;
int FillState = LOW;

int Fill[12] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

// ResetButton
int ResetPin = 10;
int ResetState = LOW;
int LastResetState = LOW;

// BankButton
int BankPin = 0;
int BankState = LOW;
int LastBankState = LOW;

int bank = 0;
int BankLED = 3;

int RUN = LOW;        // Clock is running (MIDI)
int ADVANCE = LOW;    // Pattern advance
int PC = 0;           // Pulse Count
int POS = 0;          // Current pattern position

void StateNoteSend(int truth, byte pitch, int channel) {
  if(truth == HIGH){
    noteOn(channel, pitch, 127);
  } else if(truth == LOW){
    noteOff(channel, pitch, 0);
  }
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void setup()
{
  Serial.begin(115200);
  SpeedState = analogRead(SpeedSelect);
  PatternState = analogRead(PatternSelect);
  ChannelState = analogRead(ChannelSelect);
  for(int it = 0; it < 6; it++){
    pinMode(Outs[it], OUTPUT);
  }

  pinMode(BankLED, OUTPUT);
  pinMode(BankPin, INPUT);

  pinMode(ButtonPin, INPUT);
  pinMode(ClearPin, INPUT);
  pinMode(DeletePin, INPUT);
  pinMode(ResetPin, INPUT);
  pinMode(FillPin, INPUT);
  // Select channel
  if(ChannelState < 170) {
    Channel = 0;           // Channel 1
  } else if(ChannelState < 341) {
    Channel = 1;           // Channel 2
  } else if(ChannelState < 512) {
    Channel = 2;           // Channel 3
  } else if(ChannelState < 683) {
    Channel = 3;           // Channel 4
  } else if(ChannelState < 854) {
    Channel = 4;           // Channel 5
  } else {
    Channel = 5;           // Channel 6
  }
  
  // Select speed
  if(SpeedState < 256) {
    Steps = 24;           // Quarter Steps
    HalfStep = 12;
  } else if(SpeedState < 512) {
    Steps = 12;           // Eigth Steps
    HalfStep = 6;
  } else if(SpeedState < 768) {
    Steps = 6;            // Sixteenth Steps
    HalfStep = 3;
  } else {
    Steps = 3;           // 32th Steps
    HalfStep = 2;
  }

  // Select pattern length
  if(PatternState < 256) {
    Length = 32;
  } else if(PatternState < 512) {
    Length = 16;
  } else if(PatternState < 768) {
    Length = 8;
  } else {
    Length = 4;
  }

  digitalWrite(BankLED, HIGH);
  delay(200);
  digitalWrite(BankLED, LOW);
  delay(180);
  digitalWrite(BankLED, HIGH);
  delay(160);
  digitalWrite(BankLED, LOW);
  delay(140);
  digitalWrite(BankLED, HIGH);
  delay(120);
  digitalWrite(BankLED, LOW);
  delay(100);
  digitalWrite(BankLED, HIGH);
  delay(80);
  digitalWrite(BankLED, LOW);
  delay(60);
  digitalWrite(BankLED, HIGH);
  delay(40);
  digitalWrite(BankLED, LOW);
  delay(20);
  digitalWrite(BankLED, HIGH);
  delay(60);
  digitalWrite(BankLED, LOW);

  digitalWrite(Out1, HIGH);
  delay(100);
  digitalWrite(Out2, HIGH);
  delay(100);
  digitalWrite(Out3, HIGH);
  delay(100);
  digitalWrite(Out4, HIGH);
  delay(100);
  digitalWrite(Out5, HIGH);
  delay(100);
  digitalWrite(Out6, HIGH);
  delay(100);
  digitalWrite(Out1, LOW);
  digitalWrite(Out2, LOW);
  digitalWrite(Out3, LOW);
  digitalWrite(Out4, LOW);
  digitalWrite(Out5, LOW);
  digitalWrite(Out6, LOW);

}

midiEventPacket_t clk;

void loop() {
  clk = MidiUSB.read();
  SpeedState = analogRead(SpeedSelect);
  PatternState = analogRead(PatternSelect);
  ChannelState = analogRead(ChannelSelect);
  
  ButtonState = digitalRead(ButtonPin);
  ClearState = digitalRead(ClearPin);
  DeleteState = digitalRead(DeletePin);
  ResetState = digitalRead(ResetPin);
  BankState = digitalRead(BankPin);
  FillState = digitalRead(FillPin);

  // Select channel
  if(ChannelState < 170) {
    Channel = 0;           // Channel 1
  } else if(ChannelState < 341) {
    Channel = 1;           // Channel 2
  } else if(ChannelState < 512) {
    Channel = 2;           // Channel 3
  } else if(ChannelState < 683) {
    Channel = 3;           // Channel 4
  } else if(ChannelState < 854) {
    Channel = 4;           // Channel 5
  } else {
    Channel = 5;           // Channel 6
  }

  //Count pulses
  if(clk.byte1 == 0xF8){
     PC++;
     
     if(PC == Steps) {
        ADVANCE = HIGH;
        PC = 0;
     }
  }
  //Clock start byte
  else if(clk.byte1 == 0xFA){
    RUN = HIGH;
    ADVANCE = HIGH;
    PC = 0;
    POS = 0;
  }
  //Clock stop byte
  else if(clk.byte1 == 0xFC){
    RUN = LOW;
    PC = 0;
    POS = 0;
  }
  
  if(RUN == HIGH && ADVANCE == HIGH) {
    for(int it = 0; it < 6; it++){
      StateNoteSend(Pattern[2*it + bank][POS] || Fill[2*it], Pitch + it, ChannelChannels[it]);
      MidiUSB.flush();
      digitalWrite(Outs[it], Pattern[2*it + bank][POS] || Fill[2*it]);
    }
    ADVANCE = LOW;
    POS++;
    POS = POS % Length;
  } else {
    for(int it = 0; it < 6; it++){
      noteOff(0, Pitch + it, 0);
      digitalWrite(Outs[it], LOW);
    }
    MidiUSB.flush();
    ADVANCE = LOW;
  }
  // Select speed
  if(SpeedState < 256) {
    Steps = 24;           // Quarter Steps
    HalfStep = 12;
  } else if(SpeedState < 512) {
    Steps = 12;           // Eigth Steps
    HalfStep = 6;
  } else if(SpeedState < 768) {
    Steps = 6;            // Sixteenth Steps
    HalfStep = 3;
  } else {
    Steps = 3;           // 32th Steps
    HalfStep = 2;
  }
  // Select pattern length
  if(PatternState < 256) {
    Length = 32;
  } else if(PatternState < 512) {
    Length = 16;
  } else if(PatternState < 768) {
    Length = 8;
  } else {
    Length = 4;
  }
  if(Steps <= PC) {
    PC = 0;
  }
  if(Length <= POS) {
    POS = 0;
  }

  // Clear handling
  if(ClearState == HIGH){
    for(int i = 0; i < 12; i++) {
      Fill[i] = LOW;
      for(int j = 0; j < 32; j++){
        Pattern[i][j] = LOW;
      }
    }
  }
  // Button handling
  if(ButtonState == HIGH && LastButtonState == LOW){
    StepFind = (POS) % Length;
    Pattern[2*Channel + bank][StepFind] = HIGH;
    LastButtonState = HIGH;
  }
  
  if(ButtonState == LOW) {
      LastButtonState = LOW;
  }

  // Delete handling
  if(DeleteState == HIGH){
    Pattern[2*Channel + bank][POS] = LOW;
  }
  
  // Reset handling
  if(ResetState == HIGH && LastResetState == LOW){
    POS = 0;
  }
  LastResetState = ResetState;

  // Bank handling
  if(BankState == HIGH && LastBankState == LOW){
    if(bank == 0){
      bank = 1;
      digitalWrite(BankLED, HIGH);
    } else {
      bank = 0;
      digitalWrite(BankLED, LOW);
    }
  }
  LastBankState = BankState;

  // Fill button
  Fill[2*Channel] = FillState;
}
