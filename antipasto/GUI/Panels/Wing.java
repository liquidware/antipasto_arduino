package antipasto.GUI.Panels;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;

public class Wing extends JPanel{
	private JButton extenderButton;
	private boolean isCollapsed = false;
	
	public Wing(ImageIcon image, String borderLayoutSide){
		this.setLayout(new BorderLayout());
		extenderButton = new JButton(image);
		extenderButton.addMouseListener(new MouseListener(){

			public void mouseClicked(MouseEvent arg0) {
				if(isCollapsed){
					expand();
				}else{
					collapse();
				}
			}

			public void mouseEntered(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			public void mouseExited(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			public void mousePressed(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			public void mouseReleased(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}
			
			}
		);
		this.add(extenderButton, borderLayoutSide);
		extenderButton.show();
	}
	
	private void expand(){
		Component[] components = this.getComponents();
		for(int i = 0; i < components.length; i++){
			components[i].setVisible(true);
		}
		this.isCollapsed = false;
		this.repaint();
	}
	
	private void collapse(){
		Component[] components = this.getComponents();
		for(int i = 0; i < components.length; i++){
			if(components[i] != extenderButton){
				components[i].setVisible(true);
			}
		}
		this.isCollapsed = true;
		this.repaint();
	}
	
	public Component add(Component comp){
		Component ret = super.add(comp);
		comp.setVisible(!isCollapsed);
		return ret;
	}
	
	public Component add(Component comp, int index ){
		Component ret = super.add(comp, index);
		ret.setVisible(!isCollapsed);
		return ret;
	}
}
