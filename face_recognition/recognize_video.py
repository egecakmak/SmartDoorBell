
from imutils.video import VideoStream
from imutils.video import FPS
import numpy as np
import argparse
import imutils
import pickle
import time
import cv2
import os
import pyfakewebcam
import sys

def run():
	args = {"detector": "face_detection_model", "embedding_model": "openface_nn4.small2.v1.t7",
		"recognizer": "output/recognizer.pickle", "le": "output/le.pickle", "confidence": 0.5,
		}
	protoPath = os.path.sep.join([args["detector"], "deploy.prototxt"])
	modelPath = os.path.sep.join([args["detector"],
								  "res10_300x300_ssd_iter_140000.caffemodel"])
	detector = cv2.dnn.readNetFromCaffe(protoPath, modelPath)

	embedder = cv2.dnn.readNetFromTorch(args["embedding_model"])

	recognizer = pickle.loads(open(args["recognizer"], "rb").read())
	le = pickle.loads(open(args["le"], "rb").read())

	camera = pyfakewebcam.FakeWebcam('/dev/video1', 300, 300)
	vs = VideoStream(src=0).start()
	time.sleep(2.0)

	verifications = {}

	while True:
		frame = vs.read()

		frame = imutils.resize(frame, width=600)
		(h, w) = frame.shape[:2]

		imageBlob = cv2.dnn.blobFromImage(
			cv2.resize(frame, (100, 100)), 1.0, (100, 100),
			(104.0, 177.0, 123.0), swapRB=False, crop=False)

		detector.setInput(imageBlob)
		detections = detector.forward()

		for i in range(0, detections.shape[2]):
			confidence = detections[0, 0, i, 2]

			if confidence > args["confidence"]:
				box = detections[0, 0, i, 3:7] * np.array([w, h, w, h])
				(startX, startY, endX, endY) = box.astype("int")

				face = frame[startY:endY, startX:endX]
				(fH, fW) = face.shape[:2]

				if fW < 20 or fH < 20:
					continue

				faceBlob = cv2.dnn.blobFromImage(face, 1.0 / 255,
												 (96, 96), (0, 0, 0), swapRB=True, crop=False)
				embedder.setInput(faceBlob)
				vec = embedder.forward()

				preds = recognizer.predict_proba(vec)[0]
				j = np.argmax(preds)
				proba = preds[j]
				name = le.classes_[j]
				if name in verifications:
					verifications[name] += 1
				else:
					verifications[name] = 0
				return name

			
