namespace videoUtils {
    void calculateFixedDimensions(float xPos, float yPos, float imgW, float imgH, float & fixedXPos, float & fixedYPos, float & fixedW, float & fixedH) {
        float aspectRatio = imgW / imgH;
        
        if(imgW > imgH) {
            // Letterboxing
            fixedW = width;
            fixedH = width / aspectRatio;
            
            fixedXPos = xPos;
            fixedYPos = yPos + ((height - fixedH) / 2.0);
            
        } else {
            // Pillarboxing
            fixedW = height * aspectRatio;
            fixedH = height;
            
            fixedXPos = xPos + ((width - fixedW) / 2.0);
            fixedYPos = yPos;
        }
    }
}
