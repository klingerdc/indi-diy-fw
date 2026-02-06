# Arduino Upload Guide

## Required Materials
- Arduino Nano Board
- USB A to B cable
- Computer with Arduino IDE installed

## Step-by-step Instructions

### 1. Install Arduino IDE  
If you haven't already, download and install the Arduino IDE from the [official Arduino website](https://www.arduino.cc/en/software). 

### 2. Connect the Arduino Nano  
Use the USB A to B cable to connect your Arduino Nano to your computer. Make sure the connection is secure.

### 3. Open Arduino IDE  
Launch the Arduino IDE software on your computer.

### 4. Select the Arduino Nano from the Board Menu  
- Go to `Tools` > `Board` > `Arduino AVR Boards` > `Arduino Nano`.

### 5. Select the Correct Processor  
- Still under the `Tools` menu, select the correct processor based on your Nano version:  
  - `ATmega328P` (Old Bootloader) for previously manufactured boards.  
  - `ATmega328P` for newer boards.

### 6. Select the COM Port  
- Navigate to `Tools` > `Port` and select the COM port corresponding to your Arduino Nano (e.g., COM3 or /dev/ttyUSB0).

### 7. Open or Create a New Sketch  
- You can either open an existing Arduino sketch or create a new one through `File` > `New`.

### 8. Upload the Sketch  
- To upload the sketch, click on the right arrow icon (Upload) in the toolbar or press `Ctrl + U`.  
- The IDE will compile the code and then upload it to your Arduino Nano.

### 9. Monitor the Upload Process  
Keep an eye on the bottom console of the Arduino IDE for upload progress and any errors.

### 10. Verify the Upload  
Once the upload finishes, you should see the message `Done uploading` in the console. You can test your sketch functionality according to its purpose.

## Troubleshooting
- If the upload fails, double-check your connections, select the right COM port, and ensure the correct board and processor are selected.  
- Restart the Arduino IDE if necessary and try again.

## Conclusion  
You have successfully uploaded your sketch to the Arduino Nano! Now you can start working on your project.