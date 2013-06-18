import javax.swing.JOptionPane;

public class ExceptionHandlingWITHTry{
	public static void main( String [] args ){
		int n = -1;
		boolean goodInput = false; // flag variable

		// priming read
		String s = JOptionPane.showInputDialog( null, "Enter an integer larger than 0:" );
		while(!goodInput){
			if(s==null) //Test for User Pressing Cancel/Close
				goodInput = true;

			//Exception Handle: Number smaller than 0; second due to exception handle of the following S overwriting cancel.
			if(n<1 && n!=-1) 
				s = JOptionPane.showInputDialog( null,s + " is not Greater than 0. " + "Enter an new number:" );
			
			//Exception Handle: Improper Input
			try{ 
				// attempt to convert the String to an int
				n = Integer.parseInt( s );
				//System.out.println("\nN is value: " + n);
				if(n>-1)
					goodInput = true; // Catch n being parsed correctly
				
			}	
			catch(NumberFormatException nfe){
				if(s!=null)	//If null wasn't pressed ask for correct input:
					s = JOptionPane.showInputDialog( null," Incorrect input: "+ s + " Enter an new number:" );
			} 
		}
		if(n<0){ //User Canceled/Closed 
			JOptionPane.showMessageDialog( null, "Thanks for using the Program!");
		}
		else{	// Normal Close
			JOptionPane.showMessageDialog( null, "The integer is: " + n );
		}
	}
}
