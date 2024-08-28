import json
data = {
    '士林夜市':{
        'url' : 'https://www.youtube.com/watch?v=1lzlw6f3zZ8&list=PLDm4hXBol5DE9QmiOdgyzYmS2jRzenhUv&index=262',
        'mask': [[0,120,0,800]]
    },
    '大稻埕':{
        'url' : "https://www.youtube.com/watch?v=ei7DiryrrQA&list=PLDm4hXBol5DE9QmiOdgyzYmS2jRzenhUv&index=203",
        'mask': [[0,147,1252,1920],[0,74,650,1252]]
    },
    '九份':{
        'url' : 'https://www.youtube.com/watch?v=dhs_KJBpIR8&list=PLDm4hXBol5DE9QmiOdgyzYmS2jRzenhUv&index=206',
        'mask': [[0,110,0,475],[1025,1080,0,1920],[0,1080,1770,1920]]
    },
    '大溪老街':{
        'url' : 'https://www.youtube.com/watch?v=XUWjAsajKXg&list=PLDm4hXBol5DE9QmiOdgyzYmS2jRzenhUv&index=99',
        'mask': [[0,42,0,270]]
    },
    '三峽老街':{
        'url' : 'https://www.youtube.com/watch?v=nW6SvKEyQkY&list=PLDm4hXBol5DE9QmiOdgyzYmS2jRzenhUv&index=86',
        'mask': [[980,1080,0,325],[0,100,1318,1920]]
    },
    '多良車站':{
        'url' : 'https://www.youtube.com/watch?v=UCG1aXVO8H8&list=PLlBCNJxYNaFzdUPNDmPbe4uXftZ8octo2&index=104',
        'mask': [[0,85,1720,1920],[1020,1080,1220,1920]]
    },
    '高美濕地':{
        'url' : 'https://www.youtube.com/watch?v=fjhg3gAnMFg&list=PLlBCNJxYNaFzdUPNDmPbe4uXftZ8octo2&index=65',
        'mask': []
    }
}
with open('live_information.json', 'w', encoding='utf-8') as file:
    json.dump(data, file, indent=4)
