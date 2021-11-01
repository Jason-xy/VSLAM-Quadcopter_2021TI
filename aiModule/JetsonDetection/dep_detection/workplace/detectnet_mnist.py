#from os import openpty
import jetson.inference
import jetson.utils

import argparse
import sys


class detectionet_class:
	def __init__(self) -> None:
		is_headless = [""]
		self.net = jetson.inference.detectNet('ssd-mobilenet-v2', ['./detectnet.py', '/dev/video0'], 0.5)# threshold=0.5
		self.input = jetson.utils.videoSource('v4l2:///dev/video0', argv=['./detectnet.py', '/dev/video0'])
		self.output = jetson.utils.videoOutput('', argv=['./detectnet.py', '/dev/video0']+is_headless)

	# func_a
	def judge_detection(self):
		return (not self.input.IsStreaming() or not self.output.IsStreaming())

	# init_detection()
	def detection_run(self):
	    # capture the next image
	    img = self.input.Capture()

	    # detect objects in the image (with overlay)
	    detections = self.net.Detect(img, overlay='box,labels,conf')

	    # print the detections
	    # print("detected {:d} objects in image".format(len(detections)))

	    # for detection in detections:
			# print(detection)

	    # render the image
	    self.output.Render(img)

	    # update the title bar
	    self.output.SetStatus("{:s} | Network {:.0f} FPS".format('ssd-mobilenet-v2', self.net.GetNetworkFPS()))

	    # print out performance info
	    self.net.PrintProfilerTimes()

	