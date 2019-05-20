#ifndef Thumbnail_h
#define Thumbnail_h
#endif /* Thumbnail_h */

#include "ofMain.h"
#include "Player.h"

#define NAME_HEIGHT 10
#define BORDER_WIDTH 2

class Thumbnail{
    static const string THUMBNAILS_PATH;
    
    // By default, 1 frame per second
    const float PREVIEW_FPS = 1.0;
    
    float xPos, yPos;
    
    float width, height;
    
    bool selected, enabled, hasPreviewFrames;
    
    shared_ptr<Player> player;
    
    ofImage thumbnail;
    string videoPath, videoName;
    vector<string> keywords;
    
    ofColor textColor;
    
    ofEventListener clickListener;
    
    vector<ofTexture> previewFrames;
    
public:    
    Thumbnail(float x, float y, float w, float h, string vidPath, shared_ptr<Player> plyr);
    void draw();
    void update();
    
    void updatePreviewFrames(vector<ofTexture> frames);
    
    void mouseMoved(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void mousePressed(ofMouseEventArgs & args);
    void mouseReleased(ofMouseEventArgs & args);
    void mouseScrolled(ofMouseEventArgs & args);
    void mouseEntered(ofMouseEventArgs & args);
    void mouseExited(ofMouseEventArgs & args);
    
    
    void onMouseReleased(ofMouseEventArgs & args);
    
    bool mouseOver();
    
    bool inside(float x, float y);
    void setSelected(bool isSelected);
    void setEnabled(bool isEnabled);
    string extractVideoName(string path);
    
    void addKeyword(const string kw);
    void addKeywords(vector<string> kwds);
    
protected:
    void calcFixedDimensions(ofTexture frame, float & fixedXPos, float & fixedYPos, float & fixedW, float & fixedH);
    
};
