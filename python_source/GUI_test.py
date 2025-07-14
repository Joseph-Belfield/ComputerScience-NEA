
# GUI

import dearpygui.dearpygui as dpg

def helloWorld():
    dpg.set_value(item='john',value='helloWorld')       # Function which links to tag with a matching value.

def save_value():
    inputText = dpg.get_value(item='inputText')
    dpg.set_value(item='outputText',value=inputText)

# This creates the window, defines its name and size, etc.
dpg.create_context()
dpg.create_viewport(title='testFile', width=600, height=300)

# This is everything in the window. Hover over 'window' to check parameters that can be set. 
with dpg.window(label="Main Page", width=600, height=300):
    dpg.add_text("Hello, world")        # Prints text.
    outputText = dpg.add_text(tag='john')           # Assigns variable a tag, which is like an ID to be called later. Prints associated variable when tag called.
    dpg.add_button(label="Save", callback=helloWorld)           # Simple button. Calls above function when pressed.

    dpg.add_input_text(label="string", default_value="Quick brown fox",tag='inputText')     # This set of code, along with the save_value function takes an input and prints it
    dpg.add_button(label='Submit',callback=save_value)                                      # A text box is filled, submit is pressed and then the input is printed below
    dpg.add_text(tag='outputText')

    dpg.add_slider_float(label="float", default_value=0.273, max_value=1)

dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()