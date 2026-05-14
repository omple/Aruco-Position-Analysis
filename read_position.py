import argparse
import sys
import cv2
import numpy as np


def load_calibration(path):
    data = np.load(path)
    # support multiple possible key names
    if 'camera_matrix' in data:
        mtx = data['camera_matrix']
    elif 'camera_matrix' in data.files:
        mtx = data['camera_matrix']
    elif 'mtx' in data:
        mtx = data['mtx']
    else:
        raise KeyError('camera matrix not found in calibration file')

    if 'dist_coeff' in data:
        dist = data['dist_coeff']
    elif 'dist' in data:
        dist = data['dist']
    elif 'dist_coeffs' in data:
        dist = data['dist_coeffs']
    else:
        # allow empty
        dist = np.zeros((5,))

    return mtx, dist


def main():
    p = argparse.ArgumentParser()
    p.add_argument('--calib', '-c', default='CameraCalibration/calibration.npz', help='path to calibration .npz')
    p.add_argument('--camera', type=int, default=0)
    p.add_argument('--marker-cm', type=float, default=5.0, help='physical marker side length in cm')
    args = p.parse_args()

    try:
        camera_matrix, dist_coeffs = load_calibration(args.calib)
    except Exception as e:
        print('Failed to load calibration:', e)
        sys.exit(1)

    fx = camera_matrix[0, 0]
    fy = camera_matrix[1, 1]
    cx = camera_matrix[0, 2]
    cy = camera_matrix[1, 2]

    marker_length_m = args.marker_cm / 100.0

    cap = cv2.VideoCapture(args.camera)
    if not cap.isOpened():
        print('Cannot open camera')
        sys.exit(1)

    aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_5X5_250)
    parameters = cv2.aruco.DetectorParameters()

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        corners, ids, rejected = cv2.aruco.detectMarkers(gray, aruco_dict, parameters=parameters)

        if ids is not None:
            rvecs, tvecs, _ = cv2.aruco.estimatePoseSingleMarkers(corners, marker_length_m, camera_matrix, dist_coeffs)
            for i, marker_id in enumerate(ids.flatten()):
                # ensure corner array shape (4,2)
                c = corners[i]
                if c is None:
                    continue
                c = np.array(c).reshape(-1, 2)
                u, v = c.mean(axis=0)

                t = tvecs[i].reshape(3)
                z = t[2]
                # offsets from image center in meters
                dx_m = (u - cx) * z / fx
                dy_m = (v - cy) * z / fy

                dist_cm = np.linalg.norm(t) * 100.0
                dx_cm = dx_m * 100.0
                dy_cm = dy_m * 100.0

                # compute tag orientation (Euler angles) from rotation vector
                r = rvecs[i].reshape(3)
                R_mat, _ = cv2.Rodrigues(r)
                # Tait-Bryan angles ZYX (yaw, pitch, roll)
                sy = np.sqrt(R_mat[0, 0] * R_mat[0, 0] + R_mat[1, 0] * R_mat[1, 0])
                singular = sy < 1e-6
                if not singular:
                    rx = np.arctan2(R_mat[2, 1], R_mat[2, 2])
                    ry = np.arctan2(-R_mat[2, 0], sy)
                    rz = np.arctan2(R_mat[1, 0], R_mat[0, 0])
                else:
                    rx = np.arctan2(-R_mat[1, 2], R_mat[1, 1])
                    ry = np.arctan2(-R_mat[2, 0], sy)
                    rz = 0.0

                roll_deg = np.degrees(rx)
                pitch_deg = np.degrees(ry)
                yaw_deg = np.degrees(rz)

                # display on frame
                cv2.aruco.drawDetectedMarkers(frame, [corners[i]])
                cv2.drawFrameAxes(frame, camera_matrix, dist_coeffs, rvecs[i], tvecs[i], marker_length_m * 0.5)

                txt = f'ID:{int(marker_id)} D:{dist_cm:.1f}cm OffX:{dx_cm:.1f}cm OffY:{dy_cm:.1f}cm'
                cv2.putText(frame, txt, (int(u) - 100, int(v) - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
                
                txt_rot = f'Roll:{roll_deg:.1f} Degrees Pitch:{pitch_deg:.1f} Degrees Yaw:{yaw_deg:.1f} Degrees'
                cv2.putText(frame, txt_rot, (int(u) - 100, int(v) + 15), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

                print(f'{int(marker_id)} {dist_cm:.1f}cm dx={dx_cm:.1f}cm dy={dy_cm:.1f}cm')

        cv2.imshow('aruco', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    main()
