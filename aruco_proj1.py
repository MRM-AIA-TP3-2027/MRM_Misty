import cv2
import numpy as np

# Load ArUco dictionary
aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)

parameters = cv2.aruco.DetectorParameters()

# Start webcam
cap = cv2.VideoCapture(0)

# Fake camera calibration (for beginners)
camera_matrix = np.array([[800,0,320],
                          [0,800,240],
                          [0,0,1]])

dist_coeffs = np.zeros((5,1))

marker_length = 0.05  # meters

while True:

    ret, frame = cap.read()

    if not ret:
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect markers
    corners, ids, rejected = cv2.aruco.detectMarkers(
        gray,
        aruco_dict,
        parameters=parameters
    )

    if ids is not None:

        cv2.aruco.drawDetectedMarkers(frame, corners, ids)

        # Estimate pose
        rvecs, tvecs, _ = cv2.aruco.estimatePoseSingleMarkers(
            corners,
            marker_length,
            camera_matrix,
            dist_coeffs
        )

        for i in range(len(ids)):

          rvec = rvecs[i]
          tvec = tvecs[i]

          print("Marker ID:", ids[i])
          print("Rotation Vector (rvec):", rvec)
          print("Translation Vector (tvec):", tvec)
          print("----------------------")

          cv2.drawFrameAxes(
         frame,
         camera_matrix,
         dist_coeffs,
         rvec,
         tvec,
         0.03)

    cv2.imshow("Aruco Pose Estimation", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()