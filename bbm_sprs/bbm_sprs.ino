/*
camera.ino - Simple camera example sketch
 */


#include <SDHCI.h>
#include <stdio.h>
#include <Camera.h>
#include <GNSS.h>
#include <Watchdog.h>
#include <Wire.h>

#define BAUDRATE (1000000)
#define BAUDRATE2 (31250)

#define STRING_BUFFER_SIZE  128       /**< %Buffer size */
#define RESTART_CYCLE       (60 * 5)  /**< positioning test term */
static SpGnss Gnss;                   /**< SpGnss object */

String pico_message;
bool Launch;
uint8_t phase;



// GPS関数

/**
 * @enum ParamSat
 * @brief Satellite system
 */
enum ParamSat {
  eSatGps,            /**< GPS                     World wide coverage  */
  eSatGlonass,        /**< GLONASS                 World wide coverage  */
  eSatGpsSbas,        /**< GPS+SBAS                North America        */
  eSatGpsGlonass,     /**< GPS+Glonass             World wide coverage  */
  eSatGpsBeidou,      /**< GPS+BeiDou              World wide coverage  */
  eSatGpsGalileo,     /**< GPS+Galileo             World wide coverage  */
  eSatGpsQz1c,        /**< GPS+QZSS_L1CA           East Asia & Oceania  */
  eSatGpsGlonassQz1c, /**< GPS+Glonass+QZSS_L1CA   East Asia & Oceania  */
  eSatGpsBeidouQz1c,  /**< GPS+BeiDou+QZSS_L1CA    East Asia & Oceania  */
  eSatGpsGalileoQz1c, /**< GPS+Galileo+QZSS_L1CA   East Asia & Oceania  */
  eSatGpsQz1cQz1S,    /**< GPS+QZSS_L1CA+QZSS_L1S  Japan                */
};

/* Set this parameter depending on your current region. */
static enum ParamSat satType =  eSatGps;

/**
 * @brief Turn on / off the LED0 for CPU active notification.
 */
static void Led_isActive(void)
{
  static int state = 1;
  if (state == 1)
  {
    ledOn(PIN_LED0);
    state = 0;
  }
  else
  {
    ledOff(PIN_LED0);
    state = 1;
  }
}

/**
 * @brief Turn on / off the LED1 for positioning state notification.
 *
 * @param [in] state Positioning state
 */
static void Led_isPosfix(bool state)
{
  if (state)
  {
    ledOn(PIN_LED1);
  }
  else
  {
    ledOff(PIN_LED1);
  }
}

/**
 * @brief Turn on / off the LED3 for error notification.
 *
 * @param [in] state Error state
 */
static void Led_isError(bool state)
{
  if (state)
  {
    ledOn(PIN_LED3);
  }
  else
  {
    ledOff(PIN_LED3);
  }
}


/**
 * @brief %Print position information.
 */
static void print_pos(SpNavData *pNavData)
{
  char StringBuffer[STRING_BUFFER_SIZE];

  /* print time */
  snprintf(StringBuffer, STRING_BUFFER_SIZE, ":Tim%04d%02d%02d", pNavData->time.year, pNavData->time.month, pNavData->time.day);
  Serial.print(StringBuffer);
  Serial2.print(StringBuffer);
  I2Cprint(StringBuffer);


  snprintf(StringBuffer, STRING_BUFFER_SIZE, "%02d%02d%02d", pNavData->time.hour, pNavData->time.minute, pNavData->time.sec);
  Serial.println(StringBuffer);
  Serial2.println(StringBuffer);
  I2Cprint(StringBuffer);
  I2Cprint("\n");

  /* print satellites count */
  // snprintf(StringBuffer, STRING_BUFFER_SIZE, "numSat:%2d, ", pNavData->numSatellites);
  // Serial.print(StringBuffer);

  /* print position data */

  
  if (pNavData->posFixMode == FixInvalid)
  {
    Serial.println("No-Fix");
    I2Cprint("No-Fix\n");
  }
  else
  {
    Serial.println("Fix");
    I2Cprint("Fix\n");
  }
  if (pNavData->posDataExist == 0)
  {
    Serial.println("No Position");
    I2Cprint("No Position");

    Serial.println(":Lat0.00000000");
    Serial2.println(":Lat0.00000000");
    I2Cprint(":Lat0.00000000");

    Serial.println(":Lon0.00000000");
    Serial2.println(":Lon0.00000000");
    I2Cprint(":Lon0.00000000");
    
  }
  else
  {
    snprintf(StringBuffer, STRING_BUFFER_SIZE, ":Lat%08f", pNavData->latitude);
    Serial.println(StringBuffer);
    Serial2.println(StringBuffer);
    I2Cprint(StringBuffer);

    snprintf(StringBuffer, STRING_BUFFER_SIZE, ":Lon%08f", pNavData->longitude);
    Serial.println(StringBuffer);
    Serial2.println(StringBuffer);
    I2Cprint(StringBuffer);

  }
}

/**
 * @brief %Print satellite condition.
 */
static void print_condition(SpNavData *pNavData)
{
  char StringBuffer[STRING_BUFFER_SIZE];
  unsigned long cnt;

  /* Print satellite count. */
  snprintf(StringBuffer, STRING_BUFFER_SIZE, "numSatellites:%2d\n", pNavData->numSatellites);
  Serial.print(StringBuffer);

  for (cnt = 0; cnt < pNavData->numSatellites; cnt++)
  {
    const char *pType = "---";
    SpSatelliteType sattype = pNavData->getSatelliteType(cnt);

    /* Get satellite type. */
    /* Keep it to three letters. */
    switch (sattype)
    {
      case GPS:
        pType = "GPS";
        break;

      case GLONASS:
        pType = "GLN";
        break;

      case QZ_L1CA:
        pType = "QCA";
        break;

      case SBAS:
        pType = "SBA";
        break;

      case QZ_L1S:
        pType = "Q1S";
        break;

      case BEIDOU:
        pType = "BDS";
        break;

      case GALILEO:
        pType = "GAL";
        break;

      default:
        pType = "UKN";
        break;
    }

    /* Get print conditions. */
    unsigned long Id  = pNavData->getSatelliteId(cnt);
    unsigned long Elv = pNavData->getSatelliteElevation(cnt);
    unsigned long Azm = pNavData->getSatelliteAzimuth(cnt);
    float sigLevel = pNavData->getSatelliteSignalLevel(cnt);

    /* Print satellite condition. */
    snprintf(StringBuffer, STRING_BUFFER_SIZE, "[%2ld] Type:%s, Id:%2ld, Elv:%2ld, Azm:%3ld, CN0:", cnt, pType, Id, Elv, Azm );
    Serial.print(StringBuffer);

    Serial.println(sigLevel, 6);
  }
}





// カメラ関数

/**
 * Print error message
 */

void printError(enum CamErr err)
{
  Serial.print("Error: ");
  I2Cprint("Error: ");

  switch (err)
  {
      case CAM_ERR_NO_DEVICE:
    Serial.println("No Device");
    I2Cprint("No Device\n");
    break;
      case CAM_ERR_ILLEGAL_DEVERR:
    Serial.println("Illegal device error");
    I2Cprint("Illegal device error\n");
    break;
      case CAM_ERR_ALREADY_INITIALIZED:
    Serial.println("Already initialized");
    I2Cprint("Already initialized\n");
    break;
      case CAM_ERR_NOT_INITIALIZED:
    Serial.println("Not initialized");
    I2Cprint("Not initialized\n");
    break;
      case CAM_ERR_NOT_STILL_INITIALIZED:
    Serial.println("Still picture not initialized");
    I2Cprint("Still picture not initialized\n");
    break;
      case CAM_ERR_CANT_CREATE_THREAD:
    Serial.println("Failed to create thread");
    I2Cprint("Failed to create thread\n");
    break;
      case CAM_ERR_INVALID_PARAM:
    Serial.println("Invalid parameter");
    I2Cprint("Invalid parameter\n");
    break;
      case CAM_ERR_NO_MEMORY:
    Serial.println("No memory");
    I2Cprint("No memory\n");
    break;
      case CAM_ERR_USR_INUSED:
    Serial.println("Buffer already in use");
    I2Cprint("Buffer already in use\n");
    break;
      case CAM_ERR_NOT_PERMITTED:
    Serial.println("Operation not permitted");
    I2Cprint("Operation not permitted\n");
    break;
      default:
    break;
  }
}

/**
 * Callback from Camera library when video frame is captured.
 */

void CamCB(CamImage &img)
{
  
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

    Serial.print(":Cam");
    Serial2.print(":Cam");
    I2Cprint(":Cam");
    
    uint8_t red_result = red_detect(img);
    Serial.println(red_result);
    Serial2.println(red_result);
    I2Cprint(red_result);
    I2Cprint("\n");


    // 画像の転送(USB)
    /*
    if (Serial){
      digitalWrite(LED0, HIGH);
      Serial.flush();
      sendByteImage(img);
      Serial.flush();
      digitalWrite(LED0, LOW);
    }
    */

  }

  else {
    Serial.println("Failed to get video stream image");
    I2Cprint("Failed to get video stream image\n");
  }

}

void initCamera(){
  CamErr err;

  /* begin() without parameters means that
   * number of buffers = 1, 30FPS, QVGA, RGB565 format */
  Serial.println("Prepare camera");
  err = theCamera.begin(
    1,
    CAM_VIDEO_FPS_5,
    CAM_IMGSIZE_QVGA_H,
    CAM_IMGSIZE_QVGA_V,
    CAM_IMAGE_PIX_FMT_RGB565,
    7
  );
  if (err != CAM_ERR_SUCCESS)
  {
      printError(err);
  }

  
  // カメラストリームを受信したら CamCBを実行する
  // Serial.println("Start streaming");
  // err = theCamera.startStreaming(true, CamCB);
  // if (err != CAM_ERR_SUCCESS)
  // {
  //     printError(err);
  // }


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

  // 露出の設定
  Serial.println("Set Auto exposure");
  err = theCamera.setAutoExposure(true);
  if (err != CAM_ERR_SUCCESS)
  {
      printError(err);
  }

}

void sendByteImage (CamImage img) {
    size_t img_size = img.getImgSize();
    uint8_t byte;
    Serial.println("#Image");
    Serial.flush();
    delay(2000);
    for (size_t i = 0; i < img_size; i++)
    {
      byte = img.getImgBuff()[i];
      Serial.write(byte);
    }
    Serial.println();
    delay(1000);
    // free(p);
}


uint16_t CountRedPixel(CamImage img, uint16_t zone_begin, uint16_t zone_end){
  // Serial.println("Start red count...");
  uint16_t start_time = millis();

  size_t img_size = img.getImgSize();

  uint16_t img_height = img.getHeight();
  uint16_t img_width = img.getWidth();
  uint16_t x_coordinate = zone_begin;
  uint16_t y_coordinate = 1;

  uint16_t red_count = 0;

  while (y_coordinate <= img_height){
    // RGB565取得
    uint16_t rgb565_1 = *(img.getImgBuff() + (( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 )));
    uint16_t rgb565_2 = *(img.getImgBuff() + ((( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 )) + 1));
    uint16_t rgb565 = ( rgb565_1 << 8 ) | rgb565_2;

    // RGB565からRGB888
    uint16_t red888 = ((rgb565 >> 11) & 0x1F);
    uint16_t green888 = ((rgb565 >> 5) & 0x3F);
    uint16_t blue888 = (rgb565 & 0x1F);

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
      
      // 赤部分を緑に置換
      // uint8_t *green_ptr_1;
      // uint8_t *green_ptr_2;
      // green_ptr_1 = img.getImgBuff() + (( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ));
      // green_ptr_2 = img.getImgBuff() + ((( 2 * img_width * ( y_coordinate - 1 ) ) + 2 * ( x_coordinate - 1 ) ) + 1);
      // *green_ptr_1 = ((*green_ptr_1 >> 5) | 0x07);
      // *green_ptr_2 = ((*green_ptr_2 << 5) | 0xE0);
        
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
    Serial.println();
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


uint8_t red_detect(CamImage img) {
  // HSVに変換 -> 数える

  // 領域分け
  uint16_t img_width = img.getWidth();
  uint16_t center_begin = ( ( img_width / 2 ) - (img_width / 8) ) + 1;
  uint16_t right_begin = (img_width / 2) + (img_width / 8);
  
  uint16_t left_red;
  uint16_t center_red;
  uint16_t right_red;

  // 分けた領域中の赤ピクセルのカウント
  uint8_t result = 0;
  left_red = CountRedPixel(img, 1, center_begin - 1 );
  Serial.print("Left: ");
  Serial.println(left_red);
  I2Cprint("Left: ");
  I2Cprint(left_red);
  I2Cprint("\n");
  
  center_red = CountRedPixel(img, center_begin, right_begin - 1 );
  Serial.print("Center: ");
  Serial.println(center_red);
  I2Cprint("Center: ");
  I2Cprint(center_red);
  I2Cprint("\n");

  right_red = CountRedPixel(img, right_begin, img_width);
  Serial.print("Right: ");
  Serial.println(right_red);
  I2Cprint("Right: ");
  I2Cprint(right_red);
  I2Cprint("\n");


  Serial.print("Red object: ");
  I2Cprint("Red object: ");

  if (right_red == left_red || right_red == center_red || left_red == center_red){
    Serial.println("Failed judgement");
    I2Cprint("Failed judgement\n");
    result = 4;
  }
  else if ((left_red + center_red + right_red) <= 10){
    Serial.println("Not found");
    I2Cprint("Not found\n");
    result = 4;
  }
  else if (left_red == max(max(left_red, center_red), right_red)){
    Serial.println("Left");
    I2Cprint("Left\n");
    Watchdog.kick();
    result = 1;
  }
  else if (center_red == max(max(left_red, center_red), right_red)){
    Serial.println("Center");
    I2Cprint("Center\n");
    Watchdog.kick();
    result = 2;
  }
  else if (right_red == max(max(left_red, center_red), right_red)){
    Serial.println("Right");
    I2Cprint("Right\n");
    Watchdog.kick();
    result = 3;
  }
  else{
    Serial.println("CountRedPixel didn't success");
    I2Cprint("CountRedPixel didn't success\n");
  }
  Serial.println();

  // result: 1 -> 左 2 -> 中央 3 -> 右
  // Serial.println(result);

  return result;

}

void I2Cprint(const char* message){
  // Twe-liteのI2Cアドレス要確認
  Wire.beginTransmission(0x54);
  Wire.write(message, strlen(message));
  Wire.endTransmission();

}


void setup(){
  Serial.begin(BAUDRATE);
  Serial2.begin(BAUDRATE2);
  delay(500);
  // while (!Serial);
  
  Wire.begin();

  // GPSセットアップ
  
  int error_flag = 0;

  /* Set serial baudrate. */
  Serial.begin(1000000);

  /* Wait HW initialization done. */
  sleep(3);

  /* Turn on all LED:Setup start. */
  ledOn(PIN_LED0);
  ledOn(PIN_LED1);
  ledOn(PIN_LED2);
  ledOn(PIN_LED3);

  /* Set Debug mode to Info */
  Gnss.setDebugMode(PrintInfo);

  int result;

  /* Activate GNSS device */
  result = Gnss.begin();

  if (result != 0)
  {
    Serial.println("Gnss begin error!!");
    I2Cprint("Gnss begin error!!\n");
    error_flag = 1;
  }
  else
  {
    /* Setup GNSS
     *  It is possible to setup up to two GNSS satellites systems.
     *  Depending on your location you can improve your accuracy by selecting different GNSS system than the GPS system.
     *  See: https://developer.sony.com/develop/spresense/developer-tools/get-started-using-nuttx/nuttx-developer-guide#_gnss
     *  for detailed information.
    */
    switch (satType)
    {
    case eSatGps:
      Gnss.select(GPS);
      break;

    case eSatGpsSbas:
      Gnss.select(GPS);
      Gnss.select(SBAS);
      break;

    case eSatGlonass:
      Gnss.select(GLONASS);
      Gnss.deselect(GPS);
      break;

    case eSatGpsGlonass:
      Gnss.select(GPS);
      Gnss.select(GLONASS);
      break;

    case eSatGpsBeidou:
      Gnss.select(GPS);
      Gnss.select(BEIDOU);
      break;

    case eSatGpsGalileo:
      Gnss.select(GPS);
      Gnss.select(GALILEO);
      break;

    case eSatGpsQz1c:
      Gnss.select(GPS);
      Gnss.select(QZ_L1CA);
      break;

    case eSatGpsQz1cQz1S:
      Gnss.select(GPS);
      Gnss.select(QZ_L1CA);
      Gnss.select(QZ_L1S);
      break;

    case eSatGpsBeidouQz1c:
      Gnss.select(GPS);
      Gnss.select(BEIDOU);
      Gnss.select(QZ_L1CA);
      break;

    case eSatGpsGalileoQz1c:
      Gnss.select(GPS);
      Gnss.select(GALILEO);
      Gnss.select(QZ_L1CA);
      break;

    case eSatGpsGlonassQz1c:
    default:
      Gnss.select(GPS);
      Gnss.select(GLONASS);
      Gnss.select(QZ_L1CA);
      break;
    }

    /* Start positioning */
    result = Gnss.start(COLD_START);
    if (result != 0)
    {
      Serial.println("Gnss start error!!");
      I2Cprint("Gnss start error!!\n");
      error_flag = 1;
    }
    else
    {
      Serial.println("Gnss setup OK");
      I2Cprint("Gnss setup OK\n");
    }
  }

  /* Start 1PSS output to PIN_D02 */
  //Gnss.start1PPS();

  /* Turn off all LED:Setup done. */
  ledOff(PIN_LED0);
  ledOff(PIN_LED1);
  ledOff(PIN_LED2);
  ledOff(PIN_LED3);

  /* Set error LED. */
  if (error_flag == 1)
  {
    Led_isError(true);
    exit(0);
  }


  // カメラ起動状態
  Launch = false;
  phase = 0;

  Watchdog.begin();

}

void loop(){

  // UART受信
  if (Serial2.available() > 0){
    pico_message = Serial2.readStringUntil('\n'); 
    
    Serial.print("UART: ");
    Serial.println(pico_message);
    // I2Cprint("UART: ");
    // I2Cprint(pico_message\n);

  }
  
  if (pico_message == "CameraStart"){
    phase = 1;
  }

  switch(phase){
    case 0:
      // GPSループ
      static int LoopCount = 0;
      static int LastPrintMin = 0;

      /* Blink LED. */
      Led_isActive();

      /* Check update. */
      if (Gnss.waitUpdate(-1))
      {
        /* Get NaviData. */
        SpNavData NavData;
        Gnss.getNavData(&NavData);

        /* Set posfix LED. */
        bool LedSet = (NavData.posDataExist && (NavData.posFixMode != FixInvalid));
        Led_isPosfix(LedSet);

        /* Print satellite information every minute. */
        if (NavData.time.minute != LastPrintMin)
        {
          print_condition(&NavData);
          LastPrintMin = NavData.time.minute;
        }

        /* Print position information. */
        print_pos(&NavData);
      }
      else
      {
        /* Not update. */
        Serial.println("data not update");
        I2Cprint("data not update\n");
      }              

      /* Check loop count. */
      LoopCount++;
      if (LoopCount >= RESTART_CYCLE)
      {
        int error_flag = 0;

        /* Turn off LED0 */
        ledOff(PIN_LED0);

        /* Set posfix LED. */
        Led_isPosfix(false);

        /* Restart GNSS. */
        if (Gnss.stop() != 0)
        {
          Serial.println("Gnss stop error!!");
          I2Cprint("Gnss stop error!!\n");
          error_flag = 1;
        }
        else if (Gnss.end() != 0)
        {
          Serial.println("Gnss end error!!");
          I2Cprint("Gnss end error!!\n");
          error_flag = 1;
        }
        else
        {
          Serial.println("Gnss stop OK.");
          I2Cprint("Gnss stop OK.\n");
        }

        if (Gnss.begin() != 0)
        {
          Serial.println("Gnss begin error!!");
          I2Cprint("Gnss begin error!!\n");
          error_flag = 1;
        }
        else if (Gnss.start(HOT_START) != 0)
        {
          Serial.println("Gnss start error!!");
          I2Cprint("Gnss start error!!\n");
          error_flag = 1;
        }
        else
        {
          Serial.println("Gnss restart OK.");
          I2Cprint("Gnss restart OK.\n");
        }

        LoopCount = 0;

        /* Set error LED. */
        if (error_flag == 1)
        {
          Led_isError(true);
          exit(0);
        }
      }
      break;

    case 1:
      // カメラループ
      if (Launch == 0){
        initCamera();
        delay(10);
        Serial.println("CameraStart");
        I2Cprint("CameraStart\n");
        Watchdog.start(5000);

        theCamera.startStreaming(true, CamCB);

        Launch = true;
      }
      
      /*
      if (Launch > 0){
        Serial.end();
        Serial2.end();
        delay(500);
        Serial.begin(BAUDRATE);
        Serial2.begin(BAUDRATE2);
        Serial.println("Reset UART");
        if (CameraReset > 15){
          theCamera.end();
          initCamera();
          Serial.println("CameraReset");
          theCamera.startStreaming(true, CamCB);
          CameraReset = 0;
        }
        */

        // theCamera.startStreaming(true, CamCB);
        /*
          if (img.isAvailable())
          {
            
            // int iso = theCamera.getISOSensitivity();
            // int exposure = theCamera.getAbsoluteExposure();
            // int hdr = theCamera.getHDR();
            // Serial.print("ISO ");
            // Serial.print(iso);
            // Serial.print(",Exposure ");
            // Serial.print(exposure);
            // Serial.print(",HDR ");
            // Serial.print(hdr);
            // Serial.println();
            

            uint8_t red_result = red_detect(img);
            // Serial.println(red_result);


            // 画像の転送(USB)
            digitalWrite(LED0, HIGH);
            Serial.flush();
            sendByteImage(img);
            Serial.flush();
            digitalWrite(LED0, LOW);
              
            }
            */
    break;
  }
  Serial2.flush();
  delay(1000);

}
