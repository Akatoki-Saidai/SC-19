/*
camera.ino - Simple camera example sketch
 */


#include <SDHCI.h>
#include <stdio.h>
#include <Camera.h>

#define BAUDRATE (1000000)
// #define BAUDRATE                (115200)

CamImage img;

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
    CAM_IMAGE_PIX_FMT_RGB565);
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


void sendImageToSerial (CamImage &img) {
    int inputLen = img.getImgSize();
    uint8_t* p = img.getImgBuff();
    Serial.flush();
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
    // free(p);
}


uint16_t CountRedPixel(CamImage &img, uint8_t zone_begin, uint8_t zone_end){
  // Serial.println("Start red count...");
  uint16_t start_time = millis();

  size_t img_size = img.getImgSize();
  size_t img_bufsize = img.getImgBuffSize();

  uint16_t img_height = img.getHeight();
  uint16_t img_width = img.getWidth();
  uint16_t x_coordinate = zone_begin;
  uint16_t y_coordinate = 1;

  uint16_t red_count = 0;

  while (y_coordinate <= img_height){
    // uint8_t *rgb565_1_ptr;
    // uint8_t *rgb565_2_ptr;
    uint16_t rgb565_1 = *(img.getImgBuff() + (( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 )));
    uint16_t rgb565_2 = *(img.getImgBuff() + ((( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ) ) + 1));
    // uint16_t rgb565_1 = *rgb565_1_ptr;
    // uint16_t rgb565_2 = *rgb565_2_ptr;
    uint16_t rgb565 = ( rgb565_1 << 8 ) | rgb565_2;
    // img.getImgBuff()で取得できるアドレスとその数は8bit
    // RGB565には16bit必要 -> アドレス2つ

    // RGB565からRGB888
    uint16_t red888 = (rgb565 & 0x1F);
    uint16_t green888 = ((rgb565 >> 5) & 0x3F);
    uint16_t blue888 = ((rgb565 >> 11) & 0x1F);

    // 正規化
    red888 = (red888 << 3) | (red888 >> 2);
    green888 = (green888 << 2) | (green888 >> 4);
    blue888 = (blue888 << 3) | (blue888 >> 2);

    // RGB888からHSV
    float red_treat = red888 / 255.0;
    float green_treat = green888 / 255.0;
    float blue_treat = blue888 / 255.0;
    float rgbmax = max(max(red_treat, green_treat), blue_treat);
    float rgbmin = min(min(red_treat, green_treat), blue_treat);

    // float hue = CaluculateHue(red_treat, green_treat, blue_treat, rgbmax, rgbmin);
    float hue;

    if (rgbmax == rgbmin){
      hue = 0;
      // Serial.println("rgbmax = rgbmin");
    }
    else if (rgbmax == red_treat){
      hue = 60 * ((green_treat - red_treat) / (rgbmax - rgbmin));
    }
    else if (rgbmax == green_treat){
      hue = 60 * ((blue_treat - green_treat) / (rgbmax - rgbmin)) + 120;
    }
    else if (rgbmax == blue_treat){
      hue = 60 * ((red_treat - blue_treat) / rgbmax - rgbmin) + 240;
    }
    else{
      Serial.println("Unexpected error occured while caluculate HSV");
    }

    if (hue < 0) {
      hue = hue + 360.0;
    }
    float sat = (rgbmax - rgbmin) / rgbmax;
    float val = rgbmax;
    
    float hue_min = 338.0;
    float hue_max = 22.0;
    float sat_min = 0.46;
    float val_min = 0.41;
    // 338 22 0.46 0.41

    if (((hue <= hue_max) && (sat >= sat_min) && (val >= val_min)) || ((hue >= hue_min) && (sat >= sat_min) && (val >= val_min))){
      red_count++;
      
      uint8_t *green_ptr_1;
      uint8_t *green_ptr_2;
      green_ptr_1 = img.getImgBuff() + (( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ));
      green_ptr_2 = img.getImgBuff() + ((( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ) ) + 1);
      *green_ptr_1 = ((*green_ptr_1 >> 5) | 0x07);
      *green_ptr_2 = ((*green_ptr_2 << 5) | 0xE0);
        
      // Serial.println("green_ptr?: ");
      // Serial.println(*green_ptr_1);
      // Serial.println(*green_ptr_2);

    }
    /*
    SC-18
    hsv_min = np.array([0, 117, 104])
    hsv_max = np.array([11, 255, 255])
    
    hsv_min = np.array([169, 117, 104])
    hsv_max = np.array([179, 255, 255])
    */

    // テスト用print
    /*
    Serial.print("expected rgb565: ");
    Serial.println(*(img.getImgBuff() + (( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ))));
    Serial.println(*(img.getImgBuff() + ((( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ) ) + 1)));
    Serial.print("expected rgb565 address plusver: ");
    Serial.println((int)(img.getImgBuff() + (( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ))));
    Serial.println((int)(img.getImgBuff() + ((( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ) ) + 1)));
    Serial.print("expected rgb565 address array: ");
    Serial.println(img.getImgBuff()[(( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ))]);
    Serial.println(img.getImgBuff()[((( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ) ) + 1)]);
    Serial.print("expected rgb565 address &ver: ");
    Serial.println((int)&img.getImgBuff()[(( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ))]);
    Serial.println((int)&img.getImgBuff()[((( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ) ) + 1)]);
    Serial.print("expected rgb565 address &plusver: ");
    // *(…)とBuff[]が同じ値 => Buffが返すのはアドレスの値でBuff[]にすると数値
    // Serial.println((int)(&img.getImgBuff() + (( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ))));
    // Serial.println((int)(&img.getImgBuff() + ((( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ) ) + 1)));
    // ↑エラー

    Serial.print("size: ");
    Serial.println(img_size);
    Serial.print("buf_size: ");
    Serial.println(img_bufsize);
    Serial.print("width: ");
    Serial.println(img_width);
    Serial.print("height: ");
    Serial.println(img_height);
    
    Serial.print("rgb565: ");
    Serial.println(rgb565);
    Serial.print("red888: ");
    Serial.println(red888);
    Serial.print("green888: ");
    Serial.println(green888);
    Serial.print("blue888: ");
    Serial.println(blue888);

    Serial.print("rgbmax: ");
    Serial.println(rgbmax);
    Serial.print("rgbmin: ");
    Serial.println(rgbmin);
    Serial.print("hue: ");
    Serial.println(hue);
    Serial.print("sat: ");
    Serial.println(sat);
    Serial.print("val: ");
    Serial.println(val);
    Serial.flush();
    */
    
    if (x_coordinate == zone_end){
      x_coordinate = zone_begin;
      y_coordinate++;
    }
    else{
      x_coordinate++;
    }
    if (millis() - start_time > 1500) {
      break;
      Serial.println("count time out");
    }

    
    // テスト用print
    /*
    CAM_IMAGE_PIX_FMT pixformat = img.getPixFormat();
    Serial.println(pixformat);
    Serial.println("First Pixel: ");
    Serial.println(img.getImgBuff()[0]);
    Serial.println(img.getImgBuff()[1]);
    Serial.println();
    Serial.print("expected First Pixel: ");
    Serial.println(img.getImgBuff()[( 2 * 320 * ( 1 - 1 ) ) + 2 * ( 1 - 1 )]);
    Serial.println(img.getImgBuff()[( ( 2 * 320 * ( 1 - 1 ) ) + 2 * ( 1 - 1 ) ) + 1]);
    Serial.println();
    Serial.print("last Pixel: ");
    Serial.println(img.getImgBuff()[2 * 320 * 240 - 2]);
    Serial.println(img.getImgBuff()[2 * 320  * 240 - 1]);
    Serial.println();
    Serial.print("expected last Pixel: ");
    Serial.println(img.getImgBuff()[( 2 * 320 * ( 240 - 1 ) ) + 2 * ( 320 - 1 )]);
    Serial.println(img.getImgBuff()[( ( 2 * 320 * ( 240 - 1 ) ) + 2 * ( 320 - 1 ) ) + 1]);
    Serial.println();
    */

  }

  // Serial.print("Count time: ");
  // Serial.println(millis() - start_time);

  return red_count;

}


uint8_t red_detect(CamImage &img) {
  // HSVに変換 -> 数える

  // 領域分け
  uint8_t img_width = img.getWidth();
  uint8_t center_begin = ( ( img_width / 2 ) - (img_width / 6) ) + 1;
  uint8_t right_begin = img_width / 2 + (img_width / 6);
  
  uint16_t left_red;
  uint16_t center_red;
  uint16_t right_red;

  // 分けた領域中の赤ピクセルのカウント
  uint8_t result = 0;
  left_red = CountRedPixel(img, 1, center_begin - 1 );
  Serial.print("Left: ");
  Serial.println(left_red);
  center_red = CountRedPixel(img, center_begin, right_begin - 1 );
  Serial.print("Center: ");
  Serial.println(center_red);
  right_red = CountRedPixel(img, right_begin, img_width);
  Serial.print("Right: ");
  Serial.println(right_red);


  Serial.print("Red object: ");
  if (right_red == left_red || right_red == center_red || left_red == center_red){
    Serial.println("Failed judgement");
    result = 4;
  }
  else if ((left_red + center_red + right_red) <= 10){
    Serial.println("Not found");
    result = 4;
  }
  else if (left_red == max(max(left_red, center_red), right_red)){
    Serial.println("Left");
    result = 1;
  }
  else if (center_red == max(max(left_red, center_red), right_red)){
    Serial.println("Center");
    result = 2;
  }
  else if (right_red == max(max(left_red, center_red), right_red)){
    Serial.println("Right");
    result = 3;
  }
  else{
    Serial.println("CountRedPixel didn't success");
  }
  Serial.println();

  // result: 1 -> 左 2 -> 中央 3 -> 右
  // Serial.println(result);

  return result;

}


void setup(){
  Serial.begin(BAUDRATE);
  delay(500);
  while (!Serial);

}

void loop(){
  initCamera();
  delay(10);
  for (uint8_t i = 0; i < 50; i++) {
    img = theCamera.takePicture();
    if (img.isAvailable())
    {
      /*
      int iso = theCamera.getISOSensitivity();
      int exposure = theCamera.getAbsoluteExposure();
      int hdr = theCamera.getHDR();
      Serial.print("ISO ");
      Serial.print(iso);
      Serial.print(",Exposure ");
      Serial.print(exposure);
      Serial.print(",HDR ");
      Serial.print(hdr);
      Serial.println();
      */

      uint8_t red_result = red_detect(img);
      // Serial.println(red_result);


      // 画像の転送(USB)
      digitalWrite(LED0, HIGH);
      Serial.flush();
      sendImageToSerial(img);
      Serial.flush();
      digitalWrite(LED0, LOW);
      img.~CamImage();
      
    }
  }
  theCamera.end();

}