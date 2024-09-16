import cv2
import streamlink
from ultralytics import YOLO
import sys
import h5py
import scipy.io as io
import PIL.Image as Image
import numpy as np
from matplotlib import pyplot as plt, cm as c
from scipy.ndimage import gaussian_filter 
import scipy
import torchvision.transforms.functional as F
import torch
from torchvision import transforms
from collections import OrderedDict
from datetime import datetime
import tensorflow as tf
currentDateAndTime = datetime.now()
sys.path.append('CSRNet-pytorch')
from model import CSRNet
import json
import random
random.seed(a=None, version=2)
def blur(frame,kernel_size):
    b,g,r=cv2.split(frame)
    b=tf.constant(b,dtype=tf.float32)
    g=tf.constant(g,dtype=tf.float32)
    r=tf.constant(r,dtype=tf.float32)
    b=tf.expand_dims(b,axis=2)
    g=tf.expand_dims(g,axis=2)
    r=tf.expand_dims(r,axis=2)
    filter=np.array(np.ones((kernel_size, kernel_size)),dtype=np.float32)*(1/kernel_size**2)
    filter=tf.convert_to_tensor(filter,dtype=tf.float32)
    filter=tf.reshape(filter,[kernel_size,kernel_size,1,1])
    b=tf.nn.conv2d(tf.expand_dims(b,axis=0),filter,strides=[1,1,1,1],padding='SAME')
    b=b.numpy()
    b=b.astype(np.uint8)
    g=tf.nn.conv2d(tf.expand_dims(g,axis=0),filter,strides=[1,1,1,1],padding='SAME')
    g=g.numpy()
    g=g.astype(np.uint8)
    r=tf.nn.conv2d(tf.expand_dims(r,axis=0),filter,strides=[1,1,1,1],padding='SAME')
    r=r.numpy()
    r=r.astype(np.uint8)
    frame=cv2.merge([b[0,:,:,0],g[0,:,:,0],r[0,:,:,0]])
    return frame
transform=transforms.Compose([
                       transforms.ToTensor(),transforms.Normalize(mean=[0.485, 0.456, 0.406],
                                     std=[0.229, 0.224, 0.225]),
                   ])
with open('D:\Graduatedproject\Student-Graduation-Project\live_information.json', 'r', encoding='utf-8') as file:
    data = json.load(file)
model1 = CSRNet()

checkpoint = torch.load('D:\Graduatedproject\Student-Graduation-Project\weights.pth', map_location="cpu")
model1.load_state_dict(checkpoint)
model = YOLO("yolov8m.pt")

# YouTube 直播影片的網址
name=['士林夜市','大稻埕','九份','大溪老街','三峽老街','多良車站','高美濕地']
choice=name[random.choice([0,1,2,3,4,5,6])]
url = data[choice]['url']
# 取得直播影片的串流
streams = streamlink.streams(url)
# 選擇最佳品質的串流
stream_url = streams["best"].url
# 開啟影片串流
cap = cv2.VideoCapture(stream_url)
ret, frame = cap.read()
frame1=frame.copy()
if not ret:
    print("Cannot receive frame")
    exit()
for i in data[choice]['mask']:
    frame[i[0]:i[1],i[2]:i[3],:]=0
if currentDateAndTime.hour<=18:#改成yolo如果判斷人數小於某個數
    #frame=blur(frame,15)
    pass
img=Image.fromarray(frame)
img = transform(img.convert('RGB'))
output = model1(img.unsqueeze(0))
print("Predicted Count : ",int(output.detach().cpu().sum().numpy()))
result = model(frame1)
frame_with_detections = result[0].plot()
cv2.imwrite("live.jpg", frame)
cap.release()
cv2.destroyAllWindows()


# import cv2
# import streamlink
# from ultralytics import YOLO
# import sys
# import h5py
# import scipy.io as io
# import PIL.Image as Image
# import numpy as np
# from matplotlib import pyplot as plt, cm as c
# from scipy.ndimage import gaussian_filter 
# import scipy
# import torchvision.transforms.functional as F
# import torch
# from torchvision import transforms
# import tensorflow as tf
# from collections import OrderedDict
# from keras.models import load_model
# from PIL import Image, ImageOps
# import matplotlib as mpl
# import matplotlib.pyplot as plt
 

# sys.path.append('CSRNet-pytorch')
# from model import CSRNet

# weather_conditions = {
#     0: "此刻為多雲的天氣",
#     1: "此刻為晴天的天氣",
#     2: "此刻為雨天的天氣",
#     3: "此刻為下雪的天氣",
#     4: "此刻為多霧的天氣"
# }

# transform = transforms.Compose([
#                        transforms.ToTensor(),transforms.Normalize(mean=[0.485, 0.456, 0.406],
#                                      std=[0.229, 0.224, 0.225]),
#                    ])

# model1 = CSRNet()
# checkpoint = torch.load('D:\Graduatedproject\Student-Graduation-Project\weights.pth', map_location="cpu")
# model1.load_state_dict(checkpoint)

# model = YOLO("yolov8m.pt")


# # YouTube 直播影片的網址
# url = "https://www.youtube.com/watch?v=4qyZLflp-sI&list=PLtPJtE4_gezdb1kTZUF4GYIjmo9Vnj5Sl"
# # 取得直播影片的串流
# streams = streamlink.streams(url)
# # 選擇最佳品質的串流
# stream_url = streams["best"].url
# # 開啟影片串流
# cap = cv2.VideoCapture(stream_url)

# # Load the weather recognition model
# weather_model = load_model("D:\\Graduatedproject\\Project\\trainedModelE10.h5")

# while True:
#     ret, frame = cap.read()
    
#     if not ret:
#         print("Failed to read")
#         break
    
#     # Weather Prediction
#     image = Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
#     image = image.resize((100, 100))
#     image = ImageOps.invert(image)
#     image_array = np.array(image)
#     image_array = image_array[np.newaxis, ...]
#     image_array = image_array / 255.0
    
#     # Predict the weather
#     weather_prediction = weather_model.predict(image_array, verbose=0)
#     weather_prediction = np.argmax(weather_prediction, axis=1)
#     weather_condition = weather_conditions.get(weather_prediction[0], "Unknown")
#     print(f"Weather Prediction: {weather_condition}")
    
#     # Display the frame with weather prediction
#     display_frame = cv2.resize(frame, (1000, 600))
#     cv2.imshow('Frame', display_frame)
    
#     # Wait for 1ms, and check if 'q' is pressed to quit
#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         break

#     # Process the frame for CSRNet after showing the frame
#     img = Image.fromarray(frame)
#     img = transform(img.convert('RGB'))
#     output = model1(img.unsqueeze(0))
#     predicted_count = int(output.detach().cpu().sum().numpy())
#     print("Predicted Count : ", predicted_count)

#     # Object detection with YOLO
#     result = model(frame)
#     frame_with_detections = result[0].plot()
    
#     display1_frame = cv2.resize(frame_with_detections, (1000, 600))
#     # Optionally display the frame with detections (replace the original display frame)
#     cv2.imshow('Frame with Detections', display1_frame)

#     # Wait for 1ms, and check if 'q' is pressed to quit
#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         break

# cap.release()
# cv2.destroyAllWindows()


# # import cv2
# # import streamlink
# # from ultralytics import YOLO
# # import sys
# # import h5py
# # import scipy.io as io
# # import PIL.Image as Image
# # import numpy as np
# # from matplotlib import pyplot as plt, cm as c
# # from scipy.ndimage import gaussian_filter 
# # import scipy
# # import torchvision.transforms.functional as F
# # import torch
# # from torchvision import transforms
# # import tensorflow as tf
# # from collections import OrderedDict

# # sys.path.append('CSRNet-pytorch')
# # from model import CSRNet

# # transform = transforms.Compose([
# #                        transforms.ToTensor(),transforms.Normalize(mean=[0.485, 0.456, 0.406],
# #                                      std=[0.229, 0.224, 0.225]),
# #                    ])

# # model1 = CSRNet()
# # checkpoint = torch.load('D:\Graduatedproject\Student-Graduation-Project\weights.pth', map_location="cpu")
# # model1.load_state_dict(checkpoint)

# # model = YOLO("yolov8m.pt")


# # # YouTube 直播影片的網址
# # url = "https://www.youtube.com/watch?v=2lVMTs_Fj0M&list=PLtPJtE4_gezcIXkVoH0pzSqqjN6FSjMHF&index=60"
# # # 取得直播影片的串流
# # streams = streamlink.streams(url)
# # # 選擇最佳品質的串流
# # stream_url = streams["best"].url
# # # 開啟影片串流
# # cap = cv2.VideoCapture(stream_url)
# # ret, frame = cap.read()
# # frame1=frame.copy()
# # if not ret:
# #     print("Cannot receive frame")
# #     exit()
# # img=Image.fromarray(frame)
# # img = transform(img.convert('RGB'))
# # output = model1(img.unsqueeze(0))
# # print("Predicted Count : ",int(output.detach().cpu().sum().numpy()))
# # result = model(frame1)
# # frame_with_detections = result[0].plot()
# # cv2.imwrite("live.jpg", frame)
# # cap.release()
# # cv2.destroyAllWindows()