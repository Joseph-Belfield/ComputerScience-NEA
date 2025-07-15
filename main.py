# Just use functions from Python Work

import dearpygui.dearpygui as dpg

# This creates the window, defines its name and size, etc.
dpg.create_context()
dpg.create_viewport(title='Computer Science NEA', width=1200, height=800)

# This is everything in the window. Hover over 'window' to check parameters that can be set. 
with dpg.window(label="Main Page", width=1200, height=800):
    pass

# Imports a file into this space in the code - seperate pages of the GUI into seperate files this way.
import python_source.GUI.imagePage


dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()

