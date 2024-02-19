
import serial
import base64
import cv2
import numpy as np

with serial.Serial('COM4',1000000, timeout=3) as reader:
    count = 0
    
    height = 240
    width = 320
    img_size = int(240 * 320)

    list = []
    
    def rgb565ToRgb888(data_byte):

        data = int.from_bytes(data_byte, byteorder='little')
            
        r = ((data >> 11) & 0x1F)
        g = ((data >> 5) & 0x3F)
        b = (data & 0x1F)
        
        r = (r << 3) | (r >> 2);
        g = (g << 2) | (g >> 4);
        b = (b << 3) | (b >> 2);

        
        
        return (b, g, r)
    
    while count < 100:
        line = reader.readline().decode('ascii', 'ignore').rstrip()

        if line == '#Image':
            now = 0
            img = np.zeros((height, width, 3), dtype=np.uint8)
            for i in range(img_size):
                
                data = reader.read(2)
                rgb888 = rgb565ToRgb888(data)
                
                img[(now // width), (now % width)] = rgb888
                now += 1

            cv2.imshow("Image", img)


                    
        else:
            print(line)
            
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        
    reader.close()
    cv2.destroyAllWindows()
