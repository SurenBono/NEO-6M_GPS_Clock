// Circle parameters
float centerX = 50.0;  // X-coordinate of the circle's center
float centerY = 50.0;  // Y-coordinate of the circle's center
float radius = 30.0;   // Radius of the circle

// Point coordinates
float pointX = 60.0;   // X-coordinate of the point
float pointY = 70.0;   // Y-coordinate of the point

void setup() {
  Serial.begin(9600);
}

void loop() {
  float dx = pointX - centerX;
  float dy = pointY - centerY;
  float distance = sqrt(dx * dx + dy * dy);

  if (distance <= radius) {
    Serial.println("Point is inside the circle");
  } else {
    Serial.println("Point is outside the circle");
  }

  delay(1000);  // Delay for readability
}
