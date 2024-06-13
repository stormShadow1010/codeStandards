#include <SD.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>

const int arrSize = 200;
const int displayTimes = 200;
File myFile;
word mem[arrSize];
int charSize[10];

Adafruit_MCP4725 dacx;
Adafruit_MCP4725 dacy;

void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
 
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
 
  // open the file for reading:
  myFile = SD.open("laser.dat");
  
  while(!myFile){
    Serial.println("ERROR: File did not open");
    myFile = SD.open("laser.dat");
  }
    
    
} 
//    Serial.println("test.txt:");
// 
//    // read from the file until there's nothing else in it:
//    while (myFile.available()) {
//        mem.append
//    	Serial.write(myFile.read());
//    }
//    // close the file:
//    myFile.close();
//  } else {
//  	// if the file didn't open, print an error:
//    Serial.println("error opening test.txt");
//  }

boolean getNextTen(){
  // mandatory 10 chars
  // Will read 4 bytes no matter what!
  int charCount = 0;
  int byteCount = 0;
  byte temp1,temp2,temp3,temp4;
  
  while (charCount<10){
    if(!myFile.available()){
      // close and reopen the file
      myFile.close();
      myFile = SD.open("laser.dat");
    }
    temp1 = myFile.read();
    if(!myFile.available()) return true;
    temp2 = myFile.read();
    if(!myFile.available()) return true;
    temp3 = myFile.read();
    if(!myFile.available()) return true;
    temp4 = myFile.read();
    
    Serial.print(temp1);
    Serial.print(" ");
    Serial.print(temp2);
    Serial.print(" ");
    Serial.print(temp3);
    Serial.print(" ");
    Serial.println(temp4);
    
    word x = word(temp1,temp2);
    word y = word(temp3,temp4);
    
    if (x == 65535){
      charCount++;
    }else{
      //add the propper data to globals
      mem[byteCount++] = x;
      mem[byteCount++] = y;
      charSize[charCount]++;
      
    }//end if else for end of char
    
  }
  return false;
}

boolean displayLoop(){
  int i = 0;
  int count = 0; // For ease of pulling data
  // while for each character
  
  while(i < 10){
    int j = 0;
    // TODO turn on laser
    // while for each point in the char
    //  only one increas in j fore each xy (2 mem words)
    while(j<charSize[i]){
      //TODO Make sure to take out the ++ on serial after testing
      Serial.println(mem[count]);
      dacx.setVoltage(mem[count++] , false);
      Serial.println(mem[count]);
      dacy.setVoltage(mem[count++] , false);
      
      delay(30);
      j++;
    }
    i++;
    // ToDo turn laser off
  }
  
}
 
void loop(){
  // Place the next 10 characters into the array
  if(getNextTen()){
    Serial.println("There was a problem reading in the points");
  }
  //Serial.println("It was able to read the points");
  // Display the characters
  int count = 0;
  while (count<displayTimes){
    displayLoop();
    //After test add to count
  }
    
}
