
This app is helps in using Darknet app. It is replacement for writing darknet commands in terminal. You need put AppforDarknet.exe to folder containing darkent.exe then just open AppforDarknet.exe and you can choose between training and testing models. For training you must have files obj.data and yolo-obj.cfg in folder training. File obj.data must contain number classes, path to  train,test,obj.names files and backup folder. 
Example obj.data file:  
classes = 1  
train  = C:/Darknet/build/darknet/x64/training/train.txt  
valid  = C:/Darknet/build/darknet/x64/training/test.txt  
names = C:/Darknet/build/darknet/x64/training/obj.names  
backup = C:/Darknet/build/darknet/x64/backup/  
  
For testing you can choose between detection from camera,image and video. For this you need to have files yolo-obj.cfg,yolo-obj.weights, obj.data in folder training. If you choose image or video source then you must write file name, file must be in folder with darknet.exe file.
