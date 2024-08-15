from moviepy.editor import *
import os
import pygame
import pyttsx3
from moviepy.audio.fx.all import volumex
import pytube
import ssl
import cv2
import numpy as np
import tensorflow as tf
from moviepy.config import change_settings
import random
from pydub import AudioSegment
import streamlink
import math
import re
random.seed(a=None, version=2)
def get_url_frame(url):
    streams = streamlink.streams(url)
    stream_url = streams["best"].url
    cap = cv2.VideoCapture(stream_url)
    ret, frame = cap.read()
    return frame
def catch_mp3_sec(filename):
    try:
        audio = AudioSegment.from_file(filename)
        duration_in_seconds = len(audio) / 1000
        audio = AudioSegment.from_file(filename)
        duration_in_seconds1 = len(audio) / 1000
        return duration_in_seconds1
    except Exception as e:
        print(e)
        return 0
change_settings({"IMAGEMAGICK_BINARY": r"C:\\Program Files\\ImageMagick-7.1.1-Q16-HDRI\\magick.exe"})
ssl._create_default_https_context = ssl._create_stdlib_context
speach=["士林夜市是台灣知名的觀光景點，聚集了各式各樣的美食小吃，如小籠包、雞排、臭豆腐等。現在的士林夜市人潮滿滿，有17個人正在享受不同的美味佳餚。同時，1個背包在旁，等待著遊客的光臨。今天是晴天，陽光和煦地照耀在士林夜市的熱鬧氛圍上，呈現出一幅溫馨的風景。",
        "士林夜市是台北市的著名觀光地區，以各種美味小吃聞名。目前有一群人（17 individuals）在士林夜市裡享受美食。其中有一位遊客揹著有背包。天氣晴朗、天氣晴朗、天氣晴朗，晴天正在照射著士林夜市。",
        "士林夜市是台北市的知名觀光景點，有各式各樣的美食小吃，像是雞排、鹹酥雞、蚵仔麵線等。今天士林夜市有一群朋友正在享受美食，其中有17人和1個背包。由於天氣晴朗，大家在露天的攤位前面用餐，享受著美好的時光。"]
speach1=["大稻埕是台北市的歷史街區，以傳統文化和古色古香的建築聞名。目前，在大稻埕有一群17人的旅行者正在探索當地的歷史和文化，其中1人背著背包。今天的大稻埕天氣晴朗，陽光普照，風景十分美麗。",
         "大稻埕是台北市的一個著名地區，以其歷史文化和古蹟聞名。在大稻埕，您可以找到紅樓劇場、迪化街、大稻埕碼頭等知名景點。今天天氣晴朗，大稻埕的人潮也很熱鬧。在大稻埕，您可以看到17位朋友，其中1人正背著一個背包。",
         "大稻埕是台北市的一個歷史文化街區，由歷史悠久的商圈、傳統建築以及文創市集所組成。今天的大稻埕天氣晴朗，有17位遊客在大稻埕漫步，每個人都背了一個背包，欣賞著這個充滿文化氣息的美麗場景。"]
random_number = random.choice([0, 1, 2])
random_number1 = random.choice([0, 1, 2])
engine = pyttsx3.init()
s=re.split(r'[，。]',speach[random_number])
s1=re.split(r'[，。]',speach1[random_number])
for i,j in enumerate(s):
    engine.save_to_file(j, f"speak{i}.mp3")
for i,j in enumerate(s1):
    engine.save_to_file(j, f"speak_{i}.mp3")
engine.runAndWait()
now_audio=AudioFileClip("speak0.mp3")
now_sec=catch_mp3_sec("speak0.mp3")
for i,j in enumerate(s):
    if i==0 or j=='':
        continue
    sec=catch_mp3_sec(f"speak{i}.mp3")
    tmp=AudioFileClip(f"speak{i}.mp3")
    tmp=tmp.set_start(now_sec)
    now_sec=now_sec+sec
    now_audio=CompositeAudioClip([now_audio,tmp])
now_audio.write_audiofile("speak.mp3", fps=44100)
now_audio=AudioFileClip("speak_0.mp3")
now_sec=catch_mp3_sec("speak_0.mp3")
sec=0
for i,j in enumerate(s1):
    if i==0 or j=='':
        continue
    sec=catch_mp3_sec(f"speak_{i}.mp3")
    tmp=AudioFileClip(f"speak_{i}.mp3")
    tmp=tmp.set_start(now_sec)
    now_sec=now_sec+sec
    now_audio=CompositeAudioClip([now_audio,tmp])
now_audio.write_audiofile("speak-1.mp3", fps=44100)
duration_in_seconds=catch_mp3_sec("speak.mp3")
duration_in_seconds1=catch_mp3_sec("speak-1.mp3")
f1=get_url_frame("https://www.youtube.com/watch?v=DjdUEyjx8GM&list=PLhEMBf4e2Cl2ph7PUUQ738H4xE2Y6_dCP")
f2=get_url_frame("https://www.youtube.com/watch?v=Ndo_8RuefH4")
f1=cv2.resize(f1,(740,480))
f2=cv2.resize(f2,(740,480))
width, height = 740, 480
fps = 30
duration = math.floor(duration_in_seconds)
duration1 = math.floor(duration_in_seconds1)+math.floor(sec)+1
fourcc = cv2.VideoWriter_fourcc(*'mp4v')
video_writer = cv2.VideoWriter('output_video1.mp4', fourcc, fps, (width, height))
for _ in range(fps * duration):
    video_writer.write(f1)

video_writer = cv2.VideoWriter('output_video2.mp4', fourcc, fps, (width, height))
for _ in range(fps * duration1):
    video_writer.write(f2)

# 释放资源
video_writer.release()
audio_clip = AudioFileClip("speak.mp3")
audio_clip1 = AudioFileClip("speak-1.mp3")
audio_clip1 = audio_clip1.set_start(duration+0.5)
video = VideoFileClip("output_video1.mp4")
video1 = VideoFileClip("output_video2.mp4")
video1 = video1.set_start(duration-1).crossfadein(1)
output = CompositeVideoClip([video,video1])
final_audio = CompositeAudioClip([audio_clip,audio_clip1])
output=output.set_audio(final_audio)
output.write_videofile("output.mp4", fps=30, temp_audiofile="temp-audio.m4a", remove_temp=True, codec="libx264", audio_codec="aac")
print('ok')
time=[]
start=[]
position=[]
text=[]
txts=[]
now_sec=0
for i,j in enumerate(s):
    if j=='':
        continue
    sec=catch_mp3_sec(f"speak{i}.mp3")
    time.append(sec)
    start.append(now_sec)
    now_sec+=sec
    text.append(j)
    position.append('bottom')
for i,j in enumerate(s1):
    if j=='':
        continue
    sec=catch_mp3_sec(f"speak_{i}.mp3")
    time.append(sec)
    start.append(now_sec)
    now_sec+=sec
    text.append(j)
    position.append('bottom')
for i,sen in enumerate(text):
    txt=(TextClip(sen,fontsize=30,color='white', font='Creative.ttf').set_position(position[i]).set_duration(time[i]).set_start(start[i]))
    txts.append(txt)
output=CompositeVideoClip([output,*txts])
output.write_videofile("output.mp4",fps=30, temp_audiofile="temp-audio.m4a", remove_temp=True, codec="libx264", audio_codec="aac")
for i,j in enumerate(s):
    cap = cv2.VideoCapture(f"speak{i}.mp3")
    cap.release()
    os.remove(f"speak{i}.mp3")
for i,j in enumerate(s1):
    cap = cv2.VideoCapture(f"speak_{i}.mp3")
    cap.release()
    os.remove(f"speak_{i}.mp3")
os.remove(f"output_video1.mp4")
os.remove(f"output_video2.mp4")
os.remove(f"speak-1.mp3")
os.remove(f"speak.mp3")