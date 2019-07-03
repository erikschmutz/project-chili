#include "Wire.h"

/* Choices of which range the device should operate on */
class RangeChoices {
  public :
    RangeChoices() {

    }
    static const int ADDRESS = 0x11;

    static const int AD7150_RANGE_0_5 = 0x4B;
    static const int AD7150_RANGE_1 = 0x8B;
    static const int AD7150_RANGE_2 = 0x0B;
    static const int AD7150_RANGE_4 = 0xCB;

    static const float getValue(int range) {
      switch (range) {
        case 0x4B :
          return 0.5;
        case 0x8B :
          return 1;
        case 0x0B :
          return 2;
        case 0xCB :
          return 4;
      }
      return 0;
    }
};


class OffsetChoices {
  public :
    static const int ADDRESS = 0x09;

    static const int AD7150_OFFSET_DAC_AUTO  = 0x80;
    static const int AD7150_OFFSET_DAC_0 = 0x81;
    static const int AD7150_OFFSET_DAC_1 = 0x88;
    static const int AD7150_OFFSET_DAC_2 = 0x8F;
    static const int AD7150_OFFSET_DAC_3 = 0x97;
    static const int AD7150_OFFSET_DAC_4 = 0x9E;
    static const int AD7150_OFFSET_DAC_5 = 0xA7;
    static const int AD7150_OFFSET_DAC_6 = 0xAE;
    static const int AD7150_OFFSET_DAC_7 = 0xB6;
    static const int AD7150_OFFSET_DAC_8 = 0xBF;



    static const int getValueFromIndex(int index) {
      int list [9] = {0x81, 0x88, 0x8F, 0x97, 0x9E, 0xA7, 0xAE, 0xB6, 0xBF};
      return list[index];
    }
    static const float getValue(int offset) {
      int list [9] = {0x81, 0x88, 0x8F, 0x97, 0x9E, 0xA7, 0xAE, 0xB6, 0xBF};
      for (int i = 0; i < 9; i++)
        if (list[i] == offset) return i;
      return 0;
    }
};

class AD7150 {

    /* Address the device is located at */
    int I2CADDRESS = 0x48;

    int range;
    int offset;
    int autoRange = false;


  public:
    uint8_t SING_MODE = 0x12;
    uint8_t POWER_DOWN_MODE = 0x13;
    AD7150() {
      /* Initilizes connection with arduino */
      Wire.begin();

    }
    void setup() {
      /* sets sensitivity */
      writeByteToMemory(0x09, 0x08);
      writeByteToMemory(RangeChoices::ADDRESS, range);
      writeByteToMemory(0x0F, 0x12);
      writeByteToMemory(0x10, 0x00);
      writeByteToMemory(OffsetChoices::ADDRESS, offset);
    }
    void writeByteToMemory(uint8_t address, uint16_t value) {

      Wire.begin();
      Wire.beginTransmission(I2CADDRESS);
      Wire.write(address);
      Wire.write(value);
      Wire.endTransmission();
      delay(4);

    }
    uint8_t getByteFromMemory(uint8_t address){
      Wire.beginTransmission(I2CADDRESS);   // use the address found with the I2C Scanner
      Wire.write(address);    // set register pointer to Chip ID
      Wire.endTransmission();



      uint8_t data;

      Wire.requestFrom(I2CADDRESS, 1, true);
      for (int i = 0; Wire.available(); i++)
        data = Wire.read();

      return data;
        
    }
    float getCap() {
      

      //(float)(getValue()
      float value = getValue();
      if(value < 12288.0 ||  value > 40944)
      return -1;
      
      value =   (value - 12288.0) / 40944.0 * RangeChoices::getValue(range) + OffsetChoices::getValue(offset);
      return value;
    }
    int getOffset(){
      return 1;
    }
    uint16_t getValue() {

      // setup();

      uint8_t data[3];

      Wire.requestFrom(I2CADDRESS, 3, true);
      for (int i = 0; Wire.available(); i++)
        data[i] = Wire.read();
     
      
      uint16_t value =  data[1] << 8 | data[2] ;


      if ( !autoRange )
        return value;

      if ( value < 0x3000 && OffsetChoices::getValue(offset) > 0){
        setOffset(OffsetChoices::getValueFromIndex((int)(OffsetChoices::getValue(offset)) - 1));
        return value;    
      }
 

      if ( value > 0xFFA0 && OffsetChoices::getValue(offset) < 8){
        setOffset(OffsetChoices::getValueFromIndex((int)(OffsetChoices::getValue(offset)) + 1));
        return value;
      }
      
      return value;
    }

    void setOffset(int offsetIN) {
      offset = offsetIN;
    }

    void setRange(int rangeIN) {
      range = rangeIN;
    }
    void setAutoRange(boolean on) {
      autoRange = on;
    }
    void conf(uint8_t mode) {
      writeByteToMemory(0x0F, mode);
      delay(10);
    }
    int getRang() {

    }

};


AD7150 device = AD7150();


void setup()
{

  device.setOffset(OffsetChoices::AD7150_OFFSET_DAC_0);
  device.setRange(RangeChoices::AD7150_RANGE_4);
  //device.setAutoRange(true);
  device.setup();
  
  Serial.begin(115200);

}

void loop()
{
  device.conf(device.SING_MODE);
  device.conf(device.POWER_DOWN_MODE);

  float value = device.getCap();
  
  Serial.println(value, 6);
  delay(5000);
  
}
