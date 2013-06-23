#include <SoftwareSerial.h>

const int x_pin = A0;
const int y_pin = A1;
const int z_pin = A2;

const int trigger_pin = 4;

const int BUFFER_SIZE = 20;

const int OPENLOG_BAUD_RATE = 9600; // look up in config.txt
const int OPENLOG_TERM_BYTE = 26;
const int OPENLOG_TERM_REPEAT = 3;

const int RECORD_LIMIT = 200;

const int G_SELECT = 2;

const int WRITE_DELAY = 100;

const int openlog_power_pin = 12;

boolean openLogState = true;

int bufferCounter = 0;
int recordCounter = 0;

int x_buffer[BUFFER_SIZE];
int y_buffer[BUFFER_SIZE];
int z_buffer[BUFFER_SIZE];

float x_avg, y_avg, z_avg;



SoftwareSerial openLog(8, 9); // RX, TX


void enableCommandMode() {  

  openLog.write( OPENLOG_TERM_BYTE );  
  openLog.write( OPENLOG_TERM_BYTE ); 
  openLog.write( OPENLOG_TERM_BYTE ); 

}


void printReading( String name, int value ) {
  Serial.print( "\"" );
  Serial.print( name );
  Serial.print( "\"" );
  Serial.print( ": " );
  Serial.print( value );
}

int takeReading( int axis_pin ) {  
  int reading = analogRead( axis_pin );
  return map( reading, 0, 1024, -1000, 1000 );
}

void setup() {
  Serial.begin( 9600 );
  
  while( !Serial ) {
    ;
  }
  
  // set OpenLog power to input
  pinMode( openlog_power_pin, INPUT );
  digitalWrite( openlog_power_pin, HIGH );
  
  Serial.println( "Started" );
  
  // initialize OpenLog
  openLog.begin( OPENLOG_BAUD_RATE );
  
  delay( 500 );
  openLog.println( "Hello, test" );
  delay( 500 );
  openLog.println( "Second test, bye bye" );
  delay( 500 );
}

void loop() {
  
  // add value to the buffer
  x_buffer[bufferCounter] = takeReading( x_pin );
  y_buffer[bufferCounter] = takeReading( y_pin );
  z_buffer[bufferCounter] = takeReading( z_pin );

  // increase the key index
  if ( bufferCounter < BUFFER_SIZE ) {
    bufferCounter++;
  } else {
    bufferCounter = 0; 
  }
  
  // get the averages
  x_avg = getBufferAverage( x_buffer );
  y_avg = getBufferAverage( y_buffer );
  z_avg = getBufferAverage( z_buffer );
  
  
  if ( digitalRead( trigger_pin ) == HIGH ) {
    Serial.println( "Turning off..." );
    delay( 5000 );
    digitalWrite( openlog_power_pin, LOW );
    Serial.println( "Safe to remove." );
    while(1) {
      ;
    }
  }
  
  
  // subtracts the reference value
  Serial.print( "{" );
  printReading( "x", x_avg );
  Serial.print( "," );
  printReading( "y", y_avg );
  Serial.print( "," );
  printReading( "z", z_avg );
  Serial.print( "}");
  
  Serial.println();
  
  // records every nth reading, where n is recordCounter 
  if ( recordCounter < RECORD_LIMIT ) {
    recordCounter++;
  } else {
    Serial.println( "Record" );    
    recordCounter = 0;
    openLog.println( "Test" );
    openLog.write( 26 );
    delay( 1000 );
  }
  
}

float getBufferAverage( int array[] ) {
  int sum = 0;
  
  for ( int i = 0; i < BUFFER_SIZE; i++ ) {
    sum += array[i];
  }
  
  return ( sum / BUFFER_SIZE );
}

