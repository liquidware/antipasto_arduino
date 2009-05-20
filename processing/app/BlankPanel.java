package processing.app;

import java.awt.Color;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import javax.swing.JPanel;

public class BlankPanel extends JPanel implements ComponentListener{
	    JPanel panel;
		public BlankPanel(JPanel parentPanel){
			panel = parentPanel;
			this.addComponentListener(this);
			this.setBackground(new Color(0x04, 0x4F, 0x6F));
		}
		public void componentHidden(ComponentEvent arg0) {
			//do nothing
		}
		public void componentMoved(ComponentEvent arg0) {
			//do nothing
		}
		public void componentResized(ComponentEvent arg0) {
			this.setSize(panel.getSize());
			this.repaint();
		}
		public void componentShown(ComponentEvent arg0) {
			//do nothing
		}
}
