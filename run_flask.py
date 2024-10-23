from flask import Flask, request, render_template_string, jsonify
import json
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

@app.route('/', methods=['GET', 'POST'])
def save_spot():
    if request.method == 'POST':
        data1 = request.json
        try:
            with open('spot_information.json', 'r', encoding='utf-8') as file:
                data = json.load(file)
            flag = 1
            for i in data['spot']:
                if data1['spot'][0] == i:
                    flag = 0
                    break
            if flag:
                for i in data['spot']:
                    data1['spot'].append(i)
            else:
                data1 = data
        except:
            pass
        with open('spot_information.json', 'w', encoding='utf-8') as json_file:
            json.dump(data1, json_file, ensure_ascii=False, indent=4)
    return render_template_string("map_taiwan.html")

@app.route('/delete', methods=['GET', 'POST'])
def delete():
    if request.method == 'POST':
        data1 = request.json
        try:
            with open('spot_information.json', 'r', encoding='utf-8') as file:
                data = json.load(file)
            for i in data['spot']:
                if data1['spot'][0] == i:
                    data['spot'].remove(i)
                    break
        except:
            pass
        with open('spot_information.json', 'w', encoding='utf-8') as json_file:
            json.dump(data, json_file, ensure_ascii=False, indent=4)
    return render_template_string("map_taiwan.html")

@app.route('/view_my_trip', methods=['GET'])
def view_my_trip():
    try:
        with open('spot_information.json', 'r', encoding='utf-8') as file:
            data = json.load(file)
        spots = data.get('spot', [])
    except FileNotFoundError:
        spots = []
    return jsonify({'spots': spots})

@app.route('/clear_my_trip', methods=['GET'])
def clear_my_trip():
    tmp={}
    with open('spot_information.json', 'w', encoding='utf-8') as file:
        json.dump(tmp, file, ensure_ascii=False, indent=4)
    return jsonify(tmp)

@app.route('/post_my_trip', methods=['GET'])
def post_my_trip():
    tmp={}
    print("start")
    import back_generate_mp4
    return jsonify(tmp)



app.run()
