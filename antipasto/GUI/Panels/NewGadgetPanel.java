package antipasto.GUI.Panels;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.dnd.DropTargetDragEvent;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.dnd.DropTargetListener;

import javax.swing.JFrame;
import javax.swing.JWindow;

public class NewGadgetPanel extends JFrame {
	NewGadgetList gl;
	public NewGadgetPanel()
	{
		this.setSize(new Dimension(250, 250));
		gl = new NewGadgetList();
		gl.setSize(this.getSize());
		this.add(gl);
		gl.setVisible(true);
	}
	
	public static void main(String args[]){
		NewGadgetPanel panel = new NewGadgetPanel();
		panel.setVisible(true);
		panel.setDefaultCloseOperation(EXIT_ON_CLOSE);
	}
}
