# SEC-DEF-CON-33-Oreo-Badge
*Not for actual consumption

![O.R.E.O. Badge Exploded View](images/ExplodedView_sm.png?raw=true "O.R.E.O. Badge Exploded View")

![O.R.E.O. Badge Guide](images/Guide.png?raw=true "O.R.E.O. Badge Guide")

## Printable Stand

There is a 3D printable stand included to show off your badge! `Display Stand.stl`

## Hacking

See below for Arduino environment setup, programming guide, and some tips for adding your own lighting modes!

For those not needing much help, here is some pertinent technical data:
- MCU is an RP2040, the PCB is basically a Pi Pico clone
- The WS2812 string of LEDs is connected to GPIO 2. There are 23 total LEDs (16 around the outer ring, 7 in the center logo)
- The battery management IC is a TI BQ25895 and connected via I2C. <b>See below for important info on charge current.</b> The I2C address is 0x6A.
- The capactive touch is handled by an MPR121 and connected via I2C. The I2C address is 0x5A.
- I2C SDA is GPIO 0
- I2C SCL is GPIO 1
- Maximum recommended LED brightness is 70% (179 out of 255)

![O.R.E.O. Badge LED Map](images/LEDs_sm.png?raw=true "O.R.E.O. LED Map")

### Environment

The O.R.E.O. badge is effectively a Pi Pico clone with an RP2040.

Use [Arduino IDE](https://www.arduino.cc/en/software/#ide) (2.3.6+ Recommended)

Install the Pi Pico board support package
- Navigate to `File > Preferences` (on Windows/Linux) or `Arduino IDE > Preferences` (on macOS)
- In the Preferences dialog, locate the "Additional Boards Manager URLs" field
- Add the following URL to this field:
`https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json`
- Click "OK" to save the preferences
- Go to `Tools > Board > Boards Manager...`
- In the Boards Manager, search for "pico"
- Locate "Arduino Mbed OS RP2040 Boards" (or a similar name indicating support for the RP2040 microcontroller)
- Click "Install" to download and install the necessary files. This might take a few minutes
- After installation, go to `Tools > Board > Arduino Mbed OS RP2040 Boards` (or the equivalent entry)
- Select `Raspberry Pi Pico`

Install the Adafruit_MPR121 Library
- Navigate to `Sketch > Include Library > Manage Libraries...` in the Arduino IDE menu. This will open the Library Manager window.
- In the search bar of the Library Manager, type "Adafruit MPR121"
- Locate "Adafruit MPR121 by Adafruit" in the search results. Select the latest version from the dropdown menu if multiple versions are available, then click the "Install" button

Install the FastLED Library
- Navigate to `Sketch > Include Library > Manage Libraries...` in the Arduino IDE menu. This will open the Library Manager window.
- In the search bar of the Library Manager, type "FastLED"
- Locate "FastLED" in the search results. Select the latest version from the dropdown menu if multiple versions are available, then click the "Install" button

### Programming

The Badge can be programmed directly from the Arduino IDE without disassembling it
- Open the included Arduino sketch (SEC_OREO.ino)
- Ensure the target board is "Raspberry Pi Pico"
- Ensure the correct COM port is selected in `Tools > Port`
- Click the Upload Button, or click `Sketch > Upload`

An alternative method is to program the badge with bootloader/UF2 mode. This requires access to the electronics PCB inside the badge
- Ensure the power switch is set to 'Off'
- Plug the badge into the USB port of your computer
- Hold down the button on the badge while powering it on with the power switch
- The badge will now show up on your computer as a USB Mass Storage drive
- Drag `SEC_OREO_v1.uf2` or your own .uf2 file to the drive
- The badge automatically reboots and starts running your firmware

### Creating new lighting modes

You can add your own lighting modes to the badge

Create a new function for your lighting mode. The function prototype should follow others in the firmware
`void yourMode(CRGB* leds, uint8_t count)`

If you desire to have some type of linear flow up and down, there is an array provided `const uint8_t rows[][5]` to assist you in enumerating across the badge in any direction
See the example `secColors` mode for an example of its usage

You can create a separate lighting mode of the outer ring of the badge, and the inner logo of the badge

Add your function name to the following two arrays, and they will automatically insert themselves into the toggles of the lighting modes
```
AnimationFunc outerAnimations[]
AnimationFunc innerAnimations[]
```

Mix and match your own modes, or combine them with the ones provided in the firmware!

### Code considerations

Maximum recommended brightness is 179 `uint8_t ledBrightness = 179;` The LEDs won't get much brighter any higher, and this will help conserve your battery life and ensure you don't exceed the recommended current capacity of the traces on the board, or the power regulators.

The TI BQ25895 Lithium Battery Management IC is not connected to the data lines of USB. Therefore, it <b>won't exceed 500mA of current draw during charging.</b> You may be able to bypass this if you try hard enough, going through the datasheet and fiddling with the registers over I2C. For the traces placed on the board, <b>the maximum current you should charge at is 1A.</b>

The Li-Po battery included has a recommended normal charging current of 0.2C (600mA) and a maximum charging current of 1C (3A). The built in power management circuit within the Li-Po battery will always limit current at 2.5A.

