const express = require('express');
const multer = require('multer');
const { spawn } = require('child_process');
const path = require('path');
const fs = require('fs');

const app = express();
const UPLOAD_FOLDER = './uploads';
const OUTPUT_FOLDER = './output';

// Ensure upload and output directories exist
if (!fs.existsSync(UPLOAD_FOLDER)) {
    fs.mkdirSync(UPLOAD_FOLDER);
}
if (!fs.existsSync(OUTPUT_FOLDER)) {
    fs.mkdirSync(OUTPUT_FOLDER);
}

// Setup Multer for file uploads
const storage = multer.diskStorage({
    destination: (req, file, cb) => {
        cb(null, UPLOAD_FOLDER);
    },
    filename: (req, file, cb) => {
        cb(null, Date.now() + path.extname(file.originalname));
    }
});
const upload = multer({ storage });

// Serve a basic form for the upload
app.get('/', (req, res) => {
    res.send(`
        <h1>Seam Carving App</h1>
        <form action="/upload" method="POST" enctype="multipart/form-data">
            <input type="file" name="image" required>
            <input type="number" name="height" placeholder="Desired Height" required>
            <input type="number" name="width" placeholder="Desired Width" required>
            <input type="submit" value="Upload">
        </form>
    `);
});

// Handle the image upload and run the seam carving process
app.post('/upload', upload.single('image'), (req, res) => {
    const inputImagePath = req.file.path;
    const outputImagePath = path.join('output.jpg');
    
    const height = req.body.height;
    const width = req.body.width;

    // Ensure height and width are valid numbers
    if (!height || !width || isNaN(height) || isNaN(width)) {
        return res.status(400).send('Invalid height or width');
    }

    // Run the seam carving process using child_process
    const command = `./seam_carving "${inputImagePath}"`;
    const seamCarvingProcess = spawn(command, {
        shell: true,
        stdio: ['pipe', 'pipe', 'pipe']
    });

    // Pass height and width via stdin
    seamCarvingProcess.stdin.write(`${height}\n${width}\n`);
    seamCarvingProcess.stdin.end();

    seamCarvingProcess.stdout.on('data', (data) => {
        console.log(`stdout: ${data}`);
    });

    seamCarvingProcess.stderr.on('data', (data) => {
        console.error(`stderr: ${data}`);
    });

    seamCarvingProcess.on('close', (code) => {
        if (code === 0) {
            if (fs.existsSync(outputImagePath)) {
                res.sendFile(path.resolve(outputImagePath));
            } else {
                res.status(500).send('Error in seam carving process');
            }
        } else {
            res.status(500).send(`Process exited with code ${code}`);
        }
    });
});

// Start the server
const port = process.env.PORT || 3000;
app.listen(port, () => {
    console.log(`Server running on http://localhost:${port}`);
});

