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
with open('live_information.json', 'r', encoding='utf-8') as file:
    data = json.load(file)
model1 = CSRNet()

checkpoint = torch.load('CSRNet-pytorch/weights.pth', map_location="cpu")
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
