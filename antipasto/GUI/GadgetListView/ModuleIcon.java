package antipasto.GUI.GadgetListView;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.RenderingHints;
import java.awt.Shape;
import java.awt.geom.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.IOException;

import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.TransferHandler;
import javax.swing.event.EventListenerList;

import antipasto.Interfaces.IModule;

public class ModuleIcon extends JLabel {
	private IModule internalModule;
	private EventListenerList listeners = new EventListenerList();
	private GadgetList list;
	private boolean hovering = false;
	private Ellipse2D toggledShape;
	
	public ModuleIcon(IModule module, GadgetList list){
		super(new ImageIcon(module.getImage().getScaledInstance(60, 60, Image.SCALE_AREA_AVERAGING)));
		internalModule = module;
		this.list = list;
		
		toggledShape = new Ellipse2D.Double(29, 29, 25, 25);
		
		this.addMouseListener(new MouseListener(){
			public void mouseClicked(MouseEvent arg0) {
				if(hovering == true){
					if(toggledShape.contains(arg0.getX(), arg0.getY())){
						fireSelectedEvent();
					}
				}
			}
			public void mouseEntered(MouseEvent arg0) {
				hovering = true;
				repaint();
			}
			
			public void mouseExited(MouseEvent arg0) {
				hovering = false;
				repaint();
			}
			public void mousePressed(MouseEvent e) {
				JComponent jc = (JComponent)e.getSource();
		        TransferHandler th = jc.getTransferHandler();
		    }
			public void mouseReleased(MouseEvent arg0) {
				
			}
		});
	}
	
	public void paint(Graphics g) {
		super.paint(g);
        Graphics2D g2d = (Graphics2D)g;
        
        if(hovering == true){
        	g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                    RenderingHints.VALUE_ANTIALIAS_ON);
            Font font = new Font("Serif", Font.PLAIN, 36);
            g2d.setFont(font);
            g2d.setColor(Color.GRAY);
            g2d.fill(this.toggledShape);
        	g2d.setColor(Color.WHITE);
        	g2d.drawString("+", 31, 51);
        }else{
        	//don't draw it
        }
    }
	
	public IModule getModule(){
		return this.internalModule;
	}
	
	public void addSelectionListener(ISelectedItemListener item){
		listeners.add(ISelectedItemListener.class, item);
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
