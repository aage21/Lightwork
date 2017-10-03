//
//  Animator.cpp
//  Lightwork-Mapper
//
//  Created by Leo Stefansson on 11.9.2017.
//
//

#include "Animator.h"

using namespace std;

// Constructor
Animator::Animator(void) {
    cout << "Animator created" << endl;
    numLedsPerStrip = 64;
    ledBrightness = 255;
    numStrips = 2;
    ledIndex = 0; // Internal counter
    mode = ANIMATION_MODE_CHASE;
    
    testIndex = 0;
    frameCount = 0;
    
    // TODO: assign pixels for the full setup (all the channels)iter
    // TODO: Make pixels private and declare a getter
    pixels.assign(numLedsPerStrip*numStrips, ofColor(0,0,0));

    int bPatOffset = 150; // Offset to get more meaningful patterns (and avoid 000000000);
//    BinaryPattern bPat = BinaryPattern();
    for (int i = 0; i < pixels.size(); i++) {
//        binaryPatterns.push_back(BinaryPattern());
//        binaryPatterns[i].generatePattern(i+bPatOffset);
        
        
        leds.push_back(LED());
        leds[i].setAddress(0);
        leds[i].setColor(ofColor(0, 0, 0));
        leds[i].setCoord(ofPoint(0, 0));
//        bPat.generatePattern(i+bPatOffset);
//        leds[i].setBinaryPattern(bPat);
    }
    
//    binaryPattern.generatePattern(842); // A single pattern, for testing
}

// Destructor
Animator::~Animator(void) {
    cout << "Animator destroyed" << endl;
}

//////////////////////////////////////////////////////////////
// Setters and getters
//////////////////////////////////////////////////////////////
void Animator::setLedInterface(ofxOPC *interface) {
    opcClient = interface;
}

void Animator::setMode(animation_mode_t m) {
    mode = m;
    ledIndex = 0; // TODO: resetInternalVariables() method?
    testIndex = 0;
    frameCount = 0;
    resetPixels();
    
}

void Animator::setLedBrightness(int brightness) {
    ledBrightness = brightness;
    opcClient->autoWriteData(getPixels());
}

void Animator::setNumLedsPerStrip(int num) {
    ofLogNotice("Setting up Animator");
    numLedsPerStrip = num;
    resetPixels();
}

int Animator::getNumLedsPerStrip() {
    return numLedsPerStrip;
}

void Animator::setNumStrips(int num) {
    numStrips = num;
    resetPixels();
}

int Animator::getNumStrips() {
    return numStrips;
}

// Internal method to reassign pixels with a vector of the right length. Gives all pixels a value of (0,0,0) (black/off).
void Animator::resetPixels() {
    vector <ofColor> pix;
    pix.assign(numLedsPerStrip*numStrips, ofColor(0,0,0));
    pixels = pix;
    
    // TODO: Review this
//    binaryPatterns.clear();
//    int bPatOffset = 13; // Offset to get more meaningful patterns (and avoid 000000000);
//    for (int i = 0; i < pixels.size(); i++) {
//        binaryPatterns.push_back(BinaryPattern());
//        binaryPatterns[i].generatePattern(i+bPatOffset);
//    }
    
    // Update OPC client
    opcClient->setLedsPerStrip(numLedsPerStrip);
    
}

// Return pixels (to update OPC or PixelPusher)
vector <ofColor> Animator::getPixels() {
    return pixels;
}

//////////////////////////////////////////////////////////////
// Animation Methods
//////////////////////////////////////////////////////////////

void Animator::update() {
    switch(mode) {
        case ANIMATION_MODE_CHASE: {
            chase();
            break;
        }
        case ANIMATION_MODE_TEST: {
            test();
            break;
        }
        case ANIMATION_MODE_BINARY: {
            binaryAnimation();
        }
    };
    // Advance the internal counter
    frameCount++;
    
    // Update pixels on external interface
    opcClient->autoWriteData(getPixels()); // Send pixel values to OPC

}
// Update the pixels for all the strips
// This method does not return the pixels, it's up to the users to send animator.pixels to the driver (FadeCandy, PixelPusher).
void Animator::chase() {
    for (int i = 0; i <  numLedsPerStrip*numStrips; i++) {
        ofColor col;
        if (i == ledIndex) {
            col = ofColor(ledBrightness, ledBrightness, ledBrightness);
        }
        else {
            col = ofColor(0, 0, 0);
        }
        pixels.at(i) = col;
    }
    
    ledIndex++;
    if (ledIndex == numLedsPerStrip+numLedsPerStrip*numStrips) {
        ledIndex = 0;
    }
}

// Set all LEDs to the same colour (useful to turn them all on or off).
void Animator::setAllLEDColours(ofColor col) {
    for (int i = 0; i <  numLedsPerStrip*numStrips; i++) {
        pixels.at(i) = col;
    }
}

//LED Pre-flight test
void Animator::test() {
    testIndex++;
    
    if (testIndex < 30) {
        setAllLEDColours(ofColor(255, 0, 0));
    }
    else if (testIndex > 30 && testIndex < 60) {
        setAllLEDColours(ofColor(0, 255, 0));
    }
    else if (testIndex > 60 && testIndex < 90) {
        setAllLEDColours(ofColor(0, 0, 255));
    }
    
    if (testIndex > 90) {
        testIndex = 0;
    }
}

void Animator::binaryAnimation() {
//    cout << binaryPattern.state << endl;
    // LED binary state. START -> GREEN, HIGH -> BLUE, LOW -> RED, OFF -> (off)
    
    // Slow down the animation, set new state every 3 frames
    if (frameCount % 5 == 0) {
        for (int i = 0; i < pixels.size(); i++) {
            
//            cout << "patterns: " << i << " " << binaryPatterns[i].binaryPatternString << endl;
            /*
                switch (binaryPatterns[i].state){ // 0
                    case BinaryPattern::LOW: {
                        pixels.at(i) = ofColor(ledBrightness, 0, 0); // RED
                        break;
                    }
                    case BinaryPattern::HIGH: { // 1
                        pixels.at(i) = ofColor(0, 0, ledBrightness); // BLUE
                        break;
                    }
                    case BinaryPattern::START: { // 2
                        pixels.at(i) = ofColor(0, ledBrightness, 0); // GREEN
                        break;
                    }
                    case BinaryPattern::OFF: { // 3
                        pixels.at(i) = ofColor(0, 0, 0); // BLACK
                        break;
                    }
                }
                */
//                binaryPatterns[i].advance();
        }
    }
    
}
