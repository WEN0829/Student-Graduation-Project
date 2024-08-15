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
    '台北101': [25.033964, 121.564468],
    '中正紀念堂': [25.035386, 121.516994],
    '阿里山': [23.508245, 120.802216],
    '日月潭': [23.854069, 120.917998],
    '墾丁國家公園': [21.949725, 120.780817]
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
'''
import folium
import webbrowser


class Map:
    def __init__(self, center, zoom_start):
        self.center = center
        self.zoom_start = zoom_start
    
    def showMap(self):
        #Create the map
        my_map = folium.Map(location = self.center, zoom_start = self.zoom_start)

        #Display the map
        my_map.save("map.html")
        webbrowser.open("map.html")


#Define coordinates of where we want to center our map
coords = [51.5074, 0.1278]
map = Map(center = coords, zoom_start = 13)
map.showMap()
'''