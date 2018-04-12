#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
  #error Oops!  Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu.
#endif

#define LARGE_FLASH true

#define X_STEP_PIN        17
#define X_DIR_PIN         16
#define X_ENABLE_PIN      48
#define X_MIN_PIN         37
#define X_MAX_PIN         36

#define Y_STEP_PIN        54
#define Y_DIR_PIN         47
#define Y_ENABLE_PIN      55
#define Y_MIN_PIN         35
#define Y_MAX_PIN         34

#define Z_STEP_PIN        57
#define Z_DIR_PIN         56
#define Z_ENABLE_PIN      62
#define Z_MIN_PIN         33
#define Z_MAX_PIN         32

#define E0_STEP_PIN       23
#define E0_DIR_PIN        22
#define E0_ENABLE_PIN     24

#define E1_STEP_PIN       -1
#define E1_DIR_PIN        -1
#define E1_ENABLE_PIN     -1
#define SERVO0_PIN        -1
#define SERVO1_PIN        -1
#define SERVO2_PIN        -1
#define SERVO3_PIN        -1

#if ENABLED(Z_MIN_PROBE_ENDSTOP)
  // Define a pin to use as the signal pin on Arduino for the Z_PROBE endstop.
  #define Z_MIN_PROBE_PIN  33
#endif

#define FILRUNOUT_PIN     -1
#define FILWIDTH_PIN      -1 // ANALOG NUMBERING

#define FAN_PIN           8 // (Sprinter config)

#define PS_ON_PIN         45

#define HEATER_0_PIN      2
#define HEATER_1_PIN      -1
#define HEATER_2_PIN      -1

#define TEMP_0_PIN        14   // ANALOG NUMBERING
#define TEMP_1_PIN        -1   // ANALOG NUMBERING
#define TEMP_2_PIN        -1   // ANALOG NUMBERING

#define HEATER_BED_PIN    6    // BED

#define TEMP_BED_PIN      15   // ANALOG NUMBERING

#if ENABLED(Z_PROBE_SLED)
  #define SLED_PIN        -1
#endif

#define SDPOWER         -1
#define LED_PIN         13

#define BEEPER_PIN      44
#define BTN_EN1         12
#define BTN_EN2         11
#define BTN_ENC         43
#define SD_DETECT_PIN   49
#define LCD_PINS_RS     19
#define LCD_PINS_ENABLE 42
#define LCD_PINS_D4     18
#define LCD_PINS_D5     38
#define LCD_PINS_D6     41
#define LCD_PINS_D7     40
#define SDSS            53
#define KILL_PIN        46
#define SCK_PIN         52
#define MISO_PIN        50
#define MOSI_PIN        51
#define MAX6675_SS      53
  
#ifdef MINIPANEL
  #define DOGLCD_A0       41
  #define DOGLCD_CS       38
  #define LCD_PIN_BL      18
#endif

