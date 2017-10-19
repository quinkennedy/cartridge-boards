long serialSpeed = 38400;

byte* data;
uint8_t state;
uint32_t dataLength = 0;
uint32_t bytesGotten = 0;
static const uint8_t headerLength = 4;

// states
static const uint8_t WAITING_FOR_CONTACT = 0;
static const uint8_t GETTING_HEADER = WAITING_FOR_CONTACT + 1;
static const uint8_t GETTING_DATA = GETTING_HEADER + 1;
static const uint8_t STARTING_PROGRAMMING = GETTING_DATA + 1;
static const uint8_t WRITING_DATA = STARTING_PROGRAMMING + 1;
static const uint8_t DONE = WRITING_DATA + 1;


void setup() {
  state = WAITING_FOR_CONTACT;
  Serial.begin(serialSpeed);
}

void loop() {
}

void serialEvent(){
  int numBytesAvailable = Serial.available();
  if (numBytesAvailable > 0){
    if (state == WAITING_FOR_CONTACT){
      state = GETTING_HEADER;
      bytesGotten = 0;
    }
    if (state == GETTING_HEADER){
      int numBytesRead = 
        Serial.readBytes(
          ((byte*)&dataLength) + bytesGotten, 
          headerLength - bytesGotten);
      bytesGotten += numBytesRead;
      if (bytesGotten == headerLength){
        parseHeader();
      }
    }
    if (state == GETTING_DATA){
      int numBytesRead =
        Serial.readBytes(
          data + bytesGotten,
          dataLength - bytesGotten);
      bytesGotten += numBytesRead;
      if (bytesGotten == dataLength){
        state = STARTING_PROGRAMMING;
      }
    }
  }
}

void parseHeader(){
  data = (byte*)malloc(dataLength);
  bytesGotten = 0;
  state = GETTING_DATA;
}

