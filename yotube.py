import cv2
import streamlink
from ultralytics import YOLO
import sys
import PIL.Image as Image
import torch
from torchvision import transforms
from datetime import datetime
import threading
currentDateAndTime = datetime.now()
sys.path.append('CSRNet-pytorch')
from model import CSRNet
import json
import random
import re
import time
random.seed(a=None, version=2)
transform=transforms.Compose([
                       transforms.ToTensor(),transforms.Normalize(mean=[0.485, 0.456, 0.406],
                                     std=[0.229, 0.224, 0.225]),
                   ])
with open('live_information.json', 'r', encoding='utf-8') as file:
    data = json.load(file)
with open('spot_information.json', 'r', encoding='utf-8') as file:
    data1 = json.load(file)
model1 = CSRNet()

checkpoint = torch.load('CSRNet-pytorch/weights.pth', map_location="cpu")
model1.load_state_dict(checkpoint)
model = YOLO("yolov8n.pt")
RAGstr=[]
lock = threading.Lock()
yolostr=[]
CSRNetstr=[]
spotstr=[]
live_id=0
def fun(id,i):
    #name=['士林夜市','大稻埕','九份','大溪老街','三峽老街','多良車站','高美濕地']
    choice=str(i)
    url = data[choice]['url']
    streams = streamlink.streams(url)
    stream_url = streams["best"].url
    f=0
    coun=0
    # 開啟影片串流
    cap = cv2.VideoCapture(stream_url)
    width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps=30
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    out=cv2.VideoWriter(f'live{id}.mp4', fourcc, fps, (width,  height))
    while(True):
        ret, frame = cap.read()
        frame1=frame.copy()
        if not ret:
            print("Cannot receive frame")
            exit()
        for i1 in data[choice]['mask']:
            frame[i1[0]:i1[1],i1[2]:i1[3],:]=0
        img=Image.fromarray(frame)
        img = transform(img.convert('RGB'))
        output = model1(img.unsqueeze(0))
        #print("Predicted Count : ",int(output.detach().cpu().sum().numpy()))
        result = model(frame)
        match = re.sub(r"\d+ persons,\s", '',result[0].verbose())
        if(f == 0):
            with lock:
                global spotstr,yolostr,CSRNetstr,live_id
                yolostr.append(match)
                CSRNetstr.append(int(output.detach().cpu().sum().numpy()))
                spotstr.append(i)
            cv2.imwrite(f"live{live_id}.jpg",frame1)
            live_id+=1
            f=1
        coun+=1
        if(coun>=1):
            break
        out.write(frame1)
    cap.release()
    cv2.destroyAllWindows()
ts=[]
st=time.time()
for id,i in enumerate(data1['spot']):
    t=threading.Thread(target=fun, args=(id, i))
    ts.append(t)
    t.start()
for i in ts:
    i.join()
ed=time.time()
print(yolostr)
print(CSRNetstr)
print(spotstr)
print(ed-st)
