This is to develope a mini OS for Distributed Video Processing, where we focusing on Major three node processes like 
Node 1: Capture via V4L2 API, send frames over socket. 
Node 2: Apply edge detection (Sobel/Canny) using threads. 
Node 3: Display frames with OpenCV in real time.

The main Objective is to To design and implement a lightweight real-time microkernel for distributed video processing with deterministic scheduling and efficient inter-process communication on Linux.
