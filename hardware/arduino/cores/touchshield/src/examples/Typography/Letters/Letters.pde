/**
 * Letters.
 *
 * Draws letters to the screen.
 */

void setup()
{
  size(200, 200);
  background(0);
}

void loop()
{
  // Set the gray value of the letters
  stroke(255);

  // Set the left and top margin
  int margin = 6;
  int gap = 30;

  // Create a matrix of letterforms
  int counter = 0;
  for(int i=0; i<margin; i++) {
    for(int j=0; j<margin; j++) {
      char letter;

      // Select the letter
      int count = 65+(i*margin)+j;
      if(count <= 90) {
        letter = char(65+counter);
        if(letter == 'A' || letter == 'E' || letter == 'I' ||
          letter == 'O' || letter == 'U') {
          stroke(204, 204, 0);
        }
        else {
          stroke(255);
        }
      }
      else {
        stroke(153);
        letter = char(48+counter);
      }

      // Draw the letter to the screen
      text(letter, 15+j*gap, 20+i*gap);

      // Increment the counter
      counter++;
      if(counter >= 26) {
        counter = 0;
      }
    }
  }
}

