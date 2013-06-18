
//package jripley1c1;

import javax.swing.JOptionPane;

public class Main {
	/**
	 * @param args
	 */
	public static void main(String args[]) {
		//-------------------------------p115 #8: Sales Tax ------------------------------
		/*String amountPurchased;
		double salesTax = .04;
		double countyTax = .02;
		double totalTax;
		double totalSale;
		
		// Get Dollar Amount
		amountPurchased = JOptionPane.showInputDialog("Sales tax calculator: enter dollar amount.");
		salesTax = Double.parseDouble(amountPurchased) * salesTax;
		countyTax = Double.parseDouble(amountPurchased) * countyTax;
		totalTax = salesTax + countyTax;
		totalSale = Double.parseDouble(amountPurchased) + totalTax;
		JOptionPane.showMessageDialog(null, "Tax Calculator:\nAmount:       $" + amountPurchased
											+ "\nState Tax:    $" + salesTax 
											+ "\nCounty Tax: $" + countyTax 
											+ "\nTotal Tax:     $" + totalTax
											+ "\nTotal Sale:   $" + totalSale);*/
											

		//-------------------------------p115 #9: Miles-per-Gallon--------------------------
		/*String milesDriven;
		String gallonGasUsed;
		double milesPerGallon;
		
		milesDriven = JOptionPane.showInputDialog("MPG Calculator: enter miles driven.");
		gallonGasUsed = JOptionPane.showInputDialog("MPG Calculator: enter gallons used.");
		milesPerGallon = Double.parseDouble(milesDriven) / Double.parseDouble(gallonGasUsed);
		JOptionPane.showMessageDialog(null, "MPG Calculator:\nMiles:                      " + milesDriven
											+ "\nGallons:                   " + gallonGasUsed
											+ "\nMiles per gallon:   "+ milesPerGallon);	*/		
	
		//------------------------------p115 #10: Test Average------------------------------
		String firstScore;
		String secondScore;
		String thirdScore;
		int totalScore;
		int avgScore;
		bool Correct=0;
		
		while(!Correct){
			try(firstScore = JOptionPane.showInputDialog("Test Average: enter first test scores.")){
			
			}
			catch{
				JOptionPane.MessageDialog("ERROR! Please Enter Correct values!");
			}
			/*
			secondScore = JOptionPane.showInputDialog("Test Average: enter second test scores.");
			thirdScore = JOptionPane.showInputDialog("Test Average: enter third test scores.");
			totalScore = Integer.parseInt(firstScore) + Integer.parseInt(secondScore) + Integer.parseInt(thirdScore);
			avgScore = totalScore / 3;
*/
			JOptionPane.showMessageDialog(null, "Test Average\n\nFirst Score:  " + firstScore+ "\nSecond Score:  " + secondScore + "\nThird Score:  " + thirdScore + "\n\nAverage Score:  " + avgScore);
		}
	
	}
}

