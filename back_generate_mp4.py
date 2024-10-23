from moviepy.editor import *
import os
import pyttsx3
from moviepy.audio.fx.all import volumex
import ssl
import cv2
from moviepy.config import change_settings
import random
from pydub import AudioSegment
import streamlink
import math
import re
import RAG
import threading
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
        duration_in_seconds1 = len(audio) / 1000
        return duration_in_seconds1
    except Exception as e:
        print(e)
        return 0
change_settings({"IMAGEMAGICK_BINARY": r"C:\\Program Files\\ImageMagick-7.1.1-Q16-HDRI\\magick.exe"})
ssl._create_default_https_context = ssl._create_stdlib_context
res=RAG.res
def fun(id):
    engine = pyttsx3.init()
    s=re.split(r'[，。]',res[id])
    for i,j in enumerate(s):
        engine.save_to_file(j, f"{id}.speak{i}.mp3")
    engine.runAndWait()
    now_audio=AudioFileClip(f"{id}.speak0.mp3")
    now_sec=catch_mp3_sec(f"{id}.speak0.mp3")
    for i,j in enumerate(s):
        if i==0 or j=='':
            continue
        sec=catch_mp3_sec(f"{id}.speak{i}.mp3")
        tmp=AudioFileClip(f"{id}.speak{i}.mp3")
        tmp=tmp.set_start(now_sec)
        now_sec=now_sec+sec
        now_audio=CompositeAudioClip([now_audio,tmp])
    now_audio.write_audiofile(f"speak-{id}.mp3", fps=44100)
    duration_in_seconds=catch_mp3_sec(f"speak-{id}.mp3")
    f1=cv2.imread(f"live{id}.jpg")
    f1=cv2.resize(f1,(740,480))
    width, height = 740, 480
    fps = 30
    duration = math.floor(duration_in_seconds)
    #duration1 = math.floor(duration_in_seconds1)+math.floor(sec)+1
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    video_writer = cv2.VideoWriter(f'output_video{id}.mp4', fourcc, fps, (width, height))
    for _ in range(fps * duration+15):
        video_writer.write(f1)
        
    video_writer.release()
    time=[]
    start=[]
    position=[]
    text=[]
    txts=[]
    now_sec=0
    if id!=0:
        now_sec=1
    for i,j in enumerate(s):
        if j=='':
            continue
        sec=catch_mp3_sec(f"{id}.speak{i}.mp3")
        time.append(sec)
        start.append(now_sec)
        now_sec+=sec
        text.append(j)
        position.append('bottom')
    for i,sen in enumerate(text):
        txt=(TextClip(sen,fontsize=30,color='white', font='Creative.ttf').set_position(position[i]).set_duration(time[i]).set_start(start[i]))
        txts.append(txt)
    output = VideoFileClip(f"output_video{id}.mp4")
    output=CompositeVideoClip([output,*txts])
    output.write_videofile(f"output_video{id}.mp4",fps=30, temp_audiofile="temp-audio.m4a", remove_temp=True, codec="libx264", audio_codec="aac")
    for i,j in enumerate(s):
        cap = cv2.VideoCapture(f"{id}.speak{i}.mp3")
        cap.release()
        os.remove(f"{id}.speak{i}.mp3")
ts=[]
for id,i in enumerate(res):
    fun(id)
    #t=threading.Thread(target=fun,args=(id))
    #ts.append(t)
    #t.start()
#for i in ts:
#    i.join()
videos=[]
audios=[]
duration=0
for id,i in enumerate(res):
    video=VideoFileClip(f"output_video{id}.mp4")
    audio = AudioFileClip(f"speak-{id}.mp3")
    if id!=0:
        audio=audio.set_start(duration+0.5)
        video = video.set_start(duration-1).crossfadein(1)
    duration+=catch_mp3_sec(f"speak-{id}.mp3")
    videos.append(video)
    audios.append(audio)
output=CompositeVideoClip(videos)
output_audio=CompositeAudioClip(audios)
output=output.set_audio(output_audio)
output.write_videofile("output.mp4", fps=30, temp_audiofile="temp-audio.m4a", remove_temp=True, codec="libx264", audio_codec="aac")
for id,i in enumerate(res):
    os.remove(f"output_video{id}.mp4")
    os.remove(f"speak-{id}.mp3")
