package antipasto.GUI.GadgetListView;


import javax.swing.*;
import javax.swing.border.Border;

import antipasto.Interfaces.IModule;
import antipasto.Interfaces.IPackedFile;

import java.awt.*;
import java.awt.geom.RoundRectangle2D;
import java.awt.image.ColorModel;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;

public class GadgetCellRenderer extends DefaultListCellRenderer {
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
            
            Box box = Box.createVerticalBox();
            
            /* Build the left panel */
            JPanel panelLeft = new JPanel();
            panelLeft.setLayout(new BorderLayout());
            panelLeft.setBackground(Color.white);
            
            JLabel imageLabel = new JLabel(icon);
            panelLeft.add(imageLabel, BorderLayout.WEST);
            
            
            /* Build the right panel */
            JPanel panelRight = new JPanel();
            panelRight.setLayout(new BoxLayout(panelRight, BoxLayout.PAGE_AXIS));
            panelRight.setBackground(Color.white);
            
            File f = ((IPackedFile)value).getPackedFile();
            
            JLabel textLabel1 = new JLabel(" ");
            long sizeInBytes = f.length();
            JLabel textLabel2 = new JLabel("Size: " + sizeInBytes / 10000 + " kb");
            JLabel textLabel3 = new JLabel("Lines of Code: " + countLines((IModule)value));
            panelRight.add(textLabel1);
            panelRight.add(textLabel2);
            panelRight.add(textLabel3);
            
            /* Build the main container for left and right */
            JPanel panel = new JPanel();
            panel.setLayout(new BorderLayout());
            panel.setBackground(Color.white);
            panel.add(panelLeft, BorderLayout.WEST);
            panel.add(panelRight);
            return panel;
            //label.setText(gadget.getName());
            
        } else {
          // Clear old icon; needed in 1st release of JDK 1.2
          label.setIcon(null);
        }
        return(label);
      }
    
    private int countLines(IModule module){
    	File sketch = module.getSketchFile();
    	try {
			BufferedReader reader = new BufferedReader(new FileReader(sketch));
			String line;
	    	int count = 0;
	    	while((line = reader.readLine())!=null){	
	    		count++;
	    	}
	    	return count;
		} catch (FileNotFoundException e) {
			return 0;
		} catch (IOException e) {
			return 0;
		}
    }
}
