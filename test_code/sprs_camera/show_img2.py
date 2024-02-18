
import serial
import base64
import cv2
import numpy as np

with serial.Serial('COM4',1000000, timeout=3) as reader:
    count = 0
    Imagereceive = False
    
    height = 240
    width = 320
    img_size = int(240 * 320)
    
    a = 0
    b = 0

    list = []
    '''
    def rgb565_to_rgb888(image_data):
 
        r = ((image_data >> 11) & 0x1F)
        g = ((image_data >> 5) & 0x3F)
        b = (image_data & 0x1F)

        r = (r << 3) | (r >> 2)
        g = (g << 2) | (g >> 4)
        b = (b << 3) | (b >> 2)

        rgb = b << 11 | g << 5 | r
        return
    '''
    def convert_to_image(data_byte):
        height = 240
        width = 320
        # img = np.zeros((height, width, 3), dtype=np.uint8)

        print("calculating...")

        image_data = np.array(data, dtype=np.uint8).reshape(height, width)
        img = cv2.cvtColor(image_data, cv2.COLOR_RGB565_BGR)
        
        """
        for y in range(height):
            for x in range(width):
                pixel = data[y][x]
                r = (pixel >> 11) & 0b11111
                g = (pixel >> 5) & 0b111111
                b = pixel & 0b11111
                r = (r << 3) | (r >> 2)  # 5bitから8bitに拡張
                g = (g << 2) | (g >> 4)  # 6bitから8bitに拡張
                b = (b << 3) | (b >> 2)  # 5bitから8bitに拡張
                img[y][x] = [b][g][r]  # OpenCVはBGR形式を使用するので注意
        """

        return img

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

        """
        if line == '#End':
            Imagereceive = False

            b = 0
            count = count + 1
            continue
        """
        if line == '#Image':
            Imagereceive = True
            now = 0
            img = np.zeros((height, width, 3), dtype=np.uint8)
            for i in range(img_size):
                
                data = reader.read(2)
                rgb888 = rgb565ToRgb888(data)
                
                img[(now // width), (now % width)] = rgb888
                now += 1

                """
                a += 1
                
                if a >= 1000:
                    print("receiving...\n")
                    b += 1
                    print(b)
                    a = 0
                """

            cv2.imshow("Image", img)


                    
        else:
            print(line)
            
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        
    reader.close()
    cv2.destroyAllWindows()
