# SmartDoorBell
A simple Smart Door Bell with face recognition and PIN entry, implemented on a BeagleBone Black running Ubuntu.
------------------------------------------
## Explanation
We developed a smart door bell for a school project on a BeagleBone Black with an Android phone attached, that allows the automatic authentication of users using their faces. Optionally, users can authenticate manually using a PIN via four buttons attached to the board and a program developed in C++.
An Android phone transmits the live video feed through HTTP and the video is fed to the BeagleBone running Ubuntu through a loopback device. The video feed is checked by a Python script that uses OpenCV and Deep Learning to authenticate the users automatically.
A simple web UI developed with Flask allows the users view the live video feed as well as the current status, unlock the door manually and change the list of authorized faces that can unlock the door.

Partnered with Christopher Yip in this project. Thanks for all of his help.

## How to Run Face Recognition
  Running the following command inside of the folder face_recognition will start the UI and Face Recognition.
  ```
  python3 web.py
  ```
  
## Media
Here are two screenshots of the face recognition module and a demo video.
<p align="center">https://youtu.be/lT3gQQcdFoE
<img src="https://user-images.githubusercontent.com/22041191/64491733-b1d4e880-d239-11e9-8ebb-e75c0a8a0db3.png">
<img src="https://user-images.githubusercontent.com/22041191/64491735-b26d7f00-d239-11e9-90ca-1436cf9e2fc9.png">
<p>
  
## Documentation
A detailed project report is available in 4221projectreport.pdf file.
