import numpy as np 
import cv2 

video_path = "111.avi"

cap = cv2.VideoCapture(video_path)

mog = cv2.createBackgroundSubtractorMOG2()

while(1):
    ret, frame = cap.read()
    fgmask = mog.apply(frame)
    cv2.imshow('frame', fgmask)
    if cv2.waitKey(int(1000 / 12)) & 0xff == ord("q"):
      break

cap.release()
cv2.destroyAllWindows()