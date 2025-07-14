# Imports dearpygui 
import dearpygui.dearpygui as dpg


# This code only runs if this file specifically is run - not if imported
if __name__ == "__main__": 

    # Demo of GUI capabilities
    import dearpygui.dearpygui as dpg
    import dearpygui.demo as demo

    dpg.create_context()
    dpg.create_viewport(title='Custom Title', width=600, height=600)

    demo.show_demo()

    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()