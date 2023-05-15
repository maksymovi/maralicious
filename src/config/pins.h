#pragma once

//DEFINE ONE AND ONLY ONE

//#define T_DISPLAY
#define T_DISPLAY_S3

#ifdef T_DISPLAY

#define PUMP_PIN 27

//black yellow
#define MARAX_TX_PIN 26
//white orange
#define MARAX_RX_PIN 25

#define BUTTONA_PIN  0
#define BUTTONB_PIN  35


#elif defined(T_DISPLAY_S3)

#define PUMP_PIN 16

//black yellow
#define MARAX_TX_PIN 12
//white orange
#define MARAX_RX_PIN 13

#define BUTTONA_PIN  0
#define BUTTONB_PIN  35

#endif