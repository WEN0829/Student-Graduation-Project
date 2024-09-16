from flask import Flask, request, render_template_string, jsonify
import json
import subprocess
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

@app.route('/', methods=['GET', 'POST'])
def save_spot():
    if request.method == 'POST':
        data1 = request.json
        try:
            with open('D:\Graduatedproject\spot_information.json', 'r', encoding='utf-8') as file:
                data = json.load(file)
            if len(data) != 0:
                flag = 1
                for i in data['spot']:
                    if data1['spot'][0] == i:
                        if data1['type'] == 2:
                            data['spot'].remove(i)
                        flag = 0
                        break
                if flag:
                    for i in data['spot']:
                        data1['spot'].append(i)
                else:
                    data1 = data
        except:
            pass
        if data1['type'] == 1:
            with open('D:\Graduatedproject\spot_information.json', 'w', encoding='utf-8') as json_file:
                json.dump(data1, json_file, ensure_ascii=False, indent=4)
    return render_template_string("map_taiwan.html")

@app.route('/view_my_trip', methods=['GET'])
def view_my_trip():
    try:
        with open('D:\Graduatedproject\spot_information.json', 'r', encoding='utf-8') as file:
            data = json.load(file)
        spots = data.get('spot', [])
    except FileNotFoundError:
        spots = []
    return jsonify({'spots': spots})

@app.route('/clear_spots', methods=['POST'])
def clear_spots():
    try:
        with open('D:\Graduatedproject\spot_information.json', 'w', encoding='utf-8') as file:
            json.dump({"spot": []}, file, ensure_ascii=False, indent=4)
        return jsonify({"message": "All spots cleared successfully."}), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/generate_trip', methods=['GET'])
def generate_trip():
    subprocess.run(['python', 'D:\\Graduatedproject\\main.py'], check=True)
    subprocess.run(['python', 'D:\\Graduatedproject\\back_generate_mp4.py'], check=True)
    try:
        with open('D:\Graduatedproject\spot_information.json', 'r', encoding='utf-8') as file:
            data = json.load(file)
        spots = data.get('spot', [])
        
        return jsonify({'trip': spots}), 200
    except subprocess.CalledProcessError as e:
        return jsonify({"error": f"Failed to generate trip: {str(e)}"}), 500
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == "__main__":
    app.run(port=5000, host="0.0.0.0")


