#include <Control_Surface.h>
#include <Adafruit_NeoPixel.h>

USBMIDI_Interface MIDI_USB;
HardwareSerialMIDI_Interface MIDI_SERIAL{ Serial1 };

MIDI_PipeFactory<3> pipes;

#define PINPIXELS 15
#define NUMPIXELS 144
Adafruit_NeoPixel pixels(NUMPIXELS, PINPIXELS, NEO_GRB + NEO_KHZ800);

// ============================================================================
// boltPiano Colors
// ============================================================================
#define color_BLACK 0, 0, 0

uint32_t rainbowColors[] = {
  pixels.Color(232, 0, 0),    // rainbow1 - C
  pixels.Color(255, 100, 0),  // rainbow2 - C#
  pixels.Color(255, 180, 0),  // rainbow3 - D
  pixels.Color(128, 255, 0),  // rainbow4 - D#
  pixels.Color(0, 255, 0),    // rainbow5 - E
  pixels.Color(0, 255, 62),   // rainbow6 - F
  pixels.Color(0, 255, 255),  // rainbow7 - F#
  pixels.Color(0, 62, 255),   // rainbow8 - G
  pixels.Color(0, 0, 232),    // rainbow9 - G#
  pixels.Color(128, 0, 255),  // rainbow10 - A
  pixels.Color(255, 0, 255),  // rainbow11 - A#
  pixels.Color(255, 0, 128)   // rainbow12 - B
};

int noteStart = 24;
int noteMinusOne = 58;  // -1 pixel at this note (inc.)

// Colors from C 24 to C xx
/*int noteToColorIndex[] = {
  0, 1, 2, 1, 2, 11, 1, 2, 1, 2, 1, 2,
  0, 1, 2, 1, 2, 11, 1, 2, 1, 2, 1, 2,
  0, 1, 2, 1, 2, 11, 1, 2, 1, 2, 1, 2,
  0, 1, 2, 1, 2, 11, 1, 2, 1, 2, 1, 2,
  0, 1, 2, 1, 2, 11, 1, 2, 1, 2, 1, 2,
  0, 1, 2, 1, 2, 11, 1, 2, 1, 2, 1, 2,
  0, 1, 2, 1, 2, 11, 1, 2, 1, 2, 1, 2,
  0, 1, 2, 1, 2, 11, 1, 2, 1, 2, 1, 2
};*/
int noteToColorIndex[] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};

// ============================================================================
// boltPiano Callback
// ============================================================================
struct MyMIDI_Callbacks : FineGrainedMIDI_Callbacks<MyMIDI_Callbacks> {

  void onNoteOff(Channel channel, uint8_t note, uint8_t velocity, Cable cable) {
    if (note >= noteStart) {
      int index = note - noteStart;
      int pixelOffset = 0;
      if (note > noteMinusOne) {
        pixelOffset = -1;
      }
      pixels.setPixelColor(2 * index + pixelOffset, pixels.Color(color_BLACK));
      pixels.show();
    }
  }

  void onNoteOn(Channel channel, uint8_t note, uint8_t velocity, Cable cable) {
    if (note >= noteStart) {
      int index = note - noteStart;
      int pixelOffset = 0;
      if (note > noteMinusOne) {
        pixelOffset = -1;
      }
      uint32_t ledColor;
      ledColor = rainbowColors[noteToColorIndex[index]];

      if (velocity > 0) {
        /*if(velocity < 20){
          ledColor = pixels.Color(10, 10, 10);
        }*/
        pixels.setPixelColor(2 * index + pixelOffset, ledColor);
        pixels.show();
      } else {
        pixels.setPixelColor(2 * index + pixelOffset, pixels.Color(color_BLACK));
        pixels.show();
      }
    }
  }
} callback;

// ============================================================================
// Pi Pico Setup
// ============================================================================
void setup() {
  MIDI_SERIAL >> pipes >> MIDI_USB;
  MIDI_SERIAL << pipes << MIDI_USB;

  MIDI_SERIAL.begin();
  MIDI_SERIAL.setCallbacks(callback);

  MIDI_USB.begin();
  MIDI_USB.setCallbacks(callback);

  pixels.begin();
  pixels.setBrightness(80);

  boltPianoInit();

  pixels.clear();
  pixels.show();
}

// ============================================================================
// Pi Pico Loop
// ============================================================================
void loop() {
  MIDI_USB.update();
  MIDI_SERIAL.update();
}


// ============================================================================
// boltPiano PowerOn
// ============================================================================
void boltPianoInit() {
  int offsetindicateInit = 71;

  pixels.clear();
  pixels.setPixelColor(offsetindicateInit + 0, rainbowColors[0]);
  pixels.setPixelColor(offsetindicateInit + 2, rainbowColors[1]);
  pixels.setPixelColor(offsetindicateInit + 4, rainbowColors[2]);
  pixels.setPixelColor(offsetindicateInit + 6, rainbowColors[3]);
  pixels.setPixelColor(offsetindicateInit + 8, rainbowColors[4]);
  pixels.setPixelColor(offsetindicateInit + 10, rainbowColors[5]);
  pixels.setPixelColor(offsetindicateInit + 12, rainbowColors[6]);
  pixels.setPixelColor(offsetindicateInit + 14, rainbowColors[7]);
  pixels.setPixelColor(offsetindicateInit + 16, rainbowColors[8]);
  pixels.setPixelColor(offsetindicateInit + 18, rainbowColors[9]);
  pixels.setPixelColor(offsetindicateInit + 20, rainbowColors[10]);
  pixels.setPixelColor(offsetindicateInit + 22, rainbowColors[11]);
  pixels.setPixelColor(offsetindicateInit + 24, rainbowColors[0]);
  pixels.show();

  delay(2000);
}