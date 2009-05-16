package antipasto.GUI.GadgetListView;


import javax.swing.*;
import javax.swing.border.Border;

import antipasto.Interfaces.IModule;
import antipasto.Interfaces.IPackedFile;

import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.geom.RoundRectangle2D;
import java.awt.image.ColorModel;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;

/**
 * @author omarayub
 *
 */
public class GadgetCellRenderer extends DefaultListCellRenderer {
	private boolean doHover = false;
	private RoundRectangle2D outer;
	private RoundRectangle2D inner;
	public Component getListCellRendererComponent(JList list,
                                                    Object value,
                                                    int index,
                                                    boolean isSelected,
                                                    boolean hasFocus) {
        JLabel label =
          (JLabel)super.getListCellRendererComponent(list,
                                                     value,
                                                     index,
                                                     isSelected,
                                                     hasFocus);
  
        
        if (value instanceof IModule) {
            IModule gadget = (IModule)value;
            ImageIcon icon = new ImageIcon(gadget.getImage());
            
            /* Build the left panel */
            JPanel panelLeft = new JPanel();
            panelLeft.setLayout(new BorderLayout());
            panelLeft.setBackground(Color.white);
            panelLeft.setOpaque(true);
			
            JLabel imageLabel = new JLabel(icon);
            panelLeft.add(imageLabel, BorderLayout.WEST);
            
            
            /* Build the right panel */
            JPanel panelRight = new JPanel();
            panelRight.setLayout(new BoxLayout(panelRight, BoxLayout.PAGE_AXIS));
            panelRight.setBackground(Color.white);
            panelRight.setOpaque(true);
			
            File f = ((IPackedFile)value).getPackedFile();
            
            JLabel textLabel1 = new JLabel(" ");
            long sizeInBytes = f.length();
            JLabel textLabel2 = new JLabel(" Size: " + sizeInBytes / 10000 + " kb");
            JLabel textLabel3 = new JLabel(" Lines of Code: " + countLines((IModule)value));
            panelRight.add(textLabel1);
            panelRight.add(textLabel2);
            panelRight.add(textLabel3);
            
            /* Build the main container for left and right */
            JPanel panel = new JPanel();
            panel.setLayout(new BorderLayout());
            panel.setBackground(Color.white);
            panel.add(panelLeft, BorderLayout.WEST);
            panel.add(panelRight);
            
            if (isSelected) {
            	panelLeft.setBackground(Color.orange);
            	panelRight.setBackground(Color.orange);
            	panel.setBackground(Color.orange);
            } else {
            	panelLeft.setBackground(Color.white);
            	panelRight.setBackground(Color.white);
            	panel.setBackground(Color.white);            	
            }
            
            this.initBoxes();				//intialize the bounding boxes
            
            this.addMouseListener(new MouseListener(){

				public void mouseClicked(MouseEvent e) {					
				}

				public void mouseEntered(MouseEvent e) {	
					//System.out.println("Doing hover");
					doHover = true;
				}

				public void mouseExited(MouseEvent e) {	
					//System.out.println("Stopping hover..");
					doHover = false;
				}

				public void mousePressed(MouseEvent e) {	
				}

				public void mouseReleased(MouseEvent e) {		
				}
            	
            });
            
            return panel;
                  
        } else {
          // Clear old icon; needed in 1st release of JDK 1.2
          label.setIcon(null);
        }
        return(label);
      }
    
    
    public void paint(Graphics g){
    	super.paint(g);
    	Graphics2D g2d = (Graphics2D)g;
    	
    	if(this.doHover == true){
    		this.drawHover(g2d);
    	}
    }
    
    /**
     * Tells the module that its being "hovered" over
     * @param isHovering
     */
    public void doHover(boolean isHovering){
    	this.doHover = true;
    	this.repaint();
    }
    
    private void drawHover(Graphics2D g2d){
    	g2d.setColor(Color.GRAY);
		g2d.fill(outer);
		Color translucent = new Color(0.25f, 0.25f, 0.6f, 0.3f);
		g2d.setColor(translucent);
		g2d.fill(inner);
    }
    
    /**
     * Provides a count for the number of lines in the sketch
     * @param module
     * @return number of lines in the sketch
     */
    private int countLines(IModule module){
    	File sketch = module.getSketchFile();
    	try {
			BufferedReader reader = new BufferedReader(new FileReader(sketch));
			int count = 0;
	    	while((reader.readLine())!=null){	
	    		count++;
	    	}
	    	return count;
		} catch (FileNotFoundException e) {
			return 0;
		} catch (IOException e) {
			return 0;
		}
    }
    
    /**
     * Intializes the bounding boxes
     */
    private void initBoxes(){
    	this.outer = new RoundRectangle2D.Double((double)0, (double)0, (double)this.getSize().width, (double)this.getSize().height, (double)10, (double)10);
    	this.inner = new RoundRectangle2D.Double((double)0, (double)0, (double)this.getSize().width - 10, (double)this.getSize().height - 10, (double)9, (double)9);
    }
}
