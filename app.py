import os
import subprocess
from flask import Flask, request, send_file
from werkzeug.utils import secure_filename

app = Flask(__name__)

# Set the folder where images will be uploaded
UPLOAD_FOLDER = './uploads'
OUTPUT_FOLDER = './output'
ALLOWED_EXTENSIONS = {'png', 'jpg', 'jpeg'}

app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
app.config['OUTPUT_FOLDER'] = OUTPUT_FOLDER

# Make sure the upload and output directories exist
if not os.path.exists(UPLOAD_FOLDER):
    os.makedirs(UPLOAD_FOLDER)
if not os.path.exists(OUTPUT_FOLDER):
    os.makedirs(OUTPUT_FOLDER)

# Helper function to check allowed file types
def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

@app.route('/')
def index():
    return '''
        <h1>Seam Carving App</h1>
        <form action="/upload" method="POST" enctype="multipart/form-data">
            <input type="file" name="image" required>
            <input type="number" name="height" placeholder="Desired Height" required>
            <input type="number" name="width" placeholder="Desired Width" required>
            <input type="submit" value="Upload">
        </form>
    '''

@app.route('/upload', methods=['POST'])
def upload_image():
    # Check if the post request has the file part
    if 'image' not in request.files:
        return 'No file part'
    file = request.files['image']
    if file.filename == '':
        return 'No selected file'

    if file and allowed_file(file.filename):
        # Save the file
        filename = secure_filename(file.filename)
        input_image_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
        output_image_path = os.path.join(app.config['OUTPUT_FOLDER'], 'output.jpg')
        
        file.save(input_image_path)
        
        # Get height and width from form input
        height = request.form.get('height')
        width = request.form.get('width')

        # Ensure height and width are valid integers
        if not height.isdigit() or not width.isdigit():
            return 'Invalid height or width'

        height = int(height)
        width = int(width)

        # Construct the command to run the C++ executable
        command = f'./seam_carving {input_image_path}'

        try:
            # Run the C++ seam carving executable with the uploaded image
            process = subprocess.Popen(command, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            
            # Send height and width to the process's standard input
            input_data = f'{height}\n{width}\n'
            stdout, stderr = process.communicate(input=input_data)

            print(stdout)  # For debugging purposes
            print(stderr)  # For debugging purposes

            # Check if output image exists and send it back to the user
            if os.path.exists(output_image_path):
                return send_file(output_image_path, mimetype='image/jpeg')
            else:
                return 'Error in seam carving process'
        except Exception as e:
            return f'Exception occurred: {str(e)}'
    else:
        return 'File type not allowed'

if __name__ == "__main__":
    app.run(debug=True)

