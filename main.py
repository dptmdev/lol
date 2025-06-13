from fastapi import FastAPI, HTTPException
from fastapi.responses import StreamingResponse
import zipfile
import io
import os

app = FastAPI()

@app.get("/download")
async def download_zip():
    """Create and return zip file containing files from code/ directory"""
    # Create zip in memory
    zip_buffer = io.BytesIO()
    
    with zipfile.ZipFile(zip_buffer, 'w', zipfile.ZIP_DEFLATED) as zipf:
        # Walk through the code directory
        for root, dirs, files in os.walk('code'):
            for file in files:
                file_path = os.path.join(root, file)
                # Calculate arc_name (path within the zip file)
                arc_name = os.path.relpath(file_path, 'code')
                # Add file to zip
                zipf.write(file_path, arc_name)
    
    zip_buffer.seek(0)
    
    return StreamingResponse(
        io.BytesIO(zip_buffer.read()),
        media_type="application/zip",
        headers={"Content-Disposition": "attachment; filename=code.zip"}
    )

@app.get("/zip/{filename}")
async def download_existing_zip(filename: str):
    """Return an existing zip file from the zip/ directory"""
    file_path = os.path.join('zip', filename)
    if not os.path.exists(file_path) or not file_path.endswith('.zip'):
        raise HTTPException(status_code=404, detail="Zip file not found")
        
    return StreamingResponse(
        open(file_path, 'rb'),
        media_type="application/zip",
        headers={"Content-Disposition": f"attachment; filename={filename}"}
    )

@app.get("/")
async def root():
    return {"message": "Use /download endpoint to get the zip file"}