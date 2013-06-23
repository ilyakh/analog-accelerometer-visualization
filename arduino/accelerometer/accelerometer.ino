const int x_pin = A0;
const int y_pin = A1;
const int z_pin = A2;

const int calibration_pin = 13;

const int BUFFER_SIZE = 20;

int bufferCounter = 0;

int x_buffer[BUFFER_SIZE];
int y_buffer[BUFFER_SIZE];
int z_buffer[BUFFER_SIZE];

float x_avg, y_avg, z_avg;

int x_ref, y_ref, z_ref;



void printReading( String name, int value ) {
  Serial.print( "\"" );
  Serial.print( name );
  Serial.print( "\"" );
  Serial.print( ": " );
  Serial.print( value );
}

int takeReading( int axis_pin ) {
  return analogRead( axis_pin );
}

void setup() {
  Serial.begin( 9600 );
  Serial.println( "Started" );
}

void loop() {
  
  // add value to the buffer
  x_buffer[bufferCounter] = takeReading( x_pin );
  y_buffer[bufferCounter] = takeReading( y_pin );
  z_buffer[bufferCounter] = takeReading( z_pin );

  // increase the key index
  if ( bufferCounter < BUFFER_SIZE ) {
    bufferCounter++;
  } 
  else {
    bufferCounter = 0; 
  }

  // get the averages
  x_avg = getBufferAverage( x_buffer );
  y_avg = getBufferAverage( y_buffer );
  z_avg = getBufferAverage( z_buffer );
  
  if ( digitalRead( calibration_pin ) == HIGH ) {
    x_ref = x_avg;
    y_ref = y_avg;
    z_ref = z_avg;
    delay( 2000 );
  }
  
  // emulate json without the library
  Serial.print( "{" );
  printReading( "x", x_avg - x_ref );
  Serial.print( "," );
  printReading( "y", y_avg - y_ref );
  Serial.print( "," );
  printReading( "z", z_avg - z_ref );
  Serial.print( "}");
  
  Serial.println();

}

float getBufferAverage( int array[] ) {
  int sum = 0;
  
  for ( int i = 0; i < BUFFER_SIZE; i++ ) {
    sum += array[i];
  }
  
  return ( sum / BUFFER_SIZE );
}

