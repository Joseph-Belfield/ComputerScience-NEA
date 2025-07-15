import dearpygui.dearpygui as dpg

width, height, channels, data = dpg.load_image("images/test_chemMeme.jpg")

with dpg.texture_registry(show=True):
    dpg.add_static_texture(width=width, height=height, default_value=data, tag="chemMeme_tag")

with dpg.window(label="Testing Photos"):
    dpg.add_image("chemMeme_tag")