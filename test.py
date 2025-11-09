from flask import Flask, Response
import cv2

app = Flask(__name__)
cap = cv2.VideoCapture(0) # '0' adalah kamera webcam default

def gen_frames():
    while True:
        success, frame = cap.read()
        if not success:
            break
        else:
            # Encode frame sebagai JPEG
            ret, buffer = cv2.imencode('.jpg', frame)
            if not ret:
                continue
            
            # Ubah menjadi stream bytes
            frame_bytes = buffer.tobytes()
            
            # 'yield' frame sebagai 'multipart/x-mixed-replace' stream
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + frame_bytes + b'\r\n')

@app.route('/video_feed')
def video_feed():
    """Rute untuk video streaming."""
    return Response(gen_frames(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    print("\nServer video berjalan di:")
    print("http://[IP_LAPTOP_ANDA]:5000/video_feed")
    print("\nGanti [IP_LAPTOP_ANDA] di file index.html Anda!")
    app.run(host='0.0.0.0', port=5000)