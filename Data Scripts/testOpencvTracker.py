import cv2
import sys

tracker = cv2.Tracker_create("BOOSTING")
video = cv2.VideoCapture("111.avi")

ok,frame = video.read()
bbox = (287,23,86,320)

ok = tracker.init(frame,bbox)
while(1):
    ok,frame = video.read()
    ok,bbox = tracker.update(frame)
    if ok:
        p1 = (int(bbox[0]),int(bbox[1]))
        p2 = (int(bbox[0]+bbox[2]),int(bbox[1]+bbox[3]))
        cv2.rectangle(frame,p1,p2,(0,0,255))
    cv2.imshow("Tracking",frame)
    k = cv2.waitKey(1) & 0xff
    if k == 27:
        break

video.release()
cv2.destroyAllWindows()