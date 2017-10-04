#simple GUI

from Tkinter import *
import Tkinter as tk
import serial
import serial.tools.list_ports
from PIL import ImageTk, Image

ports = list(serial.tools.list_ports.comports())
for p in ports:
    print p

if "Arduino" in p[1]:
        print "This is an Arduino!"
        print p[0]
        arduinoData = serial.Serial(p[0],9600,timeout=0,writeTimeout=0)

#Functions
def start():
    arduinoData.write('101a')

def stop():
    arduinoData.write('100a')
    
def frequency_change(freq):
    input_freq = freq + 'a'
    arduinoData.write(input_freq)

def mode_change(mode):
    if mode== 'Separation Mode':
        command = '200a'
        img = ImageTk.PhotoImage(Image.open("GUI Separation Mode.jpg"))
        panel.configure(image = img)
        panel.image = img
    if mode == 'Concentration Mode':
        command = '201a'
        img = ImageTk.PhotoImage(Image.open("GUI Concentration Mode.jpg"))
        panel.configure(image = img)
        panel.image = img
    if mode == 'Reverse Separation':
        command = '202a'
        img = ImageTk.PhotoImage(Image.open("GUI Reverse Separation.jpg"))
        panel.configure(image = img)
        panel.image = img
    if mode == 'Reverse Concentration':
        command = '203a'
        img = ImageTk.PhotoImage(Image.open("GUI Reverse Concentration.jpg"))
        panel.configure(image = img)
        panel.image = img
    arduinoData.write(command)

def start_pump():
    arduinoData.write('301a')

def stop_pump():
    arduinoData.write('300a')

def prime_pump():
    arduinoData.write('302a')



#create window
root=Tk()

#modify root window
root.title("Ferrologix QMACS Control")

logo = ImageTk.PhotoImage(Image.open("FerrologixLogo.jpg"))
panel = tk.Label(root, image = logo)
panel.grid(row=0,columnspan=10)

start = Button(root, text="Run Sample",command=start)
start.grid(row=1,column=0,columnspan=2)

stop = Button(root,text="STOP RUN",command=stop)
stop.grid(row=2,column=0,columnspan=2)


mode_lbl = Label(root,text="Modes:")
mode_lbl.grid(row=3,sticky=E)

var2=tk.IntVar(root)
var2.set('Select here')
choices2=['Separation Mode','Concentration Mode','Reverse Separation','Reverse Concentration']
option2=tk.OptionMenu(root,var2,*choices2,command=mode_change)
option2.grid(row=3,column=1,padx=20,sticky=W)


freq_lbl = Label(root,text="Frequency (Hz):")
freq_lbl.grid(row=4,sticky=E)

var1=tk.IntVar(root)
var1.set('Select here')
choices1=[]
for n in range(0,11):
    choices1.insert(n,str(n+5))
    

option1 = tk.OptionMenu(root,var1,*choices1,command=frequency_change)
option1.grid(row=4,column=1,padx=20,sticky=W)

img = ImageTk.PhotoImage(Image.open("GUI QMACS Graphic.jpg"))
panel = tk.Label(root, image = img)
panel.grid(row=1, column=5,rowspan=10,columnspan=5,ipadx=50,ipady=20,sticky=N)

##pump_lbl = Label(root,text="Pump:")
##pump_lbl.grid(row=6,column=0,sticky=E)

start_pump = Button(root,text="Start Pump",command = start_pump)
start_pump.grid(row=6,column=0,sticky=E)

stop_pump = Button(root,text="Stop Pump",command = stop_pump)
stop_pump.grid(row=7,column=0,columnspan=2)

prime_pump = Button(root, text="Prime System",command = prime_pump)
prime_pump.grid(row=6, column=1,padx=20,sticky=W)

##displacement_lbl = Label(root,text="Chatter displacement:")
##displacement_lbl.grid(row=7,column=0,sticky=E)
 

##Output Feed
##def readSerial():
##    while True:
##        line = arduinoData.read(3)
##        if len(line) == 0 or line == '\r':
##            break
##            
##        else:
##            output_log.delete("1.0",END)
##            output_log.insert(END, line)
##            
##    root.after(10, readSerial)
##
##root.after(1000, readSerial)

##readout_lbl = Label(root,text="Comments:")
##readout_lbl.grid(row=8,padx=10,pady=10)

##output_log = Text(root,width=5,height=1,takefocus=0)
##output_log.grid(row=8,column=1)

#start event loop
##root.update_idletasks()
root.mainloop()


