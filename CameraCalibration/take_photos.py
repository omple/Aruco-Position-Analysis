import cv2, os
os.makedirs('calib_images', exist_ok=True)

cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)  # CAP_DSHOW for Windows USB cameras
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)

count = 0
print('Press C to capture | Q to quit')

while True:
    ret, frame = cap.read()
    if not ret: break

    preview = cv2.resize(frame, (960, 540))
    cv2.putText(preview, f'Saved: {count}', (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
    cv2.imshow('Camera', preview)

    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'): break
    elif key == ord('c'):
        fname = f'calib_images/image_{count:03d}.jpg'
        cv2.imwrite(fname, frame, [cv2.IMWRITE_JPEG_QUALITY, 97])
        count += 1
        print(f'Saved: {fname}')
cap.release()
cv2.destroyAllWindows()
