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
sys.path.append('CSRNet-pytorch')
from model import CSRNet
transform=transforms.Compose([
                       transforms.ToTensor(),transforms.Normalize(mean=[0.485, 0.456, 0.406],
                                     std=[0.229, 0.224, 0.225]),
                   ])

model1 = CSRNet()

checkpoint = torch.load('CSRNet-pytorch/weights.pth', map_location="cpu")
model1.load_state_dict(checkpoint)
model = YOLO("yolov8m.pt")

# YouTube 直播影片的網址
url = "https://www.youtube.com/watch?v=2lVMTs_Fj0M&list=PLtPJtE4_gezcIXkVoH0pzSqqjN6FSjMHF&index=60"
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
img=Image.fromarray(frame)
img = transform(img.convert('RGB'))
output = model1(img.unsqueeze(0))
print("Predicted Count : ",int(output.detach().cpu().sum().numpy()))
result = model(frame1)
frame_with_detections = result[0].plot()
cv2.imwrite("live.jpg", frame)
cap.release()
cv2.destroyAllWindows()