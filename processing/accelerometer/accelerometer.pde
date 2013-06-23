import org.json.*;

import processing.serial.*;
import java.util.regex.Pattern;

Serial serialPort;
String messageBuffer = "";

// 
int cx, cy, cz;

// previous values for delta calculation
int pcx, pcy, pcz;

// delta
int dcx, dcy, dcz;

void setup() {
  // reduce the framerate
  frameRate( 30 );
  size( 900, 300 );
  colorMode( HSB, 256 );

  println( Serial.list() );

  serialPort = new Serial( this, Serial.list()[0], 9600 );
  serialPort.clear();

  serialPort.bufferUntil( 10 );
}

boolean requested = false;

void draw() {
  
  if ( messageBuffer != "" ) {
    try {
      pcx = cx;
      pcy = cy;
      pcz = cz;
      
      JSON responseObject = JSON.parse( messageBuffer );
       cx = ( responseObject.getInt( "x" ) );
       cy = ( responseObject.getInt( "y" ) );
       cz = ( responseObject.getInt( "z" ) );
      
      dcx = pcx - cx;
      dcy = pcy - cy;
      dcz = pcz - cz;
      
      println( messageBuffer );
    } 
    
    catch ( RuntimeException e ) {

      println( "Unable to parse response" );
      println( messageBuffer );
    }
    // do not call clear() here
    messageBuffer = "";
  }
  
  fill( dcx, dcx, dcx );
  rect(0, 0, 300, 300 );
  
  fill( dcy, dcy, dcy );
  rect(300, 0, 300, 300 );
  
  fill( dcz, dcz, dcz );
  rect(600, 0, 300, 300 );
  
}

void serialEvent( Serial p ) {
  messageBuffer = p.readString();
}

