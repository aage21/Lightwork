/**
 * Blob Class
 *
 * Based on this example by Daniel Shiffman:
 * http://shiffman.net/2011/04/26/opencv-matching-faces-over-time/
 * 
 * @author: Jordi Tost (@jorditost)
 * 
 * University of Applied Sciences Potsdam, 2014
 */

class Blob {

  private PApplet parent;

  // Contour
  public Contour contour;

  // Am I available to be matched?
  public boolean available;

  // How long should I live if I have disappeared?
  private int initTimer = 200; //127;
  public int timer;

  // Unique ID for each blob
  int id;

  // Pattern Detection
  BinaryPattern detectedPattern;
  int brightness;
  int previousFrameCount; // FrameCount when last edge was detected


  // Make me
  Blob(PApplet parent, int id, Contour c) {
    this.parent = parent;
    this.id = id;
    this.contour = new Contour(parent, c.pointMat);

    this.available = true;

    this.timer = initTimer;

    detectedPattern = new BinaryPattern();
    brightness = 0; 
    previousFrameCount = 0;
  }

  // Show me
  void display() {
    float scaleX = (float)camDisplayWidth/(float)camWidth;
    float scaleY = (float)camDisplayHeight/(float)camHeight;
    
    Rectangle r = contour.getBoundingBox();

    float opacity = map(timer, 0, initTimer, 0, 255);
    fill(0, 0, 255, opacity);
    stroke(0, 0, 255);
    rect(r.x*scaleX, r.y*scaleY, r.width, r.height);
    fill(255, 0, 0);
    textSize(12);
    text(""+id, r.x*scaleX+10, r.y*scaleY+5);
    String decoded = detectedPattern.decodedString.toString();
    text(decoded, r.x*scaleX+25, r.y*scaleY+5);
  }

  void update(Contour newContour) {
    this.contour = newContour;
    this.timer = initTimer;
  }


  // Count me down, I am gone
  void countDown() {    
    timer--;
  }

  // I am dead, delete me
  boolean dead() {
    if (timer < 0) return true;
    return false;
  }

  public Rectangle getBoundingBox() {
    return contour.getBoundingBox();
  }

  void registerBrightness(int br) {
    brightness = br;
  }

  // Decode Binary Pattern
  void decode() {
    int br = brightness;
    int threshold = 180; 
    // Edge detection (rising/falling);
    int frameDelta = 0; 
    boolean didTransition = false; 
    if (br >= threshold && detectedPattern.state == 0) {
      didTransition = true; 
      detectedPattern.state = 1;
      //println(frameDelta+"],");
      //previousFrameCount = frameCount;
    } else if (br < threshold && detectedPattern.state == 1) {
      didTransition = true;
      detectedPattern.state = 0; 
      //print("0, ");  
      //println(frameCount);
    }
    if (didTransition) {
      frameDelta = frameCount-previousFrameCount;
      int frameSkip = animator.frameSkip;  // TODO: link this with Animator frameskip
      
      // temporary target pattern for testing
      
      // Find out how many instances of the previous state occurred (000 = 3, 11 = 2, 1111 = 4, etc)
      int numRepeats = frameDelta/frameSkip;
      //println(numRepeats);
      for (int i = 0; i < numRepeats; i++) {
        detectedPattern.writeNextBit(detectedPattern.state);
      }


      String targetPattern = "1010101010";
      if (targetPattern.equals(detectedPattern)) {
        println("MATCH FOUND!!!!"); 
      }

      print("["+detectedPattern.state+", "+frameDelta+"], ");
      previousFrameCount = frameCount;
    }
  }
}