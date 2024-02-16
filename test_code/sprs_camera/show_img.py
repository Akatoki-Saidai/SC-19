import serial
import base64
import cv2
import numpy as np

with serial.Serial('COM4',1000000, timeout=3) as reader:
    count = 0

    readingBase64 = False
    list = []
    while count < 100:
        line = reader.readline().decode('ascii', 'ignore').strip()
        
        # 画像部分を切り出す
        if line.startswith('#Image'):
            readingBase64 = True
            continue
        if line.startswith('#End'):
            buffer = base64.b64decode(''.join(list));
            list.clear()
            data = np.frombuffer(buffer, dtype=np.uint8)
            image = cv2.imdecode(data, cv2.IMREAD_COLOR)
            cv2.imshow("Camera",image)
            cv2.waitKey(1) 
            count = count + 1
            readingBase64 = False
            continue

        if readingBase64:
            list.append(line)
        else:
            print(line)
    cv2.destroyAllWindows()
