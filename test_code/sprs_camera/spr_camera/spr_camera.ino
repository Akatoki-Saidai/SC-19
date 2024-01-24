/*
camera.ino - Simple camera example sketch
 */


#include <SDHCI.h>
#include <stdio.h>
#include <Camera.h>

#define BAUDRATE (1000000)
//#define BAUDRATE                (115200)

const int lineSize = 3*32;
uint8_t encoded[lineSize * 4/3 + 3];

// base64 encoder
const uint8_t base64Table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
void base64Encode(uint8_t* encoded, uint8_t* data, int dataLength){
  while(dataLength > 0){
    if(dataLength >= 3){
      encoded[0] = base64Table[data[0] >> 2];
      encoded[1] = base64Table[((data[0]&0x3) << 4) | (data[1] >> 4)];
      encoded[2] = base64Table[((data[1]&0xF) << 2) | (data[2] >> 6)];
      encoded[3] = base64Table[data[2] & 0x3F]; 
      data+=3;
      dataLength -= 3;
    }else if(dataLength == 2){
      encoded[0] = base64Table[data[0] >> 2];
      encoded[1] = base64Table[((data[0]&0x3) << 4) | (data[1] >> 4)];
      encoded[2] = base64Table[(data[1]&0xF) << 2];
      encoded[3] = '=';
      dataLength = 0;
    }else{
      encoded[0] = base64Table[data[0] >> 2];
      encoded[1] = base64Table[(data[0]&0x3) << 4];
      encoded[2] = '=';
      encoded[3] = '=';
      dataLength = 0;
    }
    encoded += 4;
  }
  *encoded = '\0';
}

/**
 * Print error message
 */

void printError(enum CamErr err)
{
  Serial.print("Error: ");
  switch (err)
  {
      case CAM_ERR_NO_DEVICE:
    Serial.println("No Device");
    break;
      case CAM_ERR_ILLEGAL_DEVERR:
    Serial.println("Illegal device error");
    break;
      case CAM_ERR_ALREADY_INITIALIZED:
    Serial.println("Already initialized");
    break;
      case CAM_ERR_NOT_INITIALIZED:
    Serial.println("Not initialized");
    break;
      case CAM_ERR_NOT_STILL_INITIALIZED:
    Serial.println("Still picture not initialized");
    break;
      case CAM_ERR_CANT_CREATE_THREAD:
    Serial.println("Failed to create thread");
    break;
      case CAM_ERR_INVALID_PARAM:
    Serial.println("Invalid parameter");
    break;
      case CAM_ERR_NO_MEMORY:
    Serial.println("No memory");
    break;
      case CAM_ERR_USR_INUSED:
    Serial.println("Buffer already in use");
    break;
      case CAM_ERR_NOT_PERMITTED:
    Serial.println("Operation not permitted");
    break;
      default:
    break;
  }
}

/**
 * Callback from Camera library when video frame is captured.
 */

void CamCB(CamImage img)
{
  /* Check the img instance is available or not. */
  if (img.isAvailable())
  {
      /* If you want RGB565 data, convert image data format to RGB565 */
      img.convertPixFormat(CAM_IMAGE_PIX_FMT_RGB565);

      /* You can use image data directly by using getImgSize() and getImgBuff().
       * for displaying image to a display, etc. */

      /*
      Serial.print("Image data size = ");
      Serial.print(img.getImgSize(), DEC);
      Serial.print(" , ");
      Serial.print("buff addr = ");
      Serial.print((unsigned long)img.getImgBuff(), HEX);
      Serial.println("");
      */
  }
    else
  {
      Serial.println("Failed to get video stream image");
  }
}

void initCamera(){
  CamErr err;

  /* begin() without parameters means that
   * number of buffers = 1, 30FPS, QVGA, YUV 4:2:2 format */
  Serial.println("Prepare camera");
  err = theCamera.begin();
  if (err != CAM_ERR_SUCCESS)
  {
      printError(err);
  }

  
  // カメラストリームを受信したら CamCBを実行する
  /*
  Serial.println("Start streaming");
  err = theCamera.startStreaming(true, CamCB);
  if (err != CAM_ERR_SUCCESS)
  {
      printError(err);
  }
  */

  // ホワイトバランスの設定
  Serial.println("Set Auto white balance parameter");
  err = theCamera.setAutoWhiteBalanceMode(CAM_WHITE_BALANCE_AUTO);
  if (err != CAM_ERR_SUCCESS)
  {
      printError(err);
  }

  // 静止画フォーマットの設定
  Serial.println("Set still picture format");
  err = theCamera.setStillPictureImageFormat(
    CAM_IMGSIZE_QVGA_H,
    CAM_IMGSIZE_QVGA_V,
    CAM_IMAGE_PIX_FMT_JPG);
  if (err != CAM_ERR_SUCCESS)
  {
    printError(err);
  }
  
  // ISOの設定
  Serial.println("Set ISO Sensitivity");
  err = theCamera.setAutoISOSensitivity(true);
  if (err != CAM_ERR_SUCCESS)
  {
    printError(err);
  }
  /*
  // ISO 値を固定する場合は setAutoISOSensitivity(false) に 
  err = theCamera.setISOSensitivity(CAM_ISO_SENSITIVITY_1600);
  if (err != CAM_ERR_SUCCESS)
  {
      printError(err);
  }
  */

  // 露出の設定
  Serial.println("Set Auto exposure");
  err = theCamera.setAutoExposure(true);
  if (err != CAM_ERR_SUCCESS)
  {
      printError(err);
  }
 /*
  // 露出を固定する場合は setAutoExposure(false) に 
  int exposure = 2740; // 最大値は 2740(NO HDR)  317(HDR) な模様
  err = theCamera.setAbsoluteExposure(exposure);
  if (err != CAM_ERR_SUCCESS)
  {
      printError(err);
  }
*/

  // HDRの設定
/*
  Serial.println("Set HDR");
  err = theCamera.setHDR(CAM_HDR_MODE_ON);
  if (err != CAM_ERR_SUCCESS)
  {
      printError(err);
  }
*/
}


void sendImageToSerial (CamImage img) {
    int inputLen = img.getImgSize();
    uint8_t* p = img.getImgBuff();
    Serial.println("#Image");
    while(inputLen > 0)
    {
      int len = inputLen > lineSize ? lineSize : inputLen;
      inputLen = inputLen - len;
      base64Encode(encoded, p, len); 
      p += len;
      Serial.println((char*)encoded);
    }
    Serial.println("#End");
}


float CaluculateHue(float red, float green, float blue, float rgbmax, float rgbmin) {
  float hue;

  if (rgbmax == rgbmin){
    hue = 0;
  }
  else if (rgbmax == red){
    hue = 60 * ((green - red) / (rgbmax - rgbmin));
  }
  else if (rgbmax == green){
    hue = 60 * ((blue - green) / (rgbmax - rgbmin)) + 120;
  }
  else if (rgbmax == blue){
    hue = 60 * ((red - blue) / rgbmax - rgbmin) + 240;
  }
  else{
    Serial.println("Unexpected error occured while caluculate HSV");
  }

  return hue;
}


uint16_t CountRedPixel(CamImage img, uint8_t zone_begin, uint8_t zone_end){
  Serial.println("Start red count...");
  uint16_t start_time = millis();

  uint16_t img_height = img.getHeight();
  uint16_t img_width = img.getWidth();
  uint16_t x_coordinate = zone_begin;
  uint16_t y_coordinate = 1;

  uint16_t red_count = 0;

  while (y_coordinate <= img_height){
    uint16_t rgb565 = img.getImgBuff()[y_coordinate * img_width + x_coordinate];
    uint8_t red565 = (rgb565 >> 8) & 0xF8;
    uint8_t green565 = (rgb565 >> 3) & 0xFC;
    uint8_t blue565 = (rgb565 << 3) & 0xF8;
    // uint16_t pixelColor = img.getPixel(x_coordinate, y_coordinate);
    
    // RGB565からRGB888
    uint8_t red888 = ((rgb565 >> 8) & 0xF8) >> 3;
    uint8_t green888 = ((rgb565 >> 3) & 0xFC) >> 2;
    uint8_t blue888 = (rgb565 & 0x1F) << 3;

    // RGBからHSV
    
    float red_treat = red888 / 255.0;
    float green_treat = green888 / 255.0;
    float blue_treat = blue888 / 255.0;
    float rgbmax = max(max(red_treat, green_treat), blue_treat);
    float rgbmin = min(min(red_treat, green_treat), blue_treat);

    float hue = CaluculateHue(red_treat, green_treat, blue_treat, rgbmax, rgbmin);
    float sat = (rgbmax - rgbmin) / rgbmax;
    float val = rgbmax;
    
    // 赤色の定義(HSV)
    float hue_min = 338.0;
    float hue_max = 22.0;
    float sat_min = 0.46;
    float val_min = 0.41;

    if ((hue <= hue_max) && (sat >= sat_min) && (val >= val_min)){
      red_count++;
    }
    else if ((hue >= hue_min) && (sat >= sat_min) && (val >= val_min)){
      red_count++;
    }
    /*
    SC-18
    hsv_min = np.array([0, 117, 104])
    hsv_max = np.array([11, 255, 255])
    
    hsv_min = np.array([169, 117, 104])
    hsv_max = np.array([179, 255, 255])
    */
    
    if (x_coordinate == zone_end){
      x_coordinate = zone_begin;
      y_coordinate++;
    }
    else{
      x_coordinate++;
    }

  }

  Serial.print("Count time: ");
  Serial.println(millis() - start_time);

  return red_count;

}


uint8_t red_detect(CamImage img) {
  // HSVに変換 -> 数える

  // 領域分け
  uint16_t img_width = img.getWidth();
  uint8_t right_end = img_width / 2 - (img_width / 4);
  uint8_t left_begin = img_width / 2 + (img_width / 4);

  // 分けた領域中の赤ピクセルのカウント
  uint16_t right_red = CountRedPixel(img, 1, right_end);
  uint16_t center_red = CountRedPixel(img, ++right_end, --left_begin);
  uint16_t left_red = CountRedPixel(img, left_begin, img_width);
  uint8_t result = 0;
  Serial.println("Right: ");
  Serial.println("right_red");
  Serial.println("Center: ");
  Serial.println(center_red);
  Serial.println("Left: ");
  Serial.println(left_red);


  Serial.println("Red object: ");
  if (right_red == max(max(right_red, center_red), left_red)){
    Serial.println("Right");
    result = 1;
  }
  if ((right_red + center_red + left_red) <= 10){
    Serial.println("Center");
    result = 2;
  }
  if ((right_red + center_red + left_red) <= 10){
    Serial.println("Left");
    result = 3;
  }
  else{
    Serial.println("CountRedPixel didn't success");
  }

  // result: 1 -> 右 2 -> 中央 3 -> 左
  // Serial.println(result);

  return result;

}


void setup(){
  Serial.begin(BAUDRATE);
  while (!Serial);
  
  initCamera();
}

void loop(){
  delay(10);
  CamImage img = theCamera.takePicture();
  if (img.isAvailable())
  {
    int iso = theCamera.getISOSensitivity();
    int exposure = theCamera.getAbsoluteExposure();
    // int hdr = theCamera.getHDR();
    Serial.print("ISO ");
    Serial.print(iso);
    Serial.print(",Exposure ");
    Serial.print(exposure);
    // Serial.print(",HDR ");
    // Serial.print(hdr);
    Serial.println();

    uint8_t red_result = red_detect(img);
    Serial.println(red_result);


    // 画像の転送(USB)
    digitalWrite(LED0, HIGH);
    sendImageToSerial(img);
    digitalWrite(LED0, LOW);

  }
}