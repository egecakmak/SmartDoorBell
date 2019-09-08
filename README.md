# SmartDoorBell
A simple Smart Door Bell with face recognition and PIN entry, implemented on a BeagleBone Black running Ubuntu.
------------------------------------------
## Explanation
Developed a smart door bell for a school project on a BeagleBone Black with an Android phone attached, that allows the automatic authentication of users using their faces. Optionally, users could authenticate manually using a PIN via four buttons attached to the board and a program developed in C++.
An Android phone would transmit the live video feed through HTTP and the video would be fed to the BeagleBone running Ubuntu through a loopback device. The video feed would be checked by a Python script that uses OpenCV and Deep Learning to authenticate the users automatically.
A simple web UI developed with Flask allows the users view the live video feed, unlock the door manually and change the list of authorized faces that can unlock the door.

Partnered with Christopher Yip in this project. Thanks for all of his help.


## Media
Here are two screenshots of the face recognition module and a demo video.
<p align="center">https://youtu.be/lT3gQQcdFoE<p>
<img src="https://user-images.githubusercontent.com/22041191/64491733-b1d4e880-d239-11e9-8ebb-e75c0a8a0db3.png">
<img src="https://user-images.githubusercontent.com/22041191/64491735-b26d7f00-d239-11e9-90ca-1436cf9e2fc9.png">
