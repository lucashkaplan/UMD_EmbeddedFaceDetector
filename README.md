# UMD_EmbeddedFaceDetector
This project includes the development of an embedded face detector system (EFDS), which identifies images or 
image regions that contain human faces. We explored trade-offs among key implementation metrics, including real-time 
processing speed, detection rate (true-positive rate), and the rate of false positives. The EFDS is based on the the Viola-Jones (VJ) 
face detection algorithm.  

There are two main parts of the project design — the EFDS itself, and the training module for the EFDS. The
EFDS is designed to run on the Raspberry Pi device, and was developed using both native compilation on
a linux machine (for prototyping and testing) and cross-compilation (for performance evaluation,
further testing, and demonstration). This code is contained in the Classifier folder. The purpose of the training module is to optimize parameters in
the EFDS, and to assist the system designer in configuring the EFDS across a range of useful operating
points (trade-off options). The training module is designed to run only on a linux machine; it is not
intended to be part of the deployed EFDS. This code is contained in the Training folder.

A complete desription of the architecture used to develop the EFDS, along with an exploration of tradeoffs within this
system, is found in the About folder.
