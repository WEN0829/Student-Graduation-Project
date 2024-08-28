import folium
from folium.plugins import MousePosition
from IPython.display import display
import ipywidgets as widgets
from folium import IFrame
from ipywidgets import Button, Output, VBox
import webbrowser
output = Output()
map_taiwan = folium.Map(location=[23.6978, 120.9605], zoom_start=8, zoom_control=False, min_zoom=8, max_zoom=8)

scenic_spots = {
    '士林夜市': [25.05, 121.31],
    '九份老街': [25.111661, 121.804139],
    '大稻埕馬頭': [25.03, 121.30],
    '大溪老街': [25.026326, 121.738516],
    '墾丁國家公園': [24.933441, 121.369747]
}

for spot, coords in scenic_spots.items():
    js_code = f"""
        console.log('{spot}');
        alert('{spot}');
    """
    iframe = IFrame(f"<script>{js_code}</script>", width=0, height=0)
    popup1 = folium.Popup(iframe)
    folium.CircleMarker(
        location=coords,
        radius=8,
        color='blue',
        fill=True,
        fill_color='blue',
        popup=popup1
    ).add_to(map_taiwan)
map_taiwan.add_child(folium.ClickForMarker(popup=None).add_to(map_taiwan))
map_taiwan.save("map_taiwan.html")
display(output)
