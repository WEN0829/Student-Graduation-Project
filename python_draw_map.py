import folium
from folium.plugins import MarkerCluster
from folium import IFrame

map_taiwan = folium.Map(location=[23.6978, 120.9605], zoom_start=8, zoom_control=False, min_zoom=8, max_zoom=8)
marker_cluster = MarkerCluster().add_to(map_taiwan)

scenic_spots = {
    '士林夜市': [25.088944, 121.524583],
    '九份老街': [25.109917, 121.845222],
    '大稻埕碼頭': [25.0564, 121.50761],
    '大溪老街': [24.885128, 121.287747],
    '墾丁國家公園': [21.9503996, 120.7793784],
    '高美濕地':[24.311134, 120.549753],
    '多良車站':[22.5099205, 120.9602559],
    '合歡山武嶺亭':[24.137129, 121.2757],
    '三仙台':[23.12316, 121.40662],
    '壽山情人觀景台':[22.62548, 120.27401],
    '望高寮':[24.1451738, 120.5778491],
    '淡水漁人碼頭':[25.183110, 121.414429],
    '奮起湖':[23.5052388, 120.695607],
    '太平雲梯':[23.5599, 120.5978],
}

attractions_pictures = [
    'https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQ7xR6Mf1U1-WW1ikAgd62fr-16j3YUYJsseA&s',
    'https://newtaipei.travel/content/images/attractions/25419/1024x768_attractions-image-x_d23esh1ksgpw72a6rfkw.jpg',
    'https://www.kkday.com/zh-tw/blog/wp-content/uploads/PXL_20210408_1406285422-768x549.jpg',
    'https://snowhy.tw/wp-content/uploads/20220104200307_21.jpg',
    'https://cdn2.ettoday.net/images/531/d531255.jpg',
    'https://www.taiwanwatch.org.tw/sites/default/files/images/epaper/theme/20190726-theme.jpg',
    'https://photo.settour.com.tw/900x600/https%3A%2F%2Fwww.settour.com.tw%2Fss_img%2FGDP%2F0000%2F0007%2F77%2Fori_5294700.jpg',
    'https://cja.tw/images/Attractions/hehuanshan/wuling-960.jpg',
    'https://www.eastcoast-nsa.gov.tw/image/72384/1920x1080',
    'https://bobby.tw/wp-content/uploads/pixnet/7376b75c01f437484a1474a882e81b91.jpg',
    'https://www.welcometw.com/wp-content/uploads/2021/07/%E6%9C%9B%E9%AB%98%E5%AF%AE@jaylove791206-e1626315475158.jpg',
    'https://img.ltn.com.tw/Upload/news/600/2020/02/12/3065834_1_1.jpg',
    'https://lingling.blog/wp-content/uploads/20201029095945_20.jpg',
    'https://res.klook.com/images/fl_lossy.progressive,q_65/c_fill,w_1295,h_863/w_80,x_15,y_15,g_south_west,l_Klook_water_br_trans_yhcmh3/activities/voe05fq6fqekqsbfhpmz/%E5%98%89%E7%BE%A9%E5%A4%AA%E5%B9%B3%E9%9B%B2%E6%A2%AF%E9%96%80%E7%A5%A8.webp',
]

icon_url = 'https://raw.githubusercontent.com/HanInfinity/iron2018_FoliumAndLeaflet/master/dist/icon/marker.png'

for idx, (spot, coords) in enumerate(scenic_spots.items()):
    picture_url = attractions_pictures[idx]
    html_code = f"""
    <div>
        <h4>{spot}</h4>
        <img src="{picture_url}" width="270" height="180">
        <br><br>
        <button type="button" onclick="sendData(event)">加入我的行程</button><br><br>
        <button type="button" onclick="deletData(event)">取消我的行程</button>
    </div>
    <script>
        function sendData(event) {{
            event.preventDefault();
            var url = "http://127.0.0.1:5000";
            var data = {{ "spot": ["{spot}"],
                          "type": 1 }};
            console.log(data);
            fetch(url, {{
                method: "POST",
                body: JSON.stringify(data),
                headers: new Headers({{
                    "Content-Type": "application/json;charset=UTF-8",
                }}),
            }})
            .then((res) => res.json())
            .catch((error) => console.error("Error:", error))
            .then((response) => console.log("Success:", response));
        }}
        function deletData(event) {{
            event.preventDefault();
            var url = "http://127.0.0.1:5000";
            var data = {{ "spot": ["{spot}"],
                          "type": 2 }};
            console.log(data);
            fetch(url, {{
                method: "POST",
                body: JSON.stringify(data),
                headers: new Headers({{
                    "Content-Type": "application/json;charset=UTF-8",
                }}),
            }})
            .then((res) => res.json())
            .catch((error) => console.error("Error:", error))
            .then((response) => console.log("Success:", response));
        }}
    </script>
    """
    
    iframe = IFrame(html_code, width=320, height=300)
    popup = folium.Popup(iframe, max_width=320)
    folium.Marker(
        location=coords,
        popup=popup,
        icon=folium.features.CustomIcon(icon_url, icon_size=(30, 30))
    ).add_to(map_taiwan)

# Adding a button to view all added spots
view_button_html = """
<style>
    .view-button {
        position: absolute;
        bottom: 20px;
        right: 20px;
        z-index: 1000;
        background-color: #4CAF50;
        color: white;
        padding: 10px 24px;
        border: none;
        border-radius: 4px;
        cursor: pointer;
    }
    .modal {
        display: none;
        position: fixed;
        z-index: 1000;
        left: 0;
        top: 0;
        width: 100%;
        height: 100%;
        overflow: auto;
        background-color: rgb(0,0,0);
        background-color: rgba(0,0,0,0.4);
        padding-top: 60px;
    }
    .modal-content {
        background-color: #fefefe;
        margin: 5% auto;
        padding: 20px;
        border: 1px solid #888;
        width: 80%;
    }
    .close {
        color: #aaa;
        float: right;
        font-size: 28px;
        font-weight: bold;
    }
    .close:hover,
    .close:focus {
        color: black;
        text-decoration: none;
        cursor: pointer;
    }
</style>
<button type="button" class="view-button" onclick="viewSpots()">查看我的行程</button>
<div id="myModal" class="modal">
    <div class="modal-content">
        <span class="close" onclick="closeModal()">&times;</span>
        <h2>我的行程</h2>
        <div id="spotsContainer"></div>
    </div>
</div>
<script>
    function viewSpots() {
        var url = "http://127.0.0.1:5000/view_my_trip";
        fetch(url)
            .then(response => response.json())
            .then(data => {
                var modal = document.getElementById("myModal");
                var spotsContainer = document.getElementById("spotsContainer");
                spotsContainer.innerHTML = "";
                if (data.spots.length > 0) {
                    var ul = document.createElement("ul");
                    data.spots.forEach(spot => {
                        var li = document.createElement("li");
                        li.textContent = spot;
                        ul.appendChild(li);
                    });
                    spotsContainer.appendChild(ul);
                } else {
                    spotsContainer.innerHTML = "<p>你的行程還沒有添加任何景點。</p>";
                }
                modal.style.display = "block";
            })
            .catch(error => console.error('Error:', error));
    }
    
    function closeModal() {
        var modal = document.getElementById("myModal");
        modal.style.display = "none";
    }
    
    window.onclick = function(event) {
        var modal = document.getElementById("myModal");
        if (event.target == modal) {
            modal.style.display = "none";
        }
    }
    
</script>
"""
#Use the HTML string directly with folium.Element
view_button_element = folium.Element(view_button_html)
map_taiwan.get_root().html.add_child(view_button_element)

clear_button_html = """
<style>
    .clear-button {
        position: absolute;
        bottom: 20px;
        right: 160px; /* 調整右側位置，避免和其他按鈕重疊 */
        z-index: 1000;
        background-color: #f44336;
        color: white;
        padding: 10px 24px;
        border: none;
        border-radius: 4px;
        cursor: pointer;
    }
</style>
<button type="button" class="clear-button" onclick="clearSpots()">清除所有景點</button>
<script>
    function clearSpots() {
        var url = "http://127.0.0.1:5000/clear_spots";
        fetch(url, {
            method: "POST",
        })
        .then(response => response.json())
        .then(data => {
            alert(data.message);
            // Optional: 更新UI或重新載入頁面
        })
        .catch(error => console.error('Error:', error));
    }
</script>
"""

clear_button_element = folium.Element(clear_button_html)
map_taiwan.get_root().html.add_child(clear_button_element)

generate_trip_button_html = """
<style>
    .generate-trip-button {
        position: absolute;
        bottom: 20px;
        right: 300px;
        z-index: 1000;
        background-color: #2196F3;
        color: white;
        padding: 10px 24px;
        border: none;
        border-radius: 4px;
        cursor: pointer;
    }
</style>
<button type="button" class="generate-trip-button" onclick="generateTrip()">生成我的行程</button>
<script>
    function generateTrip() {
        var url = "http://127.0.0.1:5000/generate_trip";
        fetch(url)
            .then(response => response.json())
            .then(data => {
                // Optionally handle generated trip data here, e.g., display on map
                alert("行程已生成！請查看我的行程。");
            })
            .catch(error => console.error('Error:', error));
    }
</script>
"""

generate_trip_button_element = folium.Element(generate_trip_button_html)
map_taiwan.get_root().html.add_child(generate_trip_button_element)


map_taiwan.save(r"D:\\Graduatedproject\\index.html")