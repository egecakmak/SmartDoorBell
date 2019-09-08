# web.py

from flask import Flask, render_template, Response, jsonify, request
from camera import VideoCamera
import os
import recognize_video
import authorizationweb

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        if request.form["unlock"] == "Unlock Door":
            return render_template('index.html', unlock="The door has been unlocked.")
    else:
        return render_template('index.html')

def generator(camera):
    while True:
        frame = camera.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

@app.route('/video_feed')
def video_feed():
    return Response(generator(VideoCamera()),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/recognized_person')
def recognized_person():
    result = recognize_video.run()
    if authorizationweb.is_authorized(result):
        info = {
            "name": result,
            "unlock": "Person is authorized. The door has been unlocked."
        }
        return jsonify(info)
    else:
        info = {
            "name": result,
            "unlock": "Person is not authorized. The door can only be unlocked manually."
        }
        return jsonify(info)


@app.route('/authorize', methods=['GET', 'POST'])
def authorize_person():
    if request.method == "POST":
        if request.form["authorize"] == "Authorize User":
            result = authorizationweb.add_authorized_user(request.form['username'])
            text = "User "
            if result:
                text += request.form['username'] + " is successfully authorized."
            else:
                text += request.form['username'] + " is not successfully authorized."
            users = []
            for username in os.listdir('./dataset'):
                user = {'name': username, 'is_authorized': authorizationweb.is_authorized(username)}
                users.append(user)
            return render_template('authorize.html', users=users, string=text)
    else:
        users = []
        for username in os.listdir('./dataset'):
            user = {'name': username, 'is_authorized': authorizationweb.is_authorized(username)}
            users.append(user)
        return render_template('authorize.html', users=users)


@app.route('/deauthorize', methods=['GET', 'POST'])
def deauthorize_person():
    if request.method == "POST":
        if request.form["authorize"] == "Authorize User":
            result = authorizationweb.delete_authorized_user(request.form['username'])
            text = "User "
            if result:
                text += request.form['username'] + " is successfully deauthorized."
            else:
                text += request.form['username'] + " is not successfully deauthorized."
            users = []
            for username in os.listdir('./dataset'):
                user = {'name': username, 'is_authorized': authorizationweb.is_authorized(username)}
                users.append(user)
            return render_template('authorize.html', users=users, string=text)
    else:
        users = []
        for username in os.listdir('./dataset'):
            user = {'name': username, 'is_authorized': authorizationweb.is_authorized(username)}
            users.append(user)
        return render_template('authorize.html', users=users)

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)
