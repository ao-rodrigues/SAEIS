#include "Player.hpp"

Player::Player(string path,
               float xPos,
               float yPos,
               float width,
               float height)
{
    this->path = path;
    this->player = ofVideoPlayer();
    this->playerWidth = width;
    this->playerHeight = height;
    this->playerXPosition = xPos;
    this->playerYPosition = yPos;
    
    player.setLoopState(OF_LOOP_NORMAL);
    
    loadVideo(this->path);
}

Player::~Player()
{
}

void Player::loadVideo(const string path)
{
    if (player.isLoaded())
    {
        player.closeMovie();
    }
    
    player.load(path);
    
    videoWidth = player.getWidth();
    videoHeight = player.getHeight();
    
    float proportion;
    
    
    if (playerHeight > playerWidth)
    {
        if (videoWidth >= videoHeight)
        {
            proportion = playerWidth / videoWidth;
        }
        else
        {
            proportion = playerHeight / videoHeight;
        }
    }
    else
    {
        if (videoWidth <= videoHeight)
        {
            proportion = playerHeight / videoHeight;
        }
        else
        {
            proportion = playerWidth / videoWidth;
        }
    }
    
    videoWidth *= proportion;
    videoHeight *= proportion;
    
    // player.play();
}

void Player::keyPress(int key)
{
    float fps = player.getSpeed();
    switch (key)
    {
            // play / pause
        case OF_KEY_F1:
            player.setPaused(player.isPlaying());
            break;
            
        case OF_KEY_RIGHT:
            player.setFrame(player.getCurrentFrame() + fps * 5.0f);
            break;
            
        case OF_KEY_LEFT:
            player.setFrame(player.getCurrentFrame() - fps * 5.0f);
            break;
            
        default:
            break;
    }
}

void Player::draw()
{
    ofSetColor(ofColor::white);
    player.draw(playerXPosition,
                playerYPosition,
                videoWidth,
                videoHeight);
}

void Player::update()
{
    player.update();
}
