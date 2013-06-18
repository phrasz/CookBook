import javax.swing.JOptionPane;
 
public class ExceptionHandling{
  public static void main( String [] args ){
    // declare and initialize variables that will be
    // assigned values in the try block
    int n = 0;
    boolean goodInput = false; // flag variable
 
    // priming read
    String s = JOptionPane.showInputDialog( null,
                             "Enter an integer" );
    do
    {
      try
      {
        // attempt to convert the String to an int
        n = Integer.parseInt( s );
        goodInput = true;
      }
      catch ( NumberFormatException nfe )
      {
        s = JOptionPane.showInputDialog( null,
                             s + " is not an integer. "
                             + "Enter an integer" );
      }
    } while ( !goodInput );
 
    JOptionPane.showMessageDialog( null, "The integer is " + n );
  }
}
