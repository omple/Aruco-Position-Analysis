import cv2, numpy as np, glob

# Press C to save, Q to quit

CHECKERBOARD = (9, 6)   # your board's INNER corners
SQUARE_SIZE  = 0.024    # metres per square — measure yours!

objp = np.zeros((CHECKERBOARD[0] * CHECKERBOARD[1], 3), np.float32)
objp[:, :2] = np.mgrid[0:CHECKERBOARD[0],
                        0:CHECKERBOARD[1]].T.reshape(-1, 2)
objp *= SQUARE_SIZE

obj_points, img_points, img_size = [], [], None

for fname in glob.glob('calib_images/*.jpg'):
    img  = cv2.imread(fname)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    img_size = gray.shape[::-1]

    ret, corners = cv2.findChessboardCorners(gray, CHECKERBOARD, None)
    if ret:
        criteria = (cv2.TERM_CRITERIA_EPS +
                    cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
        corners = cv2.cornerSubPix(gray, corners,
                                   (11, 11), (-1, -1), criteria)
        obj_points.append(objp)
        img_points.append(corners)

ret, K, D, rvecs, tvecs = cv2.calibrateCamera(
    obj_points, img_points, img_size, None, None)

print('Camera matrix K:\n', K)
print('Distortion D:\n', D.ravel())
np.savez('calibration.npz', camera_matrix=K, dist_coeff=D)
