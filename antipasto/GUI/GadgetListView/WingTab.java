package antipasto.GUI.GadgetListView;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.font.FontRenderContext;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;
import java.io.File;

import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import antipasto.Interfaces.*;

public class WingTab extends JComponent implements DocumentListener, MouseListener {
	
	RoundRectangle2D roundRect;
	Rectangle2D rect;
	RoundRectangle2D roundRect2;
	Rectangle2D rect2;
	
	boolean isUpdated = false;
	String txt;
	Font font;
	Object obj;
	int updates = 0;
	
	private Color bgDefaultColor = new Color(0x54, 0x91, 0x9e);
	private Color bgHoverColor = Color.WHITE;
	private Color bgColor;
	
	private Color textDefaultColor = Color.BLACK;
	private Color textHoverColor = Color.BLACK;
	private Color textColor;
	
	private double width = 0; 
	private double height = 25;
	private boolean isFocused = false;
	
	public WingTab(String title, Object valueHolder){
		//We need to create a rounded tab
		this.txt = title;
		this.bgColor = bgDefaultColor;
		this.textColor = textDefaultColor;
		
		String family = "Lucida Sans Typewriter";
        int style = Font.PLAIN;
        int size = 18;
		
		this.font = new Font(family, style, size);
		this.setVisible(true);
		obj = valueHolder;
		
		this.addMouseListener(this);

		this.setPreferredSize(new Dimension((int)this.width, (int)this.height));
		this.repaint();
		
	}
	
	 public void paint(Graphics g) {
	        Graphics2D graphics2 = (Graphics2D) g;
	        
	        this.width = font.getStringBounds(this.txt,graphics2.getFontRenderContext()).getWidth()-10;
	        
			String printTxt = this.txt;
			
			roundRect = new RoundRectangle2D.Double(0, 5, width, 25 , 10, 10);
			rect = new Rectangle2D.Double(0,10,width, 15);
			
			if (isFocused) {
				this.bgColor = this.bgHoverColor;
			}
			
			graphics2.setColor(this.bgColor);
			graphics2.fill(roundRect);
			graphics2.fill(rect);
			graphics2.draw(roundRect);
			graphics2.draw(rect);
			
	        graphics2.setColor(this.textColor);
	        graphics2.drawString(printTxt, 5,(int)height - 2 );
			this.setPreferredSize(new Dimension((int) this.width+20, (int)height));
	    }

	 /**
	  * 
	  * @param focus Sets the tab focus.
	  */
	public void setFocus(boolean focus) {
		this.isFocused = focus;
	}
	
	public void changedUpdate(DocumentEvent arg0) {
		this.isUpdated = true;
		if(arg0.getLength() == 0){
			this.isUpdated = false;
		}
		System.out.println("Changed Update");
		this.repaint();
	}
	public void insertUpdate(DocumentEvent arg0) {
		this.isUpdated = true;
		updates++;
		this.repaint();
	}

	public void removeUpdate(DocumentEvent arg0) {
		this.isUpdated = true;
		updates--;
		if(updates == 0){
			isUpdated = false;
		}
		this.repaint();
	}

	public void mouseClicked(MouseEvent arg0) {
		// TODO Auto-generated method stub
		System.out.println("Clicked " + this.txt + "tab");
	}

	public void mouseEntered(MouseEvent arg0) {
		// TODO Auto-generated method stub
		this.textColor = this.textHoverColor;
		this.bgColor = this.bgHoverColor;
		this.repaint();
	}

	public void mouseExited(MouseEvent arg0) {
		// TODO Auto-generated method stub
		this.textColor = this.textDefaultColor;
		this.bgColor = this.bgDefaultColor;
		this.repaint();
	}

	public void mousePressed(MouseEvent arg0) {
		// TODO Auto-generated method stub
	}

	public void mouseReleased(MouseEvent arg0) {
		// TODO Auto-generated method stub
		
	}
}
