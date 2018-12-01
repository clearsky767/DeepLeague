import cv2
import argparse
import os

def main():
    parser = argparse.ArgumentParser(description="slice video frames to images")
    parser.add_argument('--video_path', help='path of video')
    parser.add_argument('--save_path', default='./save_images', help='path to save images')
    parser.add_argument('--image_size', type= int,default=512, help='resize to image size')
    args = parser.parse_args()
    print("main")
    if not args.video_path:
        print("args.video_path value exception!")
    else:
        print(args.video_path)
        print(args.save_path)
        if not os.path.exists(args.save_path):
            os.makedirs(args.save_path)
        get_frames(args.video_path,args.save_path,args.image_size)

def get_frames(video_path, save_path,image_size):
    video = cv2.VideoCapture(video_path)
    print("cv open file: {}".format(video_path))
    success = True
    count = 0
    while success:
        success, frame = video.read()
        if not success:
            break
        count += 1
        print(" frame: {}".format(count))
        h,w,c = frame.shape
        s = abs((h-w)/2)
        if h>w:
            l = w
            cropped = frame[s:s+l, 0:l]
        else:
            l = h
            cropped = frame[0:l, s:s+l]
        resized = cv2.resize(cropped,(image_size, image_size), interpolation=cv2.INTER_CUBIC)
        #im = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        cv2.imwrite("./%s/frame_%d.jpg" %  (save_path, count), resized)
        cv2.waitKey(1)
    print("Saved %d frames" % (count) )
    video.release()

if __name__=='__main__':
    main()