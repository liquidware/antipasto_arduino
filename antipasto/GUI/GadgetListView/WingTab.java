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
import javax.swing.event.EventListenerList;

import antipasto.Interfaces.*;

public class WingTab extends JComponent implements MouseListener {
	private EventListenerList listeners = new EventListenerList();
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
	
	private double width; 
	private double height = 32;
	private boolean isFocused = false;
	private boolean isSelected = false;
	
	public WingTab(String title, Object valueHolder){
		
		this.txt = title;
		this.bgColor = bgDefaultColor;
		this.textColor = textDefaultColor;
		
		String family = "Lucida Sans Typewriter";
        int style = Font.PLAIN;
        int size = 18;
		
		this.font = new Font(family, style, size);
		obj = valueHolder;
		
		this.addMouseListener(this);
		
		/* Dynamically adjust size based on text width */
		this.width = (this.txt.length() * 7) + 10;
		
		this.setPreferredSize(new Dimension((int)this.width, (int)this.height));
		
	}
	
	 public void paint(Graphics g) {
	        Graphics2D graphics2 = (Graphics2D) g;
	        
		String printTxt = this.txt;
		
		roundRect = new RoundRectangle2D.Double(0, 13, width, 35, 10, 10);
		rect = new Rectangle2D.Double(0,18,width, 35);
		
		graphics2.setColor(this.bgColor);
		graphics2.fill(roundRect);
		graphics2.fill(rect);
		graphics2.draw(roundRect);
		graphics2.draw(rect);
			
	        graphics2.setColor(this.textColor);
	        graphics2.drawString(printTxt, 5,(int)height - 2 );
	    }
	 
	 /**
	  * 
	  * @param focus Sets the tab focus.
	  */
	public void setFocused(boolean focus) {
		this.isFocused = focus;
		
		if (isFocused) {
			this.bgColor   = this.bgHoverColor;
			this.textColor = this.textHoverColor;
		} else {
			this.bgColor   = this.bgDefaultColor;
			this.textColor = this.textDefaultColor;
		}
		
		this.repaint();
	}
	
	/**
	 * 
	 * @param selected Sets the selected parameter of the class.
	 */
	public void setSelected(boolean selected) {
		this.isSelected = selected;
	}
	
	public boolean getSelected() {
		return this.isSelected;
	}

	public void mouseClicked(MouseEvent arg0) {
		// TODO Auto-generated method stub

	}

	public void mouseEntered(MouseEvent arg0) {
		// TODO Auto-generated method stub
		if (!isFocused) {
			this.textColor = this.textHoverColor;
			this.bgColor = this.bgHoverColor;
			this.repaint();
		}
	}

	public void mouseExited(MouseEvent arg0) {
		// TODO Auto-generated method stub
		
		if (!isFocused) {
			this.textColor = this.textDefaultColor;
			this.bgColor = this.bgDefaultColor;
			this.repaint();
		}
	}

	public void mousePressed(MouseEvent arg0) {
		// TODO Auto-generated method stub
		this.isSelected = true;
		fireSelectedEvent();
	}

	public void mouseReleased(MouseEvent arg0) {
		// TODO Auto-generated method stub
		
	}
	
	public void addSelectionListener(ISelectedItemListener item){
		this.listeners.add(ISelectedItemListener.class, item);
	}
	
	private void fireSelectedEvent(){
		Object[] listeners = this.listeners.getListenerList();
	     // loop through each listener and pass on the event if needed
	     int numListeners = listeners.length;
	     for (int i = 0; i<numListeners; i+=2) 
	     {
	          if (listeners[i]==ISelectedItemListener.class) 
	          {
	               // pass the event to the listeners event dispatch method
	                ((ISelectedItemListener)listeners[i+1]).onSelected(this);
	          }            
	     }

	}
}
